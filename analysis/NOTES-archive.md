# NOTES archive: per-loop and per-iteration history

Moved verbatim from `analysis/NOTES.md` (everything from "Loop 13" to the end). The conventions that still apply are distilled into NOTES.md's "Decomp conventions learned so far", with pointers to the sections here.

### Loop 13: declaration order versus initialization order

- Local compilation observation: in setup dialog `FUN_004a2440`, retaining
  `bool found;` before the devices reference, then assigning `found = false`
  after that reference, produces an exact 3099-byte match. Moving the declaration
  itself changes stack allocation. Separate declaration from assignment before
  trying broader type or control-flow changes for store-order mismatches.
- Original assembly stores the devices pointer at `0x4a2480` and clears the
  flag at `0x4a2484`. REA Evidence:
  `ev_3de128c55c8795402d1db99e9d74f0e0f21c8a436af851fa2a12481fe81a84f5`.
- Word-reader structure corroborated by REA Evidence
  `ev_d44540e40de6eeeeeeea1c5b7074008c8b5dab590d2c74ae0e365dd85de02dd0`.
  Local experiments using a pointer in the loop condition or recomputing it
  from the position produced 120 bytes rather than the original 130; no match.
- REA decompilation of `0x408170` crosses into the following accessor after
  the throw; do not infer function boundaries from that pseudocode alone.
  Evidence: `ev_51a48f8b9f8c7908d55d0b4b0a28d2f83daaa84628d2495113f9e7e2b636991c`.
  The existing inventory and local assembly give 532 bytes for this function.

### Loop 14: configuration definitions and storage

- `Config::Define` (`0x4076c0`) takes a table and signed integer count,
  calls Close, creates one typed entry per row, copies its name, clears its
  assigned flag, and appends it. Invalid tags throw runtime_error with
  `illegal storage type`. Count <= 0 only closes the previous definitions.
  REA Evidence: `ev_7d00785b9c8cfbaeb53491369a1b4a259a97601aa43a885caaf0fc30ec321fe0`.
  Ghidra incorrectly continues into following code after the throw; only the
  original function range and switch table are reconstructed here.
- The common entry header is 0x1c bytes: vptr +0, std::string name +4,
  type +0x14, assigned flag +0x18. The constructor initializes the empty name
  and assigned flag; it leaves type to the derived constructor. Evidence:
  `ev_fc58925276465295a7d264685ca82d73dc3cb7aacba3a26f361809604d198b73`.
- Define's allocations are 0x2c/0x20/0x20/0x24/0x28/0x2c bytes for tags
  0..5. Tags 0..2 are string, int and float (string/int also corroborated by
  existing accessors). The float parser writes one float at +0x1c and names
  it in its missing-value error (Evidence:
  `ev_eb66e614c479a1a52de0f2dcbe030b4f3b00004c38fc25bf21a3eac135edf3a5`). Tags 3..5 are Vector2/3/4: their parsers explicitly
  name these types in errors and store floats starting at +0x1c, four bytes
  apart. Evidence: `ev_1b8de9816de66a87600982c8c3b4eace797a413d348313a05eaa0c3c9ce066eb`,
  `ev_ce5b73b10ac49c25fca869de81499912b431ad7377a12c2dbbfd6c256b43dcd3`,
  `ev_dcb84a4f2a7ea3480e1c089e4fd044786d7ad1aef2c1fc4e504f377733ca4e86`.
- Config+0x10 is a borrowed filename pointer, assigned by Load and used
  for file_error messages, not a separator character. Load dispatches each
  entry by type and sets +0x18 after its parser returns. Evidence:
  `ev_6ba1892aeb984904d064d5579ea51db5e2ed10833463c40b9b826aad40e19c61`.
- Local compilation observations: placing `m_assigned = 0` in the base
  constructor body matches the vptr-store order; putting it in the member
  initializer list does not. The inline base constructor is defined before
  Define so VC6 can inline it. An inline SetType helper in the typed entry
  constructor changes Define from 772 to the exact 760 bytes. This is a
  matching source reconstruction, not proof of the original helper's name.
- GetInt experiments: operator== reduces the build to 532 bytes, but caches
  an entry pointer differently from the original 519-byte function. Reversed
  equality, inequality wrappers, reference/pointer locals, accessors, iterator
  variants, signed indices and declaration reordering did not match. Keep
  the existing implementation until a better supported source form is found.

### Loop 15: typed configuration accessors

- Local compilation observation: all six accessor functions use the same
  successful reconstruction: an unsigned index, `m_entries.size()` loop bound,
  and direct `m_entries[i]` expressions for the name, type and result. A cached
  begin pointer changes the optimizer's inlining and register allocation even
  when it expresses the same lookup. GetInt now matches 519 bytes without
  changing compiler flags or forcing a helper out of line. CompareName and
  NameEquals wrappers did not solve this; none were retained.
- All six accessors require matching name and type; they continue searching
  past a same-named entry of another type and throw runtime_error with the
  existing missing-variable message if no entry qualifies. The assigned flag
  is not tested. Original GetInt assembly Evidence:
  `ev_6cefac2986d5bb5cc394eea905bc3a0a8c88900990ff020fa15b9c89a61f9f3c`.
- GetFloat (`0x407f60`, tag 2) returns the float at entry+0x1c. Evidence:
  `ev_2a6280ea8303f355f3b6e58275c0d01451160cfeb2f6bbb7c6fe4c7a5e20093f`.
  Ghidra crosses into subsequent accessors after the throw; use the inventory
  and assembly boundaries rather than the full pseudocode body for porting.
- GetString (`0x408170`, tag 0) returns the stored string's c_str(), including
  the standard empty-string fallback for an empty buffer. Assembly Evidence:
  `ev_df7e62fa5636faba1be2ca1c7ef47feaa313cc4b6b32a51f760d2bcac173fcae`.
- GetVector2/3/4 (`0x408390`/`0x4085e0`/`0x408820`, tags 3/4/5) return the
  address of the payload at entry+0x1c. This is represented as a const reference
  in C++; reference-versus-pointer and constness are source-level inferences.
  Each original registers an empty local-static destructor on first use,
  despite throwing on a failed lookup. Empty vector destructors plus an unused
  local static reproduce this guard/atexit behavior exactly; no failure path
  was changed to return a default vector. Evidence:
  `ev_2272b6891f333a66e808265fd44652b320200287e37255bf1ac465f4fd6b8af0`,
  `ev_2995e84f5a28d35f5a572bbbdca280c97b8309f1b8f315bee688c9eebd1624eb`,
  `ev_bfdad86661b390ca5d54ae3377c806c16f91a6c451d039d3545db59e53d46727`.
  Plain objdump observation: the generated destructor thunk at 0x4085d0 is ret.
