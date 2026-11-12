#!/usr/bin/env python3
"""Report game-code progress and refresh analysis/PROGRESS-REPORT.md.

Counts every `// FUNCTION:` (exact match) and `// NONMATCHING:` (written, not yet
byte-exact) marker under src/, weighs functions by their size from
analysis/functions.csv and analysis/functions_extra.csv, and compares them with
all functions in the game code range 0x401000-0x4bcfc6 (library code after it is
not decompiled). FUNCTION markers count as exact because match.py must pass
before every commit; run match.py first if in doubt.

Per-module numbers come from analysis/modules.csv (name,start,end,description;
hex addresses, end exclusive; edit by hand to add modules). Functions in the game range outside those module ranges are reported as "Outside mapped modules". A memory-map and
per-module chart and function map are written under analysis/.

usage: progress.py           print the summary and update analysis/PROGRESS-REPORT.md + progress.svg
       progress.py --print   print only (writes no files)
"""
import csv
import datetime
import glob
import os
import re
import sys
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAME_START, GAME_END = 0x401000, 0x4bcfc7
MARKER = re.compile(r"\b(FUNCTION|NONMATCHING):\s*RALLYTROPHY\s+0x([0-9a-fA-F]+)")
SVG_NAME = "progress.svg"
ANALYSIS_DIR = os.path.join(ROOT, "analysis")
BEGIN, END = "<!-- progress:begin -->", "<!-- progress:end -->"


def game_functions():
    sizes = {}
    for name in ("functions.csv", "functions_extra.csv"):
        for row in csv.DictReader(open(os.path.join(ROOT, "analysis", name))):
            address = int(row["entry"], 16)
            if GAME_START <= address < GAME_END:
                sizes[address] = int(row["size"])
    return sizes


def marked():
    exact, near = set(), set()
    for path in glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True):
        for line in open(path, errors="replace"):
            m = MARKER.search(line)
            if m:
                (exact if m.group(1) == "FUNCTION" else near).add(int(m.group(2), 16))
    return exact, near - exact


def modules():
    mods = []
    for row in csv.DictReader(open(os.path.join(ROOT, "analysis", "modules.csv"))):
        mods.append((row["name"], int(row["start"], 16), int(row["end"], 16)))
    mods.sort(key=lambda m: m[1])
    for a, b in zip(mods, mods[1:]):
        if a[2] > b[1]:
            sys.exit(f"modules.csv: {a[0]} overlaps {b[0]}")
    return mods


def status(addr, exact, near):
    return 2 if addr in exact else 1 if addr in near else 0


def module_stats(sizes, exact, near, mods):
    """Rows of [name, start, end, n, exact_n, near_n, bytes, exact_b, near_b]."""
    rows = [[n, s, e, 0, 0, 0, 0, 0, 0] for n, s, e in mods]
    rest = ["Outside mapped modules", GAME_START, GAME_END, 0, 0, 0, 0, 0, 0]
    for a, size in sizes.items():
        row = rest
        for r in rows:
            if r[1] <= a < r[2]:
                row = r
                break
        st = status(a, exact, near)
        row[3] += 1
        row[6] += size
        if st == 2:
            row[4] += 1
            row[7] += size
        elif st == 1:
            row[5] += 1
            row[8] += size
    return rows, rest


def module_table(rows, rest):
    out = [
        "| Module | Range | Exact / written / total | % exact | % written | |",
        "|---|---|---|---|---|---|",
    ]
    for n, s, e, tn, en, nn, tb, eb, nb in rows + [rest]:
        pe = 100 * eb / tb if tb else 0
        pw = 100 * (eb + nb) / tb if tb else 0
        rng = "" if n == rest[0] else f"`{s:#x}`–`{e:#x}`"
        out.append(
            f"| {n} | {rng} | {en} / {en + nn} / {tn} | {pe:.1f}% | {pw:.1f}% "
            f"| `{bar(pw / 100, 12)}` |"
        )
    return "\n".join(out)


C_EXACT, C_NEAR, C_TODO = "#3fb950", "#e3b341", "#484f58"


