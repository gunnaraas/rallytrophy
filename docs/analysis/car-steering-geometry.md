# Car steering geometry (`0x45ad40`)

REA Evidence `ev_44efd80de74fa57abbe051afd7c89aabb5cbbfca52bc9ff6c566f2d37ba828ec`
identifies a contiguous 1,404-byte `FUN_0045ad40` body with an EBP frame of
`0x20` bytes and no SEH. The assembly shows the spring-reference and base-angle
values being stored in frame slots around both the front-toe and rear-camber
branches. The behavior map is in
[`../../analysis/range-459890.md`](../../analysis/range-459890.md).

## Matching probes

The existing source compiled to 1,405 bytes with 731 differing bytes outside
relocations. Making the `Camber` helper's rear reference height volatile
produced 1,402 bytes / 696 differing bytes. This is the best tested source form
so far and remains `NONMATCHING`.

The `Toe` helper's front reference height alone made 1,402 / 1,027 differences;
making both reference heights volatile gave 1,399 / 1,024. Volatile output
locals for the front-toe calculations were also worse: `x` produced 1,402 / 1,045
and `y` produced 1,408 / 935. Introducing a shared `one` local for repeated
`1.0f` expressions left the 1,405 / 731 result unchanged. These variants were
reverted. Making the rear base-angle value volatile worsened the rear-only
candidate to 1,405 / 720. A volatile `Toe` base parameter and a volatile local
home for that base both worsened the best candidate to 1,396 bytes / 992 and
993 differences, respectively. Sharing the `toeBase` field through one local
for both front wheels compiled to 1,389 / 1,047 differences; making that local
volatile gave 1,407 / 1,011. Focused matcher status for this source is 9 exact
functions and 5 `NONMATCHING`, with no compile failures.

Factoring the shared toe-base addition out of the `Toe` branch join produced
1,378 bytes / 1,053 differences, worse than the 1,402 / 696 candidate, and was
reverted.

Forcing only a one-time volatile store of the front-reference value into a
regular local produced 1,410 bytes / 1,018 differences, also worse than the
best candidate; the direct local load was restored.

Adding one volatile home for the `Toe` spring parameter produced 1,395 bytes /
1,064 differences, worse than the 1,402 / 696 candidate. Although the target
homes a spring value during the Toe branch (`0x45ae53`–`0x45ae56`), making the
shared helper's input volatile changes both call paths and does not reproduce
that selective lifetime. The ordinary parameter remains. Evidence:
`ev_705298a690bfc9e4c71743852d838e00e3ce4bb6150eb34ed10bf91c54529d1d`.
