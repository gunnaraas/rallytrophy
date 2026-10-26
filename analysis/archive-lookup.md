# Archive-backed file lookup

`FUN_004a5920` copies the requested path into shared scratch storage at
`DAT_0056b9a0`, scans backward for the final slash or backslash, replaces the
directory suffix with `.bzf`, and uses the leaf name as the index key. It
searches the cached archive vector by archive path using a begin/end count and
index. It scans the full vector and retains the last matching entry, as shown
by the loop's compare branch and index advance in Evidence
`ev_f2c24e3ed96ecb7e79d41787b395807036ca94177dc641f5f4a0b57425cf8655`. On a
cache miss it checks for the sibling archive, reads its 12-byte index header and `count * 0x35`
bytes of records, decodes the records with the game's 256-byte XOR key, and
adds the cache to the vector. It then compares each record's name at offset
`+0x0d` case-insensitively. These behaviors are observed in Evidence
`ev_f2c24e3ed96ecb7e79d41787b395807036ca94177dc641f5f4a0b57425cf8655`.
The Ghidra data type at `DAT_0056b9a0` is only `undefined4`; the source's
0x104-byte scratch extent is an inference from the function's string operations
and the 0x104-byte address distance to `DAT_0056baa4`, not a recovered type
(`ev_00912103d8566ac1a62f8c914ead10c3b8e29644c97f9fa51a3c4412b887a366`).
The address-name and initialized-byte observations are also retained in
`evidence-2026-10-23-archive-scratch.json`
(`ev_00912103d8566ac1a62f8c914ead10c3b8e29644c97f9fa51a3c4412b887a366`,
`ev_1b03ca572cca07bfc64027471b62ea2f424d3c72927098b4ea25c6d8aa9ee89c`).

The behavior reconstruction is in `src/file_archive.cpp`. The lookup keeps
the archive stream allocation and constructor call inline, as in the original
function's exception-handling region. This reduced the matcher difference
count for `FUN_004a5920` while leaving it NONMATCHING (895 built bytes versus
982 original); `File::OpenArchiveEntry` is NONMATCHING at 1236/1253. The cache state is a
VC6 `std::vector<FileArchiveCache*>`: its object starts with a one-byte
allocator marker, followed by begin/end/capacity pointers. This layout is
confirmed by the vector initialization sequence at `0x4a4c60` and the insert
helper called by the original lookup. A scratch VC6 compile showed that
`vector::insert(end(), value)` inlines insertion machinery here, while the
shipped binary calls a shared helper. The source retains an explicit append
over the recovered fields; treating the separate C globals as a vector object
was rejected because their emitted object-file order differs from the vector
field order. The archive suffix is written directly over the final separator,
matching the original's five byte stores. Replacing two local path strings
with the observed shared scratch buffer and removing the `strcat` scan
shortened `FUN_004a5920` from 1135 to 895 bytes; it remains `NONMATCHING`
against 982 original bytes. The buffer's 0x208-byte declared
extent is inferred, not recovered: Ghidra identifies the start as `undefined4`,
and its uses treat it as a C string. No runtime archive fixture was available
for an end-to-end check. The retained pseudocode and assembly are in
`evidence-2026-10-22-loop17.json`.
