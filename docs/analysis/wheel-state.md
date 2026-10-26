# Wheel state (1.01)

## Reset (`0x46c6b0`)

The 214-byte routine clears its state with integer zero stores. The first
three stores clear the vector at +0x2cc/+0x2d0/+0x2d4 in z/y/x order; only
then does it load `0x3f800000` into EDX. It uses that value to set +0x32c
and the normal's y component at +0x2b8 to `1.0f`, and sets the normal's
x/z components to zero. The final two stores set +0x2c0 to `0.005f`
(`0x3ba3d70a`) and +0x2c4 to `2.0f` (`0x40000000`). It makes no calls.
Assembly and pseudocode evidence:
`ev_0c15dff8d085af1e417247cb7b4733d3841de445d06b0ae4852cb5506bd89e1e`.

Source/matcher observation: the existing field values and store order were
already correct. Giving the first three zero stores a volatile integer view
keeps the constant load at its observed position and matches all 214 bytes.
This verifies instruction bytes, not runtime vehicle dynamics or the complete
meaning of the unnamed fields. Existing layout mapping is in
`analysis/range-46a8a0.md`, section 4.
