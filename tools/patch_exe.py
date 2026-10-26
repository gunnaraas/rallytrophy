#!/usr/bin/env python3
"""Build a runnable patched RallyTrophy.exe by hooking original functions to our compiled copies.

Usage: python3 -I tools/patch_exe.py [--build] [--include-nonmatching]
           [--only 0xADDR,...] [--files src/x.cpp,...] [--base other.exe] [-o build/RallyTrophy_patched.exe]
Default selection: every `// FUNCTION:` function (byte-exact => no-op sanity check).
--files/--only also pull in NONMATCHING drafts. --build runs match.py first to refresh build/obj.
Code and obj-local data go into one new RWX section ".rtpatch"; each original function
starts with `jmp rel32` to its copy. Calls to other marked functions go to the originals.
IAT calls (__imp_X) use the exe's real import table, not the SecuROM-redirected slots the original code uses.
shortcut: single RWX section, obj-local statics are duplicated (not shared with the original);
          base relocs are not extended, so DYNAMIC_BASE is cleared.
"""
import csv, glob, os, re, struct, subprocess, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import match  # noqa: E402
from securom_api_map import IMAGE_BASE, PE  # noqa: E402

ROOT = match.ROOT
ADDR_IN_NAME = re.compile(r"(?:FUN|DAT|PTR|LAB|SUB|thunk_FUN|switchD|caseD)_?(?:[A-Za-z_]*_)?([0-9a-fA-F]{8})\b", re.I)
ALIGN = 16


# CRT/runtime aliases; each address verified as a function start in analysis/functions.csv.
ALIASES = {
    "??2@YAPAXI@Z": 0x4c1276,                 # operator new (FUN_004c1276/PoolAlloc in src/)
    "??3@YAXPAX@Z": 0x4bd000,                 # operator delete
    "_atexit": 0x4c0d01,                      # static-init stubs register via FUN_004c0d01
    "??_L@YGXPAXIHP6EX0@Z1@Z": 0x4c11f4,      # eh vector ctor iterator
    "??_M@YGXPAXIHP6EX0@Z@Z": 0x4c1116,       # eh vector dtor iterator
    "_itoa": 0x4c1902,                        # CRT itoa
    "_rand": 0x4c1998,                        # CRT rand
    "_floor": 0x4c1404,                       # CRT floor
    "__ftol": 0x4c19bc,                       # src/unk_420490.cpp FUN_004c19bc "_ftol" (src comment says guess)
}


class Obj:
    def __init__(self, path):
        self.path, b = path, open(path, "rb").read()
        self.b = b
        _, nsec, _, symptr, nsym, optsz, _ = struct.unpack_from("<HHIIIHH", b, 0)
        strtab = symptr + 18 * nsym

        def name_at(off):
            raw = b[off:off + 8]
            if raw[:4] == b"\0\0\0\0":
                s = strtab + struct.unpack_from("<I", raw, 4)[0]
                return b[s:b.index(b"\0", s)].decode()
            return raw.rstrip(b"\0").decode()

        self.secs = []  # 1-based via index-1: (name, rawsize, rawptr, relptr, nrel, flags)
        for i in range(nsec):
            n, _, _, rs, rp, rl, _, nr, _, fl = struct.unpack_from("<8sIIIIIIHHI", b, 20 + optsz + 40 * i)
            self.secs.append((name_at(20 + optsz + 40 * i), rs, rp, rl, nr, fl))
        self.syms, i = {}, 0  # raw index -> (name, value, secnum, sclass)
        while i < nsym:
            off = symptr + 18 * i
            value, secnum, _, sclass, naux = struct.unpack_from("<IhHBB", b, off + 8)
            self.syms[i] = (name_at(off), value, secnum, sclass)
            i += 1 + naux
        self.by_name = {v[0]: k for k, v in self.syms.items() if v[2] > 0 and v[3] in (2, 3)}

    def find(self, name):
        """Mirror objcmp.coff_function's symbol matching; None if absent/ambiguous."""
        if "::" in name:
            cls, mem = name.rsplit("::", 1)
            cls = "@".join(reversed(cls.split("::")))
            first = cls.split("@")[0]
            pre = (f"??0{cls}@" if mem == first else f"??1{cls}@" if mem == "~" + first else f"?{mem}@{cls}@",)
            plain = None
        else:
            plain = name.lstrip("_")
            pre = (f"?{plain}@", f"_{plain}@")
        m = [k for k, s in self.syms.items() if s[2] > 0 and (
            s[0] == name or (plain and s[0] == "_" + plain) or s[0].startswith(pre))]
        return m[0] if len(m) == 1 else None

    def extent(self, idx):
        _, start, sec, _ = self.syms[idx]
        ends = [v for n, v, s, _ in self.syms.values() if s == sec and v > start and not n.startswith("$")]
        return start, min(ends + [self.secs[sec - 1][1]])

    def data(self, sec, lo, hi):
        _, rs, rp, _, _, fl = self.secs[sec - 1]
        return bytearray(b"\0" * (hi - lo)) if fl & 0x80 else bytearray(self.b[rp + lo:rp + hi])

    def relocs(self, sec, lo, hi):
        _, _, _, rl, nr, _ = self.secs[sec - 1]
        for k in range(nr):
            va, si, ty = struct.unpack_from("<IIH", self.b, rl + 10 * k)
            if lo <= va < hi:
                yield va - lo, si, ty


