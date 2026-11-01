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
ebp and esp frames compare; spilled values are forwarded, locals read before any write are
renamed by first use, globals and constants are all 'G' (relocations are unresolved in the
object), call results are 'retN'. Control flow is walked linearly; a function whose esp or x87
depth disagrees at a jump target is marked 'unreliable'.

usage: x87check.py [--all] [--quiet] [--sym] [SRC:ADDR ...]
  --all    every function listed in analysis/nonmatching-triage.md
  --quiet  one summary line per function (arithmetic chunks / differing events)
  --sym    compare expression trees of float stores and compares (see above)
"""
import difflib
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from asm_helpers import ROOT, pe_reader  # noqa: E402
from match import marked_functions  # noqa: E402
from objcmp import coff_function  # noqa: E402

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
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(blob)
        f.flush()
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-mi386", "-Mintel", f.name],
                             capture_output=True, text=True).stdout
    seq = []
    for line in out.splitlines():
        parts = line.split("\t")
        if len(parts) < 3 or not parts[-1].strip():
            continue
        op, _, ops = parts[-1].strip().partition(" ")
        seq.append((int(parts[0].strip().rstrip(":"), 16), op, ops.strip()))
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

    def __init__(self):
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
            return ("fs" if "fs:" in ops else "abs", 0)
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
            return "G" if base == "abs" else base
        return f"M[{base}{off:+#x}]" if off else f"M[{base}]"

    def store(self, k, v, fpu):
        isfloat = fpu or isinstance(v, tuple) and v[0] != "@"
        if k[0] == "abs":
            if isfloat:
                self.ev.append(("G", v))  # never forwarded: every global is 'G'
            return
        self.mem[k] = v
        if isfloat and k[0] != "frame":
            self.ev.append((self.name(k), v))
        elif isfloat and self.esp() <= k[1] < self.esp() + self.pushed:
            self.ev.append(("arg", v))  # outgoing call argument

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

    def step(self, addr, op, ops, after_jump):
        if addr in self.saved:
            esp, ebp, st = self.saved[addr]
            if after_jump:
                self.reg["esp"], self.reg["ebp"], self.st = esp, ebp, list(st)
            elif esp != self.reg["esp"] or len(st) != len(self.st):
                self.bad += 1
        args = [a.strip() for a in ops.split(",")] if ops else []
        skip, self.skip_add = self.skip_add, 0
        if op.startswith("j") or op == "loop":
            if IMM.fullmatch(ops):
                self.saved.setdefault(int(ops, 16), (self.reg["esp"], self.reg["ebp"], list(self.st)))
        elif op.startswith("f"):
            self.fpu(op, args, ops, "[" in ops or "ds:" in ops)
        elif op == "push":
            v = self.value(ops)
            self.add("esp", -4)
            self.pushed += 4
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
            self.mem = {k: v for k, v in self.mem.items() if k[0] == "frame"}
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
            self.reg[args[0]] = ("@",) + self.key(args[1])
        elif op == "xor" and len(args) == 2 and args[0] == args[1] and args[0] in REGS:
            self.reg[args[0]] = ("@", "", 0)
        elif op in ("cmp", "test"):
            for v in {repr(self.value(a)): self.value(a) for a in args}.values():
                if isinstance(v, tuple) and v[0] != "@":
                    self.ev.append(("bits", v))  # integer compare of a float's bits
        elif args and args[0] in REGS:
            self.reg[args[0]] = f"{op}({','.join(text(self.value(a)) for a in args)})"
        elif args and "[" in args[0]:
            self.mem.pop(self.key(args[0]), None)

    def fpu(self, op, args, ops, mem):
        base = op[:-1] if op.endswith("p") and op not in ("fcomp", "fucomp", "fcompp", "fucompp") else op
        if op in ("fld", "fild"):
            v = self.load(self.key(ops)) if mem else self.top(self.sti(args[0]))
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
                d, x, y = 0, self.top(), self.load(self.key(ops))
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
            other = self.load(self.key(ops)) if mem else self.top(self.sti(args[0]) if args else 1)
            self.ev.append(("cmp", tuple(sorted((self.top(), other), key=repr))))
            for _ in range(op.count("p")):
                self.pop()
        elif op == "ftst":
            self.ev.append(("cmp", (self.top(), "0")))


def events(blob):
    """Return (event strings, unreliability count) for one function's code."""
    s = Sym()
    prev = ""
    for addr, op, ops in disasm(blob):
        if prev == "ret" and all(t < addr for t in s.saved):
            break  # end of the body: what follows (EH funclets, padding) is not this function
        s.step(addr, op, ops, prev in ("jmp", "ret"))
        prev = op
    names = {}

    def ren(t):  # stack slots differ between builds: name uninitialised locals by first use
        t = text(t)
        for m in re.findall(r"loc-0x[0-9a-f]+", t):
            names.setdefault(m, f"L{len(names)}")
        return re.sub(r"loc-0x[0-9a-f]+", lambda m: names[m.group(0)], t)
    out = []
    for k, v in s.ev:
        if k == "cmp":
            out.append("cmp " + " ? ".join(ren(x) for x in v))
        else:
            out.append(f"{ren(k)} = {ren(v)}")
    return out, s.bad


def check_sym(exe, off, source, address, size):
    symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    o, bad_o = events(exe[off(address):off(address) + size])
    b, bad_b = events(coff_function(obj, symbol)[0])
    sm = difflib.SequenceMatcher(a=o, b=b, autojunk=False)
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
            if "--quiet" not in sys.argv:
                for oc, bc in diffs:
                    print("\n".join(["  orig : " + e for e in oc] + ["  built: " + e for e in bc] + ["  --"]))
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