- Exact accessor sizes are 519, 519, 532, 561, 559 and 559 bytes, respectively
  (local match.py observations). The vector destructor declarations preserve
  the previously matched Define and entry constructor as well.

### Loop 16: value parsers and file errors

- file_error inherits std::runtime_error. Its own message string is at +0x1c,
  filename string at +0x2c, total size 0x3c. Its implicit copy constructor
  copies the runtime_error state followed by those two strings. Evidence:
  `ev_1ed71632634e9389779d9f017fd3a7870b45f9982e06954eab6ae5f64fdb8efa`.
- The reconstructed inline constructor takes filename then message by const
  string reference. It builds `File error: ` + message + space + filename,
  saves message then filename, and calls the empty release diagnostic with
  format `file error: %s %s`, filename then message. Local compilation:
  this matches the complete exception sequences in all five scalar/vector
  parsers without changing WinMain. This is a source-level reconstruction;
  argument names are descriptive. Parser Evidence:
  `ev_4e254d7e382494b152c5d2ab79da3ed88b3835537221891d4033602e100328ee`.
  Plain PE string observations: literals at 0x4f2e44, 0x4f2e40, 0x4f2e2c.
- ParseInt/ParseFloat read one word into a 256-byte local buffer; an absent
  token throws file_error with `missing integer value`/`missing float value`.
  The implementation uses CRT atoi/atof, with the float conversion stored
  into the payload at +0x1c. No additional numeric-validation check occurs.
  Evidence: integer record above and
  `ev_eb66e614c479a1a52de0f2dcbe030b4f3b00004c38fc25bf21a3eac135edf3a5`
  (retained in loop 14's bundle).
- Vector2/3/4 parsers read and store each float component before reading the
  next. Preserve partial writes if a later component is missing; errors name
  the vector type. Unrolled reads/assignments match all three original bodies.
  Evidence: `ev_1b8de9816de66a87600982c8c3b4eace797a413d348313a05eaa0c3c9ce066eb`,
  `ev_ce5b73b10ac49c25fca869de81499912b431ad7377a12c2dbbfd6c256b43dcd3`,
  `ev_dcb84a4f2a7ea3480e1c089e4fd044786d7ad1aef2c1fc4e504f377733ca4e86`
  (loop 14). Load, rather than these parsers, sets the assigned flag.
- ParseString scans the remaining statement for a quote, clears the stored
  value, then appends characters until the next quote. It has no escape
  handling and does not update the shared statement position. An exhausted
  position or missing closing quote throws `missing string`; the existing
  value is preserved for the former and cleared/partially replaced for the
  latter. Evidence: `ev_8bc1321e11b7e52951e48b294d457ebf400f80e05b732471a615ebdbc46180d2`
  (loop 13). NONMATCHING: direct STL calls give 899 vs 1069 bytes; extra inline
  clearing/appending wrappers give 1103 bytes but incorrect register/temporary
  layout and were not retained.
- Validate returns 1 for an empty definition list or when every assigned
  flag is nonzero. At the first unassigned entry it builds a missing-variable
  message containing that entry's name, then throws file_error.
  Evidence: `ev_0eb4b19f842b0d2d6229e308b5ace8685af2352f567fc23fcd2e122f4211cefb`.
  Local compilation: default-constructing/assigning both message and filename
  gives the correct 707 bytes, 71 bytes different in filename-load/store and
  exception-state placement. Direct filename construction gives 671 bytes.
- Exact parser sizes from local match.py: 487, 487, 1078, 1092, 1279. These
  routines use the existing text-word reader; there is no tokenization rewrite.

### Loop 17: loading definitions and file existence

- Config::Load (`0x406ce0`) has filename, boolean requireAll, boolean
  verifyChecksum, and unsigned 32-bit expectedChecksum arguments. It checks
  existence, constructs File(filename, 0x81), reads the exact file size into
  the shared buffer, then parses semicolon-terminated statements. The word
  scratch buffer is 0x400 bytes (inferred from original stack offsets).
  Evidence: `ev_6ba1892aeb984904d064d5579ea51db5e2ed10833463c40b9b826aad40e19c61`
  and assembly `ev_6112eef74c3acfad01be5523201617523229e6d6e72ff871b175576b3d548626`.
  Ghidra loses stack tracking across the large stack probe; use assembly for
  the signature, scratch size and return-path details.
- Optional checksum verification uses the 16-bit table at 0x4f2c10 with seed
  0xdead. Each input byte indexes the table using the old high byte XOR input;
  the result is XORed with the old low byte shifted left eight. The final
  16-bit value is compared with the full 32-bit argument. A mismatch throws
  file_error if DAT_00501454 is nonzero; otherwise it sets byte DAT_00501473
  to 1 and continues. Load does not clear that flag. Evidence: assembly above.
- Each statement requires a name found in the definitions, then an '=' word,
  then a value parsed according to tags 0..5. A successful parser sets the
  entry's assigned flag. An unexpected tag bypasses parsing but still sets
  the flag; no extra default error was introduced. Repeated Loads preserve
  assignment flags from earlier loads. requireAll runs Validate after freeing
  the input buffer. Evidence: the Load records above.
- Original lifetime behavior is retained: raw input storage is freed only on
  normal completion, is not freed on checksum/parse exceptions, and its global
  pointer is not nulled after free. The File object still unwinds normally.
  Missing-file failure throws runtime_error with the literal
  `storage file not found` concatenated directly with the filename. Checksum,
  missing-name, unknown-name and missing-'=' failures use file_error; exact
  newline-bearing literals are kept. Evidence: Load assembly; plain PE string
  observations at 0x4f2f30/0x4f2f20/0x4f2f04/0x4f2ef0/0x4f2edc.
- Local compilation: the first Load build was 2520 vs 2516 bytes. Declaring
  the unknown-variable message empty and assigning its initial prefix in a
  separate statement reproduces the original string/EH inlining and matches
  the whole function, including its 24-byte switch table. No flags or toolchain
  changes were required. The checksum and normal parse path already matched.
- File::Exists (`0x4a6050`) returns true for any successful CRT _stat; on
  failure it calls `0x4a5920`, whose return is an integer boolean. This fallback
  rewrites the last directory component into a .bzf path, locates/caches the
  archive and searches the entry name. Its effects are retained by calling it;
  this iteration does not reconstruct its body. Evidence:
  `ev_248dc0859ca7be2407d30856b2de16131ea0f84ac64c7541f5683ce85f375e9e`,
  `ev_cf7dff3e8134f587d6fa12c3ceadb85c1b39b90ff222b6fa02a6001226d45977`.
- Local compilation: an initialized bool accumulated through the _stat/lookup
  if/else-if branches matches Exists's 60 bytes. Immediate-return variants
  produce 52/53-byte bodies, and assigning only in the fallback gives a
  same-sized body with seven different bytes.

### Sound system T_56b0b0 (Claude, sound iteration 1)

- Module `0x4982c0`–`0x49b630`, `src/sound.cpp` + `include/sound.h`. A Miles
  Sound System wrapper: `AIL_*` imports (and Bink's `BinkSetSoundSystem` /
  `BinkOpenMiles`) are declared in `sound.h` as `extern "C" __declspec(dllimport)
  __stdcall`; no SDK headers. REA pseudocode Evidence:
  `ev_ef99599ff5a2718ca6e056cd2dbefd8a810543fc8ff096cd34ff1bafec6c7073`
  (ctor, dtor, Startup, Shutdown, Open, deque destructor),
  `ev_09d2234a06aef1cc35597fdccbae656b0380fe532f68afb6dc93d1d6b7e23d4e`
  (AllocateVoices, ReleaseVoices, LoadSample, ReleaseSample, OnSampleEnd, Play),
  `ev_2e72baca55e6be721f27c0d69e6c3d2ac0b46c38c181f77f1e448e4b8f1bdc47`
  (Stop through the speech queue and the out-of-line ~List at `0x49ad30`).
- Layout (0x2ac bytes): `std::vector<DeviceInfo>` +0 (name, HPROVIDER;
  [0] is "No Sound"), default provider index +0x10, open provider +0x14, 3D
  voice count +0x18, listener matrix +0x1c, its inverse +0x5c, two Vector3
  +0x9c/+0xa8, open() parameters +0xb4..+0xc4, digital driver +0xc8, paused
  +0xcc, disabled +0xcd, 2D/3D `std::vector<Voice>` +0xd0/+0xe0 (0x14-byte
  voices; voice 0 plays speech), lists of samples +0xf0, 3D sources +0x104,
  2D sources +0x118, per-update audible sources +0x12c, stream +0x140, stream
  volume +0x144, 0x44 speech samples +0x148, speech priority +0x258,
  `std::deque<SpeechItem>` +0x25c (8-byte items, 4 KB blocks), master volume
  +0x28c/+0x290, CRITICAL_SECTION +0x294.
- EH states confirm member order: the destructor's unwind states show four
  members with empty destructors between `m_devices` (state 0) and the 2D voice
  vector (state 5): the two matrices and two vectors. Giving Matrix/Vector3 an
  empty destructor reproduces this.
- **A second list template.** The sound lists' destructors call the
  out-of-line RemoveNode (`0x414490`) in a loop, and AddTail is inlined with a
  zeroing node constructor (`new` node: prev, next, data = 0), while `List<T>`
  in rallytrophy.h frees with inline RemoveHead code (T_56b980, `0x49ad30`).
  Making the shared List match both was not possible: making RemoveNode
  inline breaks T_56b980's destructor (its explicit RemoveNode calls get
  inlined). `ObjectList<T>` in sound.h reproduces the sound pattern exactly.
  Plain byte-scan observation: the other `0x414490` callers at `0x40f1bd`–
  `0x40f248` and `0x461d78`–`0x461dd2` look like the same three-list destructor
  shape, so ObjectList will likely move to a shared header when those modules
  are done. The out-of-line AddTail copy `0x4aecf0` has the same algorithm.
