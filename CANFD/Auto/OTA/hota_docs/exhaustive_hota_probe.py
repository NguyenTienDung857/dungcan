"""
Exhaustive-but-controlled H-OTA click probe.

Run this from an elevated shell. It reuses the currently loaded H-OTA window,
clicks/toggles visible controls that are reasonable to exercise while the ECU is
disconnected, attempts Start/Stop probes with short waits, and writes a full
plain-text/JSON record.
"""

from __future__ import annotations

import ctypes
import json
import time
from datetime import datetime
from pathlib import Path

import win32con
import win32gui
from pywinauto import Application, Desktop
from pywinauto.keyboard import send_keys


BASE_DIR = Path(__file__).resolve().parents[1]
DOC_DIR = BASE_DIR / "hota_docs"
RUN_DIR = DOC_DIR / f"exhaustive_probe_{datetime.now().strftime('%Y%m%d_%H%M%S')}"


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


def list_top_windows() -> list[dict]:
    rows = []

    def callback(hwnd, sink):
        if not win32gui.IsWindowVisible(hwnd):
            return
        title = win32gui.GetWindowText(hwnd)
        if not title:
            return
        sink.append(
            {
                "hwnd": hwnd,
                "title": title,
                "class_name": win32gui.GetClassName(hwnd),
                "rect": list(safe_call(lambda: win32gui.GetWindowRect(hwnd), (0, 0, 0, 0))),
            }
        )

    win32gui.EnumWindows(callback, rows)
    return rows


def find_hota_window() -> int:
    found = 0

    def callback(hwnd, _):
        nonlocal found
        if not win32gui.IsWindowVisible(hwnd):
            return
        title = win32gui.GetWindowText(hwnd)
        if "H-OTA Studio" in title or "ReprogramEditor" in title:
            found = hwnd

    win32gui.EnumWindows(callback, None)
    return found


def force_foreground(hwnd: int) -> None:
    win32gui.ShowWindow(hwnd, win32con.SW_MAXIMIZE)
    time.sleep(0.3)
    try:
        win32gui.SetForegroundWindow(hwnd)
    except Exception:
        send_keys("%")
        safe_call(lambda: win32gui.SetForegroundWindow(hwnd))
    time.sleep(0.5)


def connect_main(hwnd: int):
    app = Application(backend="uia").connect(handle=hwnd)
    return app.window(handle=hwnd)


def control_meta(ctrl) -> dict:
    info = ctrl.element_info
    return {
        "name": safe_call(lambda: ctrl.window_text(), "") or info.name or "",
        "auto_id": info.automation_id or "",
        "control_type": info.control_type or "",
        "class_name": info.class_name or "",
        "enabled": safe_call(lambda: ctrl.is_enabled(), None),
        "visible": safe_call(lambda: ctrl.is_visible(), None),
        "rect": safe_call(lambda: [ctrl.rectangle().left, ctrl.rectangle().top, ctrl.rectangle().right, ctrl.rectangle().bottom], []),
    }


def dump_tree(main_win) -> list[dict]:
    rows = []

    def walk(ctrl, depth):
        meta = control_meta(ctrl)
        meta["depth"] = depth
        rows.append(meta)
        for child in safe_call(lambda: ctrl.children(), []) or []:
            walk(child, depth + 1)

    walk(main_win, 0)
    return rows


def get_output_log_lines(main_win) -> list[str]:
    lines = []
    try:
        pane = main_win.child_window(title="Output", auto_id="32803", control_type="Pane")
        list_ctrl = pane.child_window(auto_id="2", control_type="List")
        for item in list_ctrl.descendants(control_type="ListItem"):
            text = item.window_text().strip()
            if text:
                lines.append(text)
    except Exception:
        pass
    return lines


def close_new_windows(before: dict[int, dict], keep_titles: set[str]) -> list[dict]:
    after = {item["hwnd"]: item for item in list_top_windows()}
    new_windows = []
    for hwnd, item in after.items():
        if hwnd in before:
            continue
        title = item.get("title", "")
        new_windows.append(item)
        if any(keep in title for keep in keep_titles):
            continue
        safe_call(lambda hwnd=hwnd: win32gui.PostMessage(hwnd, win32con.WM_CLOSE, 0, 0))
    return new_windows


