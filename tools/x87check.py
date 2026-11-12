#!/usr/bin/env python3
"""Compare the x87 arithmetic of NONMATCHING functions with the original.

Aligns the original's and the built object's x87 instruction streams (memory operands masked
for alignment) and reports the differing chunks that contain arithmetic (fadd/fsub/fmul/fdiv),
with real operands, so wrong sum association, dropped terms or wrong fields stand out from
pure spill/stack-slot noise. Run match.py on the source first so build/obj is current.

--sym evaluates the x87 stack symbolically instead: every float store to a field, global or call
argument, every fist and every float compare becomes an event holding its expression tree
(a+b == b+a and a*b == b*a, but (a+b)+c != a+(b+c)); the event lists are aligned and only
events whose trees differ are printed. Frame slots are tracked relative to the entry esp, so
ebp and esp frames compare (an aligned frame's `and esp,-8` is ignored); the prologue (SEH frame,
saved registers, locals) is not taken for call arguments. Spilled values are forwarded; locals
read before any write and call results are renamed by first use within each event (L0.., R0..).
Float constants are compared by value: the original's .rdata operands are read from the exe, the
object's relocations resolve __real@ literals, read-only data defined in the object and
DAT_xxxxxxxx externs (read from the exe); writable globals are 'G'. An integer immediate stored to
a slot and read back as a float (mov [x],0x3f800000) is its float value. Control flow is walked
linearly: frame/field memory at a jump target reached after jmp/ret is restored from the jump,
values that disagree where a jump meets fall-through become phi(..), and a call forgets frame
slots within 0x40 bytes of any lea'd slot. A function whose esp or x87 depth disagrees at a jump
target is marked 'unreliable'. Every event carries its address (original VA / object offset).

usage: x87check.py [--all] [--quiet] [--sym [--list]] [SRC:ADDR ...]
  --all    every function listed in analysis/nonmatching-triage.md
  --quiet  one summary line per function (arithmetic chunks / differing events)
  --sym    compare expression trees of float stores and compares (see above)
  --list   with --sym: one line per differing event, `orig_addr | built_off | orig tree | built tree`
           (aligned pairs in order; an unpaired side is '-')
"""
import difflib
import math
import os
import re
import subprocess
import sys
import struct
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from asm_helpers import ROOT, pe_reader  # noqa: E402
from match import marked_functions  # noqa: E402
from objcmp import coff_function, disasm as objcmp_disasm  # noqa: E402

X87 = re.compile(r"^f(add|sub|mul|div|ld|st|xch|chs|sqrt|com|ucom|abs)")
ARITH = re.compile(r"^f(add|sub|mul|div)")


def x87(blob):
    """Return [(masked, real)] for the x87 instructions in a code blob."""
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(blob)
        f.flush()
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-mi386", "-Mintel", f.name],
                             capture_output=True, text=True).stdout
    seq = []
    for line in out.splitlines():
        if line.count("\t") < 2:
            continue
        ins = line.split("\t")[-1].strip()
        op = ins.split()[0] if ins else ""
        if X87.match(op):
            ops = ins[len(op):].strip()
            seq.append((f"{op} {'mem' if 'PTR' in ops else ops}".strip(), ins.replace("DWORD PTR ", "")))
    return seq


def check(exe, off, source, address, size):
    symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    o = x87(exe[off(address):off(address) + size])
    b = x87(coff_function(obj, symbol)[0])
    sm = difflib.SequenceMatcher(a=[m for m, _ in o], b=[m for m, _ in b], autojunk=False)
    chunks = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != "equal" and any(ARITH.match(m) for m, _ in o[i1:i2] + b[j1:j2]):
            chunks.append(([r for _, r in o[max(0, i1 - 4):i2 + 2]], [r for _, r in b[max(0, j1 - 4):j2 + 2]]))
    return len(o), len(b), chunks


REGS = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp")
IMM = re.compile(r"0x[0-9a-f]+|\d+")