- Open (`0x4989c0`) matched only after a full permutation search of its six
  member stores (`try_variants` over all 720 orders): the matching order is
  the parameter order, not the order the stores appear in the binary. Treat
  "store order = source order" as a first guess; scheduling can move stores.
- qsort comparator `0x498360`: `fld a; fld b; fcompp` (two loads, no memory
  operand) comes from comparing through an inline getter
  (`SoundSource::GetPriority()`), and the `xor esi,esi` from an `int result =
  0` with `if (a > b) result = -1; else if (a < b) result = 1; return result;`.
- Inlining budget again: `push_back` vs. `insert(end(), 1, x)` and whether
  Voice has a constructor change which STL helpers VC6 inlines. AllocateVoices
  reaches the right size (748) only with a constructor-less Voice zeroed by
  four plain assignments and `push_back` for both vectors (the 2D insert stays
  out of line, the 3D one is inlined, as in the original).
- Speech: `PlaySpeech`-style entries queue `{sample, int* done}`; the next
  item starts on voice 0 when `AIL_sample_status != 4` (playing), at volume
  `DAT_00501440 * 127 / 100`, and sets `*done = 1`. The 2D end-of-sample
  callback for voice 0 starts the next item.
- Data-only functions added to `functions_extra.csv`: EOS callbacks `0x4994e0`
  and `0x499500` (23 bytes each) and the comparator `0x498360` (59 bytes).

### Sound system, iteration 2 (Claude)

- **Inline assembly.** Play3D, Update3D and Update transform source positions
  into listener space with a hand-written x87 block (`fld [ecx]; fmul [edx];
  ...; fstp [eax+8]; fstp [eax+4]`). Tell-tales: an `ebp` frame in an
  otherwise frame-pointer-free function, and pointer locals stored to
  `[ebp-x]` and immediately reloaded into ecx/edx/eax before the FPU code.
  Reproduced with `__asm` (macro `TRANSFORM_POINT` in sound.cpp) reading
  `point`, `matrix` and `result` locals. Functions with `__asm` keep most
  variables in memory, so register-allocation mismatches are common there.
- **Bitfields.** SoundSource+0x5c is a bitfield word (`SoundFlags`: bits 2-4,
  5-7 and 8). The original clears each field with its own load/AND/store,
  and Update3D copies the whole word into a local before testing bits
  0x100/0x8/0x20/0x80. Clearing with `&=` masks merges into one `and mem,imm`.
  VC6 still merged three adjacent bitfield clears at the end of Update3D;
  signed/volatile fields, a pointer and chained assignments did not help.
- **A local initialised before an unrelated test**: `int loops = 1;` placed
  before the `if (!rate)` fallback reproduces the original's early
  `mov [loops], 1` (Play3D, Update3D).
- **Header side effects on other files (fragile).** Adding member functions
  (even declarations only) to `Matrix` in sound.h changes the setup dialog
  `0x4a2440` in winmain.cpp (1973 differing bytes), like the File layout
  issue in loop 18. A derived struct or free functions do not. Always run the
  full `match.py` after header changes, and test header experiments in a
  scratch copy of decomp/ (symlink toolchain and ../orig) so the other agent's
  builds aren't affected. Running match.py while the other agent's full run
  compiles the same file can race on `build/obj/*.obj` (objcmp struct.error);
  rerun.
