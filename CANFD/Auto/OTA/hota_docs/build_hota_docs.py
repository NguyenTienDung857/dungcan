"""
Build stable H-OTA documentation from local evidence.

Inputs:
- installed H-OTA GEN2 files under Program Files
- current H-OTA runtime windows, if open
- existing Auto/OTA Python runners
- scan_* folders produced by scan_hota_ui.py

Outputs are written to Auto/OTA/hota_docs as Markdown + JSON artifacts.
"""

from __future__ import annotations

import configparser
import csv
import json
import os
import re
import sys
from collections import Counter, defaultdict
from datetime import datetime
from pathlib import Path
from xml.etree import ElementTree as ET

import win32gui


BASE_DIR = Path(__file__).resolve().parents[1]
DOC_DIR = BASE_DIR / "hota_docs"
INSTALL_DIR = Path(r"C:\Program Files (x86)\GIT\H-OTA Studio GEN2")
SHORTCUT_PATH = BASE_DIR / "H-OTA Studio.lnk"


def write_json(path: Path, data) -> None:
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False), encoding="utf-8")


def local_name(tag: str) -> str:
    return tag.rsplit("}", 1)[-1] if "}" in tag else tag


def safe_read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def list_files(root: Path, patterns: tuple[str, ...] | None = None) -> list[dict]:
    rows = []
    if not root.exists():
        return rows
    for path in sorted(p for p in root.rglob("*") if p.is_file()):
        if patterns and not any(path.match(pattern) for pattern in patterns):
            continue
        try:
            stat = path.stat()
        except OSError:
            continue
        rows.append(
            {
                "path": str(path),
                "relative_path": str(path.relative_to(root)),
                "size": stat.st_size,
                "modified": datetime.fromtimestamp(stat.st_mtime).isoformat(timespec="seconds"),
            }
        )
    return rows


def parse_ini(path: Path) -> dict:
    parser = configparser.ConfigParser(strict=False)
    parser.optionxform = str
    text = safe_read_text(path)
    parser.read_string(text)
    return {section: dict(parser.items(section)) for section in parser.sections()}


def parse_property_xml(path: Path) -> list[dict]:
    rows = []
    if not path.exists():
        return rows
    try:
        root = ET.parse(path).getroot()
    except ET.ParseError as exc:
        return [{"error": str(exc), "file": str(path)}]

    def walk(node, lineage: list[str]) -> None:
        name = local_name(node.tag)
        next_lineage = lineage
        if name != "property":
            label = node.attrib.get("name") or name
            next_lineage = lineage + [label]
        if name == "property":
            rows.append(
                {
                    "file": path.name,
                    "section_path": "/".join(lineage),
                    "name": node.attrib.get("name", ""),
                    "type": node.attrib.get("type", ""),
                    "conventions": node.attrib.get("conventions", ""),
                    "unit": node.attrib.get("unit", ""),
                    "length": node.attrib.get("length", ""),
                    "default": node.attrib.get("default", ""),
                    "value": node.attrib.get("value", ""),
                    "comment": node.attrib.get("comment", ""),
                }
            )
        for child in list(node):
            walk(child, next_lineage)

    walk(root, [])
    return rows


def parse_xsd_summary(path: Path) -> dict:
    result = {"file": str(path), "elements": [], "complex_types": [], "simple_types": []}
    try:
        root = ET.parse(path).getroot()
    except ET.ParseError as exc:
        result["error"] = str(exc)
        return result
    for elem in root.iter():
        lname = local_name(elem.tag)
        name = elem.attrib.get("name")
        if not name:
            continue
        if lname == "element":
            result["elements"].append(name)
        elif lname == "complexType":
            result["complex_types"].append(name)
        elif lname == "simpleType":
            result["simple_types"].append(name)
    for key in ("elements", "complex_types", "simple_types"):
        result[key] = sorted(set(result[key]))
    return result


def parse_vehicle_info(path: Path) -> dict:
    if not path.exists():
        return {}
    data = json.loads(safe_read_text(path))
    payload = data.get("data", {}).get("data", {})
    cars = payload.get("carList", []) or []
    return {
        "success": data.get("success"),
        "status_code": data.get("data", {}).get("statusCode"),
        "car_count": len(cars),
        "first_40_cars": cars[:40],
    }


