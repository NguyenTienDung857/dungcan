"""
Runtime scanner for H-OTA Studio.

This script is intentionally separate from the production OTA runners. It opens
H-OTA Studio, loads a sample *.hcfg-r config, probes UI Automation metadata, and
writes documentation artifacts under hota_docs/.
"""

from __future__ import annotations

import ctypes
import json
import os
import re
import sys
import time
from datetime import datetime
from pathlib import Path

import win32com.client
import win32con
import win32gui
from pywinauto import Application, Desktop
from pywinauto.keyboard import send_keys


BASE_DIR = Path(__file__).resolve().parents[1]
DOC_DIR = BASE_DIR / "hota_docs"
SHORTCUT_PATH = BASE_DIR / "H-OTA Studio.lnk"
MAX_UI_NODES = 5000


def now_stamp() -> str:
    return datetime.now().strftime("%Y%m%d_%H%M%S")


def safe_call(func, default=None):
    try:
        return func()
    except Exception:
        return default


def is_admin() -> bool:
    try:
        return bool(ctypes.windll.shell32.IsUserAnAdmin())
    except Exception:
        return False


def resolve_shortcut(path: Path) -> dict:
    shell = win32com.client.Dispatch("WScript.Shell")
    shortcut = shell.CreateShortcut(str(path))
    return {
        "shortcut": str(path),
        "target_path": shortcut.TargetPath,
        "arguments": shortcut.Arguments,
        "working_directory": shortcut.WorkingDirectory,
        "icon_location": shortcut.IconLocation,
        "description": shortcut.Description,
    }


def find_hota_window() -> int:
    found = 0

    def callback(hwnd, _extra):
        nonlocal found
        if not win32gui.IsWindowVisible(hwnd):
            return
        title = win32gui.GetWindowText(hwnd)
        if "H-OTA Studio" in title or "ReprogramEditor" in title:
            found = hwnd

    win32gui.EnumWindows(callback, None)
    return found


def list_top_windows() -> list[dict]:
    rows = []

    def callback(hwnd, _extra):
        if not win32gui.IsWindowVisible(hwnd):
            return
        title = win32gui.GetWindowText(hwnd)
        if not title:
            return
        rect = safe_call(lambda: win32gui.GetWindowRect(hwnd), (0, 0, 0, 0))
        rows.append(
            {
                "hwnd": hwnd,
                "title": title,
                "class_name": safe_call(lambda: win32gui.GetClassName(hwnd), ""),
                "rect": list(rect),
            }
        )

    win32gui.EnumWindows(callback, None)
    return rows


def force_foreground(hwnd: int) -> None:
    win32gui.ShowWindow(hwnd, win32con.SW_MAXIMIZE)
    time.sleep(0.3)
    try:
        win32gui.SetForegroundWindow(hwnd)
    except Exception:
        send_keys("%")
        time.sleep(0.2)
        safe_call(lambda: win32gui.SetForegroundWindow(hwnd))
    time.sleep(0.7)


def launch_or_connect(action_log: list[dict]) -> int:
    hwnd = find_hota_window()
    if hwnd:
        action_log.append({"action": "reuse_existing_hota_window", "hwnd": hwnd})
        return hwnd

    action_log.append({"action": "launch_shortcut", "path": str(SHORTCUT_PATH)})
    os.startfile(str(SHORTCUT_PATH))
    for _ in range(45):
        hwnd = find_hota_window()
        if hwnd:
            action_log.append({"action": "hota_window_found", "hwnd": hwnd})
            return hwnd
        time.sleep(1)
    raise RuntimeError("Could not find H-OTA Studio/ReprogramEditor window after launch.")


def connect_uia(hwnd: int):
    app = Application(backend="uia").connect(handle=hwnd)
    return app.window(handle=hwnd)


def rect_to_list(rect) -> list[int]:
    try:
        return [rect.left, rect.top, rect.right, rect.bottom]
    except Exception:
        return []


def control_info(ctrl, index: int, depth: int) -> dict:
    info = ctrl.element_info
    item = {
        "index": index,
        "depth": depth,
        "name": safe_call(lambda: ctrl.window_text(), "") or info.name or "",
        "automation_id": info.automation_id or "",
        "control_type": info.control_type or "",
        "class_name": info.class_name or "",
        "framework_id": info.framework_id or "",
        "handle": info.handle,
        "enabled": safe_call(lambda: ctrl.is_enabled(), None),
        "visible": safe_call(lambda: ctrl.is_visible(), None),
        "rectangle": rect_to_list(safe_call(lambda: ctrl.rectangle(), None)),
    }
    value = safe_call(lambda: ctrl.get_value(), None)
    if value not in (None, ""):
        item["value"] = value
    texts = safe_call(lambda: ctrl.texts(), None)
    if texts:
        item["texts"] = texts[:20]
    return item