- Update (`0x499f80`): rebuild m_view = inverse(listener) (position copied
  into row 3, rigid inverse, then Transpose3x3 `0x49b570`), collect audible
  3D sources (state 1, playing or culled) into m_audible, stop sources with
  state 2. With more audible sources than 3D voices it scores each by
  `gain + DAT_00500438[priorityClass & 15]` (gain 1 within nearDistance,
  near/dist beyond, 0 past farDistance), qsorts DAT_00569e74 with the
  comparator, stops the overflow (marking looping ones culled) and
  plays/updates the rest; otherwise it culls by squared far distance. Both
  paths widen minDistance by `distance * rolloff + 0.001` when the source is
  near and its volume isn't 127, flagging changed3D. m_volumeDirty is cleared
  only when not paused.
- LoadSample (`0x498ec0`): reuses a loaded sample by case-insensitive name
  (refcount), else checks File::Exists (throws runtime_error "sample file not
  found " + name), reads the file with File(name, 0x81), and switches on
  AIL_file_type: 1 (WAV) as is, 13 via AIL_decompress_ASI, 2 via WAV_info +
  AIL_decompress_ADPCM (both free the file buffer and mark Miles memory),
  else throws "unknown audio format". Update3D re-transforms but does not use
  the position when the sample changed; kept.

### Marking compiler-generated functions (Claude, sound iteration 3)

- STL template instances and implicit members (vector insert/_Ucopy/_Ufill,
  deque pop_front/_Buyback, deleting destructors, implicit destructors) often
  match as soon as the hand-written code that instantiates them is right. List
  the object's symbols (`objdump -t build/obj/X.obj`), compare candidates with
  `tools/objcmp.py OBJ SYMBOL EXE ADDR SIZE LIMIT`, and mark the matches at the
  end of the source file: `// FUNCTION:` + `// SYMBOL: <COFF name>` + a
  `typedef void Generated_<addr>();` line. The typedef only gives match.py a
  definition line to anchor on and has no effect on code generation.
- A template instance at an address inside a module is not necessarily that
  module's: `0x4982c0` (erase on 0x30-byte elements) and `0x498300` (pointer
  copy) sit before the sound module but are only called from other modules.

### Game controllers T_56b0b4 (Claude, input iteration 1)

- DirectInput 8 action mapping (`EnumDevicesBySemantics`, `BuildActionMap`,
  `SetActionMap`) with the DIACTIONA table at `0x4ffb68` (37 x 0x28 bytes;
  names filled from the text table) and a DIACTIONFORMAT at T_56b0b4+0x28.
  Interfaces are declared by vtable slot in `include/dinput8.h`. REA Evidence:
  `ev_605d524bcf69400cd17534fe76619df09e59f5a66c7937de08e14c9c49e2502a`
  (ctor, settings, dtor, helpers), `ev_5b8e19de8f4f975a29504be98d3cd3dbdf68791c5c89bf35e52f49268bb1f67c`
  (Init, callbacks, Poll, effects).
- input.ini: BufferSize (capped at 32), ForceDigitalSteering, OverrideRanges,
  AxisMin/MaxValue, Analog X/Y/Z/RX/RY/RZ ranges stored as {min, 1/range}
  (ranges with |range| <= 1 become 1), and steering/throttle/brake exponents.
  `(float)fabs(x) <= 1.0f` is needed for a float (DWORD) compare; plain
  `fabs(x) <= 1.0f` compares in double. A `ConfigVector2 range;` declared
  before the first assignment puts the EH state store before the call.
- **Header fragility, again:** the setup dialog `0x4a2440` changes when
  winmain.cpp sees the full T_56b0b4 (and separately when it sees classes
  with virtual methods). The module header is therefore included only by its
  own .cpp, before rallytrophy.h, and rallytrophy.h keeps the opaque
  declaration under `#ifndef UNK_495640_H`. Use the same pattern for other
  WinMain-owned classes.
- `InputDevice& entry = m_devices[i];` (rather than repeated indexing or a
  device-pointer local) matched both ReleaseDevices and Unacquire.

### Game controllers, iteration 2 (Claude)

- An EH frame with no state stores in a trivial constructor came from a
  `resize(0)` in the body (its default-argument temporary), even though the
  call itself folds away on a fresh vector: `ForceEffect() { m_effects.resize(0);
  m_type = 0; }` matches `0x497c20`; `clear()` does not produce the frame.
- `if (x != K) { single } else { loop }` reproduced the original block layout
  where `if (x == K) { loop } else { single }` inverted it (SetForceGain).

### Game controllers, iteration 3 (Claude)

- A jump table where only a few case values have bodies still tells you
  which values share a body: read the table (8 entries at `0x49775c` for
  cases 1..8) instead of trusting the decompiler's case list. In
  ForceEffect::SetParameters case 2 shares the periodic body with
  0x20/0x40/0x80/0x100; an empty `case 2: break;` turns the table into a
  compare chain.
- Small `memset`s of a local struct compile to plain zero stores (4 dwords)
  or `rep stosd` (6+ dwords); both come from `memset(&s, 0, sizeof(s))`.
- InputDevice::CreateEffect returns 0 unless every bit of `type` is in the
  device's supported-effect mask (InputDevice+0). Its `type` is a signed
  int (`jg` in the dispatch).

### Game controllers, iteration 4 (Claude)

- Loops over a NULL-terminated pointer array matched with an index
  (`for (i = 0; devices[i]; i++)`) rather than a walking pointer, even though
  the original's code walks pointers (EnumerateDevices `0x4966d0`).
- Old iostream.h: `cerr << "..." << endl;` produces calls to
  `ostream::operator<<(const char*)`, `operator<<(char)` ('\n') and the
  out-of-line `operator<<(ostream& (*)(ostream&))` with `flush`; `endl`
  itself is inlined. The manipulator operator (`0x496fa0`) and `flush`
  (`0x4970e0`) are COMDAT instances emitted into this module.
- A struct declared at function scope instead of inside the loop body
  changed the store/vtable-load order around a COM call (CreateEffects).
- EnumerateDevices stores the address of a local DIDEVICEINSTANCE in each
  InputDevice (`instance`, +0x28): a dangling pointer once it returns. Kept.

### Game controllers, iteration 5 (Claude)

- `0x495640` is a one-byte empty function in this module that GetAxisRange
  calls with the axis GUID (a release-build trace). It stays separate from
  the shared empty logger `0x402c90`, which GetAxisRange also calls with
  "input: get objectinfo failed".
