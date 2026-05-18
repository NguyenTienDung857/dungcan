# Limitations And Next Steps

## What was successfully done

- H-OTA Studio was launched/reused through the repo shortcut.
- A visible H-OTA Studio window and floating Output window were detected.
- Runtime Win32 child windows were enumerated.
- Elevated runtime scans dumped UIA trees, expanded visible combo boxes, and captured screenshots.
- Utility/dialog buttons were clicked and their opened windows were recorded.
- The disconnected exhaustive probe clicked Manual, Technical Support, EditMode, TesterMode, option/report/help dialogs, checkboxes, radio buttons, Apply buttons, and the enabled Testcase Rule Start button.
- Existing Python runner selectors and flow comments were parsed.
- Installed H-OTA schema/template/procedure/testcase/config files were inventoried.

## What was blocked

- H-OTA is a 32-bit elevated app; pywinauto warns that 32-bit Python is preferred for full Win32 automation.
- Non-admin Python can inspect only limited state and cannot reliably send Ctrl+O/clicks.
- `SAVE` was skipped because it can write the loaded `.hcfg-r` config.
- `Logout` was skipped because it changes the current H-OTA login/session state.
- Action Rule Start, Stop, `Add Processing-Preperation(OBD EMS)`, and `Add Processing` were disabled during the disconnected probe.
- Testcase Rule Start opened the expected H-OTA message: `There is no selected testcase. Please check the Properties window.`

## Next runtime scan command

Run from an Administrator 32-bit Python if available:

```powershell
cd D:\project\CANoe\CANFD\Auto\OTA
python hota_docs\scan_hota_ui.py
```

Then rerun:

```powershell
python hota_docs\build_hota_docs.py
```
