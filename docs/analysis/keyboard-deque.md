# Keyboard deque template instances (1.01)

The observations below come from local byte comparison and disassembly, not
new REA analysis. Reference executable: a locally prepared copy with a resolved import table. Matching uses the official
`orig/RallyTrophy.exe` through the repository's existing matcher.

| Original address | Size | VC6 STL instance |
|---|---:|---|
| `FUN_004b73c0` | 48 bytes | `std::deque<unsigned char>` allocator constructor |
| `FUN_004b73f0` | 107 bytes | `std::deque<unsigned char>::_Growmap` |
| `FUN_004b7490` | 139 bytes | `std::deque<unsigned char>` destructor |

Plain observations: `template class std::deque<unsigned char>;` in
`src/input_deque.cpp` emits all three with byte-exact code under the
configured VC6 SP5 compiler. Exact COFF symbols in that source select each
instance for comparison. No hand-written implementation of STL internals is
needed. The existing keyboard character queue remains defined in `input.cpp`.

To reproduce the focused validation, run from the repository root:

```
python3 -I tools/match.py --quiet src/input_deque.cpp
```

Plain assembly observations from `tools/scratch_asm.py`: the constructor
copies the allocator byte and clears eleven following DWORD fields; `_Growmap`
allocates a pointer array, copies the active block pointers, and frees the old
array; the destructor releases character blocks and then the pointer array.
These describe generated library code, not recovered original source.

The global queue's initialization wrappers and input callback mismatches are
outside this change. No linked executable or runtime input test was performed.