def enumerate_windows() -> dict:
    top = []

    def top_cb(hwnd, sink):
        if not win32gui.IsWindowVisible(hwnd):
            return
        title = win32gui.GetWindowText(hwnd)
        if not title:
            return
        try:
            rect = win32gui.GetWindowRect(hwnd)
        except Exception:
            rect = (0, 0, 0, 0)
        sink.append(
            {
                "hwnd": hwnd,
                "title": title,
                "class_name": win32gui.GetClassName(hwnd),
                "rect": list(rect),
            }
        )

    win32gui.EnumWindows(top_cb, top)
    hota = [w for w in top if "H-OTA Studio" in w["title"] or "ReprogramEditor" in w["title"]]
    output = [w for w in top if w["title"] == "Output"]

    def children(hwnd: int) -> list[dict]:
        rows = []

        def child_cb(child, sink):
            try:
                rect = win32gui.GetWindowRect(child)
            except Exception:
                rect = (0, 0, 0, 0)
            try:
                ctrl_id = win32gui.GetDlgCtrlID(child)
            except Exception:
                ctrl_id = None
            sink.append(
                {
                    "hwnd": child,
                    "id": ctrl_id,
                    "title": win32gui.GetWindowText(child),
                    "class_name": win32gui.GetClassName(child),
                    "rect": list(rect),
                }
            )

        win32gui.EnumChildWindows(hwnd, child_cb, rows)
        return rows

    return {
        "top_windows": top,
        "hota_windows": [{**w, "children": children(w["hwnd"])} for w in hota],
        "output_windows": [{**w, "children": children(w["hwnd"])} for w in output],
    }


def parse_runner_scripts() -> dict:
    selector_re = re.compile(r"child_window\((.*?)\)", re.DOTALL)
    auto_id_re = re.compile(r"auto_id\s*=\s*[\"']([^\"']+)[\"']")
    title_re = re.compile(r"title(?:_re)?\s*=\s*[\"']([^\"']+)[\"']")
    control_type_re = re.compile(r"control_type\s*=\s*[\"']([^\"']+)[\"']")
    send_keys_re = re.compile(r"send_keys\((.*?)\)")
    action_words = [
        "TesterMode",
        "Gateway Type",
        "SourceAddr",
        "Wireless-Rules",
        "Wired-Rules",
        "Apply",
        "Start",
        "Output",
        "AppState",
        "Add Activation",
        "Add Rollback",
        "OEUK",
        "ERT",
    ]

    files = sorted(BASE_DIR.glob("*.py"))
    selectors = []
    flows = []
    action_hits = defaultdict(list)
    for path in files:
        text = safe_read_text(path)
        for match in selector_re.finditer(text):
            body = " ".join(match.group(1).split())
            selectors.append(
                {
                    "file": path.name,
                    "line": text[: match.start()].count("\n") + 1,
                    "selector": body,
                    "auto_ids": auto_id_re.findall(match.group(1)),
                    "titles": title_re.findall(match.group(1)),
                    "control_types": control_type_re.findall(match.group(1)),
                }
            )
        for match in send_keys_re.finditer(text):
            selectors.append(
                {
                    "file": path.name,
                    "line": text[: match.start()].count("\n") + 1,
                    "send_keys": " ".join(match.group(1).split()),
                }
            )
        header = []
        for raw_line in text.splitlines()[:40]:
            if raw_line.startswith("#   ") or raw_line.startswith("#      "):
                header.append(raw_line.lstrip("# ").rstrip())
        if header:
            flows.append({"file": path.name, "flow": header})
        for word in action_words:
            if word in text:
                action_hits[word].append(path.name)

    auto_ids = Counter()
    titles = Counter()
    control_types = Counter()
    for row in selectors:
        for value in row.get("auto_ids", []):
            auto_ids[value] += 1
        for value in row.get("titles", []):
            titles[value] += 1
        for value in row.get("control_types", []):
            control_types[value] += 1

    return {
        "script_count": len(files),
        "selectors": selectors,
        "flows": flows,
        "auto_id_counts": dict(auto_ids.most_common()),
        "title_counts": dict(titles.most_common()),
        "control_type_counts": dict(control_types.most_common()),
        "action_hits": {key: sorted(set(value)) for key, value in action_hits.items()},
    }


