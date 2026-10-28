# Wheel spin matrix (`0x46c550`)

`Wheel::UpdateSpinMatrix` advances the spin angle, wraps it with `fmod`, builds
the steer and spin matrices, and multiplies them into the tire object's matrix.
The function is 352 bytes. Its target instructions were reviewed from the
shipped PE; the broader field and call map is in
[`analysis/range-46a8a0.md`](../../analysis/range-46a8a0.md). These are local
disassembly observations, not REA pseudocode.

The target stores the integrated angle to the wheel before calling `fmod`.
After the call, it stores the result to the local `a` and then back to the
wheel. The previous C++ form reproduced the first store but emitted those last
two stores in the opposite order. Assigning the integrated value through a
volatile lvalue preserves its memory store; nesting the `a` assignment inside
the `angle` assignment gives the required post-call order:

```cpp
*(volatile float*)&angle = dt * spin + angle;
float a;
angle = (a = WrapAngle(angle));
```

The matcher confirms an exact 352-byte object body with 10 relocation fields.
Focused validation reports nine exact functions, two NONMATCHING functions,
and no failures. The straightforward split form (`float a = WrapAngle(angle);
angle = a;`) removes the target's pre-call store and produces a 346-byte body.