def dump_ui_tree(root, state_name: str, out_dir: Path, action_log: list[dict]) -> list[dict]:
    nodes = []

    def walk(ctrl, depth: int) -> None:
        if len(nodes) >= MAX_UI_NODES:
            return
        nodes.append(control_info(ctrl, len(nodes), depth))
        children = safe_call(lambda: ctrl.children(), []) or []
        for child in children:
            walk(child, depth + 1)

    walk(root, 0)
    path = out_dir / f"ui_tree_{state_name}.json"
    path.write_text(json.dumps(nodes, indent=2, ensure_ascii=False), encoding="utf-8")
    action_log.append({"action": "dump_ui_tree", "state": state_name, "nodes": len(nodes), "file": str(path)})
    return nodes


def capture_window(hwnd: int, out_dir: Path, state_name: str, action_log: list[dict]) -> None:
    try:
        from PIL import ImageGrab
    except Exception as exc:
        action_log.append({"action": "capture_window", "state": state_name, "error": f"PIL unavailable: {exc}"})
        return

    rect = win32gui.GetWindowRect(hwnd)
    if rect[2] <= rect[0] or rect[3] <= rect[1]:
        action_log.append({"action": "capture_window", "state": state_name, "error": "invalid window rect"})
        return

    screenshot_dir = out_dir / "screenshots"
    screenshot_dir.mkdir(parents=True, exist_ok=True)
    path = screenshot_dir / f"{state_name}.png"
    try:
        img = ImageGrab.grab(bbox=rect, all_screens=True)
    except TypeError:
        img = ImageGrab.grab(bbox=rect)
    img.save(path)
    action_log.append({"action": "capture_window", "state": state_name, "file": str(path), "rect": list(rect)})


def find_config_file(folder_name: str = "0") -> Path:
    config_dir = BASE_DIR / folder_name
    matches = sorted(config_dir.glob("*.hcfg-r"))
    if len(matches) != 1:
        raise RuntimeError(f"Expected exactly one *.hcfg-r in {config_dir}, found {len(matches)}")
    return matches[0]


def load_config(hwnd: int, config_path: Path, action_log: list[dict]) -> None:
    force_foreground(hwnd)
    send_keys("^o")
    time.sleep(2)

    open_dlg = None
    try:
        app = Application(backend="win32").connect(handle=hwnd)
        candidate = app.window(title="Open")
        candidate.wait("ready", timeout=5)
        open_dlg = candidate.wrapper_object()
    except Exception:
        open_dlg = None
        for window in Desktop(backend="win32").windows():
            if window.is_visible() and "Open" in window.window_text():
                open_dlg = window
                break

    if not open_dlg:
        raise RuntimeError("Open dialog was not found after Ctrl+O.")

    open_dlg.set_focus()
    time.sleep(0.5)
    send_keys(str(config_path), with_spaces=True)
    time.sleep(0.5)
    send_keys("{ENTER}")
    action_log.append({"action": "load_config", "file": str(config_path)})
    time.sleep(4)


def click_tester_mode(main_win, action_log: list[dict]) -> None:
    try:
        tester = main_win.child_window(title="TesterMode", control_type="Button")
        tester.wait("visible", timeout=10)
        tester.invoke()
        action_log.append({"action": "click", "target": "TesterMode", "status": "ok"})
        time.sleep(2)
    except Exception as exc:
        action_log.append({"action": "click", "target": "TesterMode", "status": "failed", "error": str(exc)})


def collect_combo_options(main_win, out_dir: Path, action_log: list[dict]) -> list[dict]:
    combos = safe_call(lambda: main_win.descendants(control_type="ComboBox"), []) or []
    rows = []
    for idx, combo in enumerate(combos):
        meta = control_info(combo, idx, 0)
        row = {"combo": meta, "options": [], "status": "not_expanded"}
        try:
            combo.set_focus()
            combo.expand()
            time.sleep(0.8)
            options = []
            for item in safe_call(lambda: combo.descendants(control_type="ListItem"), []) or []:
                text = safe_call(lambda: item.window_text(), "")
                if text and text not in options:
                    options.append(text)
            row["options"] = options
            row["status"] = "expanded"
            send_keys("{ESC}")
            time.sleep(0.3)
        except Exception as exc:
            row["status"] = "failed"
            row["error"] = str(exc)
            safe_call(lambda: send_keys("{ESC}"))
        rows.append(row)

    path = out_dir / "combo_options.json"
    path.write_text(json.dumps(rows, indent=2, ensure_ascii=False), encoding="utf-8")
    action_log.append({"action": "collect_combo_options", "count": len(rows), "file": str(path)})
    return rows