- GetAxisRange returns true only when the device's own DIPROP_RANGE was
  used; with the input.ini fallback it fills min/scale but returns false.

### Game controllers, iteration 6 (Claude)

- Poll's action ids (DIACTIONA::uAppData): 1 steering axis, 2 throttle axis,
  3 brake axis, 11 combined pedal axis (negative = brake), 4-8 and 15 button
  states passed through, 9/10 a -1/+1 step (gear?), 12-14 and 16-18 edge
  actions called only while pressed. Controls methods are at `0x458950`-
  `0x458ac0` (another module; names are guesses for the axes only).
- The player index passed to Poll is 1-based (`m_players[player - 1]`); a
  `[reg - 0x10]` operand into a 0x14-byte array gave it away.
- The original tests a float's sign with `test dword ptr [x], 0x80000000`
  rather than an FPU compare; `value < 0.0f` produces `fcomp`.

### Game controllers, iteration 7 (Claude)

- `tools/scratch_asm.py` fails to assemble functions that call the
  statically linked `DirectInput8Create` thunk (`0x4a4b06`); a plain
  `objdump -b binary` of the range works for reading the code.
- Init builds the player names as "<text 0x94> <n>" into one buffer sized
  strlen(text) + 3 per player and walks it with m_cursor; with more than one
  player the names are not NUL-separated in the copied data. Original
  behaviour, kept.

### Game controllers, iteration 8 (Claude)

- Calls through renderer-plugin vtable slots that rallytrophy.h's
  RenderDevice declares without parameters can be written against a
  module-local class that only spells out the needed slots (filler virtual
  methods via a macro) and a cast of `DAT_0056b0ac`. This avoids touching the
  shared header, which other files' code generation is sensitive to.
- ConfigureDevices deletes the "config" image right after the UI closes but
  leaves m_configImage (+0x184) pointing at it; DrawConfigImage only runs
  from the UI callback, so the dangling pointer is never used. Kept.

### Networking T_56b0b8 (Claude, net iteration 1)

- T_56b0b8 is the DirectPlay 8 object: CoCreateInstance of a peer
  (CLSID `0x4e2810`, IID `0x4e2800`) and a lobbied application (`0x4e27e0`,
  `0x4e27d0`), "Default player"/"Default Session" wide names, an 8-entry
  command table at `0x4fe7a0` ("connect", ...). Evidence:
  `ev_a50767b23f3954212da02ebc8cf2faf97f103ca116f5a0f2c16a0150d5d25a19`.
- Two dword pairs copied in the constructor from `0x501314`/`0x501320` (the
  dword at `0x50131c` is skipped) compile like two 8-byte (double) copies;
  copying four separate dwords or a GUID gives a different store order.
- A module boundary shows up as a run of tiny functions that set float
  globals and register `atexit` destructors: those are the next file's
  static initialisers (`0x4856f0` onwards here).
- DirectPlay 8 message structs (DPN_APPLICATION_DESC, DPL_CONNECTION_SETTINGS,
  DPN_BUFFER_DESC) are declared locally in `include/unk_4843e0.h`; the handlers
  `switch` on DPN_MSGID values (`0xffff0000 + n`), which VC6 compiles to a byte
  index table plus a jump table. Case bodies appear in source order.
- A `memset(&desc, 0, sizeof desc)` of a 0x48-byte struct is a `rep stos` of
  0x12 dwords; `= {0}` would store the first dword separately.
- `GLOBAL = m_member = value;` stores the member first, then the global.
- Loop increments matter: `for (...; info++, i++)` and `for (...; i++, info++)`
  schedule the pointer and counter updates differently (`0x4844b0`).
- VC6 turns `if (hr == C) return 0; return hr;` (after a call) into
  `sete`/`dec`/`and`; the original sometimes keeps a branch (`0x485020`,
  unresolved).
- Vtable-only functions (no direct calls) are often missing from
  `functions.csv`: dump the class vtable and add each slot target to
  `functions_extra.csv` with its size (`0x481700` etc.).
- Static initialisers sit at the end of their module (`0x4843b0`/`0x4843d0`
  close the network race module), not at the start of the next one.
- The same small function can inline `vector::insert` in a short test file
  and call it out of line in the original; write the module's earlier
  functions before giving up on such a difference (`0x4828e0`).
- qsort comparators in the network race module use only `<`:
  `if (b < a) return 1; if (a < b) return -1;` (or the same with a
  `result` variable). Writing `a > b` swaps the compare operands.
- An EH frame around a function with only POD-looking locals can come
  from a small class with an empty user destructor (ARGB `Color` in
  `0x483f60`); the unwind entry calls the folded empty function `0x402c90`.
- Arrays of a class with empty out-of-line ctor/dtor (Vec2 in `0x484190`)
  are built with the EH vector constructor iterator (`0x4c11f4`), ctor
  folded to `0x490ad0`.
- A float copied through the FPU (`fld`/`fstp`) instead of an integer
  register usually passed through an inline function's float parameter:
  an inline constructor of a message struct (`MsgStage` in `0x4839a0`)
  or an inline setter (`TextRenderer::SetScale` in `0x483f60`).
- Locals of an inlined function get their own stack slots; if the
  original shares a buffer/handle slot between two sends, declare those
  at function scope and inline only the part that needs it.
- VC6 overlaps the stack slots of locals in sibling `if`/`else` blocks. When
  the original gives each branch separate slots (frame larger than the
  sum of one branch), declare the locals at function scope (`0x4825d0`).
- A `for` loop copying `count` pointers compiles to `rep movsd` behind a
  `count > 0` guard; `memcpy` adds the `and ecx,3` tail instead.
- Out-of-line STL calls (`vector::size` 0x414460, `erase` 0x484380, `_Destroy`
  0x403130) inside otherwise-inlined code: derive a wrapper (`PlayerVector`)
  whose `Size()`/`Erase()`/`Destroy()` are defined in the .cpp between
  `#pragma auto_inline(off)` / `(on)` (members defined in the class body stay
  inline regardless), and an inline `Resize(n, x = T())` written as
  `if (n && Size() < n) insert(...); else if (n < Size()) Erase(...);`.
  `#pragma inline_depth` does not change these decisions.
- A call argument computed by another call (`inet_addr(ansi)`) is evaluated
  after the other arguments; if the original calls it first, use a named temp.
- `delete p` emits its own null check even right after `if (p)`, which
  doubles the compare (`0x486590`).
- In player loops, repeated `m_players[i]->...` (a dead `lea` of the
  element and a reload after calls) compiles differently from a cached
  `Player* p = m_players[i]`; check which form the original uses per function.