def disasm(blob):
    """Return [(address, mnemonic, operand text)] for every instruction in a code blob."""
    seq = []
    for a, t in objcmp_disasm(blob):
        op, _, ops = t.partition(" ")
        seq.append((a, op, ops.strip()))
    return seq


def text(t):
    if not isinstance(t, tuple):
        return str(t)
    if t[0] == "@" and t[1] == "frame":
        return "&" + Sym.name(t[1:])  # address of a stack slot
    if t[0] == "@":
        return t[1] + (f"{t[2]:+#x}" if t[2] else "") if t[1] else f"{t[2]:#x}"
    if len(t) == 2:
        return f"{t[0]}({text(t[1])})"
    return f"({text(t[1])} {t[0]} {text(t[2])})"


def arith(base, x, y):
    if base in ("fadd", "fmul"):
        a, b = sorted((x, y), key=repr)  # commutative only, never re-associated
        return ("+" if base == "fadd" else "*", a, b)
    if base.endswith("r"):
        x, y = y, x
    return ("-" if base.startswith("fsub") else "/", x, y)


class Sym:
    """Linear symbolic walk of one function: integer registers, frame slots and the x87 stack.

    Integer values that may be addresses are ('@', base, offset); esp starts as the frame
    pointer, so ebp and esp frames give the same slot names (offsets from the entry esp)."""

    def __init__(self, const=lambda addr, nxt, ops: "G"):
        self.const = const    # (insn address, next address, operand text) -> constant label or 'G'
        self.cur = self.nxt = 0
        self.prologue = True
        self.leas = set()     # frame offsets whose address was taken (lea)
        self.msaved = {}      # jump target -> frame/field memory at the jump
        self.reg = {r: ("@", r + "0", 0) for r in REGS}
        self.reg["ecx"] = ("@", "this", 0)
        self.reg["esp"] = ("@", "frame", 0)
        self.pushed = 0       # bytes pushed since the last call (its arguments)
        self.skip_add = 0     # the call just popped its arguments; ignore a cdecl `add esp`
        self.mem, self.st, self.ev = {}, [], []
        self.ncall, self.bad = 0, 0
        self.saved = {}       # jump target -> (esp, ebp, x87 stack)

    @staticmethod
    def ptr(v):
        return v[1:] if isinstance(v, tuple) and v[0] == "@" else (text(v), 0)

    def key(self, ops):
        """Canonical memory operand (base, offset); base 'frame' = entry esp, 'abs' = absolute (read as 'G')."""
        m = re.search(r"\[(.*)\]", ops)
        if "fs:" in ops or not m:
            return ("fs", 0) if "fs:" in ops else ("abs", self.const(self.cur, self.nxt, ops) if "ds:" in ops else "G")
        base, off, rest = None, 0, []
        for t in re.findall(r"[+-]?[^+-]+", m.group(1).replace(" ", "")):
            sign, v = (-1, t[1:]) if t[0] == "-" else (1, t.lstrip("+"))
            if IMM.fullmatch(v):
                off += sign * int(v, 0)
            elif v in REGS and base is None:
                base, o = self.ptr(self.reg[v])
                off += o
            else:
                r, _, scale = v.partition("*")
                rest.append(text(self.reg.get(r, r)) + ("*" + scale if scale else ""))
        base = "+".join([base or "abs"] + rest) if rest else base or "abs"
        return (base, off)

    def load(self, k):
        if k in self.mem:
            return self.mem[k]
        return self.name(k)

    @staticmethod
    def name(k):
        base, off = k
        if base == "frame":
            return f"arg{off:+#x}" if off > 0 else f"loc{off:+#x}"
        if base in ("abs", "fs"):
            return (off if isinstance(off, str) else "G") if base == "abs" else base
        return f"M[{base}{off:+#x}]" if off else f"M[{base}]"

    def store(self, k, v, fpu):
        isfloat = fpu or isinstance(v, tuple) and v[0] != "@"
        if k[0] == "abs":
            if isfloat:
                self.ev.append((self.cur, self.name(k), v))  # never forwarded
            return
        self.mem[k] = v
        if isfloat and k[0] != "frame":
            self.ev.append((self.cur, self.name(k), v))
        elif isfloat and self.esp() <= k[1] < self.esp() + self.pushed:
            self.ev.append((self.cur, "arg", v))  # outgoing call argument

    def esp(self):
        b, o = self.ptr(self.reg["esp"])
        return o if b == "frame" else 0

    def top(self, i=0):
        return self.st[-1 - i] if len(self.st) > i else f"ret{self.ncall}"

    def put(self, i, v):
        while len(self.st) <= i:
            self.st.insert(0, f"ret{self.ncall}")
        self.st[-1 - i] = v

    def pop(self):
        return self.st.pop() if self.st else f"ret{self.ncall}"

    @staticmethod
    def sti(t):
        m = re.fullmatch(r"st\((\d)\)", t)
        return int(m.group(1)) if m else 0

    def value(self, a):
        if a in REGS:
            return self.reg[a]
        if "[" in a or "ds:" in a:
            return self.load(self.key(a))
        return ("@", "", int(a, 0)) if IMM.fullmatch(a) else a

    def add(self, r, n):
        b, o = self.ptr(self.reg[r])
        self.reg[r] = ("@", b, o + n)

    def step(self, addr, op, ops, after_jump, nxt=0):
        self.cur, self.nxt = addr, nxt
        if addr in self.msaved:
            if after_jump:  # reached only by a jump: memory is what it was at the jump
                self.mem = dict(self.msaved[addr])
            else:  # a merge: values the two paths disagree on are unknown
                for k in set(self.msaved[addr]) | set(self.mem):
                    if self.msaved[addr].get(k) != self.mem.get(k):
                        self.mem[k] = "phi(" + ("slot" if k[0] == "frame" else self.name(k)) + ")"
        if addr in self.saved:
            esp, ebp, st = self.saved[addr]
            if after_jump:
                self.reg["esp"], self.reg["ebp"], self.st = esp, ebp, list(st)
            elif esp != self.reg["esp"] or len(st) != len(self.st):
                self.bad += 1
        args = [a.strip() for a in ops.split(",")] if ops else []
        if self.prologue and not (op in ("push", "sub", "and") or op == "mov" and ("fs:" in ops or ops == "ebp,esp")):
            self.prologue, self.pushed = False, 0  # SEH frame, saved registers and locals are not arguments
        skip, self.skip_add = self.skip_add, 0
        if op.startswith("j") or op == "loop":
            if IMM.fullmatch(ops):
                self.saved.setdefault(int(ops, 16), (self.reg["esp"], self.reg["ebp"], list(self.st)))
                self.msaved.setdefault(int(ops, 16), dict(self.mem))
        elif op.startswith("f"):
            self.fpu(op, args, ops, "[" in ops or "ds:" in ops)
        elif op == "push":
            v = self.value(ops)
            self.add("esp", -4)
            self.pushed += 4 * (v != ("@", ops + "0", 0))  # saving a callee-saved register is not an argument
            self.store(("frame", self.esp()), v, False)
        elif op == "pop":
            if ops in REGS:
                self.reg[ops] = self.mem.get(("frame", self.esp()), "popped")
            self.add("esp", 4)
            self.pushed = max(0, self.pushed - 4)
        elif op == "call":
            self.ncall += 1
            self.add("esp", self.pushed)  # thiscall/stdcall callee pops its arguments
            self.skip_add, self.pushed = self.pushed, 0
            for r in ("eax", "ecx", "edx"):
                self.reg[r] = f"ret{self.ncall}"
            # shortcut: a lea'd slot is assumed to cover 0x40 bytes; track object sizes if that bites
            self.mem = {k: v for k, v in self.mem.items()
                        if k[0] == "frame" and not any(o <= k[1] < o + 0x40 and (k[1] < 0) == (o < 0)
                                                       for o in self.leas)}
        elif op == "leave":
            self.reg["esp"] = self.reg["ebp"]
            self.reg["ebp"] = self.mem.get(("frame", self.esp()), "popped")
            self.add("esp", 4)
        elif op in ("add", "sub") and len(args) == 2 and args[0] in REGS and IMM.fullmatch(args[1]):
            n = int(args[1], 0) * (1 if op == "add" else -1)
            if args[0] == "esp":
                if skip and n > 0:
                    return
                self.pushed = max(0, self.pushed - n)
            self.add(args[0], n)
        elif op in ("mov", "movsx", "movzx") and len(args) == 2:
            v = self.value(args[1])
            if args[0] in REGS:
                self.reg[args[0]] = v
            else:
                self.store(self.key(args[0]), v, False)
        elif op == "lea" and len(args) == 2 and args[0] in REGS:
            k = self.key(args[1])
            self.reg[args[0]] = ("@",) + k
            if k[0] == "frame":
                self.leas.add(k[1])
        elif op == "and" and args and args[0] == "esp":
            return  # aligned frame (and esp,-8): keep entry-esp offsets; esp slots are off by the padding only
        elif op == "xor" and len(args) == 2 and args[0] == args[1] and args[0] in REGS:
            self.reg[args[0]] = ("@", "", 0)
        elif op in ("cmp", "test"):
            for v in {repr(self.value(a)): self.value(a) for a in args}.values():
                if isinstance(v, tuple) and v[0] != "@":
                    self.ev.append((self.cur, "bits", v))  # integer compare of a float's bits
        elif args and args[0] in REGS:
            self.reg[args[0]] = f"{op}({','.join(text(self.value(a)) for a in args)})"
        elif args and "[" in args[0]:
            self.mem.pop(self.key(args[0]), None)

    def fload(self, ops):
        """A float memory operand; an integer immediate stored there (mov [x],0x3f800000) is its float value."""
        v = self.load(self.key(ops))
        if isinstance(v, tuple) and v[:2] == ("@", "") and "DWORD" in ops:
            return label(struct.unpack("<f", struct.pack("<I", v[2] & 0xffffffff))[0], False)
        return v

    def fpu(self, op, args, ops, mem):
        base = op[:-1] if op.endswith("p") and op not in ("fcomp", "fucomp", "fcompp", "fucompp") else op
        if op in ("fld", "fild"):
            v = (self.load(self.key(ops)) if op == "fild" else self.fload(ops)) if mem else self.top(self.sti(args[0]))
            self.st.append(("int", v) if op == "fild" else v)
        elif op in ("fld1", "fldz"):
            self.st.append("1" if op == "fld1" else "0")
        elif base in ("fst", "fist"):
            v = ("fist", self.top()) if base == "fist" else self.top()
            if mem:
                self.store(self.key(ops), v, True)
            else:
                self.put(self.sti(args[0]), v)
            if op.endswith("p"):
                self.pop()
        elif base in ("fadd", "fsub", "fsubr", "fmul", "fdiv", "fdivr"):
            if mem:
                d, x, y = 0, self.top(), self.fload(ops)
            else:
                args = args or ["st(1)", "st"]
                args = args if len(args) == 2 else ["st", args[0]]
                d = self.sti(args[0])
                x, y = self.top(d), self.top(self.sti(args[1]))
            self.put(d, arith(base, x, y))
            if op.endswith("p"):
                self.pop()
        elif op == "fxch":
            i = self.sti(args[0]) if args else 1
            a, b = self.top(), self.top(i)
            self.put(0, b)
            self.put(i, a)
        elif op in ("fchs", "fabs", "fsqrt", "fsin", "fcos", "frndint"):
            self.put(0, (op[1:], self.top()))
        elif op == "fpatan":
            x = self.pop()
            self.put(0, ("atan2", self.top(), x))
        elif op.startswith(("fcom", "fucom")):
            other = self.fload(ops) if mem else self.top(self.sti(args[0]) if args else 1)
            self.ev.append((self.cur, "cmp", tuple(sorted((self.top(), other), key=repr))))
            for _ in range(op.count("p")):
                self.pop()
        elif op == "ftst":
            self.ev.append((self.cur, "cmp", (self.top(), "0")))