def probe_safe_buttons(main_win, out_dir: Path, action_log: list[dict]) -> list[dict]:
    allow_names = [
        "Hex2Binary",
        "RomPakage",
        "XML Conversion",
        "LogView",
        "Generator",
        "RFS",
        "Bi-Authenticaiton",
        "Option",
        "Error Lookup",
        "Report Config",
        "About",
    ]
    skip_titles = {"Output"}
    before_windows = {item["hwnd"]: item for item in list_top_windows()}
    probes = []

    for name in allow_names:
        row = {"name": name, "status": "not_found", "new_windows": []}
        try:
            button = main_win.child_window(title=name, control_type="Button")
            button.wait("visible", timeout=3)
            row["status"] = "found"
            try:
                button.invoke()
                row["invoke_method"] = "invoke"
            except Exception:
                button.click_input()
                row["invoke_method"] = "click_input"
            time.sleep(2)
            after_windows = {item["hwnd"]: item for item in list_top_windows()}
            new_windows = [
                value
                for hwnd, value in after_windows.items()
                if hwnd not in before_windows and value.get("title") not in skip_titles
            ]
            row["new_windows"] = new_windows
            for window in new_windows:
                title = window.get("title", "")
                if "H-OTA Studio" in title or "ReprogramEditor" in title:
                    continue
                safe_call(lambda hwnd=window["hwnd"]: win32gui.PostMessage(hwnd, win32con.WM_CLOSE, 0, 0))
            safe_call(lambda: send_keys("{ESC}"))
            time.sleep(0.5)
            row["status"] = "invoked"
        except Exception as exc:
            row["status"] = row["status"] if row["status"] != "found" else "failed"
            row["error"] = str(exc)
        probes.append(row)

    path = out_dir / "safe_button_probes.json"
    path.write_text(json.dumps(probes, indent=2, ensure_ascii=False), encoding="utf-8")
    action_log.append({"action": "probe_safe_buttons", "count": len(probes), "file": str(path)})
    return probes


def collect_control_inventory(nodes: list[dict], out_dir: Path) -> dict:
    groups = {}
    for node in nodes:
        control_type = node.get("control_type") or "Unknown"
        groups.setdefault(control_type, []).append(node)

    selected = {
        name: groups.get(name, [])
        for name in [
            "Button",
            "ComboBox",
            "RadioButton",
            "CheckBox",
            "Edit",
            "List",
            "ListItem",
            "Tab",
            "TabItem",
            "MenuBar",
            "Menu",
            "MenuItem",
            "Pane",
            "Group",
            "Text",
        ]
    }
    path = out_dir / "control_inventory.json"
    path.write_text(json.dumps(selected, indent=2, ensure_ascii=False), encoding="utf-8")
    return selected


def collect_static_selectors(out_dir: Path) -> dict:
    selector_re = re.compile(r"child_window\((.*?)\)", re.DOTALL)
    auto_id_re = re.compile(r"auto_id\s*=\s*[\"']([^\"']+)[\"']")
    title_re = re.compile(r"title(?:_re)?\s*=\s*[\"']([^\"']+)[\"']")
    control_type_re = re.compile(r"control_type\s*=\s*[\"']([^\"']+)[\"']")
    send_keys_re = re.compile(r"send_keys\((.*?)\)")

    files = sorted(BASE_DIR.glob("*.py"))
    rows = []
    flow_headers = []
    for path in files:
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in selector_re.finditer(text):
            snippet = " ".join(match.group(1).split())
            line = text[: match.start()].count("\n") + 1
            rows.append(
                {
                    "file": path.name,
                    "line": line,
                    "selector": snippet,
                    "auto_ids": auto_id_re.findall(match.group(1)),
                    "titles": title_re.findall(match.group(1)),
                    "control_types": control_type_re.findall(match.group(1)),
                }
            )
        for match in send_keys_re.finditer(text):
            line = text[: match.start()].count("\n") + 1
            rows.append({"file": path.name, "line": line, "send_keys": " ".join(match.group(1).split())})

        header = []
        for raw_line in text.splitlines()[:35]:
            if raw_line.startswith("#   ") or raw_line.startswith("#      "):
                header.append(raw_line.lstrip("# ").rstrip())
        if header:
            flow_headers.append({"file": path.name, "flow": header})

    data = {"script_count": len(files), "selectors_and_keys": rows, "flow_headers": flow_headers}
    path = out_dir / "static_script_selectors.json"
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False), encoding="utf-8")
    return data