- `time >= 0 ? '+' : '-'` gives setl/dec/and/add; `time < 0 ? '-' : '+'`
  gives setge.
- An inline `Vec3` copy constructor plus `operator-=` reproduces the
  `fld x`, integer copies of y/z and `fsub` sequence; an empty `~Vec3`
  gives the EH frame (unwind entry = the folded empty function `0x402c90`).
- A by-value parameter of a class with a destructor gets its own EH state in
  the callee (unwind action `lea ecx,[ebp+N]; jmp 0x402c90`). This explains a
  state-0 store before any local is constructed (`0x48e840`: by-value `Color`).
  Read FuncInfo/unwind maps to see which locals and parameters are unwound.
- Open: a -1 colour kept in a callee-saved register with a 4-byte larger
  frame (`0x484190`, `0x48e4c0`, `0x48eb20`) is not yet reproduced.
- File-scope objects with an inline ctor and empty dtor (ScreenPoint statics,
  `0x4856e0`-`0x485960`) compile to four COMDATs each: wrapper `_$E(n+3)`,
  init `_$En`, atexit registration `_$E(n+2)`, empty dtor thunk `_$E(n+1)` at
  base, +0x10, +0x30, +0x40. Mark them with `// SYMBOL: _$E<n>`.
- An EH state store disappears when the callee's body is visible and cannot
  throw; leave such callees declared but undefined in the TU.
- `match.py` masks relocated float constants, so a wrong literal can still
  MATCH: read constants from .rdata.
- Inline `__asm` (even an empty `__asm { }`) gives the function an ebp frame.
- VC6 emits `x*x + y*y + z*z` in z, y, x order.
- A by-value class argument with ctor + copy ctor + empty dtor (`ColorArg`,
  `HudPoint`) is built in place in the pushed slot with a hidden pointer slot
  and no EH state when the callee's body is visible and cannot throw
  (`TextRenderer::SetColor` defined under auto_inline(off)).
- Function-local statics of a class with empty ctor/dtor compile to a guard
  bit, `atexit(_$E<n>)` and a 1-byte thunk placed after the function; the
  fields are assigned on every call.
- `GetString(c ? a : b)` compiles branchless (neg/sbb/and/add); the original
  often uses an explicit if/else.
- Merging parallel subagent drafts: three-way merge (`git merge-file`) against
  their common base commit; when a conflict splits a function, take the
  current file and append the new functions as one block instead.

### Derived race modes 0x48f130 (Claude)

- A counted loop `for (done = 0; done < dt; done += 4)` whose body derives `step = dt - done` is turned into a
  trip-count loop by VC6 (`lea/shr`); the original `done += step` form (with `for (...;)`) keeps `done` and `step` in
  memory/ebp and matches (`0x48f2e0`, `0x48f760`).
- Argument expressions that are plain loads from nested pointers (`car->body->x`) get re-loaded per argument;
  named `CarBody* own` / `other` locals declared in that order reproduce the original register choice (`0x48f500`).
- `DAT_x[DAT_count++] = p` inside a loop keeps the `DAT_count = 0` store and the counter in a register with a
  write-back before the next call; a local `count` + `out` pointer makes VC6 drop the zero store (`0x48fb10`).
- A call through a temp: `RecordTable* t = (RecordTable*)(base + off); e = t->f(...)` is evaluated differently from
  the same expression inline (the object pointer is computed last inline).
- `DrawSplitDelta` (0x4884e0) is really a member (callers set `ecx = this`); `RaceMode::SplitDelta` is a header-only
  second declaration of it for callers whose `ecx` is not already `this`.
- Recurring unresolved: constant stores at the end of a function with `pop edi` held back by two stores, and
  `mov eax,1` placement after `xor ebp,ebp` (NetRace and ReplayRace/GhostRace Slot4/Reset).
- The guard-byte pairs at `0x48f640/0x48f660` and `0x48fdf0/0x48fe10` (bit 0 of 0x5826d5, `atexit(0x402c90)`) look like
  empty-destructor registrations of other statics; they are not written.
- `push_back` inside an inline member (`AddListener(p)`) keeps `vector::insert`
  out of line with a stack copy of the argument where a direct push_back
  inlines it (`0x480830`).
- A substep loop written `for (done = 0; done < dt;) { step = min(dt - done, 4);
  ...; done += step; }` keeps the original shape; `done += 4` in the header
  becomes a trip-count loop.
- Module-end stubs (guard bit 0 of `0x5826d5`, then `atexit(0x402c90)`, 25 + 12
  bytes) are VC6's `_$E<n>` static-initialiser pair for files that include
  the STL headers. Mark both halves with `// SYMBOL: _$E<n>` (usually
  `_$E23`/`_$E22`; look the numbers up with `objdump -t`). Each pair also
  marks a file boundary: one pair per .cpp.
- `v.resize(0)` (not `clear()`) is the original idiom for emptying a vector
  in AiPath: it gives the EH frame, the zero default-argument temporary and
  the out-of-line erase/copy/_Destroy calls; `clear()` inlines the loop.
- To get a call to a shared empty destructor (e.g. `delete node` calling
  0x48e660), declare the dtor out of line and leave it undefined in the
  TU; an inline `{}` dtor is elided.
- Load/or/store on a flags word (`mov eax,[m]; or eax,N; mov [m],eax`)
  instead of `or [m],N`: write `*(volatile unsigned*)&flags |= N;`.
- `a = b = c = 0` stores c, b, a (gives reversed z, y, x stores).
- A ctor that only calls an out-of-line `Reset()` is `T() { Reset(); }`.
- Repeatedly assigned Vec3 temporaries: declare named locals with an empty
  default ctor and assign (`edge = *a - *b;`); `Vec3 v(*a - *b)` per use
  copy-constructs fresh locals and grows the frame.
- `&arr[i]` plus a separate `arr[i].field` read keeps the induction
  pointer at the element base; reading only through the pointer biases
  it to the last-read field (0x49f400).
- Condition chains over a member state (`s == 1 && A || s == 2 && B`)
  must read the member, not a local copy, or VC6 merges the tails.
- VC6 spills FPU temporaries into dead parameter slots.
- Member objects with empty inline ctor/dtor still get EH unwind states in
  the owner's ctor/dtor; the unwind table lists their exact offsets, which
  recovers class layouts (AIDriver's 11 Vec3 members, Obstacle's).
- `int n = x - 1; ...; x = n;` gives load/dec/store; `x--` gives `dec [m]`.
- `std::string s("lit"); s += buf; s += ".ini";` gives the inline append
  with out-of-line `_Grow`/`_Eos`; default ctor + `s = "lit"` inlines `_Eos`.
