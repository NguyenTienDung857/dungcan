# =============================================================================
# update_to_00_ERT.py - ERT update helper: wired update to version 00
# =============================================================================
# TEST FLOW:
#   1. Ensure Administrator rights; relaunch elevated when needed.
#   2. Kill old H-OTA Studio processes for a clean start.
#   3. Launch H-OTA Studio from H-OTA Studio.lnk next to this script.
#   4. Find the H-OTA/ReprogramEditor window, foreground it, and maximize it.
#   5. Send Ctrl+O and load the only *.hcfg-r file from H-OTA config folder 0.
#   6. Click TesterMode.
#   7. Set Gateway Type to "CCU1 : EthDiag".
#   8. Set SourceAddr to "ERT :0x1000".
#   9. Select Wired-Rules and click Apply for Scenario Units.
#   10. Click Start in Action Rule.
#   11. Mirror new Output pane lines to the terminal while waiting.
#   12. Wait for the final H-OTA result popup.
#   13. If popup text contains success, set PythonCom::AppState = 3;
#      otherwise set PythonCom::AppState = 4.
#   14. Capture the H-OTA window and every monitor into Report/.
#   15. Close H-OTA Studio after screenshots, then wait 30s and rename any current-run LOGASC\*.asc to
#      LOGASC\<original>_<script_name>_YYYYMMDD_HHMMSS.asc.
# =============================================================================

import subprocess
import time
import sys
import ctypes
import os
from hota_config import find_hota_config_file
from python_run_log import start_python_log
from datetime import datetime
import win32gui
import win32api
import win32con
import win32com.client
from pywinauto import Application, Desktop
from pywinauto.keyboard import send_keys


def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False


def relaunch_as_admin():
    try:
        params = " ".join([f'"{arg}"' for arg in sys.argv])
        result = ctypes.windll.shell32.ShellExecuteW(
            None, "runas", sys.executable, params, None, 1
        )
        return result > 32
    except Exception as e:
        print(f"ERROR: Could not request Administrator privileges: {e}")
        return False


def force_foreground(hwnd, maximize=True):
    """Forces the specified window to the foreground."""
    if maximize:
        win32gui.ShowWindow(hwnd, win32con.SW_MAXIMIZE)
    else:
        win32gui.ShowWindow(hwnd, win32con.SW_RESTORE)

    try:
        win32gui.SetForegroundWindow(hwnd)
    except Exception as e:
        print(f"Warning: SetForegroundWindow failed: {e}")
        send_keys("%")
        try:
            win32gui.SetForegroundWindow(hwnd)
        except:
            pass

    time.sleep(1)


def find_hota_window():
    """Finds the H-OTA Studio window using win32gui."""
    hwnd_found = 0

    def callback(hwnd, extra):
        nonlocal hwnd_found
        if win32gui.IsWindowVisible(hwnd):
            title = win32gui.GetWindowText(hwnd)
            if "H-OTA Studio" in title or "ReprogramEditor" in title:
                hwnd_found = hwnd

    win32gui.EnumWindows(callback, None)
    return hwnd_found


def get_output_log_lines(main_win_uia):
    """Reads current log lines from the Output pane."""
    try:
        output_pane = main_win_uia.child_window(
            title="Output", auto_id="32803", control_type="Pane"
        )

        list_ctrl = output_pane.child_window(auto_id="2", control_type="List")
        lines = []

        try:
            for item in list_ctrl.descendants(control_type="ListItem"):
                text = item.window_text().strip()
                if text:
                    lines.append(text)
        except Exception:
            pass

        if lines:
            return lines

        fallback_lines = []
        for ctrl in output_pane.descendants():
            try:
                text = ctrl.window_text().strip()
                if text and text not in ("Output", "Log", "Log Path :"):
                    fallback_lines.append(text)
            except Exception:
                pass

        return fallback_lines
    except Exception:
        return []


