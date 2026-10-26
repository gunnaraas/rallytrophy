# x87 control-word helpers

## Evidence

- Pseudocode Evidence `ev_d0b6719c7d83b826b9cb45809de29361342048eec27c4f59491a9d6447079cd7` labels `0x4a7d90` as an empty procedure, but its assembly (`ev_15de38c04994cace398ec5caf1449630bf858933fc95ceb58ca77aa1ab07a018`) shows it loads the caller-supplied 16-bit x87 control word with `FLDCW` before returning. The 8-byte entry is reproduced exactly.
- Pseudocode Evidence `ev_93f73f3d5a763fd6ea83c73b0cf4643043c1fdfc168eb839249f1385f94ab661` describes `0x4a7da0` as returning the saved high word from `0x577bc4` combined with the current x87 control word. Assembly (`ev_deba2defda5bbc5bca9e598ca92b985596f0cd0fb307a5c42e9e8ae8b3c10a35`) confirms it snapshots the current control word, optionally loads the configured default from `0x577bc0` when `0x577bc4` is nonzero, then returns the snapshot. The 31-byte entry has two relocations.

Both functions are isolated in `src/model_triangle_cache.cpp` and verified by the focused object matcher. Their names here describe the observed operation.

## Coefficient interpolation helper

Pseudocode Evidence `ev_ef1331a9fcebd2c3a3778758ae369a9ebd068981d82fcc087a7eb8e705f997d2` identifies `0x4a7f30` as a four-sample weighted expression using three coefficients stored at `this+0/+4/+8`, then adding the third sample. Assembly `ev_60c3f3752a624b6c832313919c365ab816e754ed00e03713c9d9eec4fd79a658` preserves the x87 instruction order and reads constants at `0x4e1f30`, `0x4e138c`, `0x4e1530`, and `0x4e1eb8`. The 105-byte body with six relocations is reproduced by `SplineCoefficients::EvaluateExact` in `src/model_triangle_cache.cpp`. The specific interpolation family is not identified here; the name refers only to its coefficient inputs.
