# `File::Open`

`File::Open` (`0x4a4f50`) stores the requested name, records the text-mode bit,
and tries `File::OpenArchiveEntry` before opening a disk stream. The disk path
uses the legacy 100-byte `fstream` allocation/constructor call and initializes
the logical and raw sizes from `GetSize()` (or clears both for write mode).
Both successful paths read the fixed file signature through `File::Read`; a
failed disk open closes the file and throws `file_error("Cannot open file")`,
while a signature mismatch throws `file_error("Invalid file")`. These behaviors
are observed in Evidence `ev_99c9e6265233a208ee2d60d17289d27174e62b68f151a2d9bd0b01833ecdd22b`.

`src/file.cpp` now wires the archive helper into `File::Open` and reconstructs
the disk fallback and signature check. The implementation is marked
`NONMATCHING` (1213 bytes emitted versus 1247 in the executable); the main
remaining differences are exception construction and legacy stream setup
instruction shape. Runtime behavior has not yet been exercised against the
game's archive or disk assets.

`File::Read` (`0x4a5e00`) copies from the 8 KiB buffer, calls
`FUN_004a6090` when the active window is empty, advances the buffer cursor,
and returns the requested byte count. This behavior is observed in Evidence
`ev_2ce5b46227818d0f6834546cdf07f283f0953190b5e3c1e87accda2722f34149`.
The implementation in `src/file.cpp` follows the measured x86 instruction
sequence and now matches all 132 bytes (one relocation); this exact-match
result is a local tool observation. `FUN_004a6090`'s refill logic is also
reconstructed below from the same file-class dossier.

`FUN_004a6090` (`ev_34b3183dccd814a903100ab281812d4ec6ec2b3116815196eca87efac7bc84a6`)
refills at most `0x2000` bytes. The plain path reads at the raw offset,
advances that offset, resets the output cursor, checks the stream, and applies
the indexed byte transform when enabled. The compressed path maintains zlib's
input/output windows, reads more archive bytes when `avail_in` reaches zero,
applies the same transform to input, and inflates into the output window. The
inflate implementation is provided by zlib; no SDK body is copied. This helper
is NONMATCHING (306 vs. 308 bytes).

`FUN_004a63b0` checks for a missing or failed stream and throws
`file_error("Read error")` (Evidence
`ev_4daa441b9e318aa26c999ccc0ffb066fa2d1433f9ee28e958aac6ec7f1d5ed15`); its
reconstruction is NONMATCHING (440 vs. 469 bytes). The zlib/archive path has
not yet been exercised against game data.

`File::Close` (`0x4a5d00`) is reconstructed in `src/file.cpp` from Evidence
`ev_334d5577f328ddd50a9f52a751746462258fdfa2f20a5bdc224dc96aef65e5fa`: close
and release the stream, end the inflater when the compression flag is set,
clear the filename through the observed VC6 string helpers, and reset the
cached size to `-1`. It remains NONMATCHING (240 vs. 241 bytes; 191 differing
bytes outside relocations).

`File::GetSize` (`0x4a5ff0`) reads the cached size, returning `-1` when the
stream is absent, and lazily fills the cache from the stored path. This is
observed in Evidence `ev_e49d12803a37e03c475a2ad3c703d5c7dd3aa4392236b73cf159fdf0e491ee49`.
The implementation in `src/file.cpp` matches the original byte-for-byte
(84 bytes, 2 relocations).

`File::Write` (`0x4a5e90`) writes through the stream buffer, checks the stream,
and returns the requested count (Evidence
`ev_8dc39e86a2daa8fc3d0ae01bd967dbfc4793b359f2b56f7d3166db9efeb32200`). Its
stream check throws `file_error("Write error")` when the stream is absent or
failed (Evidence `ev_4aca27b489a723537324608b96d6e4133bc1897629df39295c1c3ddfcc9aea78`).
`File::Write` now matches byte-for-byte (38 bytes); the stream check remains
NONMATCHING (440 vs. 469 bytes).

The observed File layout is a stream pointer at `+0`, the 16-byte VC6
`std::string` name at `+4`, and remaining state at `+0x14` through `+0x4077`.
`file.cpp` uses a typed view of those fields; other translation units keep an
opaque `0x4078`-byte view. This is a compiler-shape observation: exposing the
string member declaration globally changed `FUN_004a2440` codegen, while the
opaque view restores its 3099-byte exact match. The retained constructor/open
dossiers and exact constructor matches support the field layout.

File lifecycle bodies are now present as well. The default constructor builds
the name string and resets buffer/mode/size state; the opening constructor
performs that initialization and calls `Open`; the destructor closes the file
and destroys the name string. These behaviors are observed in Evidence
`ev_4d357e48a2ddc85a3de6bbc37724772ed0763ddee70c877bb86bca0bc5e7d562`,
`ev_db530566a19bd6dc353eaee2b80b0061457a1a5a499049a2b8f4cc059165613f`, and
`ev_b67770eb4505e0acf546b4a04c05f2ac6c16354261808705bcf57336ab290156`;
the reset helper uses Evidence
`ev_6cd0bf3349a4b391be27ae337a81ef5279e5b3881a0d024704901aec9783a0e2`.
The default constructor, opening constructor, and destructor now match exactly
(76, 93, and 102 bytes). The reset helper remains NONMATCHING (223 vs. 229;
144 differing bytes outside relocations) and models the observed string
detach/grow/empty calls.

The one-byte reader `FUN_004a5ec0` matches exactly (19 bytes, 1 relocation;
Evidence `ev_69d69ad81fabefa8a33d33392002140bc88c76f7809c762b92f4b096f958d529`).
`File::ReadLine` (`0x4a5ee0`) uses it to skip leading CR/LF bytes, copy through
the next line ending, and terminate the output there. If the output limit is
reached first, it returns without adding a terminator, as observed in Evidence
`ev_7bbbf8b5fc01382272c2cea7bf5ce11df0b521f52cb22aa350a9e423e11a5158`. The
method matches byte-for-byte (96 bytes).
