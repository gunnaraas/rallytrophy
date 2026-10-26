#!/usr/bin/env python3
"""Recover the API behind each SecuROM-redirected call site in the official 1.01 exe.

SecuROM rewrote every `[__imp_X]` operand in .text to point into .cms_d. The
reference exe has identical code with the operands pointing back into a rebuilt
import table, so pairing the two at the same file offset yields the import name.

usage: securom_api_map.py OFFICIAL_EXE REFERENCE_EXE > api_callsites.csv
"""
import csv
import struct
import sys

IMAGE_BASE = 0x400000

# Opcode bytes preceding a disp32 operand that dereferences an import slot.
OPERAND_FORMS = {
    b"\xff\x15": "call",
    b"\xff\x25": "jmp",
    b"\x8b\x3d": "mov edi",
    b"\x8b\x35": "mov esi",
    b"\x8b\x1d": "mov ebx",
    b"\x8b\x2d": "mov ebp",
    b"\x8b\x0d": "mov ecx",
    b"\x8b\x15": "mov edx",
}


class PE:
    def __init__(self, path):
        self.b = open(path, "rb").read()
        pe = struct.unpack_from("<I", self.b, 0x3C)[0]
        nsec = struct.unpack_from("<H", self.b, pe + 6)[0]
        optsz = struct.unpack_from("<H", self.b, pe + 20)[0]
        opt = pe + 24
        self.import_rva = struct.unpack_from("<I", self.b, opt + 96 + 8)[0]
        self.sections = {}
        for i in range(nsec):
            o = opt + optsz + 40 * i
            name = self.b[o:o + 8].rstrip(b"\0").decode()
            vsize, va, rsize, raw = struct.unpack_from("<IIII", self.b, o + 8)
            self.sections[name] = (va, vsize, rsize, raw)

    def rva_to_off(self, rva):
        for va, vsize, rsize, raw in self.sections.values():
            if va <= rva < va + max(vsize, rsize):
                return raw + rva - va
        raise ValueError(hex(rva))

    def cstr(self, rva):
        o = self.rva_to_off(rva)
        return self.b[o:self.b.index(b"\0", o)].decode()

    def iat(self):
        """Map IAT slot VA -> 'DLL!Name' from the import directory."""
        slots = {}
        d = self.rva_to_off(self.import_rva)
        while True:
            ilt, _, _, name_rva, iat_rva = struct.unpack_from("<IIIII", self.b, d)
            if not name_rva:
                return slots
            dll = self.cstr(name_rva)
            lookup = ilt or iat_rva
            for k in range(0x10000):
                entry = struct.unpack_from("<I", self.b, self.rva_to_off(lookup) + 4 * k)[0]
                if not entry:
                    break
                if entry & 0x80000000:
                    sym = "#%d" % (entry & 0xFFFF)
                else:
                    sym = self.cstr(entry + 2)
                slots[IMAGE_BASE + iat_rva + 4 * k] = f"{dll}!{sym}"
            d += 20


def main(official_path, reference_path):
    official, reference = PE(official_path), PE(reference_path)
    slots = reference.iat()
    cms_va, cms_vsize, _, _ = official.sections[".cms_d"]
    cms_lo, cms_hi = IMAGE_BASE + cms_va, IMAGE_BASE + cms_va + cms_vsize
    text_va, _, text_rsize, text_raw = official.sections[".text"]
    ref_text_raw = reference.sections[".text"][3]
    ref_text_end = ref_text_raw + reference.sections[".text"][2]

    out = csv.writer(sys.stdout)
    out.writerow(["site", "form", "redirect_slot", "api"])
    unresolved = 0
    for i in range(2, text_rsize - 4):
        target = struct.unpack_from("<I", official.b, text_raw + i)[0]
        if not cms_lo <= target < cms_hi:
            continue
        form = OPERAND_FORMS.get(official.b[text_raw + i - 2:text_raw + i])
        if form is None:
            continue
        ref_off = ref_text_raw + i
        api = None
        if ref_off + 4 <= ref_text_end:
            api = slots.get(struct.unpack_from("<I", reference.b, ref_off)[0])
        if api is None:
            unresolved += 1
            api = "?"
        site = IMAGE_BASE + text_va + i - 2
        out.writerow([f"0x{site:x}", form, f"0x{target:x}", api])
    print(f"unresolved: {unresolved}", file=sys.stderr)


if __name__ == "__main__":
    main(*sys.argv[1:3])
