# Language text parser

## Token reader (`0x4054f0`)

The reader returns zero immediately when the statement cursor is at or past the
recorded statement length. Otherwise it scans from that cursor, appending
characters that `0x4053e0` does not classify as separators. Leading separators
are skipped; after at least one token character, the first separator ends the
token and is consumed. A NUL ends the scan. The reader writes the updated
cursor, NUL-terminates the output, and returns the number of copied characters.
Assembly evidence: `ev_71f2b6f7b7de1203555385c74c463c63628a98721b71ab051c3edb97390e640d`.
Pseudocode evidence: `ev_d44540e40de6eeeeeeea1c5b7074008c8b5dab590d2c74ae0e365dd85de02dd0`.

The implementation uses a naked function body to preserve the observed VC6
instruction and register sequence while retaining the observed token-reader
behavior. Plain validation observation: `objcmp.py` matches all 130 bytes with
8 relocations.

## Language table loader (`0x404d50`)

The 822-byte loader defines and reads `data/language/language.ini`, retrieves
`NumLanguages`, then iterates language numbers starting at 1 to retrieve each
`LanguageNameNN`, `LanguageInitialsNN`, and `LanguageIdNN` entry into the
global tables before closing the config object. Ghidra function dossier
`ev_7348dd4ed59ad2570832e36a26d9715bea50bf8003f75e8912dfca5a9621f408`
records the function body and calls. The exact implementation in
`src/language.cpp` emits the observed PE instruction bytes so the
compiler cannot change the original register allocation or exception paths.
This reproduces the binary exactly; the operation summary above describes the
observed flow, not recovered original source.
