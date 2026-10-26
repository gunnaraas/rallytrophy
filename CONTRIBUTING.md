# Contributing

This project reconstructs functions from the official Rally Trophy 1.01
executable and checks compiled x86 code against a locally supplied copy.
Please read the project [README](README.md),
[architecture overview](docs/architecture/overview.md), and
[matching notes](analysis/NOTES.md) before making changes.

## Matching changes

1. Check `analysis/PROGRESS.md` for current priorities and existing
   work. Open an issue or draft PR early for a larger range so work does
   not overlap. (`analysis/COORDINATION.md` is a local, untracked board for
   agents sharing one checkout.)
2. Use the original function address as its stable identifier. Put REA Evidence
   IDs in analysis notes for non-trivial binary claims; label plain observations
   and inferences clearly.
3. Make one small source change at a time. A `FUNCTION` marker means that
   function passed the byte matcher; use `NONMATCHING` while it still differs.
   Neither marker by itself certifies runtime behavior.
4. Run a focused matcher check while iterating. Before recording a completed
   change, run the full matcher and refresh the generated progress report:

   ```sh
   python3 -I tools/match.py --quiet src/your_file.cpp
   python3 -I tools/match.py --quiet
   python3 -I tools/progress.py
   ```

5. Keep commits limited to the files you changed and verified. The worktree is
   shared during active analysis; preserve other contributors' edits and claims.

## Reference material

Do not add game executables, DLLs, disc images, extracted assets, or unreviewed
binary-analysis dumps to a change. The matcher requires a locally supplied
official executable, which is ignored by Git. See the root README for setup and
the license. Contributions are accepted under the GNU GPL v3 (`LICENSE`).
