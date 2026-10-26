#!/usr/bin/env python3
"""Print a compact digest of a saved REA MCP tool-result JSON file.

Usage: rea_digest.py <path> [section ...]
Sections: identity, pseudocode, callees, callers, strings, comments,
          refs, blocks, api, info, flow, all (default: identity pseudocode)
The tool-result files live under the session's tool-results/ directory.
"""
import json
import sys


def load(path: str):
    raw = open(path, encoding="utf-8", errors="replace").read()
    dec = json.JSONDecoder()
    d, _ = dec.raw_decode(raw[raw.find("{"):])
    return d.get("result") or d


def main() -> None:
    path = sys.argv[1]
    sections = sys.argv[2:] or ["identity", "pseudocode"]
    d = load(path)
    nr = d.get("normalized_result")
    op = d.get("operation")
    print(f"# operation={op} evidence={d.get('evidence_id', '')[:20]}…")
    for sec in sections:
        if sec == "identity":
            sub = d.get("subject", {})
            print("## subject:", sub.get("name"), sub.get("digest", {}).get("sha256", "")[:16])
            if isinstance(nr, dict) and "procedure" in nr:
                print("## procedure:", json.dumps(nr["procedure"])[:300])
        elif sec == "pseudocode":
            if isinstance(nr, dict):
                v = nr.get("pseudocode")
            else:
                v = nr
            if isinstance(v, str):
                print(v)
            else:
                print("## pseudocode: <none>")
        elif sec == "info":
            if isinstance(nr, dict):
                for k in ("body_ranges", "signature", "locals", "size", "block_count"):
                    if k in nr:
                        print(f"## {k}:", json.dumps(nr[k])[:800])
        else:
            if isinstance(nr, dict) and sec in nr:
                print(f"## {sec}:", json.dumps(nr[sec])[:3000])


if __name__ == "__main__":
    main()