def scan_runs() -> list[dict]:
    rows = []
    for run_dir in sorted(DOC_DIR.glob("scan_*")):
        if not run_dir.is_dir():
            continue
        files = []
        for path in sorted(p for p in run_dir.rglob("*") if p.is_file()):
            files.append(str(path.relative_to(run_dir)))
        rows.append({"run_dir": str(run_dir), "files": files})
    return rows


def latest_complete_scan_dir() -> Path | None:
    candidates = []
    for run_dir in DOC_DIR.glob("scan_*"):
        if not run_dir.is_dir():
            continue
        if (run_dir / "ui_tree_after_tester_mode.json").exists() and (run_dir / "action_log.json").exists():
            candidates.append(run_dir)
    return sorted(candidates)[-1] if candidates else None


def selector_for_node(node: dict) -> str:
    parts = []
    name = node.get("name") or ""
    auto_id = node.get("automation_id") or ""
    control_type = node.get("control_type") or ""
    if name:
        parts.append(f'title="{name}"')
    if auto_id:
        parts.append(f'auto_id="{auto_id}"')
    if control_type:
        parts.append(f'control_type="{control_type}"')
    return f"child_window({', '.join(parts)})" if parts else "child_window()"


def write_full_txt_dump(data: dict) -> None:
    run_dir = latest_complete_scan_dir()
    lines = []
    lines.append("H-OTA FULL UI DUMP")
    lines.append(f"Generated: {data['generated_at']}")
    lines.append(f"Source scan: {run_dir if run_dir else 'none'}")
    lines.append("")

    lines.append("=== RUNTIME WINDOWS / WIN32 CHILD IDS ===")
    for window in data["runtime"].get("hota_windows", []):
        lines.append(f"WINDOW hwnd={window['hwnd']} title={window['title']!r} class={window['class_name']!r} rect={window['rect']}")
        for child in window.get("children", []):
            lines.append(
                f"  CHILD hwnd={child['hwnd']} id={child['id']} class={child['class_name']!r} "
                f"title={child['title']!r} rect={child['rect']}"
            )
    for window in data["runtime"].get("output_windows", []):
        lines.append(f"OUTPUT hwnd={window['hwnd']} title={window['title']!r} class={window['class_name']!r} rect={window['rect']}")
        for child in window.get("children", []):
            lines.append(
                f"  OUTPUT_CHILD hwnd={child['hwnd']} id={child['id']} class={child['class_name']!r} "
                f"title={child['title']!r} rect={child['rect']}"
            )
    lines.append("")

    if run_dir:
        for state in ("initial", "after_config_load", "after_tester_mode"):
            path = run_dir / f"ui_tree_{state}.json"
            if not path.exists():
                continue
            nodes = json.loads(safe_read_text(path))
            lines.append(f"=== UIA CONTROL TREE: {state} ({len(nodes)} nodes) ===")
            for node in nodes:
                indent = "   |" * int(node.get("depth", 0))
                name = node.get("name") or ""
                ctype = node.get("control_type") or ""
                rect = node.get("rectangle") or []
                auto_id = node.get("automation_id") or ""
                cls = node.get("class_name") or ""
                lines.append(f"{indent} {ctype} - {name!r} rect={rect}")
                lines.append(f"{indent} selector={selector_for_node(node)} auto_id={auto_id!r} class={cls!r} enabled={node.get('enabled')} visible={node.get('visible')}")
                if node.get("value") not in (None, ""):
                    lines.append(f"{indent} value={node.get('value')!r}")
                if node.get("texts"):
                    lines.append(f"{indent} texts={node.get('texts')!r}")
            lines.append("")

        combo_path = run_dir / "combo_options.json"
        if combo_path.exists():
            combos = json.loads(safe_read_text(combo_path))
            lines.append("=== COMBO BOX OPTIONS ===")
            for row in combos:
                combo = row.get("combo", {})
                lines.append(
                    f"COMBO name={combo.get('name')!r} auto_id={combo.get('automation_id')!r} "
                    f"status={row.get('status')!r} rect={combo.get('rectangle')}"
                )
                for option in row.get("options", []):
                    lines.append(f"  OPTION {option!r}")
                if row.get("error"):
                    lines.append(f"  ERROR {row.get('error')}")
            lines.append("")

        probe_path = run_dir / "safe_button_probes.json"
        if probe_path.exists():
            probes = json.loads(safe_read_text(probe_path))
            lines.append("=== SAFE BUTTON CLICK PROBES ===")
            for row in probes:
                lines.append(f"BUTTON {row.get('name')!r} status={row.get('status')!r} method={row.get('invoke_method')!r}")
                for window in row.get("new_windows", []):
                    lines.append(
                        f"  OPENED hwnd={window.get('hwnd')} title={window.get('title')!r} "
                        f"class={window.get('class_name')!r} rect={window.get('rect')}"
                    )
                if row.get("error"):
                    lines.append(f"  ERROR {row.get('error')}")
            lines.append("")

        action_path = run_dir / "action_log.json"
        if action_path.exists():
            actions = json.loads(safe_read_text(action_path))
            lines.append("=== SCAN ACTION LOG ===")
            for row in actions:
                lines.append(json.dumps(row, ensure_ascii=False))
            lines.append("")

    lines.append("=== EXISTING PYTHON RUNNER SELECTORS ===")
    runner = data.get("runner", {})
    for row in runner.get("selectors", []):
        if "selector" in row:
            lines.append(f"{row['file']}:{row['line']} selector {row['selector']}")
        elif "send_keys" in row:
            lines.append(f"{row['file']}:{row['line']} send_keys {row['send_keys']}")
    lines.append("")

    lines.append("=== INSTALLED CONFIG / RESOURCE SUMMARY ===")
    lines.append(f"Installed folder: {INSTALL_DIR}")
    lines.append(f"Property definitions: {len(data.get('properties', []))}")
    lines.append(f"Schema files: {len(data.get('schemas', []))}")
    lines.append(f"Parsed runner scripts: {runner.get('script_count', 0)}")
    lines.append("")
    lines.append("=== SAFETY NOTES ===")
    lines.append("The regular scanner avoids real update-changing actions during normal discovery.")
    lines.append("The disconnected exhaustive probe clicked the Testcase Rule Start button and recorded the H-OTA popup: there is no selected testcase.")
    lines.append("Action Rule Start and Stop were disabled in that state; SAVE and Logout were intentionally skipped because they change user/config state.")
    lines.append("Utility/dialog buttons were invoked and their spawned windows were closed.")
    lines.append("For real OTA execution, use the existing case_*.py runner flow with CANoe AppState handling.")

    exhaustive_dump = DOC_DIR / "hota_exhaustive_click_dump.txt"
    if exhaustive_dump.exists():
        lines.append("")
        lines.append("=== EXHAUSTIVE CLICK PROBE REPORT ===")
        lines.append(safe_read_text(exhaustive_dump).rstrip())

    (DOC_DIR / "hota_full_ui_dump.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_markdown_files(data: dict) -> None:
    install = data["install"]
    runtime = data["runtime"]
    runner = data["runner"]
    properties = data["properties"]
    schemas = data["schemas"]
    config = data["config"]

    readme = []
    readme.append("# Tai lieu H-OTA Studio")
    readme.append("")
    readme.append(f"Generated: {data['generated_at']}")
    readme.append("")
    readme.append("## Muc tieu")
    readme.append("")
    readme.append("Bo tai lieu nay luu cau truc H-OTA Studio tren may nay de AI agent sau nay co the code automation bang selector thay vi toa do.")
    readme.append("")
    readme.append("## Ket qua chinh")
    readme.append("")
    readme.append(f"- Shortcut: `{SHORTCUT_PATH}`")
    readme.append("- Shortcut target: `C:\\Windows\\Installer\\{08463601-4A22-4255-956E-19A04EC5DA92}\\_2BD3CFB986DC9672563360.exe`")
    readme.append(f"- Installed folder: `{INSTALL_DIR}`")
    readme.append(f"- H-OTA runtime windows found: {len(runtime.get('hota_windows', []))}")
    readme.append(f"- Output windows found: {len(runtime.get('output_windows', []))}")
    readme.append(f"- Existing OTA Python runner files parsed: {runner.get('script_count', 0)}")
    readme.append(f"- Property definitions parsed: {len(properties)}")
    readme.append(f"- XSD schema files summarized: {len(schemas)}")
    readme.append("")
    readme.append("## Tai lieu con")
    readme.append("")
    readme.append("- `runtime_ui_scan.md`: cua so/runtime controls doc duoc tu Win32/UIA.")
    readme.append("- `automation_selectors.md`: selector va flow Python dang dung trong repo.")
    readme.append("- `installed_structure.md`: executable, doc, schema, template, procedure, testcase resource.")
    readme.append("- `config_and_schema.md`: ini, property XML, XSD, vehicle/controller metadata.")
    readme.append("- `hota_full_ui_dump.txt`: full plain-text dump similar to the older `ui_dump.txt`, including layout, selectors, combo options, click probes, exhaustive click report, and runner selectors.")
    readme.append("- `hota_exhaustive_click_dump.txt`: controlled click/toggle report for the disconnected H-OTA session, including skipped and disabled controls.")
    readme.append("- `limitations.md`: phan nao da test/click, phan nao bi chan boi quyen Windows.")
    readme.append("")
    readme.append("## Nguyen tac automation")
    readme.append("")
    readme.append("- Uu tien UIA/Win32 selector: title, auto_id, control_type, child window id, class name.")
    readme.append("- Khong dung toa do neu control co selector on dinh.")
    readme.append("- Nut `Start` la thao tac chay update that, chi invoke trong runner co CANoe/AppState va Output polling.")
    readme.append("- Neu H-OTA chay elevated, Python cung phai chay Administrator va nen dung 32-bit Python cho app 32-bit.")
    (DOC_DIR / "README.md").write_text("\n".join(readme) + "\n", encoding="utf-8")

    runtime_md = []
    runtime_md.append("# Runtime UI Scan")
    runtime_md.append("")
    runtime_md.append("## Top-level H-OTA windows")
    runtime_md.append("")
    for window in runtime.get("hota_windows", []):
        runtime_md.append(f"- hwnd={window['hwnd']} title=`{window['title']}` class=`{window['class_name']}` rect={window['rect']}")
        for child in window.get("children", [])[:80]:
            runtime_md.append(f"  - child hwnd={child['hwnd']} id={child['id']} class=`{child['class_name']}` title=`{child['title']}` rect={child['rect']}")
    runtime_md.append("")
    runtime_md.append("## Output window")
    runtime_md.append("")
    for window in runtime.get("output_windows", []):
        runtime_md.append(f"- hwnd={window['hwnd']} title=`{window['title']}` class=`{window['class_name']}` rect={window['rect']}")
        for child in window.get("children", []):
            runtime_md.append(f"  - child hwnd={child['hwnd']} id={child['id']} class=`{child['class_name']}` title=`{child['title']}` rect={child['rect']}")
    runtime_md.append("")
    runtime_md.append("## Scan folders")
    runtime_md.append("")
    for run in data["scan_runs"]:
        runtime_md.append(f"- `{run['run_dir']}`")
        for file_name in run["files"]:
            runtime_md.append(f"  - `{file_name}`")
    runtime_md.append("")
    runtime_md.append("Raw data: `runtime_ui_scan.json`.")
    (DOC_DIR / "runtime_ui_scan.md").write_text("\n".join(runtime_md) + "\n", encoding="utf-8")

    selectors_md = []
    selectors_md.append("# Automation Selectors")
    selectors_md.append("")
    selectors_md.append("## Stable selectors from existing runners")
    selectors_md.append("")
    known = [
        ("Main window", "Find visible top window whose title contains `H-OTA Studio` or `ReprogramEditor`."),
        ("Open file", "`send_keys(\"^o\")`, then Windows `Open` dialog, then type full `*.hcfg-r` path and Enter."),
        ("TesterMode", "`child_window(title=\"TesterMode\", control_type=\"Button\")`."),
        ("Gateway Type", "`child_window(auto_id=\"1241\", control_type=\"ComboBox\")`, select `CCU1 : EthDiag`."),
        ("SourceAddr", "`child_window(auto_id=\"1233\", control_type=\"ComboBox\")`, select `ERT :0x1000` for ERT flows."),
        ("Wireless-Rules", "`child_window(auto_id=\"1274\", control_type=\"RadioButton\")`."),
        ("Wired-Rules", "`child_window(title_re=\".*Wired.*Rule.*\", control_type=\"RadioButton\")`."),
        ("Scenario Units Apply", "`child_window(auto_id=\"1277\", control_type=\"Button\")`."),
        ("Action Rule Start", "`child_window(auto_id=\"1232\", control_type=\"Button\")`."),
        ("Output pane", "`child_window(title=\"Output\", auto_id=\"32803\", control_type=\"Pane\")`."),
        ("Output list", "child list `auto_id=\"2\"` under Output pane; Win32 class can appear as `ListBox`."),
    ]
    for label, selector in known:
        selectors_md.append(f"- {label}: {selector}")
    selectors_md.append("")
    selectors_md.append("## auto_id frequency")
    selectors_md.append("")
    for key, count in runner.get("auto_id_counts", {}).items():
        selectors_md.append(f"- `{key}`: {count}")
    selectors_md.append("")
    selectors_md.append("## action keyword coverage")
    selectors_md.append("")
    for key, files in runner.get("action_hits", {}).items():
        selectors_md.append(f"- `{key}`: {len(files)} files")
    selectors_md.append("")
    selectors_md.append("Raw data: `automation_selectors.json`.")
    (DOC_DIR / "automation_selectors.md").write_text("\n".join(selectors_md) + "\n", encoding="utf-8")

    installed_md = []
    installed_md.append("# Installed Structure")
    installed_md.append("")
    installed_md.append("## Main executables and DLL/resource areas")
    installed_md.append("")
    for row in install["top_level"][:120]:
        installed_md.append(f"- `{row['relative_path']}` size={row['size']}")
    installed_md.append("")
    installed_md.append("## Template resources")
    installed_md.append("")
    for row in install["templates"]:
        installed_md.append(f"- `{row['relative_path']}` size={row['size']}")
    installed_md.append("")
    installed_md.append("## Procedure resources")
    installed_md.append("")
    for row in install["procedures"]:
        installed_md.append(f"- `{row['relative_path']}` size={row['size']}")
    installed_md.append("")
    installed_md.append("## TestCase resources")
    installed_md.append("")
    for row in install["testcases"]:
        installed_md.append(f"- `{row['relative_path']}` size={row['size']}")
    installed_md.append("")
    installed_md.append("Raw data: `installed_structure.json`.")
    (DOC_DIR / "installed_structure.md").write_text("\n".join(installed_md) + "\n", encoding="utf-8")

    config_md = []
    config_md.append("# Config And Schema")
    config_md.append("")
    config_md.append("## Current INI state")
    config_md.append("")
    for ini_name, ini_data in config.items():
        config_md.append(f"### {ini_name}")
        for section, values in ini_data.items():
            config_md.append(f"- [{section}]")
            for key, value in values.items():
                config_md.append(f"  - `{key}` = `{value}`")
    config_md.append("")
    config_md.append("## Property XML summary")
    config_md.append("")
    by_section = defaultdict(list)
    for row in properties:
        by_section[row.get("section_path", "")].append(row)
    for section, rows in list(by_section.items())[:80]:
        config_md.append(f"### {section}")
        for row in rows[:40]:
            config_md.append(f"- `{row.get('name')}` type=`{row.get('type')}` default=`{row.get('default')}` values=`{row.get('value')}`")
    config_md.append("")
    config_md.append("## Schema families")
    config_md.append("")
    for schema in schemas:
        rel = Path(schema["file"]).relative_to(INSTALL_DIR)
        config_md.append(f"- `{rel}` elements={len(schema.get('elements', []))} complexTypes={len(schema.get('complex_types', []))}")
    config_md.append("")
    config_md.append("## Vehicle/controller info")
    config_md.append("")
    vehicle = data["vehicle_info"]
    config_md.append(f"- status_code: `{vehicle.get('status_code')}`")
    config_md.append(f"- car_count: `{vehicle.get('car_count')}`")
    for car in vehicle.get("first_40_cars", []):
        config_md.append(f"- car `{car.get('carName')}` id=`{car.get('carId')}`")
    config_md.append("")
    config_md.append("Raw data: `config_and_schema.json`.")
    (DOC_DIR / "config_and_schema.md").write_text("\n".join(config_md) + "\n", encoding="utf-8")

    limitations = []
    limitations.append("# Limitations And Next Steps")
    limitations.append("")
    limitations.append("## What was successfully done")
    limitations.append("")
    limitations.append("- H-OTA Studio was launched/reused through the repo shortcut.")
    limitations.append("- A visible H-OTA Studio window and floating Output window were detected.")
    limitations.append("- Runtime Win32 child windows were enumerated.")
    limitations.append("- Elevated runtime scans dumped UIA trees, expanded visible combo boxes, and captured screenshots.")
    limitations.append("- Utility/dialog buttons were clicked and their opened windows were recorded.")
    limitations.append("- The disconnected exhaustive probe clicked Manual, Technical Support, EditMode, TesterMode, option/report/help dialogs, checkboxes, radio buttons, Apply buttons, and the enabled Testcase Rule Start button.")
    limitations.append("- Existing Python runner selectors and flow comments were parsed.")
    limitations.append("- Installed H-OTA schema/template/procedure/testcase/config files were inventoried.")
    limitations.append("")
    limitations.append("## What was blocked")
    limitations.append("")
    limitations.append("- H-OTA is a 32-bit elevated app; pywinauto warns that 32-bit Python is preferred for full Win32 automation.")
    limitations.append("- Non-admin Python can inspect only limited state and cannot reliably send Ctrl+O/clicks.")
    limitations.append("- `SAVE` was skipped because it can write the loaded `.hcfg-r` config.")
    limitations.append("- `Logout` was skipped because it changes the current H-OTA login/session state.")
    limitations.append("- Action Rule Start, Stop, `Add Processing-Preperation(OBD EMS)`, and `Add Processing` were disabled during the disconnected probe.")
    limitations.append("- Testcase Rule Start opened the expected H-OTA message: `There is no selected testcase. Please check the Properties window.`")
    limitations.append("")
    limitations.append("## Next runtime scan command")
    limitations.append("")
    limitations.append("Run from an Administrator 32-bit Python if available:")
    limitations.append("")
    limitations.append("```powershell")
    limitations.append("cd D:\\project\\CANoe\\CANFD\\Auto\\OTA")
    limitations.append("python hota_docs\\scan_hota_ui.py")
    limitations.append("```")
    limitations.append("")
    limitations.append("Then rerun:")
    limitations.append("")
    limitations.append("```powershell")
    limitations.append("python hota_docs\\build_hota_docs.py")
    limitations.append("```")
    (DOC_DIR / "limitations.md").write_text("\n".join(limitations) + "\n", encoding="utf-8")
    write_full_txt_dump(data)


def main() -> int:
    DOC_DIR.mkdir(parents=True, exist_ok=True)
    install = {
        "top_level": list_files(INSTALL_DIR),
        "templates": list_files(INSTALL_DIR / "Templates"),
        "procedures": list_files(INSTALL_DIR / "procedure"),
        "testcases": list_files(INSTALL_DIR / "TestCase"),
        "docs": list_files(INSTALL_DIR / "doc"),
    }
    config = {}
    for name in ("configuration.ini", "EditorConfig.ini", "Testcase2.ini", "version.ini"):
        path = INSTALL_DIR / name
        if path.exists():
            try:
                config[name] = parse_ini(path)
            except Exception as exc:
                config[name] = {"error": str(exc)}

    properties = parse_property_xml(INSTALL_DIR / "property.xml") + parse_property_xml(INSTALL_DIR / "property2.xml")
    schemas = [parse_xsd_summary(path) for path in sorted((INSTALL_DIR / "Schema").rglob("*.xsd"))]
    data = {
        "generated_at": datetime.now().isoformat(timespec="seconds"),
        "install": install,
        "config": config,
        "properties": properties,
        "schemas": schemas,
        "vehicle_info": parse_vehicle_info(INSTALL_DIR / "vehicle_controller_info.json"),
        "runtime": enumerate_windows(),
        "runner": parse_runner_scripts(),
        "scan_runs": scan_runs(),
    }

    write_json(DOC_DIR / "installed_structure.json", install)
    write_json(DOC_DIR / "config_and_schema.json", {"config": config, "properties": properties, "schemas": schemas, "vehicle_info": data["vehicle_info"]})
    write_json(DOC_DIR / "runtime_ui_scan.json", data["runtime"])
    write_json(DOC_DIR / "automation_selectors.json", data["runner"])
    write_markdown_files(data)
    manifest_files = sorted(p.name for p in DOC_DIR.iterdir() if p.name != "__pycache__")
    write_json(DOC_DIR / "scan_manifest.json", {"generated_at": data["generated_at"], "files": manifest_files})
    print(f"H-OTA docs built in {DOC_DIR}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
