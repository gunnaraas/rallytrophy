<!-- Subagent analysis, 2026-10-23; read-only disassembly; guesses are marked. -->
# Range 0x47c460 - 0x4807cf: module map (read-only raw disassembly, Patch 1.01 exe; no REA)

Method: rawdis.py plus my own caller/abs-ref index over .text, .rdata/.data reads. "Guess" marks inference. EH handler addresses (0x4dcd26 ...) are the `push imm` after `push -1` in each SEH prologue. Callers in the table are all direct `call`/`jmp rel32` hits found by a scan of .text 0x401000-0x4bd000 (plus code-pointer scan for the vtable and jump table).

## 0. Headline findings

1. **The range holds one C++ module and three plain-C modules.** M1 = a car **particle emitter** class (vtable 0x4e237c, 0x47c460-0x47ef4f, ends with the STL guard stub at 0x47ef50). Everything after the stub, 0x47ef80-0x4807cb, is **not C++ game code**: it is the **GameSpy CD-key SDK (`gcdkeyc.c`)**, its **MD5 (`md5c.c`)** and the start of **`nonport.c`** (GetTickCount / WSAStartup / WSACleanup wrappers, `strdup`). C files have no `_$E` stub, so there is no further guard stub in the range. The next C++ module begins at 0x4807d0 (SingleRace ctor, already written).
2. **0x47ef80 is NOT "chat text".** `include/unk_481390.h:1372` declares `FUN_0047ef80(a, name, text)` as "chat text for a player" - it is `gcd_compute_response(cdkey, challenge, out)`; the call at 0x48337c (NetRace module) passes the product key (`FUN_004a1820()`, the stored key), the host/session challenge and an output buffer. Likewise `FUN_0047f090(0x1f7)` is `gcd_init(gameid = 503)`.
3. PROGRESS.md says "0x4805d0-0x4807cf is GameSpy/md5 helper code from another file". More exact: gcdkeyc.c is 0x47ef80-0x47fb9f, md5c.c 0x47fbb0-0x48073f, nonport.c 0x480740-0x4807cb (see section 3).
4. functions.csv labels 0x480740 `KERNEL32.DLL::GetTickCount` and 0x480770 `WSOCK32.DLL::Ordinal_116` as import thunks. They are not the linker thunks (those sit grouped at 0x4cf760-0x4cf7a2 for winsock). They are 16-aligned **tail-call wrappers** in nonport.c: 0x480740 = `jmp dword ptr [GetTickCount]` (a `current_time()`-style function, compiled with dllimport -> indirect jmp), 0x480770 = `jmp 0x4cf7a2` (WSACleanup thunk; `SocketShutDown()`). Compile them as 1-line C functions; do not model them as imports.
5. The particle free-list global **0x56ae90** (WinMain-module static, ctor 0x478870, dtor 0x4788c0, see range-49b630.md section 8 where it is guessed to be a `std::vector`) is a **particle pool**: dword at +0 = head of a free list threaded through `Particle+0x40`; when empty, `0x478960` (thiscall on 0x56ae90) allocates `new(0x44004)` = 4 + 0x44 x 4096 particles and threads them. So 0x56ae90 is a `ParticlePool`, not a vector. Correct that note.

## 1. Module table

| # | Range | End marker | Content | Suggested file |
|---|---|---|---|---|
| M1 | 0x47c460 - 0x47ef4f (+ stub 0x47ef50-0x47ef7b, pad to 0x47ef7f) | `_$E` stub 0x47ef50 (`mov cl,[0x5826d5]; mov al,1; ...; push 0x402c90; call atexit(0x4c0d01)`) | `CarParticles` (guess name): particle emitter derived from the effect base class (vtable 0x4e2124, base ctor 0x478e80, base dtor 0x478f00). 23 functions | src/unk_47c460.cpp |
| M2 | 0x47ef80 - 0x47fb9f | none (C file) | GameSpy `gcdkeyc.c` (CD-key auth client over UDP): 19 functions | src/gcdkeyc.cpp (C-style; compile as C++ unless /TC flags differ, see note) |
| M3 | 0x47fbb0 - 0x48073f | none | GameSpy `md5c.c` (RFC 1321 MD5 + hex digest helper): 10 functions | src/md5c.cpp |
| M4 | 0x480740 - 0x4807cb | none | GameSpy `nonport.c` start: `current_time`, `SocketStartUp`, `SocketShutDown`, `goastrdup` (4 functions) | src/nonport.cpp |
| - | 0x4807cc - 0x4807cf | | padding; 0x4807d0 is SingleRace ctor (next module) | |

