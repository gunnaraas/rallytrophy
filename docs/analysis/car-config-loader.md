# Car configuration loader (`0x462350`)

Status: NONMATCHING; tracked body retains the two-`Vec3` axis-lifetime form,
which restores the target frame and unwind states but still differs in other
parts of the function.

REA instruction evidence
`ev_0ce566d01f60510d418fce5fc71a04d892081d17fc3943977f83829eab8823b6`
covers the 4,484-byte function in the current reference executable (SHA-256
`2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`).

## Axis temporaries

The target allocates a 0x88-byte frame. Local unwind-map inspection shows state
8 calls the folded empty destructor for the object at steady `ESP+0x5c`, and
state 9 does so for the object at `ESP+0x50`; state 9 is installed at
`0x4632c5`. Plain PE disassembly further shows `GetFloat("TrunkAngle")`
returns before the hood-angle multiply, and its x87 value remains live through
the hood cosine/sine and stores; it is multiplied only afterward for the trunk
axes. These observations establish two nested destructible local lifetimes,
but do not identify their types by themselves. Hood/trunk axes use
cosine and sine with `(0, cos, sin)` and `(0, -sin, cos)` components. Local
spills at `0x463319`–`0x46333b` retain cosine, sine and negated sine. Inference:
the two locals are persistent `Vec3` axes reused for both members.

The two-local form reproduces the target frame size, both unwind states, and
the local slots. In the tracked source it emits 4,504 bytes versus 4,484 (the
prior source emitted 4,439); `objcmp` reports 2,339 differing bytes outside
relocations, down from 2,630. The current tracked ordering initializes both angle
values before calculating the hood cosine; an attempted delayed `TrunkAngle`
read increased the body to 4,512 bytes / 2,331 differences and moved its call
into the target cosine/sine sequence, so it was reverted. That probe produced a
slightly lower raw differing-byte count but a worse local instruction schedule;
byte counts alone do not establish source fidelity. Another tested ordering variant emits 4,508
bytes and was rejected. The remaining axis hunk is a push-order difference:
the target emits `fst [esp+0x30]; push 0x4fb77c; fcos`, while this candidate
emits `fst [esp+0x30]; fcos; push 0`. The split lifetime places EH state 9
after the cosine and aligns the x87 store, but not that push schedule. A
three-float-constructor variant emitted 4,516 bytes and was rejected. The pushed
address `0x4fb77c` is the `HoodMaxAngle` string (plain PE string-table
observation). Hoisting the full getter-and-scale expression before the axis
calculations produced 4,512/2,330; hoisting only the raw getter and leaving
the scale/store after the axes produced 4,512/2,335. All three forms changed the
local slots, call timing, or stores, so they were reverted; the full-expression probe
had a slightly lower whole-body diff count without improving the local evidence. Direct PE disassembly gives the exact boundary: the key is pushed at
`0x4632be`, followed by `fcos`, EH state 9 and the two local zero stores; the
hood sine/cosine stores complete before the delayed `GetFloat` call at
`0x463355`. Ordinary source-level getter movement cannot reproduce this split: it
moves the call together with the argument or changes the frame. The next useful
probe must control argument setup separately from the call while preserving the
current frame and unwind states.
Exception-string boundaries, earlier configuration copies, mass handling,
and wiper setup also differ, so whole-function byte-position counts do not
isolate the axis result. A `register` hint on `trunkAngle` leaves the emitted
body unchanged, confirming that the remaining FPU lifetime needs a different
source shape than a storage-class hint. These are local PE/unwind observations; the `Vec3`
identification remains an inference.

Probes: ignored `build/probes/car_ini_axis_locals.cpp` and
`build/probes/car_ini/`. The focused matcher currently reports 20 marked
functions, 4 NONMATCHING, and no DIFF or compile failures.