- A `Vec3` local with an empty dtor gets an EH state even if it is only
  field-assigned; `Vec3 d = expr` built in place gets none. Dead Vec3
  locals can be needed to reproduce the original's EH state numbering.
- VC6 folds a known `v.y = 0` into later `v.y*v.y` terms; the original
  sometimes keeps them (no source form found yet that stops the fold).
- A .cpp whose only code is the STL `_$E23`/`_$E22` pair (includes, no
  functions) reproduces the stub-only modules, e.g. `src/unk_4a16a0.cpp`.
- A ternary over two float members compiles to fld/fstp; casting both to
  int (`*(int*)&a`) gives the original's integer-register selection.
- File-scope `static const float` objects are not folded by VC6: use them
  where the original loads a constant from memory, e.g. the guarded
  function statics `static float k = 1.0f / (HI - LO);` (guard byte,
  fsub/fdivr) or `rand() * kInv32767` kept as two multiplies. Literals fold.
- A float member stored with `mov reg,[.rdata]; mov [m],reg` comes from an
  extern/global `const float`, not a literal (a literal is an immediate).
- Non-negative float compares done with integer `cmp` in the original:
  write `*(const int*)&a < *(const int*)&b`.
- `_$E<n>` numbers depend on the whole TU (function-level static guards
  use up numbers): re-read them with match.py's symbol list after merging.
- Per-array guard bits in one shared byte (e.g. 0x561eb8) come from static
  data members of a class (`Vec2 C::a[N];`), not file-scope arrays; each
  gives four `_$E` pieces (wrapper, vector ctor, atexit, guarded dtor).
  Such declarations use up `_$E`/`_$S` numbers in every including file, so
  keep them in a header only their module includes.
- Functions with inline asm keep variables in memory with register caching:
  expect `mov [ebp-N],reg` mirror stores after assignments.
- Sprite-row tables with a folded bias (`[ecx*4+0x4e2164]` = `T[row-0x11]`):
  an `extern const float T[]` re-loads per use; a `static const` array is
  CSE'd. A {cos, sin} table is a struct array, not a flat float[2048].
- Writing a Vec3 member through a pointer (`Vec3* pv = &p->velocity;
  pv->x = ...`) gives the shared base register; struct or per-field
  member stores address differently.
- Casting a vtable-returned matrix to a struct pointer
  (`(const ViewMat*)device->GetViewMatrix()`) changed register use enough
  to reach the original size; a plain `const float*` local did not.
- `(float)cos(a)` / `(float)sin(a)` compile to inline fcos/fsin here; an
  inline helper wrapping `__asm fcos` crashed the compiler (C1001).
- A constant used in a compare before calls and in stores after them is
  hoisted into a callee-saved register and can steal it from a loop count.
- `/FAs` listings show each local's frame slot (`_name$ = -N`): a quick
  way to compare stack layouts with the original.
- `far` is a keyword in VC6.
- A file-static object whose class has an out-of-line ctor/dtor (declared,
  not defined in the TU) gives four `_$E` pieces: wrapper, ctor thunk
  (`mov ecx,obj; jmp ctor`), atexit registrar, dtor thunk.
- `static float a = 1.0f / k;` with `static const float k` gives a `jmp`
  wrapper plus an fld/fdiv/fstp body; wrappers follow source order.
- A user-declared copy ctor on a small POD (Vec2) enables NRV in an
  out-of-line operator (the dead `mov [esp],0` flag store).
- Eleven consecutive dword field copies give the eax/ecx/edx load-ahead
  pattern; a 44-byte struct assignment gives rep movsd.
- Members with an empty dtor (a Vec3, or a class derived from std::vector)
  add unwind states to the owner's dtor; use a POD struct or the plain
  vector (with a cast helper for out-of-line methods) when the original's
  state numbers are lower.
- VC6 bug: in `for (i...) { int b = i * 4; *idx++ = b; ... }` with a zero
  register live, `b` can be folded to 0. `i << 2` compiles correctly.
- A function that calls functions defined later in the file keeps its EH
  frame; callees defined earlier can be proven non-throwing and the frame
  disappears. Keep source order = address order.
- VC6 allocates aggregates (arrays, structs) after scalars in the frame:
  grouping unrelated float locals into `float k[3]` moves them down.
- Callee-saved registers can be pushed mid-function, after early exits
  (no special source needed).
- Searching source variants is fast when many variants of one function are
  compiled in one TU as members of throwaway derived classes.
- `Name() = other.Name()` (std::string operator=) gives the out-of-line
  `assign(src,0,npos)` call 0x4024f0; an explicit assign() is inlined.
- `x / 0.25f` with a literal becomes a multiply by the reciprocal; a
  `static const float k = 0.25f` keeps the fdiv from memory.
- `const char* n = ""; if (o->name) n = o->name;` gives store-then-override;
  the `?:` form does not.
- VC6 re-associates float products (`a*b*c*(1-k*d)` multiplies the last
  factor first) and factors common multipliers out of sums; split the
  chain into float locals or `r = x; r *= e; r *= w;` statements.
- `(float)fabs(x)` at each use: plain fabs returns double and spills
  QWORD temps.
- `x ? 0x11 : 0` compiles branchless (neg/sbb/and); if/else keeps branches.
- A switch whose default returns 0.0 keeps the 0.0 in a stack slot when
  written `float r = 0; switch {...} return r;`.
- The EH unwind table of a ctor lists every member with a non-trivial (even
  empty) dtor in construction order: read it from the exe to decide which
  members are Vec3 and which are plain float[3].
- Car member files read fields through per-file view structs cast from
  `this` (CarSoundView, RaceCar, CarModelView, CarRenderView, ...); Car
  itself only gets method declarations, which keeps parallel edits mergeable.
- VC6 hoists the most-used constant into a register, and byte stores of 1
  count: inlining repeated initialisations can move `1` into ebx.
- The declaration order of float accumulators sets the initial x87 stack
  layout of a long function (2038 -> 950 differing bytes in UpdateSounds).
- A float argument pushed as `push ecx; fstp [esp]` comes from an inline
  accessor returning the float; a plain member is pushed with `push reg`.
- `#define V ((View*)this)` keeps `this` in ecx where a local view pointer
  would take another register.
- `int g = v->gear; s->gear = g;` keeps the dword load; `BYTE = int` makes
  VC6 narrow the load to a byte.
- Later-declared scalar locals get lower frame addresses; arrays/structs go
  below the scalars. Separate int locals and an int[4] array are laid out
  differently.