def real(name):
    """Value label of an MSVC literal __real@4@<80-bit hex> (float) / __real@8@... (double)."""
    size, h = name.split("@")[1], int(name.split("@")[2], 16)
    e, mant = (h >> 64) & 0x7fff, h & ((1 << 64) - 1)
    v = (-1) ** (h >> 79) * math.ldexp(mant, e - 16383 - 63) if e else 0.0
    return label(v, size == "8")


def label(v, double):
    if double:
        return f"{v:.17g}d"
    return f"{struct.unpack('<f', struct.pack('<f', v))[0]:.9g}"


def rdata(exe):
    """(start, end) VAs of the original's .rdata, where its float constants live."""
    pe = struct.unpack_from("<I", exe, 0x3c)[0]
    count, opt = struct.unpack_from("<H", exe, pe + 6)[0], struct.unpack_from("<H", exe, pe + 20)[0]
    base = struct.unpack_from("<I", exe, pe + 52)[0]
    for i in range(count):
        name, vs, rva = struct.unpack_from("<8sII", exe, pe + 24 + opt + 40 * i)
        if name.rstrip(b"\0") == b".rdata":
            return base + rva, base + rva + vs
    return 0, 0


def exe_const(exe, off, rd):
    """Constant resolver for the original: .rdata float/double operands become their value."""
    def const(addr, nxt, ops, va=None):
        m = re.search(r"ds:(0x[0-9a-f]+)", ops)
        va = int(m.group(1), 16) if va is None else va
        if not rd[0] <= va < rd[1] or not re.search(r"[DQ]WORD PTR", ops):
            return "G"  # writable data (or an integer/80-bit operand): not a constant
        q = "QWORD" in ops
        return label(struct.unpack_from("<d" if q else "<f", exe, off(va))[0], q)
    return const