def print_new_output_log_lines(main_win_uia, output_state):
    """Prints only newly appended lines from Output pane to terminal."""
    current_lines = get_output_log_lines(main_win_uia)
    previous_lines = output_state.get("last_lines", [])

    if not current_lines:
        return []

    start_index = 0
    if (
        len(current_lines) >= len(previous_lines)
        and current_lines[: len(previous_lines)] == previous_lines
    ):
        start_index = len(previous_lines)
    else:
        max_overlap = min(len(previous_lines), len(current_lines))
        overlap = 0
        for k in range(max_overlap, 0, -1):
            if previous_lines[-k:] == current_lines[:k]:
                overlap = k
                break
        start_index = overlap

    new_lines = current_lines[start_index:]
    for line in new_lines:
        print(f"[OUTPUT] {line}")

    output_state["last_lines"] = current_lines
    return new_lines


def notify_canoe_app_state(value=1, variable_name="PythonCom::AppState"):
    """Writes a value to CANoe system variable through COM API."""
    try:
        canoe_app = win32com.client.Dispatch("CANoe.Application")
        parts = variable_name.split("::")
        if len(parts) != 2:
            raise ValueError(
                f"Invalid variable format: {variable_name}. Expected Namespace::Variable"
            )

        ns_name = parts[0]
        var_name = parts[1]

        namespace = canoe_app.System.Namespaces(ns_name)
        sys_var = namespace.Variables(var_name)
        sys_var.Value = value
        print(f"CANoe notified: {variable_name} = {value}")
        return True
    except Exception as e:
        print(f"Warning: Could not notify CANoe via COM ({variable_name}={value}): {e}")
        return False


def terminate_current_hota_process():
    """Force-closes H-OTA Studio so LOGASC files are flushed to disk."""
    try:
        subprocess.run(
            ["taskkill", "/F", "/IM", "H-OTA Studio.exe", "/T"],
            capture_output=True,
            check=False,
        )
        print("H-OTA Studio process was closed to flush LOGASC files.")
        return True
    except Exception as e:
        print(f"Warning: Could not close H-OTA Studio process: {e}")
        return False


def rename_recent_asc_files(script_name, script_dir, run_started_at, wait_seconds=30):
    """Waits after screenshots, then renames current-run LOGASC/*.asc files."""
    print(f"Waiting {wait_seconds}s before checking LOGASC\\*.asc files...")
    time.sleep(wait_seconds)

    logasc_dir = os.path.join(script_dir, "LOGASC")
    if not os.path.isdir(logasc_dir):
        print(f"No LOGASC folder found: {logasc_dir}")
        return False

    cutoff_time = run_started_at - 5
    asc_files = []

    try:
        for file_name in os.listdir(logasc_dir):
            if not file_name.lower().endswith(".asc"):
                continue

            source_path = os.path.join(logasc_dir, file_name)
            if not os.path.isfile(source_path):
                continue

            try:
                created_at = os.path.getctime(source_path)
                modified_at = os.path.getmtime(source_path)
            except Exception as e:
                print(f"Could not read file timestamp for {source_path}: {e}")
                continue

            if created_at >= cutoff_time or modified_at >= cutoff_time:
                asc_files.append((source_path, file_name, max(created_at, modified_at)))
    except Exception as e:
        print(f"Could not list LOGASC folder {logasc_dir}: {e}")
        return False

    if not asc_files:
        print("No current-run .asc files found in LOGASC. Skipping rename.")
        return False

    rename_ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    renamed_any = False

    for source_path, file_name, _file_time in sorted(asc_files, key=lambda item: item[2]):
        source_base = os.path.splitext(file_name)[0]
        target_path = os.path.join(
            logasc_dir, f"{source_base}_{script_name}_{rename_ts}.asc"
        )

        suffix = 1
        while os.path.exists(target_path):
            target_path = os.path.join(
                logasc_dir, f"{source_base}_{script_name}_{rename_ts}_{suffix}.asc"
            )
            suffix += 1

        try:
            os.replace(source_path, target_path)
            print(f"Renamed LOGASC file: {source_path} -> {target_path}")
            renamed_any = True
        except Exception as e:
            print(f"Could not rename {source_path}: {e}")

    return renamed_any

def get_runtime_base_dir():
    """Returns folder containing script in dev mode or exe in frozen mode."""
    if getattr(sys, "frozen", False):
        return os.path.dirname(os.path.abspath(sys.executable))
    return os.path.dirname(os.path.abspath(__file__))


