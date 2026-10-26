# Archive cache object (Patch 1.01)

Reconstructed in `include/file_archive.h` and
`src/file_archive.cpp`. `FileArchiveCache` is an analyst-chosen name.
Addresses in this document belong to Patch 1.01.

Evidence retained in `analysis/evidence-2026-10-22-loop20.json`:

- Constructor pseudocode, `0x4a4b10`:
  `ev_60d1e9f244b6f3a73797e5940aeea9401dc8bf41f2149ad058680afb64663905`.
- Constructor assembly:
  `ev_65dd927145d464a379d0f5cfbba14270e6e8ae6616502f0fa89958731e5150bb`.
- Destructor assembly, `0x4a4bd0`:
  `ev_7f6f72384784368b91b68afb9050086e571e380fd32c0553bb20536243a1edaf`.

The constructor initializes the string at +0, assigns the C string at
`0x5070b0`, then clears the dwords at +0x18 and +0x1c. It leaves bytes
+0x10..+0x17 untouched. The destructor frees the allocation referenced by
+0x1c, then releases the string. Evidence: the three records above.

Interpreting the object as a 0x20-byte archive cache, +0x18 as its entry count,
and +0x1c as a packed index allocation follows lookup `0x4a5920`, retained in
`analysis/evidence-2026-10-22-loop17.json`, Evidence
`ev_cf7dff3e8134f587d6fa12c3ceadb85c1b39b90ff222b6fa02a6001226d45977`.
That routine allocates 0x20 bytes, reads a 12-byte header into +0x10, allocates
`entryCount * 0x35` bytes, reads that index, and decodes it before searching
case-insensitively through names at record offset +0x0d. Those roles are
derived field interpretations; the original type names are unknown.

Plain local PE observation: the first byte at `0x5070b0` is zero in
`orig/RallyTrophy.exe`, so the initial assigned string is empty in that image.
The reconstruction retains the external address rather than substituting a
literal. It supplies no CRT or STL library implementation.

Local compilation observation: default-constructing the name and assigning
it in the constructor body matches the full 179-byte constructor. Initializing
the name directly in the member initializer list gives 147 bytes and lacks
the observed exception-unwind frame. The destructor matches all 73 bytes
using array delete followed by normal member destruction. Comparison masks
relocations; these are object-code checks, not linked runtime tests.
