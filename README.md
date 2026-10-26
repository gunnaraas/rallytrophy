# Rally Trophy Decomp

An ongoing, function-by-function C++ reconstruction of first-party code in
Rally Trophy 1.01 for Windows. Original addresses identify functions; the
project's matcher compares locally compiled object code with the shipped PE.
This is a source research project, not a playable port or a release of the game.

## AI-assisted

Most of this reconstruction was written by AI coding agents working under
human direction. Correctness is established by the byte-exact
matcher against the shipped executable, not by review of the source. Function
names, type layouts, and analysis notes outside matched functions are
inferences. See [`AGENTS.md`](AGENTS.md) for the working rules the agents follow.

## Status

The current function inventory and module breakdown are generated in
[`analysis/PROGRESS-REPORT.md`](analysis/PROGRESS-REPORT.md). The active
byte-matching queue is [`analysis/PROGRESS.md`](analysis/PROGRESS.md); function evidence
and readable findings are indexed in [`docs/README.md`](docs/README.md).

The progress report measures source-marker coverage and byte matches. “Written”
means a source body exists; it does not certify behavioral equivalence.
Third-party library ranges are listed separately from the first-party queue.

![Per-function decompilation map](analysis/function-map.svg)

Each tile represents one function. Tile area combines code size with branch
complexity; hover for the address, module, byte count, and branch count.

## Set up the matcher

The matcher uses the original Visual C++ 6.0 SP5 compiler and the official
Patch 1.01 executable. Neither the compiler toolchain nor game files are
included in this repository. The local setup currently uses Wine and a
`rt-vc6` Distrobox; see [`analysis/NOTES.md`](analysis/NOTES.md)
for the compiler details.

1. Use your own copy of the Rally Trophy disc. From the repository root,
   extract it with 7-Zip:

   ```sh
   7z x "Rally Trophy.iso" -oextracted
   ```

2. Copy the official installed executable into the ignored reference folder:

   ```sh
   mkdir -p orig
   cp "extracted/Program Files/JoWooD/Rally Trophy/RallyTrophy.exe" orig/RallyTrophy.exe
   ```

3. Set up the local VC6 SP5 toolchain and `rt-vc6` Distrobox as described in
   the analysis notes. The REA/Ghidra setup is optional for compiling and
   matching, but is needed for new binary analysis.

## Check a change

From the repository root, run a focused check for the source file you changed:

```sh
python3 -I tools/match.py --quiet src/your_file.cpp
```

Before committing a completed iteration, run the full matcher and refresh the
progress block:

```sh
python3 -I tools/match.py --quiet
python3 -I tools/progress.py
```

## Layout

```text
src/             reconstructed C++ functions
include/         reconstructed types and layouts
analysis/       function maps, claims, and matcher progress
docs/analysis/  subsystem evidence and findings
tools/          matcher, compiler helpers, and analysis utilities
orig/           locally supplied reference executable (gitignored)
extracted/      locally supplied game data (gitignored)
```

## Reference files and rights

Do not commit an ISO, executable, DLL, asset archive, or extracted game data.
These files are excluded by `.gitignore`; each contributor supplies their own
reference copy. Rally Trophy and its original code and assets belong to their
respective rights holders. The reconstructed source and tooling in this
repository are licensed under the GNU GPL v3 ([`LICENSE.md`](LICENSE.md)); that
license does not cover the original game, its code, or its assets.

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for the claim, evidence, and matching
workflow.