An additional source probe replaced the `IniString` failure expression with the
`PathString`/`TmpString`/pointer-test form from exact `LoadTiresIni` at
`0x4634e0`. It regressed from 4,439 bytes / 2,630 differing bytes to 4,437
bytes / 2,672 differences and left the 0x28-byte frame gap unchanged; the
change was reverted. The two loaders therefore need different string helper
boundaries or lifetimes despite their similar error messages. Passing `MissingText`’s `TmpString` directly to
`runtime_error` through a layout cast removes the outer `IniString` copy,
shrinks the frame from 0x88 to 0x78, and regresses to 4,393 bytes / 2,701
differences; it was reverted, confirming the extra copy lifetime is required. A layout-compatible `IniString(const TmpString&)` copy constructor keeps
the outer copy but produces 4,506/2,608; that variation was also reverted. Naming the prefix, concatenation, and copied message expands the candidate
to 4,506/2,606. Copying through `IniString(const std::string&)` plus an
`AsString(TmpString)` view emits the same 4,506/2,608 as the `TmpString`
constructor. Both variants were reverted because their failure-path EH and
local schedules diverge further. The direct standard expression
`std::runtime_error(std::string("Missing variables in file: ") + path)`
emits 4,391/2,708 and also changes the frame/EH schedule; it was reverted.
The direct target call and pointer-test sequence is recorded in assembly
evidence `ev_d2962d9bcb9b19149862f34fb5b69a4488865d877bdc3795446584363b55a79f`;
it shows `_Tidy`, `_Grow`, the missing-text concatenation, and runtime-error
construction in the failure branch.

Making only the scaled `hood` angle volatile leaves the 4,504-byte / 2,337-
difference candidate unchanged; it was reverted. The target's x87 angle store
and the position of the next pushed operand are visible in
`ev_0ce566d01f60510d418fce5fc71a04d892081d17fc3943977f83829eab8823b6`.
Reusing `hoodAngle` for its scaled value, which tests the target's overwrite of
the raw-angle stack slot, also emits identical bytes and was reverted. Changing
the inline `Scale3` mass parameter from `const float&` to `float` likewise
leaves the candidate unchanged; it was reverted. Focused matching after these
probes reports 20 marked functions, 4 NONMATCHING, and no DIFF or compile
failures.

The target loads mass once and duplicates it on the x87 stack while scaling
the three `MomentOfInertia` components (`ev_d2962d9bcb9b19149862f34fb5b69a4488865d877bdc3795446584363b55a79f`).
Fusing the vector copy with three `mass * component` stores regressed to 4,468
bytes / 2,501 differences, so the separated `COPY3` plus `Scale3` form remains.
A more literal sequence—copy all three fields explicitly, then multiply each
field by `mass`—also fails to reproduce the target schedule. It compiles to
4,468 bytes / 2,498 differences and reloads `mass` separately for each field;
the target duplicates mass on the x87 stack and copies the inertia components
into the car fields before scaling them. The explicit sequence was reverted.
Capturing the scaled X value in a float local before copying Y/Z, then storing
it afterward, produces the same 4,468/2,498 output. Removing that local and
scaling X before the Y/Z copies also produces identical bytes. VC6 emits the
X multiply before the Y/Z loads in both forms, rather than retaining the mass
and X product together on x87 as the target does.

Using the existing `TextOf` pointer-test helper in place of `.c_str()` on the
`MissingText` temporary, while keeping the `IniString` wrappers, leaves the
candidate at 4,439 bytes / 2,630 differences. The direct target load
`mov esi,[eax+4]` (`ev_d2962d9bcb9b19149862f34fb5b69a4488865d877bdc3795446584363b55a79f`)
still becomes a pointer load to `eax`, a null test, and a later copy to `esi`.
The probe was reverted; the remaining register schedule likely depends on the
surrounding EH/local lifetimes rather than the null-test helper alone.

The wiper tail target loads all three `Wiper2Pivot` components before
scaling `Wiper1Angle.x`, stores the pivot y/z between the x and y angle
conversions, then stores the converted x. Interleaving source statements did
not change VC6 scheduling; a volatile conversion stored too early, a normal
float temporary still stored before the pivot y/z fields, and component
snapshots expanded the frame. All variants were reverted.

Next work: reconcile the exception-string call boundaries and local lifetimes
with the target's EH state, then investigate the FPU/store order, mass handling,
and wiper setup. Preserve actual `std::runtime_error` behavior when tuning its
string helpers.

## Inline-assembly constraint

The delayed `HoodMaxAngle` call cannot safely be split with a small inline-asm
snippet: the key push remains live across the cosine and axis calculations, so
all intervening stack-relative accesses would need to be controlled. A single
assembly block could stage that whole interval, but the project toolchain emits
an EBP frame and memory homes for any `__asm` (see `analysis/NOTES.md`), while
the target function uses an ESP-relative 0x88-byte frame (`0x462350`–`0x462370`,
plain PE disassembly). The broad asm route therefore breaks the observed frame
shape; continue with C++ source forms unless a different verified method of
controlling the call schedule is found.