def svg(sizes, exact, near, rows, rest):
    W, LM, RM = 1200, 270, 190
    span = GAME_END - GAME_START
    SW = W - 40
    x0 = 20
    px = lambda a: x0 + (a - GAME_START) * SW / span
    nrows = len(rows) + 1
    top = 120
    strip_h = 36
    bars_y = top + strip_h + 70
    H = bars_y + nrows * 26 + 50
    o = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
        'font-family="-apple-system,Segoe UI,Helvetica,Arial,sans-serif" font-size="12">',
        '<title>Rally Trophy decompilation progress</title>',
        f'<rect width="{W}" height="{H}" rx="8" fill="#0d1117"/>',
        '<text x="20" y="28" font-size="16" font-weight="600" fill="#e6edf3">'
        'Memory map of the game code (0x401000 - 0x4bcfc6), one block per function</text>',
        f'<g transform="translate(0,52)" font-size="12" fill="#e6edf3">'
        f'<rect x="20" y="0" width="12" height="12" fill="{C_EXACT}"/><text x="38" y="11">byte-exact</text>'
        f'<rect x="130" y="0" width="12" height="12" fill="{C_NEAR}"/><text x="148" y="11">written, not exact</text>'
        f'<rect x="290" y="0" width="12" height="12" fill="{C_TODO}"/><text x="308" y="11">not written</text></g>',
        f'<rect x="{x0}" y="{top}" width="{SW}" height="{strip_h}" fill="{C_TODO}"/>',
    ]
    # merged runs of adjacent same-status written functions
    runs = []
    for a in sorted(sizes):
        st = status(a, exact, near)
        if not st:
            continue
        end = a + sizes[a]
        if runs and runs[-1][2] == st and a - runs[-1][1] <= 16:
            runs[-1][1] = end
        else:
            runs.append([a, end, st])
    for a, b, st in runs:
        x, w = px(a), max((b - a) * SW / span, 0.6)
        o.append(f'<rect x="{x:.1f}" y="{top}" width="{w:.1f}" height="{strip_h}" '
                 f'fill="{C_EXACT if st == 2 else C_NEAR}"/>')
    # module boundaries, numbered (the numbers match the bar chart below)
    for i, (n, s, e, *_r) in enumerate(rows):
        x = px(s)
        lvl = i % 3
        o.append(f'<line x1="{x:.1f}" y1="{top - 10 - 14 * lvl}" x2="{x:.1f}" y2="{top + strip_h + 4}" '
                 'stroke="#e6edf3" stroke-width="1" opacity="0.6"/>')
        o.append(f'<text x="{x + 2:.1f}" y="{top - 12 - 14 * lvl}" font-size="11" '
                 f'fill="#e6edf3">{i + 1}</text>')
    o.append(f'<text x="{x0}" y="{top + strip_h + 18}" font-size="11" fill="#8b949e">0x401000</text>')
    o.append(f'<text x="{x0 + SW}" y="{top + strip_h + 18}" font-size="11" fill="#8b949e" '
             'text-anchor="end">0x4bcfc6</text>')
    # per-module bars
    o.append(f'<text x="20" y="{bars_y - 16}" font-size="16" font-weight="600" fill="#e6edf3">'
             'Progress by module (share of code bytes)</text>')
    bw = W - LM - RM
    for i, (n, s, e, tn, en, nn, tb, eb, nb) in enumerate(rows + [rest]):
        y = bars_y + i * 26
        label = f"{i + 1}. {n}" if i < len(rows) else n
        o.append(f'<text x="{LM - 10}" y="{y + 14}" text-anchor="end" fill="#e6edf3">{escape(label)}</text>')
        o.append(f'<rect x="{LM}" y="{y}" width="{bw}" height="18" rx="2" fill="{C_TODO}"/>')
        if tb:
            we, wn = bw * eb / tb, bw * nb / tb
            if we:
                o.append(f'<rect x="{LM}" y="{y}" width="{we:.1f}" height="18" fill="{C_EXACT}"/>')
            if wn:
                o.append(f'<rect x="{LM + we:.1f}" y="{y}" width="{wn:.1f}" height="18" fill="{C_NEAR}"/>')
        pe = 100 * eb / tb if tb else 0
        pw = 100 * (eb + nb) / tb if tb else 0
        o.append(f'<text x="{LM + bw + 8}" y="{y + 14}" fill="#e6edf3">{pe:.0f}% exact / {pw:.0f}% written</text>')
    o.append("</svg>")
    return "\n".join(o) + "\n"


def bar(fraction, width=30):
    filled = int(round(fraction * width))
    return "█" * filled + "░" * (width - filled)


def summary():
    """Returns (markdown, svg text)."""
    sizes = game_functions()
    exact, near = marked()
    exact &= set(sizes)
    near &= set(sizes)
    total_n, total_b = len(sizes), sum(sizes.values())
    exact_n, exact_b = len(exact), sum(sizes[a] for a in exact)
    near_n, near_b = len(near), sum(sizes[a] for a in near)
    written_n, written_b = exact_n + near_n, exact_b + near_b
    mods = modules()
    rows, rest = module_stats(sizes, exact, near, mods)
    date = datetime.date.today().isoformat()
    lines = [
        f"_Updated {date} by `tools/progress.py` (game code `0x401000`–`0x4bcfc6`)._",
        "",
        "| | Functions | Code bytes |",
        "|---|---|---|",
        f"| Byte-exact | {exact_n} / {total_n} ({100 * exact_n / total_n:.2f}%) "
        f"| {exact_b:,} / {total_b:,} ({100 * exact_b / total_b:.2f}%) |",
        f"| Written, not yet exact | {near_n} | {near_b:,} |",
        f"| Written in total | {written_n} / {total_n} ({100 * written_n / total_n:.2f}%) "
        f"| {written_b:,} / {total_b:,} ({100 * written_b / total_b:.2f}%) |",
        "",
        "```",
        f"exact   {bar(exact_b / total_b)} {100 * exact_b / total_b:5.2f}% of code bytes",
        f"written {bar(written_b / total_b)} {100 * written_b / total_b:5.2f}% of code bytes",
        "```",
        "",
        f"![Decompilation progress]({SVG_NAME})",
        "",
        "Progress by module (ranges in `analysis/modules.csv`):",
        "",
        module_table(rows, rest),
    ]
    return "\n".join(lines), svg(sizes, exact, near, rows, rest)


def main(argv):
    text, image = summary()
    print(text)
    if "--print" in argv:
        return 0
    with open(os.path.join(ANALYSIS_DIR, SVG_NAME), "w") as f:
        f.write(image)
    readme = os.path.join(ANALYSIS_DIR, "PROGRESS-REPORT.md")
    content = open(readme).read()
    if BEGIN not in content or END not in content:
        sys.exit(f"{readme} has no {BEGIN} ... {END} block")
    head, rest = content.split(BEGIN, 1)
    _, tail = rest.split(END, 1)
    open(readme, "w").write(f"{head}{BEGIN}\n{text}\n{END}{tail}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
