# Opening an archive-backed file

`FUN_004a5430` is the archive branch used by `File::Open`. Its third argument
selects a probe-only path; normal opens retain a stream for the sibling `.bzf`
archive. A matching index entry supplies the payload offset, logical size,
stored size, and compression flag. Compressed entries initialize the zlib
state at `File+0x2c`. These behaviors and record offsets are observed in
Evidence `ev_0abca160e108a5edc19e9a82f9cede714db070a08f92a4490af93a43d3849603`.

The behavior reconstruction is `File::OpenArchiveEntry` in
`src/file_archive.cpp`, declared as a private `File` method and called by
`File::Open` before its disk fallback. Its archive stream allocations and
constructor calls now remain inline at both original call sites, following
the original exception scopes. The body is still NONMATCHING (1236 built
bytes versus 1253 original; 911 differing bytes outside relocations). The
zlib call uses the external zlib API; no SDK implementation is copied. Full
behavior has not been exercised against an archive fixture.

The helper copies the input into shared path scratch `DAT_0056baa4`; the
declared 0x200-byte extent is inferred from its C-string use and the 0x204-byte
distance to the archive-vector marker. Evidence `ev_0abca160e108a5edc19e9a82f9cede714db070a08f92a4490af93a43d3849603` records the path, record fields, stream seek, and zlib setup. Its retained dossier is in `evidence-2026-10-23-archive-open.json`.