def capture_hota_and_monitor_screenshots(main_hwnd, report_dir, script_name):
    """Captures H-OTA window and each monitor with a shared timestamp."""
    screenshot_ts = datetime.now().strftime("%Y%m%d_%H%M%S")

    try:
        from PIL import ImageGrab
    except Exception as e:
        print(f"Could not import Pillow ImageGrab: {e}")
        return False

    captured_any = False

    try:
        left, top, right, bottom = win32gui.GetWindowRect(main_hwnd)
        if right > left and bottom > top:
            hota_path = os.path.join(
                report_dir,
                f"{script_name}_{screenshot_ts}_hota.png",
            )
            try:
                hota_img = ImageGrab.grab(
                    bbox=(left, top, right, bottom), all_screens=True
                )
            except TypeError:
                hota_img = ImageGrab.grab(bbox=(left, top, right, bottom))

            hota_img.save(hota_path)
            print(f"H-OTA window screenshot saved to: {hota_path}")
            captured_any = True
        else:
            print("Invalid H-OTA window bounds. Skipping H-OTA window screenshot.")
    except Exception as e:
        print(f"Could not capture H-OTA window screenshot: {e}")

    try:
        monitors = win32api.EnumDisplayMonitors()
        if not monitors:
            raise RuntimeError("No monitors returned by EnumDisplayMonitors")

        for idx, monitor_info in enumerate(monitors, start=1):
            monitor_rect = monitor_info[2]
            m_left, m_top, m_right, m_bottom = monitor_rect
            monitor_path = os.path.join(
                report_dir,
                f"{script_name}_{screenshot_ts}_screen{idx}.png",
            )
            try:
                monitor_img = ImageGrab.grab(
                    bbox=(m_left, m_top, m_right, m_bottom), all_screens=True
                )
            except TypeError:
                monitor_img = ImageGrab.grab(bbox=(m_left, m_top, m_right, m_bottom))

            monitor_img.save(monitor_path)
            print(f"Monitor {idx} screenshot saved to: {monitor_path}")
            captured_any = True
    except Exception as e:
        print(f"Could not capture per-monitor screenshots: {e}")

    return captured_any