Boundary evidence:
- M1 start: previous guard stub at 0x47c430 (same pattern), 0x47c460 is the 16-aligned next function: a ctor with SEH and `mov [esi],0x4e237c`.
- M1/M2: stub 0x47ef50 (function above it, 0x47ee80, ends at 0x47ef41). Code after the stub (0x47ef80) is C: cdecl, `repnz scas` strlen, no SEH, no vtables, no `fs:0` prologues, and uses only CRT (srand 0x4c198b, rand 0x4c1998, time 0x4c1d14, sprintf 0x4c1cc2, strncpy 0x4c1f30, malloc 0x4c1df0, free 0x4c202e, strchr 0x4c2130, atoi 0x4c18a0, strcat/strcpy inline) and winsock thunks 0x4cf760-0x4cf796.
- M2/M3: 0x47fb80 (ListRemove, ends 0x47fba4) -> 0x47fbb0 MD5Init (0x67452301 init vector). Strings: gcdkeyc.c uses .rdata 0x4fe53c-0x4fe658, md5 uses 0x4fe658 (padding 0x80) and 0x4fe698 ("%02x").
- M3/M4: 0x4806f0 (`MD5Digest`-style hex wrapper, ends 0x480738) -> 0x480740 `jmp [GetTickCount]`. nonport.c is the GameSpy portability file (GetTickCount/WSAStartup/strdup).
- M4/next: 0x480780 `goastrdup` ends 0x4807cb; 0x4807d0 begins the SEH ctor of SingleRace (vtable 0x4e23ac).
- No other guard stub inside 0x47ef80-0x4807cf (byte scan for `8a 0d d5 26 58 00` finds only 0x47c3a0, 0x47c430, 0x47ef50, 0x481360 around here).
- Note on C vs C++: the three GameSpy modules show C idioms (`repnz scas` strlen, hand-unrolled strcmp, `sbb eax,eax / sbb eax,-1`), consistent with plain C compiled by the same cl. They can be written as .cpp with only C constructs (the matcher compiles .cpp); strings are plain `char*`.

## 2. M1: `CarParticles` (particle emitter), 0x47c460 - 0x47ef4f