class Unresolved(Exception):
    pass


class Patcher:
    def __init__(self, exe, objs, base_va):
        self.pe, self.objs, self.base = PE(exe), objs, base_va
        self.buf = bytearray()
        self.marked = {}   # (objpath, symidx) -> original address
        self.names = {}    # csv / IAT names -> address
        for f in ("functions.csv", "functions_extra.csv"):
            for r in csv.DictReader(open(os.path.join(ROOT, "analysis", f))):
                if r.get("name"):
                    self.names.setdefault(r["name"], int(r["entry"], 16))
        for slot, n in self.pe.iat().items():
            self.names["__imp__" + n.split("!", 1)[1]] = slot
        self.names.update(ALIASES)
        self.conf, self.rbase, self.bad, self.dup_ro = set(), {}, set(), set()  # read-only data reuse bookkeeping
        self.cur, self.site, self.orig, self.pe0 = None, None, None, None
        self.cache = {}    # (objpath, sec, lo, hi) -> VA of copy (shared blobs)

    def alloc(self, size):
        self.buf += b"\0" * (-len(self.buf) % ALIGN)
        off = len(self.buf)
        self.buf += b"\0" * size
        return off

    def external(self, obj, idx):
        name = obj.syms[idx][0]
        m = ADDR_IN_NAME.search(name)
        if m:
            return int(m.group(1), 16)
        for k in (name, name.lstrip("_"), re.sub(r"@\d+$", "", name), re.sub(r"@\d+$", "", name).lstrip("_")):
            if k in self.names:
                return self.names[k]
        return None

    def sym_addr(self, obj, idx, stack):
        if (obj.path, idx) in self.marked:
            return self.marked[(obj.path, idx)]
        a = self.external(obj, idx)
        if a is not None:
            return a
        name, val, sec, sclass = obj.syms[idx]
        if sec <= 0:
            raise Unresolved(name)
        flags = obj.secs[sec - 1][5]
        if flags & 0x20 and sclass == 2:  # code symbol: own extent
            lo, hi = obj.extent(idx)
            return self.blob(obj, sec, lo, hi, stack) + 0
        if flags & 0x20:  # label ($Lnnn) or section symbol: locate the enclosing function
            fns = [k for k, v in obj.syms.items() if v[2] == sec and v[3] == 2 and v[1] <= val]
            if not fns:
                raise Unresolved(name)
            fi = max(fns, key=lambda k: obj.syms[k][1])
            lo, hi = obj.extent(fi)
            if not lo <= val < hi:
                raise Unresolved(name)
            base = self.marked.get((obj.path, fi)) or self.blob(obj, sec, lo, hi, stack)
            return base + (val - lo)
        if flags & 0x80000080:  # writable or uninitialised: a copy would diverge from the original's
            raise Unresolved("writable static: " + name)
        return self.ro_data(obj, sec, val, stack)

    def ro_data(self, obj, sec, val, stack):
        """Read-only data: prefer the original's address (pointer identity), else duplicate."""
        k = (obj.path, sec)
        if k in self.rbase:
            if self.cur and self.site and not self.cur[1] and not stack[1:]:
                u = struct.unpack_from("<I", self.orig, self.pe0.rva_to_off(self.cur[0] - IMAGE_BASE) + self.site[0])[0]
                if (u - self.site[1] - val) & 0xFFFFFFFF != self.rbase[k] and k not in self.conf:
                    self.conf.add(k)
                    print(f"ro-data CONFLICT {os.path.basename(k[0])} sec {k[1]} at {self.cur[0]:#x}: using per-site original address")
                if (u - self.site[1] - val) & 0xFFFFFFFF != self.rbase[k] and IMAGE_BASE <= u < self.base:
                    return (u - self.site[1]) & 0xFFFFFFFF  # this site's own original target
            return self.rbase[k] + val
        size = obj.secs[sec - 1][1]
        base = None
        if k not in self.bad and self.cur and not stack[1:]:
            addr, nm, site = self.cur[0], self.cur[1], self.site
            if site and not nm:
                u = struct.unpack_from("<I", self.orig, self.pe0.rva_to_off(addr - IMAGE_BASE) + site[0])[0]
                b = (u - site[1] - val) & 0xFFFFFFFF
                if IMAGE_BASE <= b < self.base:
                    base = b
        if base is None and k not in self.bad and size >= 4 and not obj.relocs(sec, 0, size):
            d = obj.data(sec, 0, size)
            if d.strip(b"\0"):
                hits = []
                for n, (va, vs, rs, raw) in self.pe0.sections.items():
                    if n.startswith((".rdata", ".data")):
                        hits += [IMAGE_BASE + va + m.start() for m in re.finditer(re.escape(bytes(d)), self.orig[raw:raw + rs])]
                if len(hits) == 1:
                    base = hits[0]
        if base is not None:
            self.rbase[k] = base
            return base + val
        self.dup_ro.add(k)
        return self.blob(obj, sec, 0, size, stack) + val

    def blob(self, obj, sec, lo, hi, stack):
        key = (obj.path, sec, lo, hi)
        if key in self.cache:
            return self.cache[key]
        off = self.alloc(hi - lo)
        va = self.base + off
        self.cache[key] = va
        self.fill(obj, sec, lo, hi, off, stack + [key])
        return va

    def fill(self, obj, sec, lo, hi, off, stack):
        code = obj.data(sec, lo, hi)
        for roff, si, ty in obj.relocs(sec, lo, hi):
            if ty not in (6, 7, 0x14):
                raise Unresolved(f"reloc-type-{ty:#x}")
            nm, val, tsec, _ = obj.syms[si]
            add = struct.unpack_from("<i", code, roff)[0]
            self.site = (roff, add) if len(stack) == 1 and ty == 6 else None
            if (obj.path, si) in self.marked or self.external(obj, si) is not None or tsec <= 0:
                tgt = self.sym_addr(obj, si, stack)
            elif tsec == sec and lo <= val + (add if ty == 6 else 0) < hi + 0 and obj.secs[sec - 1][5] & 0x20:
                tgt = self.base + off + (val - lo)  # inside this function's own extent
                if ty == 6:
                    tgt += 0  # addend added below; keep self-relative to the copy
                    self.internal.add(roff)
            else:
                tgt = self.sym_addr(obj, si, stack)
            if len(stack) == 1 and (tgt >= self.base or nm.startswith('__imp_')):
                self.internal.add(roff)  # points into our copied blobs: differs by design
            if len(stack) == 1:
                self.relocoffs.add(roff)
            if len(stack) == 1 and ty == 0x14:
                self.rel32.add(roff)
            if ty == 0x14:
                v = (tgt + add - (self.base + off + roff + 4)) & 0xFFFFFFFF
            else:
                v = (tgt + add) & 0xFFFFFFFF
            struct.pack_into("<I", code, roff, v)
        self.buf[off:off + len(code)] = code

    internal = None
    rel32 = None
    relocoffs = None

    def emit(self, obj, idx):
        """Copy function symbol idx; return (VA, masked-internal-offsets). Transactional."""
        snap = (len(self.buf), dict(self.cache))
        self.internal, self.rel32, self.relocoffs = set(), set(), set()
        try:
            _, _, sec, _ = obj.syms[idx]
            lo, hi = obj.extent(idx)
            key = (obj.path, sec, lo, hi)
            self.cache.pop(key, None)
            return self.blob(obj, sec, lo, hi, []), self.internal
        except Unresolved:
            del self.buf[snap[0]:]
            self.cache = snap[1]
            raise