def summarize_controls(nodes: list[dict]) -> dict:
    summary = {}
    for node in nodes:
        key = node.get("control_type") or "Unknown"
        summary[key] = summary.get(key, 0) + 1
    return dict(sorted(summary.items()))


def write_markdown_summary(
    out_dir: Path,
    shortcut_info: dict,
    config_path: Path,
    initial_nodes: list[dict],
    loaded_nodes: list[dict],
    tester_nodes: list[dict],
    combo_rows: list[dict],
    static_data: dict,
    action_log: list[dict],
) -> None:
    inventory = collect_control_inventory(tester_nodes or loaded_nodes or initial_nodes, out_dir)

    known_selectors = [
        ("Main window", "title contains H-OTA Studio or ReprogramEditor"),
        ("Open config", "Ctrl+O, then Windows Open dialog"),
        ("TesterMode button", 'title="TesterMode", control_type="Button"'),
        ("Output pane", 'title="Output", auto_id="32803", control_type="Pane"'),
        ("Output list", 'auto_id="2", control_type="List" under Output pane'),
        ("Gateway Type combo", 'auto_id="1241", control_type="ComboBox"'),
        ("SourceAddr combo", 'auto_id="1233", control_type="ComboBox"'),
        ("Wireless-Rules radio", 'auto_id="1274", control_type="RadioButton"'),
        ("Scenario Units Apply", 'auto_id="1277", control_type="Button"'),
        ("Action Rule Start", 'auto_id="1232", control_type="Button"'),
    ]

    lines = []
    lines.append("# H-OTA Studio UI Documentation")
    lines.append("")
    lines.append(f"- Generated: {datetime.now().isoformat(timespec='seconds')}")
    lines.append(f"- Scanner admin: {is_admin()}")
    lines.append(f"- Shortcut: `{shortcut_info['shortcut']}`")
    lines.append(f"- Target: `{shortcut_info['target_path']}`")
    lines.append(f"- Working directory: `{shortcut_info['working_directory']}`")
    lines.append(f"- Loaded config for scan: `{config_path}`")
    lines.append("")
    lines.append("## What was probed")
    lines.append("")
    lines.append("- Launched or reused the H-OTA/ReprogramEditor window.")
    lines.append("- Captured UI Automation trees before config load, after config load, and after clicking TesterMode.")
    lines.append("- Expanded visible ComboBox controls and recorded options when UIA exposed them.")
    lines.append("- Captured screenshots of the H-OTA window for the same major states.")
    lines.append("- Parsed existing OTA Python runners for selectors and keyboard actions.")
    lines.append("- Invoked selected safe utility/dialog buttons and logged any windows they opened.")
    lines.append("- Did not click `Start` or any update-execution action; those are documented as unsafe to invoke during discovery.")
    lines.append("")
    lines.append("## UI tree node counts")
    lines.append("")
    for state, nodes in [("initial", initial_nodes), ("after_config_load", loaded_nodes), ("after_tester_mode", tester_nodes)]:
        lines.append(f"### {state}")
        for ctype, count in summarize_controls(nodes).items():
            lines.append(f"- {ctype}: {count}")
        lines.append("")
    lines.append("## Known automation selectors")
    lines.append("")
    for name, selector in known_selectors:
        lines.append(f"- {name}: `{selector}`")
    lines.append("")
    lines.append("## Runtime controls by type")
    lines.append("")
    for ctype, rows in inventory.items():
        if not rows:
            continue
        lines.append(f"### {ctype}")
        for node in rows[:80]:
            name = node.get("name") or ""
            auto_id = node.get("automation_id") or ""
            cls = node.get("class_name") or ""
            rect = node.get("rectangle") or []
            lines.append(f"- name=`{name}` auto_id=`{auto_id}` class=`{cls}` rect={rect}")
        if len(rows) > 80:
            lines.append(f"- ... {len(rows) - 80} more in `control_inventory.json`")
        lines.append("")
    lines.append("## ComboBox options observed")
    lines.append("")
    if combo_rows:
        for row in combo_rows:
            combo = row.get("combo", {})
            label = combo.get("name") or combo.get("automation_id") or f"combo_{len(lines)}"
            lines.append(f"- `{label}` auto_id=`{combo.get('automation_id', '')}` status=`{row.get('status')}`")
            for option in row.get("options", [])[:60]:
                lines.append(f"  - `{option}`")
            if len(row.get("options", [])) > 60:
                lines.append("  - ... more in `combo_options.json`")
    else:
        lines.append("- No ComboBox controls were exposed by UIA in this scan state.")
    lines.append("")
    lines.append("## Existing Python runner model")
    lines.append("")
    lines.append(f"- Parsed Python files: {static_data.get('script_count', 0)}")
    lines.append("- Common flow: launch shortcut, Ctrl+O load `*.hcfg-r`, click TesterMode, set Gateway Type, choose Wireless/Wired rule, Apply, Start, mirror Output pane, wait result popup, capture screenshots, close H-OTA, rename ASC logs.")
    lines.append("- Existing runner selectors and send_keys calls are stored in `static_script_selectors.json`.")
    lines.append("")
    lines.append("## Files in this documentation set")
    lines.append("")
    for name in [
        "ui_tree_initial.json",
        "ui_tree_after_config_load.json",
        "ui_tree_after_tester_mode.json",
        "control_inventory.json",
        "combo_options.json",
        "safe_button_probes.json",
        "static_script_selectors.json",
        "action_log.json",
        "top_windows.json",
        "screenshots/",
    ]:
        lines.append(f"- `{name}`")
    lines.append("")
    lines.append("## Automation guidance")
    lines.append("")
    lines.append("- Prefer UIA selectors (`title`, `auto_id`, `control_type`) over coordinate clicks.")
    lines.append("- Treat `Start` (`auto_id=1232`) as a destructive execution control; only invoke it inside a testcase runner with CANoe/AppState handling active.")
    lines.append("- Keep the Output pane polling selector stable: `Output` pane `auto_id=32803`, child list `auto_id=2`.")
    lines.append("- If a combo option cannot be selected by UIA `.select(...)`, existing runners fall back to focus + `send_keys(\"label{ENTER}\")`.")
    lines.append("- For ERT flows, set Gateway Type first, then SourceAddr (`auto_id=1233`) before selecting Wired-Rules.")
    lines.append("")

    (out_dir / "README.md").write_text("\n".join(lines), encoding="utf-8")
    (out_dir / "action_log.json").write_text(json.dumps(action_log, indent=2, ensure_ascii=False), encoding="utf-8")


