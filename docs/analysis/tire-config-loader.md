# Tire configuration loader (`0x4634e0`)

REA instruction evidence
`ev_f34df2e54dac5785961eb4b177397d9b94bee907864cccd72be02b175cf7e745`
reports the contiguous 1,115-byte body and its helper calls. Local official-PE
and COFF observations establish the comparisons below.

The initial draft was 1,122 bytes with 548 unmasked differences. Its error
path expanded the runtime-error message copy's Tidy, hoisting zero into ebx;
its loop also anchored the surface pointer at record+8 instead of record+0x18.

Two discarded inline-budget calls at the end of the function retain the target
string-copy boundaries: runtime_error and string construction still expand,
but Tidy and copy-string assign remain calls. The actual std::runtime_error
type and standard header definition are preserved. One discarded call had no
effect; three produce the same body as two. Calls before throw and scoped
inline-depth probes did not change the baseline. A factory returning the real
exception type changed calls/lifetimes and was rejected, as was a named message
whose frame and copy behavior grew substantially.

A persistent byte cursor is anchored at the third z-stiffness word, record+0x18,
and advances by sizeof(TyreSurface), 0x40. Scalar and word stores retain the
original addresses. Declaring the loop index before the cursor preserves the
original initialization order. Aggregate copies produced extra address setup;
copy helpers and volatile stores worsened scheduling. The byte cursor avoids
using a float-array pointer to access neighboring fields.

The final seven-byte schedule difference came from the three z-stiffness
stores. Keeping those stores behind a typed `TyreF3*` causes VC6 to prepare the
next config call's `this` address before the middle word store, matching the
target instruction order. The resulting body is an exact 1,115-byte match
with 62 relocation fields. Focused validation reports 20 exact functions,
four NONMATCHING functions, and no failures.

```text
target:     lea ecx,[esp+0x18]; mov [esi-4],edx
matched:    lea ecx,[esp+0x18]; mov [esi-4],edx
```

No unmasked byte differences remain. The changed stores retain the same
addresses and values as the earlier byte-cursor form.

All 45 TireKeys names and types agree with the original table at 0x4fac70.
Direct-call operands agree with the original helper destinations, including
Tidy at 0x402700 and copy-string assign at 0x4024f0. Five formerly unbound
PathString/TmpString/MissingText aliases now resolve to existing standard-string
definitions, with compile-time string-layout checks. MissingText retains the
cdecl hidden-result ABI of operator+(string,const char*).

The exact marker is backed by the focused matcher and object comparison. The
call-boundary and key-table audits above remain applicable; a linked game and
complete exception metadata have not been separately verified.
