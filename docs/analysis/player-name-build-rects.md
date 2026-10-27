# Player-name menu hit rectangles

`PlayerNameMenu::BuildRects` (`0x43ee70`) builds the six name-row hit boxes,
Continue and Back boxes, and the two arrow boxes for each active row. The
original body is 1,415 bytes, from `0x43ee70` through `0x43f3f6`, with handler
`0x4d8b8f`. REA evidence: `ev_da7ed8b81af567d9496d671ab8bdb1d8721ec075f7a0c69f6cf4463f594c899a`.

Reusing the first loop's `i` for the arrow loop reproduces the target stack
temporary allocation and instruction schedule. This is a compiler observation,
not a claim about the original source. The focused matcher reports all 14
exact-marked functions in `src/unk_43d770.cpp` passing, with two remaining
`NONMATCHING` entries in that file.

An independent COFF/PE audit confirms 31 absolute relocations at the target's
31 base-relocation sites, 32 direct calls to the corresponding target
functions, and three `__except_list` references. The object has 66 relocations
within the 1,415-byte body. These are local artifact observations.

The target handler at `0x4d8b8f` loads FuncInfo `0x4e89c8` and transfers to
`__CxxFrameHandler`; byte evidence:
`ev_d92b8cf529fff0bdaf49dcd1ccb91e338be56abf6df7452f81aa8a3bd205ca47`.
The FuncInfo has magic `0x19930520`, max state 12, unwind map `0x4e89e8`, and
no try or IP maps; evidence:
`ev_e952070f607395277b0971d96810e046b531f2fd5f6723dcce3858e185979e1a`.
The generated FuncInfo has the same shape and twelve unwind states
`[-1, 0, 1, 2, 1, 4, 1, 6, 1, 8, 1, 10]`. All cleanup action offsets match the
target, including point destruction and allocation-failure deletes. These are
local object and PE observations.

The relocation-copy helper's external-symbol table does not currently resolve
the generated `__CxxFrameHandler` reference for this function, so a patched
runtime copy was not verified. The matcher and independent relocation/EH audit
are the available checks.