def align(x, a):
    return (x + a - 1) // a * a


def write_pe(exe, out, buf, base_va):
    b = bytearray(open(exe, "rb").read())
    pe = struct.unpack_from("<I", b, 0x3C)[0]
    nsec = struct.unpack_from("<H", b, pe + 6)[0]
    optsz = struct.unpack_from("<H", b, pe + 20)[0]
    opt = pe + 24
    salign, falign = struct.unpack_from("<II", b, opt + 32)
    tab = opt + optsz
    first_raw = min(struct.unpack_from("<I", b, tab + 40 * i + 20)[0] for i in range(nsec))
    if tab + 40 * (nsec + 1) > first_raw:
        sys.exit("no room for another section header")
    raw = align(len(b), falign)
    b += b"\0" * (raw - len(b))
    data = bytes(buf) + b"\0" * (align(len(buf), falign) - len(buf))
    b += data
    rva = base_va - IMAGE_BASE
    b[tab + 40 * nsec:tab + 40 * nsec + 40] = struct.pack(
        "<8sIIIIIIHHI", b".rtpatch", len(buf), rva, len(data), raw, 0, 0, 0, 0, 0xE0000060)
    struct.pack_into("<H", b, pe + 6, nsec + 1)
    struct.pack_into("<I", b, opt + 56, align(rva + len(buf), salign))
    struct.pack_into("<I", b, opt + 64, 0)  # checksum
    dc = struct.unpack_from("<H", b, opt + 70)[0]
    struct.pack_into("<H", b, opt + 70, dc & ~0x40)  # clear DYNAMIC_BASE
    return b