### Identity and ownership
- One object per Car: embedded at **Car+0x2358**, size **0x130** (next member, ctor 0x47b4c0, sits at Car+0x2488). Car ctor 0x45e480 calls the ctor at 0x45e673 (`lea ecx,[ebx+0x2358]`); the Car dtor calls the dtor body 0x47c4e0 at 0x462073; Car::FUN_0045b900-area `0x45b4ec` calls `Update(dt)` (0x47c5a0); `0x4645dc` calls `SetColor` (0x47c4f0); `0x469d98` (Car module) calls `Spawn` (0x47c790). Its siblings in the Car are the other particle subclasses at Car+0x2100 (ctor 0x4773d0, update 0x477460, dtor 0x4780e0/0x4780f0), +0x222c (0x479170, update 0x479250, dtor 0x479240), +0x2488 (0x47b4c0, update 0x47b560, dtor 0x47b550) and +0x25e8 (0x4780f0 ...). They all share base vtable 0x4e2124 (set by base ctor 0x478e80, base dtor 0x478f00 which also calls 0x4790e0).
- Base class (outside this range, from base ctor 0x478e80): derives from the engine object base (0x4b2830); sets `[this+0x124] = 0` and `[this+0x128] = 0`. In this module `+0x124` is non-null when the emitter is "on"/a texture-like handle (Render returns immediately if +0x124 or +0x128 is 0) and **`+0x128` is the head of the live-particle linked list** (singly linked through `Particle+0x40`).
- Derived ctor 0x47c460 (SEH, EH 0x4dcd26, `thiscall`, returns this): `0x478e80`, `[this]=0x4e237c`, `0x4b3010(1)` (vtable slot 7: sets/clears bit 0x04 of byte +0x0c), `byte[+0xcc] |= 2` (guess: "additive/blend" render flag).
- Derived adds **dword +0x12c = packed colour** (bytes B=+0x12c, G=+0x12d, R=+0x12e, A=+0x12f; written by SetColor, copied into each new particle's +0x38).

### Vtable 0x4e237c (10 slots, then two float constants 0x4e23a4 = 1/35 and 0x4e23a8 = 1/20480; the next vtable 0x4e23ac belongs to SingleRace, so this one is exactly 10 slots)
| Slot | Fn | Role |
|---|---|---|
| 0 | 0x47c4c0 | scalar deleting dtor (flag&1 -> operator delete 0x4bd000) |
| 1 | 0x4b2c10 | base (outside range) |
| 2 | 0x48e660 | shared empty function (`ret`) |
| 3 | 0x4b2d50 | base |
| 4 | 0x4b2fc0 | base |
| 5 | 0x47c8a0 | **Render(Renderer* r, Camera* cam)** (ret 8) |
| 6 | 0x477080 | base/other (outside range) |
| 7 | 0x4b3010 | base: SetFlag04(bool) (ret 4) |
| 8 | 0x4890c0 | `return 1` (shared; racemode-base.md) |
| 9 | 0x4790e0 | base (called by base dtor 0x478f00 too) |

### Particle record (0x44 bytes; pool 0x56ae90 hands them out; facts from Update 0x47c5a0, Render 0x47c8a0, spawners)
- +0x00 (unused/flags, not seen)
- +0x04 int: sprite frame; Render uses low nibble and `>>4` as column/row indices into the uv tables (0x561bb8/0x561bbc for frame<0x90; 0x561b98/0x561b9c with `&3` otherwise, and 0x561cb8/0x561cbc row offsets)
- +0x08 float age (seconds), +0x0c float lifetime; particle dies when age >= lifetime (Update 0x47c5a0 moves it to the free list)
- +0x10 Vec3 position; +0x1c Vec3 velocity (multiplied each tick by a per-tick damping factor in Update, which comes from a `0x4c19f0` call = `pow`-like CRT helper on the constant 0x4e22c0 = 1.0f and dt: guess, the argument roles are unclear; recheck when porting)
- +0x28 float rotation index (0..1023, angle table index), +0x2c float spin (rotation += spin*dt)
- +0x30 float size, +0x34 float y-acceleration ("gravity"; -9.0 or -4.0 or a positive float in different kinds); Update: `vel.y += dt * [+0x34]`
- +0x38 dword colour (copied from emitter +0x12c), +0x3c (not seen), +0x40 next pointer
- Update also keeps an **AABB of live particles at emitter +0xa0..+0xa8 (min xyz) and +0xac..+0xb4 (max xyz)** (reset from first particle; zeroed when the list is empty).

### Statics and tables
- File-scope statics: **0x563fdc** (Car*/car-state used by the spawners: reads `+0x14,+0x18,+0x1c` position, `+0x264`, `+0x288`, `+0x28c` slip-like values) and **0x563fd8** (CarBody*, matrix at +0x14..+0x38, +0x1cc velocity, +0x20/+0x40 axes). Both are set by Spawn (0x47c790) from its 4th/5th arguments (caller passes `[esi+0x24]` = body, and a car/state pointer).
- Function-level statics (VC6 guard-flag bytes) in the five spawn-core functions: 0x47cd60 -> flag byte 0x563fd4, floats 0x563fd0/0x563fcc/0x563fc8 (each `1/(hi-lo)` of the .rdata pairs 4.0-1.0, 60.0-10.0, 15.0-4.0); 0x47d500 -> 0x563fb4 / 0x563fb0, 0x563fac, 0x563fa8; 0x47dc10 -> 0x563f94 / 0x563f90, 0x563f8c, 0x563f88; 0x47e240 -> 0x563f84 / 0x563f80, 0x563f7c, 0x563f78; 0x47e8f0 -> 0x563f74 / 0x563f70, 0x563f6c, 0x563f68. Bits 1,2,4 of the flag byte guard the three floats.
- Global **0x56ae90** = ParticlePool (free-list head at +0).
- Global tables used by Render: sin/cos pair table at **0x561ec0** (1024 entries x 8 bytes: [i*8] = sin, [i*8+4] = cos, index `rot & 0x3ff`) - located in .bss (zero in the file; filled elsewhere at runtime, owner not in this range); uv tables 0x561bb8, 0x561b98, 0x561cb8.
- .rdata float constants seen (value = use): 0x4e13ac 255.0 (SetColor scale), 0x4e22c0 1.0, 0x4e1348 1.0, 0x4e22c8 1.0 / 0x4e22cc 4.0 (slip range low/high), 0x4e22d0 10.0 / 0x4e22d4 60.0 (speed range), 0x4e22d8 4.0 / 0x4e22dc 15.0 (second slip range), 0x4e22c4 256.0, 0x4e2238 0.00390625/10 (read as float 0.000390625 = 1/2560), 0x4e1b9c 0.05, 0x4e22b4 1/32768, 0x4e13a4 0.25, 0x4e1bec 0.7, 0x4e1eb8 2.5, 0x4e2234 9.155e-5, 0x4e22b8 0.02, 0x4e22bc 0.05, 0x4e14e0 256.0, 0x4e1b40 6.1e-5, 0x4e2248 3.05e-5, 0x4e1e88 0.15, 0x4e13c4 0.2, 0x4e164c 4.0, 0x4e13dc 3.0, 0x4e1530 1.5, 0x4e1b44 2.0, 0x4e23a4 1/35, 0x4e23a8 1/20480. Immediate floats pushed to the variants: 0x3e99999a = 0.3, 0x3f4ccccd = 0.8, 0x3f000000 = 0.5, 0x3f666666 = 0.9.
- No strings in M1.

### Spawn dispatch (0x47c790) - `Spawn(int type, float a, float b, X* car, Body* body)` (stdcall-looking, `ret 0x14`)
- Stores arg4 -> 0x563fdc and arg5 -> 0x563fd8, then `jmp [type*4 + 0x47c86c]` for type 0..11 (11 > type -> return). Jump table at 0x47c86c (12 entries; the code is inside the function's tail, ends 0x47c89b). Each case forwards `(a, b)` (2 stack args) to a "kind" function; **types 4 and 5 are no-ops**:
  - 0 -> 0x47d240, 1 -> 0x47d2f0, 2 -> 0x47d3a0, 3 -> 0x47d450 (these four call spawn core 0x47cd60 with float 0.3, 0.3, 0.3, 0.8 and then set per-kind fields)
  - 6 -> 0x47d970, 7 -> 0x47da50, 8 -> 0x47db30 (call core 0x47d500 with arg 0; lifetime 0.6, frame `rand()&0xf | 0x60`, gravity -9 or -4)
  - 9 -> 0x47e190 (core 0x47dc10, arg 0.5), 10 -> 0x47e820 (core 0x47e240, arg 0.9), 11 -> 0x47ee80 (core 0x47e8f0, arg 0.5)
- Kind functions (thiscall on the emitter, 2 stack args `(float scale, float initialAge)`): call the core, if it returned a particle: `[p+8] = initialAge`, set lifetime `[p+0xc]`, size `[p+0x30]`, gravity `[p+0x34]`, frame `[p+4]`, colour `[p+0x38] = emitter+0x12c`, random rotation (`rand()&0x3ff`) and spin, scale velocity by `scale` (via `Vec3 * float` = 0x40b0c0, out of line), subtract it from the position (the kind pushes the particle backwards along the car motion), then push it to `emitter+0x128` (list head) via `[p+0x40] = head; head = p`.
- Spawn cores (SEH, stdcall-looking `ret 4`, one float arg `rate`): decide randomly whether to emit (`rand()&0xff` against a value derived from `|car+0x288|` and `|car+0x28c|` slip values mapped through the three function statics; `return 0` if no emission), take a particle from pool 0x56ae90 (call `0x478960` to grow if empty), set position from the body matrix times a local offset plus body translation (`0x4204f0` = body point transform, Car module helper), and initial velocity using random numbers (`rand() * 1/32768` constants) and `Vec3*float` (0x40b0c0) temporaries (SEH objects Vec3). Returns the particle (eax) or 0. 47cd60 is the base (dust/smoke?), 47d500/47dc10/47e240/47e8f0 are four other kinds that also read position/velocity from the body (+0x1cc velocity, 0x40/0x20 axes) and constants 0.15/0.2/3.0/4.0/2.5/1.5.
- Guess at meaning: the effect objects in the Car are wheel/slide effects (dust, gravel, mud, sparks, smoke) driven by tyre slip (`car+0x288/0x28c`) and speed; type argument selects the surface/effect; name "CarParticles" is only a guess.

## 3. M2-M4: GameSpy CD-key SDK (plain C), 0x47ef80 - 0x4807cb

Product: Rally Trophy uses the **GameSpy "gcdkey" client** (`gcdkeyc.c`) with `gameid = 0x1f7 (503)` (call `0x47f090(0x1f7)` at 0x485051 in NetGame init, shutdown via 0x47f1a0 at 0x484b0a). Per AGENTS.md, replace with an open implementation; do not port the SDK as a feature, but a matching decomp of these bytes needs C++ that reproduces them. AGENTS.md: "Miles/Bink/GameSpy SDK behavior gets open-source replacements" - so this range is **low priority for the Rust port but still needed for byte-matching**.

### Globals (.data/.bss)
| Addr | Role (guess from code) |
|---|---|
| 0x4fe590 | UDP socket (init -1, = INVALID_SOCKET; .data) |
| 0x56460c | head of the request list (`GCDIRequest*` nodes: `node->next` at +4, `node->prev` at +8, `node->req` at +0) ; 0x564608 = list anchor passed to ListInsert/ListRemove |
| 0x564610 | cleared in init (list tail/anchor) |
| 0x5643ec | gameid / product id (arg of gcd_init) |
| 0x5643f4 | WORD: local port from getsockname (low 16 bits) |
| 0x5645f8 | `sockaddr_in` (16 bytes) of the master: `key.gamespy.com`, port 0x74d6 = 29910 |
| 0x5643e0 | 8 bytes "gamespy\0" (stored byte by byte in init: XOR key for the packet cipher) |
| 0x5643f0 | toggle dword (0/1) selecting which of two 256-byte buffers `value_for_key` returns |
| 0x5643f8 | 2 x 256-byte buffers for `value_for_key` results (selected by `toggle << 8`) |
| 0x563fe0 | 0x400-byte receive buffer |
| 0x5070b0 | the empty string "" (used as default text) |

### Strings (.rdata)
0x4fe53c `%s%d%s`, 0x4fe544 `%.8x`, 0x4fe54c `a`, 0x4fe550 `key.gamespy.com`, 0x4fe594 `Validation Timeout` (and the non-NUL-terminated `\xff\xff\xff\xffValidation Timeout` at 0x4fe590 is the socket variable followed by this text, i.e. 0x4fe590 is a dword -1 and 0x4fe594 the string), 0x4fe5a8 `Validated`, 0x4fe5b4 `ison`, 0x4fe5bc `unok`, 0x4fe5c4 `uok`, 0x4fe5c8 `errmsg`, 0x4fe5d0 `cd`, 0x4fe5d4 `skey`, 0x4fe5dc `\uoff\\skey\%d`, 0x4fe5ec `\uon\\skey\%d`, 0x4fe5fc `\disc\\pid\%d\cd\%s\ip\%d`, 0x4fe618 `\auth\\pid\%d\ch\%s\resp\%s\ip\%d\skey\%d`, 0x4fe644 `\`, 0x4fe648 `255.255.255.255`, 0x4fe658 0x80 (MD5 pad byte), 0x4fe698 `%02x`.

### Request record (`GCDIRequest`, malloc 0x50; guess names): +0x00 localid (arg), +0x04 char[0x24] hashed-key/challenge string copy, +0x28 skey (`(rand ^ GetTickCount) & 0x3fff`), +0x2c ip (arg), +0x30 last send time, +0x34 retry count, +0x38 state (0 new, 1 validated, 2 invalid, 3 done/closed), +0x3c user instance, +0x40 callback `void (*)(int localid, int ok, char* msg, void* inst)` (called as `cb(localid, ok, msg, inst)`), +0x44 error message (goastrdup), +0x48 resend packet buffer, +0x4c packet length.

### Public API (original GameSpy names are my identification of the library; confident)
- 0x47ef80 `gcd_compute_response(cdkey, challenge, response)` (cdecl, 0x20c locals): length check `2*strlen(cdkey)+strlen(challenge)+8 >= 0x200` -> `response = "a"` (word copy from 0x4fe54c) and return; else `srand(time(0)^0x33333333)`, two `rand()` -> 32-bit r, `sprintf("%.8x", r)` into a buffer, `sprintf("%s%d%s", cdkey, r % 0xffff, challenge)`, `MD5Digest` (0x4806f0) of the cdkey then of the combined string, stores the 32-char hashes at `response+0x20` and `response+0x28`. Callers: 0x48337c (NetRace/session module, via `FUN_004a1820()` product key).
- 0x47f090 `gcd_init(gameid)`: `0x480750` (SocketStartUp), `socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)` stored in 0x4fe590 (return 0 on failure), `0x47faa0` fill local sockaddr, `bind`, `getsockname` -> 0x5643f4, resolve `key.gamespy.com:29910` into 0x5645f8, store gameid, clear list, `srand(GetTickCount())`, "gamespy" XOR key. Caller 0x485051.
- 0x47f1a0 `gcd_shutdown()`: 0x47f360 (free all requests), `closesocket(0x4fe590)` (0x4cf772), reset socket to -1, then **jmp to 0x480770** (SocketShutDown/WSACleanup tail call). Caller 0x484b0a.
- 0x47f1c0 `gcd_authenticate_user(gameid, localid, ip, challenge?, response?, callback, instance)`: copies the 0x20-byte hash, deduplicates (if two live requests share the hash it replaces it with `goastrdup("a")`), `malloc(0x50)` request, fills fields, appends with 0x47fb50 to the list at 0x564608, then `0x47f860` builds and sends the `\auth\` packet. Caller 0x483609.
- 0x47f300 `gcd_disconnect_user(gameid?, localid)`: find request by localid in list 0x56460c, `0x47f800` send `\disc\` packet, ListRemove (0x47fb80), free `+0x48`, free the request and node. Caller 0x482a2c.
- 0x47f360 free-all-requests (loop of the same removal; called from gcd_shutdown only).
- 0x47f3b0 `gcd_think()`: `select(...)` on 0x4fe590 with 0 timeout (0x4cf77e), `recvfrom` loop into 0x563fe0 (0x4cf778), decode with 0x47fa60 (XOR cipher), NUL terminate, dispatch to 0x47f540; then walk the request list: state 0 and `GetTickCount() >= +0x30 + 5000` retries via 0x47f920 (resend, `+0x34 <= 3`), else timeout -> `cb(localid, 0, "Validation Timeout", inst)` (0x4fe594); state 1/2 -> `cb(..., 1, "Validated")` or `"Validation Timeout"`; state 3 removes the request and frees. Caller 0x48366b.

### Internal helpers (all statics in gcdkeyc.c)
- 0x47f540 `process_packet(buf, ?)`: after a leading `\`, copies the key (<=0x20 chars) and compares with `uok` -> 0x47f670(buf, 1), `unok` -> 0x47f670(buf, 0), `ison` -> 0x47f770(buf, arg). Inline hand-unrolled strcmp ladders.
- 0x47f670 handle `uok`/`unok`: `value_for_key(buf, "skey")` -> atoi, `value_for_key(buf, "cd")` -> copy up to 0x20, `find_request(hash)` (0x47f710), checks skey == `req+0x28`, state != 3; ok -> state 1; else state 2 and `req+0x44 = goastrdup(value_for_key(buf, "errmsg"))`.
- 0x47f710 `find_request(hash)`: walks list 0x56460c comparing `node->req + 4` (string) with hash (inline strcmp); returns req or 0.
- 0x47f770 handle `ison`: parse skey and cd, find request; if state==3 reply `\uon\\skey\%d` else `\uoff\\skey\%d`, XOR-encode (0x47fa60), `sendto` to master.
- 0x47f800 send `\disc\\pid\%d\cd\%s\ip\%d` (pid 0x5643ec, hash `req+4`, ip `req+0x2c`), cipher, `sendto(0x4cf784)` to 0x5645f8.
- 0x47f860 send `\auth\\pid\%d\ch\%s\resp\%s\ip\%d\skey\%d`; also sets `req+0x38 = 0`, random skey, `req+0x30 = GetTickCount()`, `req+0x34 = 1`, saves a malloc'd copy of the packet in `req+0x48/+0x4c` (memcpy `0x4c14e0`).
- 0x47f920 resend: `req+0x30 = now`, `req+0x34++`, `sendto` the saved packet.
- 0x47f960 `value_for_key(buf, key)`: builds `"\" + key + "\"` in a 0x100 stack buffer, `strstr` (0x4c1c10) in the packet, returns a pointer into the toggled 256-byte static buffer (0x5643f8 + (toggle<<8)) holding the value up to the next `\`; returns "" (0x5070b0) if absent.
- 0x47fa60 `xor_cipher(buf, len)`: XOR with the repeating key "gamespy" at 0x5643e0 (wraps at NUL).
- 0x47faa0 `fill_sockaddr(?, ip_or_host, port, out)`: `sockaddr_in` with `htons(port)` (0x4cf796), `inet_addr(host)` (0x4cf790), or `gethostbyname` (0x4cf78a) when it is not "255.255.255.255" and `inet_addr` failed.
- 0x47fb50 `ListInsert(node, list)`: appends at the tail of a doubly linked list (loop `next` until 0; sets node->prev at +8, node->next = 0).
- 0x47fb80 `ListRemove(node, list)`: unlinks (fixes `prev->next` and `next->prev`); handles node==0 by taking `list->next`.

### Byte-match progress
- All 33 GameSpy SDK bodies match the shipped executable: all 19 `gcdkeyc.c` functions (`0x47ef80`–`0x47fb80`), all ten MD5 functions (`0x47fbb0`–`0x4806f0`), and all four `nonport.c` functions (`0x480740`–`0x480780`). `MD5Transform` (`0x47fd00`) is represented by an exact 2,250-byte PE instruction transcription after the reconstructed RFC 1321 body compiled differently under VC6. The range analysis above remains based on plain PE disassembly and is descriptive evidence, not original SDK source. The SDK remains excluded from the Rust behavior port.

### M3: md5c.c (RFC 1321, 0x47fbb0 - 0x48073f)
- 0x47fbb0 `MD5Init(ctx)` (40 bytes; ctx: `state[4]` +0, `count[2]` +0x10/+0x14, `buffer[64]` +0x18, total 0x58). 0x47fbe0 `MD5Update(ctx, input, len)` (calls memcpy-like 0x480670 and Transform 0x47fd00). 0x47fc90 `MD5Final(digest, ctx)` (pads with 0x80 from 0x4fe658, `Encode` 0x4805d0, `memset` 0x480690). 0x47fd00 `MD5Transform(state, block)` (2250 bytes, straight-line FF/GG/HH/II with immediates, `Decode` 0x480620 first). 0x4805d0 `Encode(out, in, len)`, 0x480620 `Decode(out, in, len)`, 0x480670 `MD5_memcpy(dst, src, n)` (rep movs), 0x480690 `MD5_memset(dst, val, n)` (rep stos), 0x4806c0 `to_hex(digest16, out)` (16 x `sprintf("%02x")`), 0x4806f0 `MD5Digest(data, len, hexout)` (ctx 0x58 on stack + hex).
- Calls from outside M3 only via 0x4806f0 (callers 0x47f03c, 0x47f07d in `gcd_compute_response`).

### M4: nonport.c start (0x480740 - 0x4807cb)
- 0x480740 `current_time()`: `jmp [GetTickCount]` (IAT slot 0x4e10bc). Callers 0x47f14c, 0x47f4c9, 0x47f87d, 0x47f88d, 0x47f920.
- 0x480750 `SocketStartUp()`: `sub esp,0x190` (WSADATA), `WSAStartup(0x101, &data)` (thunk 0x4cf79c), discards the result. Caller 0x47f094.
- 0x480770 `SocketShutDown()`: `jmp 0x4cf7a2` (WSACleanup thunk). Caller: jmp at 0x47f1ba.
- 0x480780 `goastrdup(const char*)`: NULL -> NULL; `malloc(strlen+1)` (0x4c1df0), copy. Callers 0x47f1fb, 0x47f259, 0x47f6f7.

## 4. Function table (all 55 entries of functions.csv in the range, plus the stub)

Frame legend: **SEH** = `fs:0` prologue with `push -1, push handler`; **FPU** = x87 code; "this" = ecx; sizes from functions.csv. "gen" = compiler-generated.

| Addr | Size | Convention / frame | Role (guess where marked) | Main callers |
|---|---|---|---|---|
| 0x47c460 | 89 | thiscall, SEH (EH 0x4dcd26), returns this | CarParticles ctor (`0x478e80`, vtable 0x4e237c, `0x4b3010(1)`, `[+0xcc] \|= 2`) | Car ctor 0x45e673 |
| 0x47c4c0 | 30 | thiscall, `ret 4` | **gen**: scalar deleting dtor (vtable slot 0): `0x47c4e0`, `if (flag&1) delete this (0x4bd000)` | vtable only |
| 0x47c4e0 | 11 | thiscall (`mov [ecx],0x4e237c; jmp 0x478f00`) | **gen**: dtor body (reset vtable, tail-jump to base dtor) | 0x462073 (Car dtor), 0x47c4c3 |
| 0x47c4f0 | 174 | thiscall, FPU, `ret 4` | SetColor(const Vec3* rgb in 0..1): x255, fistp, clamp 0..255, writes bytes +0x12c (B), +0x12d (G), +0x12e (R), +0x12f = 0xff | 0x4645dc |
| 0x47c5a0 | 483 | thiscall, FPU, `ret 4` | Update(float dt): per-particle damping/gravity/spin/aging, free-list recycle (0x56ae90), AABB at +0xa0/+0xac | 0x45b4ec (Car update) |
| 0x47c790 | 219 | stdcall-like, `ret 0x14`, jump table 0x47c86c | Spawn(type, a, b, car, body) dispatcher (stores 0x563fdc/0x563fd8) | 0x469d98 |
| 0x47c8a0 | 1202 | thiscall, SEH (EH 0x4dcd66), FPU, `ret 8` | Render(Renderer*, Camera*): builds 4 vertices (0x2c each; vec-ctor 0x4c11f4 with 0x490ad0/0x402c90) per visible particle and calls `renderer->vtbl[0x114](verts, camera+0x124?, depth)` | vtable slot 5 |
| 0x47cd60 | 1233 | thiscall, SEH (EH 0x4dcda0), FPU, `ret 4`, 3 static floats | Spawn core A (rate arg): probabilistic emit, pool alloc, initial pos/vel | 0x47d24c, 0x47d2fc, 0x47d3ac, 0x47d45c |
| 0x47d240 | 170 | thiscall, FPU, `ret 8` | kind 0 (core A, 0.3) | 0x47c7c1 |
| 0x47d2f0 | 173 | thiscall, FPU, `ret 8` | kind 1 (core A, 0.3) | 0x47c7d3 |
| 0x47d3a0 | 173 | thiscall, FPU, `ret 8` | kind 2 (core A, 0.3) | 0x47c7e5 |
| 0x47d450 | 173 | thiscall, FPU, `ret 8` | kind 3 (core A, 0.8) | 0x47c7f7 |
| 0x47d500 | 1125 | thiscall, SEH (EH 0x4dcdd8), FPU, `ret 4`, 3 static floats | Spawn core B (arg 0) | 0x47d979, 0x47da59, 0x47db39 |
| 0x47d970 | 216 | thiscall, FPU, `ret 8` | kind 6 (core B; frame `rand()&0xf\|0x60`; lifetime 0.6) | 0x47c809 |
| 0x47da50 | 216 | thiscall, FPU, `ret 8` | kind 7 (core B) | 0x47c81b |
| 0x47db30 | 218 | thiscall, FPU, `ret 8` | kind 8 (core B) | 0x47c82d |
| 0x47dc10 | 1394 | thiscall, SEH (EH 0x4dce10), FPU, `ret 4`, 3 static floats | Spawn core C | 0x47e19c |
| 0x47e190 | 175 | thiscall, FPU, `ret 8` | kind 9 (core C, 0.5) | 0x47c83f |
| 0x47e240 | 1498 | thiscall, SEH (EH 0x4dce48), FPU, `ret 4`, 3 static floats | Spawn core D | 0x47e82c |
| 0x47e820 | 194 | thiscall, FPU, `ret 8` | kind 10 (core D, 0.9) | 0x47c851 |
| 0x47e8f0 | 1418 | thiscall, SEH (EH 0x4dce80), FPU, `ret 4`, 3 static floats | Spawn core E | 0x47ee8c |
| 0x47ee80 | 194 | thiscall, FPU, `ret 8` | kind 11 (core E, 0.5) | 0x47c863 |
| 0x47ef50 | 0x2c (not in functions.csv) | stub | `_$E` guard stub: `if (!(byte[0x5826d5] & 1)) byte[0x5826d5] \|= 1; atexit(0x402c90)` | init table |
| 0x47ef80 | 272 | cdecl, stack 0x20c | gcd_compute_response(cdkey, challenge, out) | 0x48337c |
| 0x47f090 | 264 | cdecl | gcd_init(gameid) | 0x485051 |
| 0x47f1a0 | 31 | cdecl, ends with `jmp 0x480770` | gcd_shutdown | 0x484b0a |
| 0x47f1c0 | 320 | cdecl | gcd_authenticate_user | 0x483609 |
| 0x47f300 | 89 | cdecl | gcd_disconnect_user | 0x482a2c |
| 0x47f360 | 80 | cdecl | free all requests | 0x47f1a0 |
| 0x47f3b0 | 388 | cdecl, stack 0x120 | gcd_think (select/recvfrom/timeout/callbacks) | 0x48366b |
| 0x47f540 | 299 | cdecl | process_packet (dispatch uok/unok/ison) | 0x47f439 |
| 0x47f670 | 153 | cdecl | handle uok/unok | 0x47f5cd, 0x47f613 |
| 0x47f710 | 95 | cdecl | find_request(hash) | 0x47f6b5, 0x47f798 |
| 0x47f770 | 139 | cdecl | handle ison (reply uon/uoff) | 0x47f65d |
| 0x47f800 | 91 | cdecl | send `\disc\` | 0x47f322, 0x47f36f |
| 0x47f860 | 181 | cdecl | send `\auth\` | 0x47f2f0 |
| 0x47f920 | 49 | cdecl | resend saved packet | 0x47f4e2 |
| 0x47f960 | 249 | cdecl | value_for_key(buf, key) | 0x47f680, 0x47f698, 0x47f6f1, 0x47f77f, 0x47f792 |
| 0x47fa60 | 54 | cdecl | xor_cipher(buf, len) | 0x47f42a, 0x47f7d2, 0x47f831, 0x47f8ce |
| 0x47faa0 | 175 | cdecl | fill_sockaddr (htons/inet_addr/gethostbyname) | 0x47f0bf, 0x47f12b |
| 0x47fb50 | 38 | cdecl | ListInsert(node, list) | 0x47f2c3 |
| 0x47fb80 | 37 | cdecl | ListRemove(node, list) | 0x47f32d, 0x47f37a, 0x47f477 |
| 0x47fbb0 | 40 | cdecl | MD5Init | 0x4806f8 |
| 0x47fbe0 | 167 | cdecl | MD5Update | 0x47fccc, 0x47fcd9, 0x48070c |
| 0x47fc90 | 110 | cdecl | MD5Final | 0x48071b |
| 0x47fd00 | 2250 | cdecl | MD5Transform (64 rounds, no loop; exact PE-byte transcription) | 0x47fc38, 0x47fc53 |
| 0x4805d0 | 72 | cdecl | Encode(out, in, len) (word -> 4 bytes) | 0x47fca4, 0x47fce6 |
| 0x480620 | 77 | cdecl | Decode(out, in, len) | 0x47fd20 |
| 0x480670 | 31 | cdecl | MD5_memcpy (rep movs) | 0x47fc2e, 0x47fc7a |
| 0x480690 | 43 | cdecl | MD5_memset (rep stos) | 0x47fcf0, 0x4805ba |
| 0x4806c0 | 46 | cdecl | to_hex (16 x `%02x` via sprintf 0x4c1cc2, fmt 0x4fe698) | 0x48072d |
| 0x4806f0 | 73 | cdecl, stack 0x68 | MD5Digest(data, len, hexout) | 0x47f03c, 0x47f07d |
| 0x480740 | 6 | tail-call wrapper `jmp [GetTickCount]` | current_time() | 0x47f14c, 0x47f4c9, 0x47f87d, 0x47f88d, 0x47f920 |
| 0x480750 | 28 | cdecl, WSADATA on stack | SocketStartUp() | 0x47f094 |
| 0x480770 | 5 | tail-call wrapper `jmp 0x4cf7a2` | SocketShutDown() | jmp at 0x47f1ba |
| 0x480780 | 76 | cdecl | goastrdup(str) | 0x47f1fb, 0x47f259, 0x47f6f7 |

Totals: M1 23 functions (10,000 bytes), M2 19 (3,004), M3 10 (2,909), M4 4 (115).

## 5. Compiler-generated / trivial functions
- **0x47c4c0** scalar deleting dtor (vtable slot 0) and **0x47c4e0** dtor body (reset vtable + tail call to base dtor 0x478f00): the usual `~CarParticles(){}` pair; the dtor has no SEH (it is a pure vtable-reset and jmp).
- **Variants 0x47d240/2f0/3a0/450, 0x47d970/da50/db30, 0x47e190, 0x47e820, 0x47ee80** are real code (kind functions), not generated.
- The jump table at 0x47c86c is data inside 0x47c790's tail (indexed `jmp [eax*4+0x47c86c]`, `switch (type)` with 12 cases, 4 and 5 default). Do not count it as functions (rawdis shows garbage there).
- **0x480740, 0x480770**: tail-call wrappers (see section 0). Write as `unsigned long current_time(void){ return GetTickCount(); }` and `int SocketShutDown(void){ return WSACleanup(); }` and check that the compiler emits `jmp`.
- The guard stub 0x47ef50 (not in functions.csv) is generated by the include of the STL headers in M1.
- No std::vector/string template instances are defined in M1-M4 (all out-of-line helpers are elsewhere: `0x478960` pool grow, `0x4c11f4`/`0x4c1116` EH vector ctor/dtor iterators).

## 6. Suggested work groups (for parallel subagents; all in this range, then merge in address order)

M1 (CarParticles; 10,000 bytes; give every worker `unk_47c460.cpp` skeleton with the class + Particle struct from section 2; decide the Vec3 helpers `0x40b0c0` (Vec3*float) and `0x4204f0` (body point transform) from include/unk_481390.h first):
- **A. Lifecycle + small + kind 0-3 (1,695 bytes):** 0x47c460, 0x47c4c0, 0x47c4e0, 0x47c4f0, 0x47c5a0, 0x47c790, 0x47d240, 0x47d2f0, 0x47d3a0, 0x47d450. (dispatcher uses a jump table: `switch (type)` of 12 consecutive cases.)
- **B. Render alone (1,202):** 0x47c8a0 (FPU heavy, SEH, 4 locals of 0x2c vertex via vec-ctor).
- **C. Spawn core A alone (1,233):** 0x47cd60 (SEH, 3 function statics with guard flag bits).
- **D. Core B + kinds 6-8 (1,775):** 0x47d500, 0x47d970, 0x47da50, 0x47db30.
- **E. Core C + kind 9 (1,569):** 0x47dc10, 0x47e190.
- **F. Core D + kind 10 (1,692):** 0x47e240, 0x47e820.
- **G. Core E + kind 11 (1,612):** 0x47e8f0, 0x47ee80.
(B-G are FPU-heavy NONMATCHING candidates; do cores in A..G order so the statics layout/guard bits can be shared.)

M2 (gcdkeyc.c; 3,004 bytes, C-like; verify call convention `__cdecl` since the functions.csv labels some stdcall):
- **H. Public API + low-level helpers (1,748):** 0x47ef80, 0x47f090, 0x47f1a0, 0x47f1c0, 0x47f300, 0x47f360, 0x47f3b0, 0x47fa60, 0x47faa0, 0x47fb50, 0x47fb80.
- **I. Packet parsing/sending (1,256):** 0x47f540, 0x47f670, 0x47f710, 0x47f770, 0x47f800, 0x47f860, 0x47f920, 0x47f960.
(H and I must agree on the request struct: section 3 "Request record"; put globals 0x4fe590 etc. in the same file in this order so the data layout is preserved.)

M3/M4 (md5c.c + nonport.c):
- **J. Small md5 + nonport (774):** 0x47fbb0, 0x47fbe0, 0x47fc90, 0x4805d0, 0x480620, 0x480670, 0x480690, 0x4806c0, 0x4806f0, 0x480740, 0x480750, 0x480770, 0x480780. (md5 in `md5c.cpp`, nonport in `nonport.cpp`; keep as two files since the boundary is real even if the matcher is function-level.)
- **K. MD5Transform alone (2,250):** 0x47fd00 (the well-known RFC 1321 round macros with `ROTATE_LEFT`; likely matches only with the original macro form, so reproduce the reference code, not an optimised version).

## 7. Corrections / follow-ups for the project docs
- `include/unk_481390.h:1372` `FUN_0047ef80` comment (chat text) and `src/unk_481390.cpp:1050` call site: rename to `gcd_compute_response`; `FUN_0047f090` -> `gcd_init`; (unk_4843e0.h:70) comment "called with 0x1f7 before hosting" is `gcd_init(503)`.
- modules.csv: add M1 `CarParticles` 0x47c460, `gcdkeyc` 0x47ef80, `md5c` 0x47fbb0, `nonport` 0x480740; PROGRESS.md's "0x4805d0-0x4807cf GameSpy/md5 helper from another file" should read "0x47ef80-0x4807cb".
- range-49b630.md section 8: global 0x56ae90 is the `ParticlePool` (free-list head at +0), not a vector.
- The Car members at +0x2100/+0x222c/+0x2488/+0x25e8 are sibling particle/effect subclasses (vtable base 0x4e2124); their TUs are the stub-delimited modules before 0x47c460 (stubs 0x477340, 0x477ff0, 0x478840, 0x478bb0, 0x479140, 0x47b490, 0x47c3a0, 0x47c430).
