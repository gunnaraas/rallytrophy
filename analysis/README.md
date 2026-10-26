# Decomp analysis index

This directory tracks the byte-matching reconstruction. The generated
[progress report](PROGRESS-REPORT.md) and the root project README provide the overview;
the [active queue and work log](PROGRESS.md) record current status.

## Inventories and workflow

- [`functions.csv`](functions.csv) and
  [`functions_extra.csv`](functions_extra.csv): original function addresses,
  sizes, and names/notes used by the matcher and progress generator.
- [`modules.csv`](modules.csv): address ranges grouped by subsystem.
- [`NOTES.md`](NOTES.md): target executable, compiler, matching environment,
  and code-generation conventions.
- `COORDINATION.md`: local, untracked board for agents sharing one checkout.

## Range maps

The `range-<address>.md` files describe the functions and source files in an
address range. Start with the range matching the function address, or consult
`modules.csv` to find a subsystem's range.

Topic-specific analysis in this directory includes
[`mesh-triangle-cache.md`](mesh-triangle-cache.md),
[`model-name-state.md`](model-name-state.md),
[`racemode-base.md`](racemode-base.md),
[`file-open.md`](file-open.md),
[`keyboard-hook.md`](keyboard-hook.md),
[`mouse-hook.md`](mouse-hook.md),
[`fpu-control.md`](fpu-control.md), and
[`list-helpers.md`](list-helpers.md).

`NOTES-archive.md` and `PROGRESS-archive.md` preserve historical snapshots;
the un-suffixed files are the current notes and queue.

## Local analysis artifacts

Large REA result dumps and scratch experiments belong under `tool-results/`
or `analysis/scratches/`; those local artifacts are ignored by Git. Some
historical evidence snapshots are tracked here because analysis notes cite
them. Parse new REA output with the repository's
[REA digest helper](../tools/rea_digest.py); do not add raw dumps.