def open_hota_and_file():
    script_dir = get_runtime_base_dir()
    script_name = os.path.splitext(os.path.basename(__file__))[0]
    python_log_path = start_python_log(script_dir, script_name)
    print(f"Python log file: {python_log_path}")

    if not is_admin():
        print("This script needs Administrator privileges. Requesting elevation...")
        if relaunch_as_admin():
            print("Relaunched as Administrator. Closing current process.")
        else:
            print(
                "ERROR: Elevation was not granted. Please run as Administrator manually."
            )
        return

    print("Checking for existing H-OTA Studio processes...")
    try:
        subprocess.run(
            ["taskkill", "/F", "/IM", "H-OTA Studio.exe", "/T"],
            capture_output=True,
            check=False,
        )
        time.sleep(1)
        print("Clean start: Any previous H-OTA processes terminated.")
    except Exception as e:
        print(f"Note: No existing processes to kill (or error: {e})")

    hota_path = os.path.join(get_runtime_base_dir(), "H-OTA Studio.lnk")

    if not os.path.exists(hota_path):
        print("================================================================")
        print(f"ERROR: Not found '{hota_path}'")
        print("Please place 'H-OTA Studio.lnk' in the same folder as this script.")
        print("================================================================")
        return

    print(f"Launching H-OTA Studio via: {hota_path}...")
    try:
        os.startfile(hota_path)
    except Exception as e:
        print(f"ERROR: Could not launch H-OTA: {e}")
        return

    print("Waiting for H-OTA Studio window...")
    main_hwnd = 0

    for i in range(30):
        main_hwnd = find_hota_window()
        if main_hwnd:
            title = win32gui.GetWindowText(main_hwnd)
            print(f"Found window: '{title}' (HWND: {main_hwnd})")
            break
        time.sleep(1)
        if i % 5 == 0 and i > 0:
            print(f"Still waiting... ({i}s)")

    if not main_hwnd:
        print("Error: Could not find H-OTA Studio window.")
        return

    print("Bringing window to front...")
    force_foreground(main_hwnd)
    time.sleep(1)

    print("Sending Ctrl + O...")
    send_keys("^o")
    time.sleep(2)

    script_dir = get_runtime_base_dir()
    script_name = os.path.splitext(os.path.basename(__file__))[0]
    report_dir = os.path.join(script_dir, "Report")
    os.makedirs(report_dir, exist_ok=True)
    file_path = find_hota_config_file(script_dir, "0")

    print("Waiting for Open dialog...")
    app = Application(backend="win32").connect(handle=main_hwnd)

    open_dlg = None
    try:
        open_dlg = app.window(title="Open")
        open_dlg.wait("ready", timeout=5)
        print("Found Open dialog.")
    except Exception:
        print("Could not find Open dialog via direct child. Trying desktop search...")
        desktop = Desktop(backend="win32")
        for w in desktop.windows():
            if w.is_visible() and "Open" in w.window_text():
                open_dlg = w
                break

    dlg_ok = False
    if open_dlg:
        try:
            dlg_ok = open_dlg.is_visible()
        except:
            dlg_ok = True

    if dlg_ok:
        print(f"Dialog ready: {open_dlg.window_text()}")
        open_dlg.set_focus()
        time.sleep(1)

        print("Typing file path...")
        send_keys(file_path, with_spaces=True)
        time.sleep(1)

        print("Pressing Enter to confirm...")
        send_keys("{ENTER}")
        print("File path sent correctly! Check the app.")

        print("Waiting 3 seconds for file to load...")
        time.sleep(3)
        try:
            print("Re-connecting via UIA to find TesterMode button...")
            app_uia = Application(backend="uia").connect(handle=main_hwnd)
            main_win_uia = app_uia.window(handle=main_hwnd)

            tester_btn = main_win_uia.child_window(
                title="TesterMode", control_type="Button"
            )
            tester_btn.wait("visible", timeout=10)
            tester_btn.invoke()
            print("Successfully clicked 'TesterMode'!")

            print("Waiting for Gateway Type to appear...")
            time.sleep(2)

            combo = main_win_uia.child_window(auto_id="1241", control_type="ComboBox")
            combo.wait("visible", timeout=10)

            combo.expand()
            time.sleep(1)

            try:
                combo.select("CCU1 : EthDiag")
            except Exception as e:
                print(f"select() failed: {e}. Trying fallback send_keys...")
                combo.click_input()
                time.sleep(0.5)
                send_keys("CCU1 : EthDiag{ENTER}", with_spaces=True)

            print("Successfully selected 'CCU1 : EthDiag' in Gateway Type!")

            print("Waiting for SourceAddr to appear...")
            time.sleep(1)

            source_combo = main_win_uia.child_window(
                auto_id="1233", control_type="ComboBox"
            )
            source_combo.wait("visible", timeout=10)

            source_combo.expand()
            time.sleep(1)

            source_candidates = [
                "ERT :0x1000",
                "ERT :0x 1000",
                "ERT:0x1000",
                "ERT: 0x1000",
            ]
            selected_source = None

            for candidate in source_candidates:
                try:
                    source_combo.select(candidate)
                    selected_source = candidate
                    break
                except Exception:
                    pass

            if selected_source is None:
                print(
                    "SourceAddr select() failed for all expected labels. Trying fallback send_keys..."
                )
                source_combo.click_input()
                time.sleep(0.5)
                send_keys("ERT :0x1000{ENTER}", with_spaces=True)
                selected_source = "ERT :0x1000"

            print(f"Successfully selected '{selected_source}' in SourceAddr!")

            print("Selecting Wired-Rules...")
            try:
                wired_radio = main_win_uia.child_window(
                    title_re=".*Wired.*Rule.*", control_type="RadioButton"
                )
                wired_radio.wait("visible", timeout=5)
                wired_radio.click_input()
                time.sleep(0.5)

                print("Clicking Apply for Scenario Units...")
                apply_btn = main_win_uia.child_window(
                    auto_id="1277", control_type="Button"
                )
                apply_btn.invoke()
                time.sleep(1)
            except Exception as e:
                print(f"Failed to apply Wired-Rule and Scenario Units: {e}")

            print("Clicking Start in Action Rule...")
            try:
                start_rule_btn = main_win_uia.child_window(
                    auto_id="1232", control_type="Button"
                )

                output_state = {"last_lines": get_output_log_lines(main_win_uia)}
                print(
                    "Output mirror started: new lines from Output pane will be printed below."
                )

                start_rule_btn.invoke()
                print("========================================")
                print("SUCCESS: Automation Sequence Completed!")
                print("========================================")

                MAX_WAIT_SECONDS = 20 * 60
                POLL_INTERVAL = 2
                print(
                    f"Waiting up to {MAX_WAIT_SECONDS // 60} minutes for popup window 'H-OTA Studio'..."
                )

                popup_hwnd = 0
                start_time = time.time()

                while time.time() - start_time < MAX_WAIT_SECONDS:
                    try:
                        print_new_output_log_lines(main_win_uia, output_state)
                    except Exception as e:
                        print(f"Warning: Could not read Output pane logs: {e}")

                    hwnds = []
                    win32gui.EnumWindows(
                        lambda hwnd, param: (
                            param.append(hwnd)
                            if win32gui.IsWindowVisible(hwnd)
                            and win32gui.GetWindowText(hwnd) == "H-OTA Studio"
                            else None
                        ),
                        hwnds,
                    )

                    for h in hwnds:
                        if h != main_hwnd:
                            popup_hwnd = h
                            break

                    if popup_hwnd:
                        break

                    elapsed = int(time.time() - start_time)
                    if elapsed % 30 == 0 and elapsed > 0:
                        print(f"  Still waiting... ({elapsed}s / {MAX_WAIT_SECONDS}s)")
                    time.sleep(POLL_INTERVAL)

                try:
                    print_new_output_log_lines(main_win_uia, output_state)
                except Exception:
                    pass

                if popup_hwnd:
                    elapsed = int(time.time() - start_time)
                    print(f"Found popup window after {elapsed}s! (HWND: {popup_hwnd})")

                    result = "UNKNOWN"
                    popup_text_content = ""
                    try:
                        popup_uia = Application(backend="uia").connect(
                            handle=popup_hwnd
                        )
                        popup_win = popup_uia.window(handle=popup_hwnd)

                        all_texts = []
                        try:
                            for ctrl in popup_win.descendants():
                                try:
                                    txt = ctrl.window_text()
                                    if txt and txt.strip():
                                        all_texts.append(txt.strip())
                                except Exception:
                                    pass
                        except Exception as e:
                            print(f"Warning: Error iterating descendants: {e}")

                        popup_text_content = "\n".join(all_texts)
                        print("--- Popup Content ---")
                        print(popup_text_content)
                        print("--- End Popup Content ---")

                        lower_text = popup_text_content.lower()
                        if "fail" in lower_text:
                            result = "FAIL"
                        elif "success" in lower_text:
                            result = "SUCCESS"
                        else:
                            result = "UNKNOWN (no 'fail' or 'success' keyword found)"

                        if result == "SUCCESS":
                            notify_canoe_app_state(3)
                        else:
                            notify_canoe_app_state(4)

                        capture_hota_and_monitor_screenshots(
                            main_hwnd=main_hwnd,
                            report_dir=report_dir,
                            script_name=script_name,
                        )

                    except Exception as e:
                        print(f"Error reading popup content: {e}")

                    print("Closing H-OTA Studio after screenshots to flush LOGASC files...")
                    terminate_current_hota_process()

                    print("========================================")
                    print(f"  H-OTA RESULT: {result}")
                    print("========================================")

                    rename_recent_asc_files(
                        script_name=script_name,
                        script_dir=script_dir,
                        run_started_at=start_time,
                        wait_seconds=30,
                    )

                else:
                    elapsed = int(time.time() - start_time)
                    print(f"Timed out after {elapsed}s waiting for popup window.")
                    print("========================================")
                    print("  H-OTA RESULT: TIMEOUT (no popup detected)")
                    print("========================================")

            except Exception as e:
                print(f"Failed to Start Action Rule: {e}")

        except Exception as e:
            print(f"Failed during TesterMode or Gateway Type setup: {e}")

    else:
        print(
            "Failed to detect Open dialog. Please check if Ctrl+O actually opens it in H-OTA Studio."
        )


if __name__ == "__main__":
    open_hota_and_file()
