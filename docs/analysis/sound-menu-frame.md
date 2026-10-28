# Sound-menu frame (`0x451460`)

Local PE disassembly and bounded COFF comparison observations: the current
object span is 1,968 bytes against the 1,915-byte inventory body, with 407
differing bytes. The draw loop has the target control flow, but its point
temporaries and operand/register schedules differ.

The target stores the `GetScale` copy at ESP `+0x4c/+0x50` and the unit point
at `+0x44/+0x48`; the candidate swaps these slots. The candidate also hoists
the unused unit-point stores outside the loop, while the target emits them
on each iteration.

Measured probes, all reverted:

- An inline unit-point constructor produced 1,988 bytes / 808 differences.
- Passing the unit point's fields to the existing scalar Print declaration,
  or initializing the point through an inline reference helper, left the
  1,968-byte / 407-difference output unchanged.
- A volatile unit point kept its stores inside the loop, but retained the
  wrong slots and store timing, producing 408 differences.
- A file-local Print declaration taking `HudPoint` by value produced 1,980
  bytes / 412 differences. It constructed an eight-byte stack argument and
  called `c_str` out of line, unlike the target's immediate scale pushes and
  inline string access. No new renderer declaration or alias was retained.
- Inline title-scaling accumulators, both generic and specialized for the two
  constants, produced 1,976 bytes / 409 differences. Both were reverted.

The baseline remains NONMATCHING. The focused module check still passes all
seven exact-marked functions, including the separately verified layout loader.
