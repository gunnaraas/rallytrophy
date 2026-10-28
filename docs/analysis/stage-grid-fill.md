# `StageGrid::FillCells` (`0x40f390`)

## Initialization ordering

Plain PE disassembly shows the target loading `DAT_004e13b4` into EAX after
the initial `invW` division, then storing it at EBP-`0x60` (`0x40f3b1`–
`0x40f3bf`). The initial candidate loaded the global before the division.
Reading the global through a volatile float reference keeps that load later
and reduces the candidate's differing bytes from 2,158 to 2,151. It remains
NONMATCHING at 2,397 bytes versus the target's 2,340.

This source form still stores `invW` before reading the margin, whereas the
target leaves the division result on the x87 stack until after its early row
setup. The volatile read only improves this local schedule; it does not
reproduce the target sequence or frame layout (`0x74` versus `0x78`).

An isolated rewrite that names the width and height squares (`width2`,
`height2`) grew the candidate to 2,395 bytes / 2,173 differing bytes and was
reverted. Swapping the operand order to `cellH * cellH + cellW * cellW`
emitted the same bytes and was reverted. Observations are from the local PE
and object files, without REA Evidence IDs.

At the first object-list radius calculation, the target loads `this->radius`
and adds the object's radius (`0x40f460`–`0x40f466`); the candidate did the
reverse. Swapping operands alone did not change VC6 output. Reading the
member through a volatile float reference emitted the target's `fld`/`fadd`
operand order and reduced differences by one more byte, to 2,150. Applying
that form at the second radius site instead increased differences back to
2,151, so only the first site retains the volatile read.

The limit calculations in the object-list loops have the same x87 operand
order: target loads `diag`, multiplies by `DAT_004e138c`, then adds `r` (for
example, `0x40f4d5` and `0x40f677`). Volatile `diag` reads at the second
through fifth limit sites produce that operand order. The second-site change
reduces differences from 2,150 to 2,149; the next three do not further reduce
the byte count but lower the instruction-multiset delta from 225 to 213.
These reads still move ahead of the target's preceding integer comparisons.
Applying the same form at the first limit site raises differences to 2,170,
so that one was reverted.

## First-list distance operand order

At `0x40f537`, the target loads the cell X coordinate and subtracts the
object X coordinate. The candidate initially loaded the object X coordinate
and subtracted the cell X coordinate; its Z subtraction already matched the
target. Reversing only the source operands in the X subtraction was a
reasonable probe because the result is squared immediately. VC6 emitted the
same instructions and bytes, including the original candidate's `fld object
X; fsub cell X` sequence. The source was restored. The focused matcher stayed
at 2,391/2,340 bytes and 2,153 differing bytes. These are plain observations
from the local PE and object file, without REA Evidence IDs.

An explicit `StageGrid* self = this` alias used for all member references was
also tested. The target saves `this` in ESI before the first division; this
form still spilled it to EBP-`0x14` and used ECX, and increased the diff to
2,172 bytes. It was reverted. This weakens the simple receiver-alias
explanation for the target's early ESI use.

## Shared radius expansion

In the last two object-list loops, target code computes `diag + DAT_004e13d4`
or `diag + DAT_004e13d0` once, then reuses the stored value for both `invW`
and `invH`. For example, the target at `0x40f9c3` and `0x40fb31` emits
`fld diag; load object pointer; fadd constant; store object; fst rd; fmul
invW`, then reloads `rd` for `invH`. The original candidate repeated the
addition in both `RoundF` expressions.

The source now uses one function-scope `rd`, assigns `diag` before loading
each object pointer, then adds the corresponding constant. This emits the
target instruction order at both sites. The candidate's object-pointer home
now matches target EBP-`0x40`; `rd` still lands at EBP-`0x2c` rather than the
target EBP-`0x44`. The candidate is 2,391 bytes with 2,153 differing bytes
and instruction-multiset delta 197 (target: 2,340 bytes). Separate block
locals had 2,154 differing bytes. The single-expression `rd = diag +
constant` variant also failed to preserve the target order. The register and
remaining stack-slot layout still need work.

Moving the `gy`/`gx` declaration after `invH`/`diag` was a separate stack-order
probe; VC6 emitted identical bytes, so the declaration order is restored.
Moving the shared `rd` declaration before `invW` also emitted identical bytes;
its stack home remained EBP-`0x2c`, so it was restored after `diag`.
Reversing the declaration order to `int gx; int gy = 0;` also emitted
identical code: the candidate remained 2,391/2,340 bytes with 2,153 differing
bytes and instruction-multiset delta 197. The original declaration is
restored. Target `gy`/`gx` homes are EBP-`0x20`/`0x1c`; candidate homes are
EBP-`0x24`/`0x20`, but this order change does not affect those slots.