def obj_rodata(path):
    """{symbol: bytes from its definition on} for symbols defined in a read-only section of a COFF object."""
    b = open(path, "rb").read()
    _, nsec, _, symptr, nsym, optsz, _ = struct.unpack_from("<HHIIIHH", b, 0)
    secs = [struct.unpack_from("<8sIIIIIIHHI", b, 20 + optsz + 40 * i) for i in range(nsec)]
    out, i = {}, 0
    while i < nsym:
        off = symptr + 18 * i
        raw = b[off:off + 8]
        if raw[:4] == b"\0\0\0\0":
            s = symptr + 18 * nsym + struct.unpack_from("<I", raw, 4)[0]
            raw = b[s:b.index(b"\0", s)]
        value, secnum, _, _, naux = struct.unpack_from("<IhHBB", b, off + 8)
        if 0 < secnum <= nsec and not secs[secnum - 1][9] & 0x80000000 and secs[secnum - 1][4]:
            start = secs[secnum - 1][4] + value  # initialised, not IMAGE_SCN_MEM_WRITE
            out[raw.rstrip(b"\0").decode()] = b[start:start + 8]
        i += 1 + naux
    return out


def obj_const(targets, exe_c, rodata):
    """Constant resolver for the object: __real@ literals, read-only data defined in the object
    and DAT_xxxxxx externs (read from the exe) via relocations."""
    def const(addr, nxt, ops):
        t = next((targets[k] for k in range(addr, nxt) if k in targets), "")
        if t.startswith("__real@"):
            return real(t)
        if t in rodata and re.search(r"[DQ]WORD PTR", ops):
            q = "QWORD" in ops
            return label(struct.unpack_from("<d" if q else "<f", rodata[t].ljust(8, b"\0"))[0], q)
        m = re.search(r"\bDAT_([0-9a-f]{8})", t)  # _DAT_x or C++ ?DAT_x@@3MA
        return exe_c(addr, nxt, ops, int(m.group(1), 16)) if m else "G"
    return const