def main() -> int:
    run_dir = DOC_DIR / f"scan_{now_stamp()}"
    run_dir.mkdir(parents=True, exist_ok=True)
    action_log: list[dict] = []

    shortcut_info = resolve_shortcut(SHORTCUT_PATH)
    (run_dir / "shortcut.json").write_text(json.dumps(shortcut_info, indent=2, ensure_ascii=False), encoding="utf-8")
    (run_dir / "top_windows_before.json").write_text(json.dumps(list_top_windows(), indent=2, ensure_ascii=False), encoding="utf-8")

    config_path = find_config_file("0")
    hwnd = launch_or_connect(action_log)
    force_foreground(hwnd)
    main_win = connect_uia(hwnd)

    initial_nodes = dump_ui_tree(main_win, "initial", run_dir, action_log)
    capture_window(hwnd, run_dir, "initial", action_log)

    load_config(hwnd, config_path, action_log)
    main_win = connect_uia(hwnd)
    loaded_nodes = dump_ui_tree(main_win, "after_config_load", run_dir, action_log)
    capture_window(hwnd, run_dir, "after_config_load", action_log)

    click_tester_mode(main_win, action_log)
    main_win = connect_uia(hwnd)
    tester_nodes = dump_ui_tree(main_win, "after_tester_mode", run_dir, action_log)
    capture_window(hwnd, run_dir, "after_tester_mode", action_log)

    combo_rows = collect_combo_options(main_win, run_dir, action_log)
    safe_button_probes = probe_safe_buttons(main_win, run_dir, action_log)
    static_data = collect_static_selectors(run_dir)
    (run_dir / "top_windows_after.json").write_text(json.dumps(list_top_windows(), indent=2, ensure_ascii=False), encoding="utf-8")

    write_markdown_summary(
        run_dir,
        shortcut_info,
        config_path,
        initial_nodes,
        loaded_nodes,
        tester_nodes,
        combo_rows,
        static_data,
        action_log,
    )
    print(f"H-OTA documentation scan complete: {run_dir}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"ERROR: {exc}")
        raise