def main(argv):
    exe = match.TARGET_EXE
    out = os.path.join(ROOT, "build", "RallyTrophy_patched.exe")
    only, files, nonm, build, base = None, None, False, False, None
    excl, exfiles = set(), set()
    a = list(argv)
    while a:
        x = a.pop(0)
        if x == "--only":
            only = {int(v, 16) for v in a.pop(0).split(",")}
        elif x == "--files":
            files = [os.path.abspath(v) for v in a.pop(0).split(",")]
        elif x == "--include-nonmatching":
            nonm = True
        elif x == "--build":
            build = True
        elif x == "--base":  # exe to patch (e.g. the no-CD 1.01 build); imports resolve against it
            base = a.pop(0)
        elif x == "--exclude":
            excl |= {int(v, 16) for v in a.pop(0).split(",")}
        elif x == "--exclude-files":
            exfiles |= {os.path.abspath(v) for v in a.pop(0).split(",")}
        elif x == "-o":
            out = a.pop(0)
        else:
            sys.exit(__doc__)
    srcs = files or sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True))
    allsrcs = sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True))
    if build:
        subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "match.py"), "--quiet", *srcs], check=False)
    nonm = nonm or bool(files) or bool(only)

    def objpath(s):
        return os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(s))[0] + ".obj")

    objs = {s: Obj(objpath(s)) for s in allsrcs if os.path.exists(objpath(s))}
    pe0 = PE(exe)
    base = base or exe
    peb = PE(base)
    peo = struct.unpack_from("<I", peb.b, 0x3C)[0]
    tab0 = peo + 24 + struct.unpack_from("<H", peb.b, peo + 20)[0]
    last = max(sum(struct.unpack_from("<II", peb.b, tab0 + 40 * i + 8))
               for i in range(struct.unpack_from("<H", peb.b, peo + 6)[0]))
    base_va = IMAGE_BASE + align(last, 0x1000)
    P = Patcher(base, objs, base_va)
    P.orig, P.pe0 = pe0.b, pe0
    cands = []  # (src, obj, idx, addr, nonmatching, name)
    for s, o in objs.items():
        for addr, name, nm in match.marked_functions(s):
            idx = o.find(name)
            if idx is None:
                print(f"skip {addr:#x} {name}: symbol not found in obj")
                continue
            P.marked[(o.path, idx)] = addr
            if (s in srcs) and s not in exfiles and addr not in excl and (only is None or addr in only) and (nonm or not nm):
                cands.append((s, o, idx, addr, nm, name))
    sizes = {int(r["entry"], 16): int(r["size"]) for f in ("functions.csv", "functions_extra.csv")
             for r in csv.DictReader(open(os.path.join(ROOT, "analysis", f)))}
    orig = pe0.b
    peoff = struct.unpack_from("<I", orig, 0x3C)[0]
    ranges = []
    for i in range(struct.unpack_from("<H", orig, peoff + 6)[0]):
        n, vs, va = struct.unpack_from("<8sII", orig, peoff + 24 + 240 + 40 * i)
        if n.startswith(b".cms_t"):
            ranges.append((IMAGE_BASE + va, IMAGE_BASE + va + vs))
    eh_count = [0]
    patched, unresolved, hooks, check = 0, [], [], [0, 0, []]
    for s, o, idx, addr, nm, name in cands:
        if any(lo <= addr < hi for lo, hi in ranges):
            unresolved.append((addr, name, "in .cms_t, untouched"))
            continue
        if sizes.get(addr, 99) < 5:
            unresolved.append((addr, name, "function < 5 bytes"))
            continue
        # resolve this function's own symbol to the copy, not the original
        saved = P.marked.pop((o.path, idx))
        P.cur = (addr, nm)
        try:
            va, internal = P.emit(o, idx)
        except Unresolved as e:
            unresolved.append((addr, name, str(e)))
            continue
        finally:
            P.marked[(o.path, idx)] = saved
        lo, hi = o.extent(idx)
        size = sizes.get(addr, hi - lo)
        o0 = va - base_va
        if P.buf[o0:o0 + 3] == b"\x6a\xff\x68" and P.buf[o0 + 7] == 0x64 and 3 not in P.relocoffs:
            # SEH prologue whose handler imm is missing from the obj (push 0): original handler for exact copies
            eh_count[0] += 1
            if nm:
                unresolved.append((addr, name, "eh handler (NONMATCHING draft has its own state tables)"))
                del P.buf[o0:]
                P.cache = {k: v for k, v in P.cache.items() if v < va}
                continue
            P.buf[o0 + 3:o0 + 7] = orig[pe0.rva_to_off(addr - IMAGE_BASE) + 3:][:4]
        hooks.append((addr, va))
        patched += 1
        if not nm:
            copy = bytes(P.buf[va - base_va:va - base_va + size])
            ref = bytearray(orig[pe0.rva_to_off(addr - IMAGE_BASE):][:size])
            for k in P.rel32:  # rel32 is position dependent: rebase to the original site
                if k + 4 <= size:
                    v = (struct.unpack_from("<I", copy, k)[0] + va - addr) & 0xFFFFFFFF
                    copy = copy[:k] + struct.pack("<I", v) + copy[k + 4:]
            for k in internal:  # relocs into the new section differ by design
                if k + 4 <= size:
                    copy = copy[:k] + ref[k:k + 4] + copy[k + 4:]
            # compare after masking the rel32 of a call to ourselves (none) -- exact compare
            ok = len(copy) == len(ref) and copy == bytes(ref)
            check[0] += ok
            check[1] += 1
            if not ok:
                check[2].append(f"{addr:#x} {name} @offsets {[k for k in range(min(len(copy), len(ref))) if copy[k] != ref[k]][:6]} copy={copy[:8].hex()} orig={bytes(ref[:8]).hex()}")
    b = write_pe(base, out, P.buf, base_va)
    for addr, va in hooks:
        off = peb.rva_to_off(addr - IMAGE_BASE)  # file layout of the exe being written
        b[off:off + 5] = b"\xe9" + struct.pack("<i", va - (addr + 5))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    open(out, "wb").write(b)
    print(f"wrote {out}: patched {patched} functions, new section {len(P.buf)} bytes at {base_va:#x}")
    print(f"read-only data blobs: reused-original {len(P.rbase)}, duplicated {len(P.dup_ro - set(P.rbase))}")
    print("  duplicated:", sorted({(os.path.basename(p), n) for p, n in P.dup_ro - set(P.rbase)})[:15])
    print(f"functions with imm-less EH prologue: {eh_count[0]}")
    print(f"self-check (FUNCTION copies == original bytes): {check[0]}/{check[1]} pass")
    for c in check[2][:20]:
        print("  mismatch", c)
    cats = {}
    for addr, name, why in unresolved:
        print(f"unresolved {addr:#x} {name}: {why}")
        cats[why.split("(")[0][:40]] = cats.get(why.split("(")[0][:40], 0) + 1
    print(f"{len(unresolved)} unresolved; top missing symbols:",
          sorted(cats.items(), key=lambda kv: -kv[1])[:10])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
