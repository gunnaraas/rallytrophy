#!/usr/bin/env python3
"""Dump the original's C++ EH FuncInfo for a function: unwind map (state -> parent state,
cleanup funclet) and try blocks. Each funclet's first instructions are shown with call/jmp
targets named, so the map lists every destructible local/temporary (`lea ecx,[ebp-X]; jmp ~T`).

usage: ehmap.py ADDRESS
"""
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from calls import names  # noqa: E402
from match import TARGET_EXE  # noqa: E402
from objcmp import IMAGE_BASE, PE, disassemble  # noqa: E402


def unwind(address, pe=None):
    """(max_state, [(state, to_state, funclet text)], [try block text]) of the original, or None without EH."""
    pe = pe or PE(TARGET_EXE)
    rd = lambda va: struct.unpack_from("<I", pe.b, pe.rva_to_off(va - IMAGE_BASE))[0]
    code = lambda va, n: pe.b[pe.rva_to_off(va - IMAGE_BASE):][:n]
    head = code(address, 24)
    i = head.find(b"\x6a\xff\x68")
    if i < 0:
        return None
    handler = struct.unpack_from("<I", head, i + 3)[0]
    thunk = code(handler, 5)
    if thunk[0] != 0xB8:
        return None
    info = struct.unpack_from("<I", thunk, 1)[0]
    magic, max_state, unwind, ntry, trymap = (rd(info + 4 * k) for k in range(5))
    known = names()

    def name(va):
        return known.get(va, f"0x{va:x}")

    def funclet(va):
        out = []
        for ins in disassemble(code(va, 24), va)[:4]:
            m = re.match(r"(jmp|call)\s+0x([0-9a-f]+)$", ins)
            out.append(f"{m.group(1)} {name(int(m.group(2), 16))}" if m else ins)
            if m or ins.startswith("ret"):
                break
        return "; ".join(out)

    states, tries = [], []
    for s in range(max_state):
        to_state, action = rd(unwind + 8 * s), rd(unwind + 8 * s + 4)
        to_state = to_state - (1 << 32) if to_state >= 1 << 31 else to_state
        states.append((s, to_state, f"0x{action:x}: {funclet(action)}" if action else "(no cleanup)"))
    for t in range(ntry):
        lo, hi, catch_hi, n, handlers = (rd(trymap + 20 * t + 4 * k) for k in range(5))
        tries.append(f"try states {lo}-{hi} (catch states to {catch_hi}):")
        for h in range(n):
            adj, typ, disp, fn = (rd(handlers + 16 * h + 4 * k) for k in range(4))
            tname = pe.cstr(typ + 8 - IMAGE_BASE) if typ else "..."
            tries.append(f"  catch {tname} (adjectives 0x{adj:x}, object [ebp{disp:+#x}]) at 0x{fn:x}")
    return max_state, states, tries


def main(address):
    address = int(address, 16)
    r = unwind(address)
    if r is None:
        sys.exit(f"0x{address:x}: no `push -1; push handler` EH prolog (no EH frame)")
    max_state, states, tries = r
    print(f"0x{address:x}: maxState {max_state}")
    for s, to_state, text in states:
        print(f"  state {s:2d} -> {to_state:3d}  {text}")
    for t in tries:
        print("  " + t)


if __name__ == "__main__":
    main(sys.argv[1])