def click_named_button(main_win, name: str, log: list[dict], close_spawned=True) -> None:
    row = {"kind": "button", "name": name, "status": "not_found"}
    before = {item["hwnd"]: item for item in list_top_windows()}
    try:
        ctrl = main_win.child_window(title=name, control_type="Button")
        ctrl.wait("visible", timeout=3)
        row["meta"] = control_meta(ctrl)
        if not row["meta"].get("enabled"):
            row["status"] = "disabled"
        else:
            try:
                ctrl.invoke()
                row["method"] = "invoke"
            except Exception:
                ctrl.click_input()
                row["method"] = "click_input"
            row["status"] = "clicked"
            time.sleep(2)
            row["new_windows"] = close_new_windows(before, {"H-OTA Studio", "ReprogramEditor"}) if close_spawned else []
            safe_call(lambda: send_keys("{ESC}"))
    except Exception as exc:
        row["status"] = "failed"
        row["error"] = str(exc)
    log.append(row)


def click_auto_id(main_win, auto_id: str, control_type: str, label: str, log: list[dict], wait_s=1.0) -> None:
    row = {"kind": control_type, "label": label, "auto_id": auto_id, "status": "not_found"}
    try:
        ctrl = main_win.child_window(auto_id=auto_id, control_type=control_type)
        ctrl.wait("visible", timeout=3)
        row["meta"] = control_meta(ctrl)
        if not row["meta"].get("enabled"):
            row["status"] = "disabled"
        else:
            try:
                ctrl.invoke()
                row["method"] = "invoke"
            except Exception:
                ctrl.click_input()
                row["method"] = "click_input"
            row["status"] = "clicked"
            time.sleep(wait_s)
    except Exception as exc:
        row["status"] = "failed"
        row["error"] = str(exc)
    log.append(row)


def toggle_auto_id_twice(main_win, auto_id: str, label: str, log: list[dict]) -> None:
    row = {"kind": "toggle", "label": label, "auto_id": auto_id, "status": "not_found"}
    try:
        ctrl = main_win.child_window(auto_id=auto_id)
        ctrl.wait("visible", timeout=3)
        row["meta"] = control_meta(ctrl)
        if not row["meta"].get("enabled"):
            row["status"] = "disabled"
        else:
            for idx in (1, 2):
                try:
                    ctrl.invoke()
                    method = "invoke"
                except Exception:
                    ctrl.click_input()
                    method = "click_input"
                row[f"toggle_{idx}"] = method
                time.sleep(0.4)
            row["status"] = "toggled_twice_restore_attempted"
    except Exception as exc:
        row["status"] = "failed"
        row["error"] = str(exc)
    log.append(row)


def probe_start_stop(main_win, start_id: str, stop_id: str, label: str, log: list[dict]) -> None:
    row = {"kind": "start_stop", "label": label, "start_id": start_id, "stop_id": stop_id}
    before_lines = get_output_log_lines(main_win)
    before_windows = {item["hwnd"]: item for item in list_top_windows()}
    try:
        start = main_win.child_window(auto_id=start_id, control_type="Button")
        start.wait("visible", timeout=3)
        row["start_meta"] = control_meta(start)
        if not row["start_meta"].get("enabled"):
            row["status"] = "start_disabled"
        else:
            try:
                start.invoke()
                row["start_method"] = "invoke"
            except Exception:
                start.click_input()
                row["start_method"] = "click_input"
            row["status"] = "start_clicked"
            time.sleep(8)
            main_win = connect_main(find_hota_window())
            stop = main_win.child_window(auto_id=stop_id, control_type="Button")
            row["stop_meta"] = control_meta(stop)
            if row["stop_meta"].get("enabled"):
                try:
                    stop.invoke()
                    row["stop_method"] = "invoke"
                except Exception:
                    stop.click_input()
                    row["stop_method"] = "click_input"
                time.sleep(2)
            row["new_windows"] = close_new_windows(before_windows, {"H-OTA Studio", "ReprogramEditor"})
    except Exception as exc:
        row["status"] = "failed"
        row["error"] = str(exc)
    after_lines = get_output_log_lines(safe_call(lambda: connect_main(find_hota_window()), main_win))
    row["output_before_count"] = len(before_lines)
    row["output_after_count"] = len(after_lines)
    row["new_output_lines"] = after_lines[len(before_lines) :] if len(after_lines) >= len(before_lines) else after_lines
    log.append(row)