def events(blob, const=None, base=0):
    """Return ([(address, event string)], unreliability count) for one function's code."""
    s = Sym(const) if const else Sym()
    prev = ""
    dis = disasm(blob)
    for i, (addr, op, ops) in enumerate(dis):
        if prev == "ret" and all(t < addr for t in s.saved):
            break  # end of the body: what follows (EH funclets, padding) is not this function
        s.step(addr, op, ops, prev in ("jmp", "ret"), dis[i + 1][0] if i + 1 < len(dis) else len(blob))
        prev = op
    def ren(t):  # stack slots and call numbers differ between builds: name them by first use in the event
        t, names = text(t), {}
        for m in re.findall(r"loc-0x[0-9a-f]+|\bret\d+", t):
            names.setdefault(m, f"L{len(names)}" if m[0] == "l" else f"R{len(names)}")
        return re.sub(r"loc-0x[0-9a-f]+|\bret\d+", lambda m: names[m.group(0)], t)
    out = []
    for a, k, v in s.ev:
        if k == "cmp":
            out.append((a + base, "cmp " + " ? ".join(ren(x) for x in v)))
        else:
            out.append((a + base, f"{ren(k)} = {ren(v)}"))
    return out, s.bad


def check_sym(exe, off, source, address, size):
    """Return (orig count, built count, [(orig [(addr, ev)], built [(off, ev)])], unreliability)."""
    symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    exe_c = exe_const(exe, off, rdata(exe))
    o, bad_o = events(exe[off(address):off(address) + size], exe_c, address)
    blob, _, targets = coff_function(obj, symbol)
    b, bad_b = events(blob, obj_const(targets, exe_c, obj_rodata(obj)))
    sm = difflib.SequenceMatcher(a=[e for _, e in o], b=[e for _, e in b], autojunk=False)
    diffs = [(o[i1:i2], b[j1:j2]) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal"]
    return len(o), len(b), diffs, bad_o + bad_b


def triage():
    for line in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md")):
        cols = [c.strip() for c in line.split("|")]
        if line.startswith("| 0x"):
            source, address = cols[2], int(cols[1], 16)
            if any(a == address and nm for a, _, nm in marked_functions(os.path.join(ROOT, source))):
                yield source, address, int(cols[6])


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    sizes = {(s, a): n for s, a, n in triage()}
    todo = list(sizes) if "--all" in sys.argv else [(s, int(a, 16)) for s, a in (x.split(":") for x in args)]
    exe, off = pe_reader()
    for source, address in todo:
        if "--sym" in sys.argv:
            try:
                no, nb, diffs, bad = check_sym(exe, off, source, address, sizes[(source, address)])
            except (OSError, KeyError, StopIteration, TypeError, ValueError, IndexError, SystemExit) as e:
                print(f"0x{address:x} {source}: skipped ({type(e).__name__})")
                continue
            n = sum(max(len(x), len(y)) for x, y in diffs)
            print(f"0x{address:x} {source}: events {no}/{nb}, {n} differ" + (", unreliable" if bad else ""))
            if "--list" in sys.argv:  # one line per differing event, paired in order; '-' = unpaired
                for oc, bc in diffs:
                    for i in range(max(len(oc), len(bc))):
                        oa, oe = oc[i] if i < len(oc) else ("-", "-")
                        ba, be = bc[i] if i < len(bc) else ("-", "-")
                        print(f"  {oa if oa == '-' else hex(oa)} | {ba if ba == '-' else hex(ba)} | {oe} | {be}")
            elif "--quiet" not in sys.argv:
                for oc, bc in diffs:
                    print("\n".join([f"  orig  {a:#x}: {e}" for a, e in oc] + [f"  built {a:#x}: {e}" for a, e in bc]
                                    + ["  --"]))
            continue
        try:
            no, nb, chunks = check(exe, off, source, address, sizes[(source, address)])
        except (OSError, KeyError, StopIteration, TypeError, SystemExit) as e:
            print(f"0x{address:x} {source}: skipped ({type(e).__name__})")
            continue
        print(f"0x{address:x} {source}: x87 {no}/{nb}, {len(chunks)} arithmetic chunks differ")
        if "--quiet" not in sys.argv:
            for oc, bc in chunks:
                print("  orig : " + " | ".join(oc))
                print("  built: " + " | ".join(bc))


if __name__ == "__main__":
    main()