- `inline float PassFloat(float v) { return v; }` forces a float member copy
  through the FPU (fld/fstp) instead of integer moves.
- `int r = 0; if (...) { if (t) r = 1; } return r;` keeps 0 in a register;
  early `return 1` / `return 0` exits fold it away.
- A class that is too big or too entangled to widen in a shared header can be
  declared again with its full layout under another name (CarObject for
  Car): match.py compares by address, so `// SYMBOL:` with the new mangled
  name works.
- A reference-bound temporary (`const std::string& t = a + b;`) consumes
  compiler counters and shifts the module-end `_$E` numbers; a plain local
  string does not.
- Whether a TU emits the STL `_$E` pair depends on its include set (it
  needs headers that instantiate STL statics); a bare unk_481390.h +
  rallytrophy.h TU emits none.
- Inline-function float arguments are evaluated right to left onto the x87
  stack; call arguments built from expressions likewise (assign named
  locals in reverse order to match).
- A cdecl struct-returning helper can be declared `T* f(T* result, ...)`
  to reproduce the single hidden temp and the 3-load/3-store copy.
- VC6 has a per-function inline budget: in a huge function the early
  std::string/vector call sites stay out-of-line calls and the later ones
  are expanded (12 identical `s += "x"`: first 5 calls, last 7 inline).
  Pin each site with declared-only wrappers or hand-expanded STL code.
- `#pragma function(strcmp)` turns the /O2 strcmp intrinsic back into a
  call; restore `#pragma intrinsic(strcmp)` afterwards.
- Functions with inline asm get memory homes for their locals; dead
  parameter and asm-pointer slots are reused by later variables.
- A thiscall that never reads `this` must stay a normal member (callers
  pass ecx, the callee returns with `ret N`).
- A POD return/local (no dtor) where the original has no EH state; a Vec3
  with its empty dtor adds one.
- /O2 frame slots follow weighted reference counts, not declaration order;
  read the original's EH unwind table (FuncInfo via the handler's
  `mov eax, imm32`) to learn which locals are EH objects and their offsets.
- A static data member array of an empty-ctor/dtor class gives a guarded
  dtor iterator; a file-scope array does not.
- Hand-expanded std::string helpers must copy with
  `std::char_traits<char>::copy`, not memcpy: it fixes the register
  allocation of the expanded copies (858 -> 63 differing bytes).
- `small` and `big` are macros in the Windows headers (rpcndr.h): don't
  use them as identifiers.
- objcmp counts a function's jump tables in its built size; the trailing
  `lea ecx,[ecx+0x0]` after `ret` is masked table data.
- A by-value class parameter with an empty dtor (Color) gives the callee
  EH state 0 and shifts the later states.
- A by-value argument class with an out-of-line copy ctor and no dtor
  consumes no EH state; one with an inline copy ctor and a dtor does.
- Where the original keeps a dead `_Xran` check, the resize helper must be
  a member; open-coding the test lets VC6 drop it.
- Mouse warps `SetPosition((Vec2*)&Point(x, y))` use an argument temporary
  with an inline ctor and an empty dtor; a named HudPoint local gives the
  wrong store order around the pushes.
- std::string's default `std::allocator<char>` argument shows as
  `mov al,[esp+X]` / `mov [str],al`; a path class with
  `Ctor(const std::allocator<char>& a = std::allocator<char>()) : alloc(a)
  { Tidy(false); }` reproduces it with the out-of-line `_Tidy` call.
- A dead store the original keeps (e.g. a record's size dword) survives as
  a struct member store; a plain local is removed.
- With /Oa VC6 elides a struct/array temporary that is copied in and
  straight back out (every form becomes one `rep movsd`); the relay
  save/load copies through a 0x28-byte temporary are still unexplained.
- `if (!flag) { x = flag; } else { x++; }` gives the original's
  jne-to-increment layout; the inverted test reverses the blocks.
- `match.py` masks relocated .rdata constants, so a float literal that is one
  ulp off still "matches": check bit patterns by hand (1/768 is
  `0.0013020834f` = 0x3aaaaaab, not `0.0013020833f`).
- `bool b = (x == y); if (b)` gives `cmp; sete al; test al,al`; an `int`
  gives `xor eax,eax; cmp; sete al; test eax,eax`.
- In `a = b = v` the right-hand target is stored first; quad corner/UV
  stores are reproduced by chains like `uv[0].x = uv[3].x = ...`.
- `text->SetScale(sx, sy)` copies through fld/fstp; member stores copy
  through integer registers.
- An unused default-constructed `std::string` local keeps its EH state and
  the allocator-byte read, but its normal-path dtor disappears.
- Methods whose callers `test eax,eax` return int, not bool (ServiceMenu
  CanTune/CanRepair).
- `v.insert(v.end(), x)` through a local `std::vector<T>& v = m_items;`
  inlines insert(it, n, x); `push_back` stays the out-of-line call.
- Open: the original often keeps a -1 colour in a callee-saved register
  (`or ebp,-1; mov [slot],ebp; push ebp`); no source form tried so far
  (Color local, DWORD, temporaries) reproduces it.
- The `// SYMBOL:` override in match.py/objcmp.py is committed now; keep it
  (generated functions and overload selection depend on it).
- `_$E` numbers of a TU's static initialisers depend on everything in the
  file: after merging drafts, re-read the defined `_$E` symbols and update
  the `// SYMBOL:` lines (module-end guard = higher number).
- Layout constants the original reads from .rdata as separate objects (equal
  values duplicated, sums not folded) are file-scope `static const float`;
  where the original multiplies `literal * scale` (fld const; fmul var) use
  a plain non-const `static float`.
- A by-value struct argument whose string arrays are copied out of line
  (0x408a80) while the first strings are copied inline: call through a
  layout twin whose array elements have declared-only copy ctor/dtor.
- `SetColor` with a trivially copyable colour pushes `mov ecx,[x]; push ecx`;
  a ColorArg with a user copy ctor makes a hidden temp.
- The int-to-float temporaries of a loop counter and counter+1 can share one
  stack slot in the original (`fild [T]; mov [T],ebp; fild [T]`); no source
  form found yet (BuildRects 0x43ee70, Setup 0x43dc70).
- Independent global stores are reordered by VC6 (CheckCheatName 0x43f8b0):
  write them in the source order that yields the original schedule, one
  statement per store.
- `vector::insert` inline budget: with many identical `push_back` sites the
  early ones stay out-of-line calls; a dummy inline call before the last site
  moves the cutoff by one (LoadLayout 0x440890).
- `std::string::resize(n)` and `+= c` are the out-of-line erase 0x408de0 and
  append(count, c) 0x408d10.
