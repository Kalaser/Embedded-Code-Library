#!/usr/bin/env python3
"""Generate a minimal C or C++ component skeleton."""
from pathlib import Path
import argparse
import re

ROOT = Path(__file__).resolve().parents[1]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("name", help="component name, e.g. device_registry")
    parser.add_argument("--language", choices=["c", "cpp"], default="c")
    parser.add_argument("--description", default="Reusable embedded component")
    args = parser.parse_args()

    name = args.name.lower()
    if not re.fullmatch(r"[a-z][a-z0-9_]*", name):
        parser.error("name must match [a-z][a-z0-9_]*")

    dest = ROOT / "components" / name
    if dest.exists():
        parser.error(f"destination already exists: {dest}")

    ext = "c" if args.language == "c" else "cpp"
    guard = f"ECL_{name.upper()}_H"
    symbol = f"ecl_{name}"
    (dest / "include").mkdir(parents=True)
    (dest / "src").mkdir()
    (dest / "tests").mkdir()

    (dest / "include" / f"{symbol}.h").write_text(
        f"#ifndef {guard}\n#define {guard}\n\nint {symbol}_init(void);\n\n#endif /* {guard} */\n", encoding="utf-8")
    (dest / "src" / f"{symbol}.{ext}").write_text(
        f'#include "{symbol}.h"\n\nint {symbol}_init(void)\n{{\n    return 0;\n}}\n', encoding="utf-8")
    (dest / "tests" / "README.md").write_text(
        "Add host-side unit tests and target integration tests here.\n", encoding="utf-8")
    (dest / "README.md").write_text(
        f"# {name}\n\n{args.description}\n\n- Language: {args.language.upper()}\n- Status: draft\n- Portability class: declare P0-P4\n- Thread safety: document explicitly\n- Memory ownership: document explicitly\n- Validation: add tests and target evidence\n", encoding="utf-8")
    print(f"Created: {dest.relative_to(ROOT)}")
    print("Next: add a component CMakeLists.txt, metadata, tests, and update the root build.")

if __name__ == "__main__":
    main()