def write_text_report(log: list[dict], tree: list[dict]) -> None:
    lines = []
    lines.append("H-OTA EXHAUSTIVE CLICK PROBE")
    lines.append(f"Generated: {datetime.now().isoformat(timespec='seconds')}")
    lines.append(f"Admin: {is_admin()}")
    lines.append("")
    lines.append("=== CLICK LOG ===")
    for row in log:
        lines.append(json.dumps(row, ensure_ascii=False))
    lines.append("")
    lines.append("=== FINAL UI TREE ===")
    for node in tree:
        indent = "   |" * int(node.get("depth", 0))
        lines.append(
            f"{indent} {node.get('control_type')} name={node.get('name')!r} "
            f"auto_id={node.get('auto_id')!r} class={node.get('class_name')!r} "
            f"enabled={node.get('enabled')} visible={node.get('visible')} rect={node.get('rect')}"
        )
    (RUN_DIR / "exhaustive_click_dump.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
    (DOC_DIR / "hota_exhaustive_click_dump.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    RUN_DIR.mkdir(parents=True, exist_ok=True)
    log = []
    if not is_admin():
        log.append({"kind": "precondition", "status": "not_admin", "note": "Run from elevated shell for full click probe."})

    hwnd = find_hota_window()
    if not hwnd:
        raise RuntimeError("H-OTA/ReprogramEditor window is not open.")
    force_foreground(hwnd)
    main_win = connect_main(hwnd)

    log.append({"kind": "window", "title": safe_call(lambda: main_win.window_text(), ""), "hwnd": hwnd})

    for name in [
        "Hex2Binary",
        "RomPakage",
        "XML Conversion",
        "LogView",
        "Generator",
        "RFS",
        "Bi-Authenticaiton",
        "EditMode",
        "TesterMode",
        "Option",
        "Error Lookup",
        "Report Config",
        "Manual",
        "Technical Support",
        "About",
    ]:
        click_named_button(main_win, name, log)
        main_win = connect_main(find_hota_window())

    for auto_id, label in [
        ("1234", "For Cooperative ECU"),
        ("1236", "Add Pre Procedure"),
        ("1260", "Add Background-Transfer"),
        ("1261", "Add Processing-Preperation(OBD EMS)"),
        ("1262", "Add Processing"),
        ("1263", "Add Activation"),
        ("1264", "Add Rollback"),
        ("1240", "Add Post Procedure"),
        ("1224", "OEUK Vehicle"),
        ("1225", "NFS"),
    ]:
        toggle_auto_id_twice(main_win, auto_id, label, log)
        main_win = connect_main(find_hota_window())

    for auto_id, label in [
        ("1273", "Wired-Rules"),
        ("1274", "Wireless-Rules"),
        ("1275", "OTA-estimatedRuntime"),
    ]:
        click_auto_id(main_win, auto_id, "RadioButton", label, log)
        main_win = connect_main(find_hota_window())

    # Restore Wireless-Rules as the normal automation default, then apply scenario/rule.
    click_auto_id(main_win, "1274", "RadioButton", "Wireless-Rules restore", log)
    click_auto_id(main_win, "1277", "Button", "Apply Scenario Units", log, wait_s=2)
    click_auto_id(main_win, "1270", "Button", "Apply Rule Units", log, wait_s=2)

    main_win = connect_main(find_hota_window())
    probe_start_stop(main_win, "1218", "1219", "Testcase Rule Start/Stop", log)
    main_win = connect_main(find_hota_window())
    probe_start_stop(main_win, "1232", "1231", "Action Rule Start/Stop", log)

    skipped = [
        {"kind": "skipped", "name": "SAVE", "reason": "Would write the loaded H-OTA config file."},
        {"kind": "skipped", "name": "Logout", "reason": "Would change the user's H-OTA login/session state."},
    ]
    log.extend(skipped)

    final_tree = dump_tree(connect_main(find_hota_window()))
    (RUN_DIR / "exhaustive_click_log.json").write_text(json.dumps(log, indent=2, ensure_ascii=False), encoding="utf-8")
    (RUN_DIR / "final_ui_tree.json").write_text(json.dumps(final_tree, indent=2, ensure_ascii=False), encoding="utf-8")
    (RUN_DIR / "top_windows_after.json").write_text(json.dumps(list_top_windows(), indent=2, ensure_ascii=False), encoding="utf-8")
    write_text_report(log, final_tree)
    print(f"Exhaustive H-OTA probe complete: {RUN_DIR}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
