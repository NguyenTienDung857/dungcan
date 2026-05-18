# Automation Selectors

## Stable selectors from existing runners

- Main window: Find visible top window whose title contains `H-OTA Studio` or `ReprogramEditor`.
- Open file: `send_keys("^o")`, then Windows `Open` dialog, then type full `*.hcfg-r` path and Enter.
- TesterMode: `child_window(title="TesterMode", control_type="Button")`.
- Gateway Type: `child_window(auto_id="1241", control_type="ComboBox")`, select `CCU1 : EthDiag`.
- SourceAddr: `child_window(auto_id="1233", control_type="ComboBox")`, select `ERT :0x1000` for ERT flows.
- Wireless-Rules: `child_window(auto_id="1274", control_type="RadioButton")`.
- Wired-Rules: `child_window(title_re=".*Wired.*Rule.*", control_type="RadioButton")`.
- Scenario Units Apply: `child_window(auto_id="1277", control_type="Button")`.
- Action Rule Start: `child_window(auto_id="1232", control_type="Button")`.
- Output pane: `child_window(title="Output", auto_id="32803", control_type="Pane")`.
- Output list: child list `auto_id="2"` under Output pane; Win32 class can appear as `ListBox`.

## auto_id frequency

- `32803`: 31
- `2`: 31
- `1241`: 31
- `1277`: 31
- `1232`: 31
- `1274`: 18
- `1233`: 8

## action keyword coverage

- `TesterMode`: 31 files
- `Gateway Type`: 31 files
- `Wireless-Rules`: 18 files
- `Apply`: 31 files
- `Start`: 31 files
- `Output`: 31 files
- `AppState`: 31 files
- `Add Activation`: 2 files
- `Add Rollback`: 3 files
- `OEUK`: 2 files
- `SourceAddr`: 8 files
- `Wired-Rules`: 8 files
- `ERT`: 8 files

Raw data: `automation_selectors.json`.
