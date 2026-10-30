# Sound-menu frame (`0x451460`)

Local PE disassembly and bounded COFF comparison observations: the retained
object span is 1,968 bytes against the 1,915-byte inventory body, with 399
differing bytes. The draw loop has the target control flow, but its point
temporaries and operand/register schedules differ. The target dossier is
`ev_de7021ab85570c998fe8cc974afda1a5d1367531432db8bee6b240f7bc7d732f`.

The target stores the `GetScale` copy at ESP `+0x4c/+0x50` and the unit point
at `+0x44/+0x48`; the candidate swaps these slots. The candidate also hoists
the unused unit-point stores outside the loop without volatile stores, while
the target emits them on each iteration. The target unit stores occur after
the inline string pointer test but before its branch.

Measured probes, all reverted unless noted:

- An inline unit-point constructor produced 1,988 bytes / 808 differences.
- Passing the unit point's fields to the existing scalar Print declaration,
  or initializing the point through an inline reference helper, left the
  earlier 1,968-byte / 407-difference output unchanged.
- A file-local Print declaration taking `HudPoint` by value produced 1,980
  bytes / 412 differences. It constructed an eight-byte stack argument and
  called `c_str` out of line, unlike the target's immediate scale pushes and
  inline string access.
- Inline title-scaling accumulators, both generic and specialized for the two
  constants, produced 1,976 bytes / 409 differences.
- The retained candidate preloads `labelText` and uses volatile unit stores;
  this improved 407 to 402 differences. Four mutable static floats, or only
  a mutable slider-width static, each regressed to 405.
- Splitting the scale copy into declaration plus assignment produced 406;
  direct `c_str` without the early pointer produced 408.
- A device local declared before the title and used for the title produced
  406. It fixed scale-copy registers, row argument order, and slider case
  register scheduling, but disturbed the title region.
- Moving that device local immediately after the title produced 411; moving
  it after the scale copy produced 409. Declaring it before the title but
  using it only for row printing produced 407.
- Moving volatile unit stores before the early `c_str` produced 408. Moving
  the unit point to function scope before the scale copy produced 406 and
  changed EH states without fixing the slots.
- A two-point aggregate ordered as unit then scale produced 414, moved the
  points to ESP `+0x74/+0x7c`, and disturbed earlier input temporaries.
- Swapping the loop-local `Color col` and `HudPoint one` declarations was
  code-neutral at 402; the retained source keeps `Color` first.
- A volatile title-device read delayed the global load but regressed to 405.
  Making the unused scale copy `const` was code-neutral at 402.
- Splitting `c_str()` into an early layout-twin pointer load and a later
  fallback branch moved the unit stores before the pointer test, but regressed
  to 409 and disturbed loop register allocation. Ordinary unit stores before
  a direct `c_str()` argument instead hoisted the stores out of the loop and
  produced the earlier 407-difference shape.

A read-only sibling review found the same scale/unit slot mismatch in five
other menu Frame implementations, so this is a shared reconstruction issue,
not a SoundMenu-specific declaration typo. The closest analogues are
`RaceOptionsMenu::Frame`, `MenuScreenA48::Frame`, and `MenuScreen2A8::Frame`.
Their convergent retained order is `selected`, `Color`, unit point, early
`labelText`, volatile unit stores, Print, then `pos`.

The baseline remains NONMATCHING. The focused module check passes all eight
exact-marked functions, including the separately verified layout loader.
