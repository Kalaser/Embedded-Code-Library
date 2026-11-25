#!/usr/bin/env python3
"""Validate the Embedded Code Library scaffold; no third-party dependencies."""
from pathlib import Path
import json
import sys

ROOT = Path(__file__).resolve().parents[1]
REQUIRED_DIRS = [
    "catalog", "components", "templates", "docs", "third_party",
    "knowledge", "examples", "tools"
]
REQUIRED_FILES = ["README.md", "CATALOG.md", "CHANGELOG.md", "LICENSE_POLICY.md", "CMakeLists.txt"]
REQUIRED_COMPONENT_FIELDS = ["id", "name", "category", "repository", "status", "license"]

errors = []
for rel in REQUIRED_DIRS:
    if not (ROOT / rel).is_dir():
        errors.append(f"Missing directory: {rel}")
for rel in REQUIRED_FILES:
    if not (ROOT / rel).is_file():
        errors.append(f"Missing file: {rel}")

manifest_path = ROOT / "catalog" / "components.json"
try:
    data = json.loads(manifest_path.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or not isinstance(data.get("components"), list):
        errors.append("catalog/components.json must contain a 'components' array")
    else:
        seen = set()
        for idx, item in enumerate(data["components"]):
            if not isinstance(item, dict):
                errors.append(f"Component entry {idx} is not an object")
                continue
            for field in REQUIRED_COMPONENT_FIELDS:
                if field not in item:
                    errors.append(f"Component entry {idx} missing '{field}'")
            ident = item.get("id")
            if ident in seen:
                errors.append(f"Duplicate component id: {ident}")
            seen.add(ident)
            url = item.get("repository", "")
            if url and not str(url).startswith("https://"):
                errors.append(f"Component {ident}: repository must use https://")
            if item.get("status") != "catalog-only":
                if not item.get("version_pin"):
                    errors.append(f"Component {ident}: non-catalog entry requires version_pin")
except Exception as exc:
    errors.append(f"Cannot parse component manifest: {exc}")

# Ensure all documentation links to local paths are not accidentally dangling.
for rel in ["README.md", "CATALOG.md", "docs/ARCHITECTURE.md", "docs/COMPONENT_CONTRACT.md"]:
    p = ROOT / rel
    if p.exists() and p.stat().st_size == 0:
        errors.append(f"Empty documentation file: {rel}")

if errors:
    print("Embedded Code Library validation: FAIL")
    for error in errors:
        print(f" - {error}")
    sys.exit(1)

print("Embedded Code Library validation: PASS")
print(f" - Components in catalog: {len(data['components'])}")
print(" - This check does not verify network availability, licenses, buildability of upstream projects, or hardware behavior.")
