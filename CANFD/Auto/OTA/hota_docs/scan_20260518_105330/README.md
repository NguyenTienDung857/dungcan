# H-OTA Studio UI Documentation

- Generated: 2026-05-18T10:53:58
- Scanner admin: True
- Shortcut: `D:\project\CANoe\CANFD\Auto\OTA\H-OTA Studio.lnk`
- Target: `C:\WINDOWS\Installer\{08463601-4A22-4255-956E-19A04EC5DA92}\_2BD3CFB986DC9672563360.exe`
- Working directory: `C:\Program Files (x86)\GIT\H-OTA Studio GEN2\`
- Loaded config for scan: `D:\project\CANoe\CANFD\Auto\OTA\0\ReprogramConfig_0_1_1_114.hcfg-r`

## What was probed

- Launched or reused the H-OTA/ReprogramEditor window.
- Captured UI Automation trees before config load, after config load, and after clicking TesterMode.
- Expanded visible ComboBox controls and recorded options when UIA exposed them.
- Captured screenshots of the H-OTA window for the same major states.
- Parsed existing OTA Python runners for selectors and keyboard actions.
- Did not click `Start` or any update-execution action; those are documented as unsafe to invoke during discovery.

## UI tree node counts

### initial
- Button: 23
- Edit: 2
- Group: 1
- List: 1
- MenuBar: 1
- MenuItem: 1
- Pane: 11
- SplitButton: 1
- Tab: 1
- TabItem: 4
- Table: 1
- Text: 1
- TitleBar: 1
- ToolBar: 9
- Window: 1

### after_config_load
- Button: 28
- Custom: 90
- DataItem: 750
- Edit: 2
- Group: 1
- Header: 15
- List: 1
- MenuBar: 1
- MenuItem: 1
- Pane: 12
- ScrollBar: 1
- SplitButton: 1
- Tab: 2
- TabItem: 5
- Table: 2
- Text: 1
- Thumb: 1
- TitleBar: 1
- ToolBar: 9
- Window: 2

### after_tester_mode
- Button: 38
- CheckBox: 10
- ComboBox: 4
- Custom: 48
- Edit: 2
- Group: 6
- Header: 2
- HeaderItem: 4
- List: 3
- MenuBar: 1
- MenuItem: 1
- Pane: 12
- ProgressBar: 2
- RadioButton: 3
- ScrollBar: 1
- SplitButton: 1
- Tab: 2
- TabItem: 5
- Table: 1
- Text: 11
- Thumb: 2
- TitleBar: 1
- ToolBar: 9
- Window: 2

## Known automation selectors

- Main window: `title contains H-OTA Studio or ReprogramEditor`
- Open config: `Ctrl+O, then Windows Open dialog`
- TesterMode button: `title="TesterMode", control_type="Button"`
- Output pane: `title="Output", auto_id="32803", control_type="Pane"`
- Output list: `auto_id="2", control_type="List" under Output pane`
- Gateway Type combo: `auto_id="1241", control_type="ComboBox"`
- SourceAddr combo: `auto_id="1233", control_type="ComboBox"`
- Wireless-Rules radio: `auto_id="1274", control_type="RadioButton"`
- Scenario Units Apply: `auto_id="1277", control_type="Button"`
- Action Rule Start: `auto_id="1232", control_type="Button"`

## Runtime controls by type

### Button
- name=`Categorized` auto_id=`` class=`` rect=[-1914, 216, -1892, 238]
- name=`Alphabetical` auto_id=`` class=`` rect=[-1892, 216, -1870, 238]
- name=`Line up` auto_id=`UpButton` class=`` rect=[0, 0, 0, 0]
- name=`Page up` auto_id=`UpPageButton` class=`` rect=[0, 0, 0, 0]
- name=`Page down` auto_id=`DownPageButton` class=`` rect=[0, 0, 0, 0]
- name=`Line down` auto_id=`DownButton` class=`` rect=[0, 0, 0, 0]
- name=`Application menu` auto_id=`` class=`` rect=[-1920, 33, -1861, 60]
- name=`SAVE` auto_id=`` class=`` rect=[-1910, 0, -1888, 28]
- name=`Hex2Binary` auto_id=`` class=`` rect=[-1912, 65, -1851, 131]
- name=`RomPakage` auto_id=`` class=`` rect=[-1851, 65, -1786, 131]
- name=`XML Conversion` auto_id=`` class=`` rect=[-1786, 65, -1723, 131]
- name=`LogView` auto_id=`` class=`` rect=[-1723, 65, -1674, 131]
- name=`Generator` auto_id=`` class=`` rect=[-1674, 65, -1617, 131]
- name=`RFS` auto_id=`` class=`` rect=[-1617, 65, -1575, 131]
- name=`Bi-Authenticaiton` auto_id=`` class=`` rect=[-1575, 65, -1480, 131]
- name=`TesterMode` auto_id=`` class=`` rect=[-1469, 65, -1404, 131]
- name=`EditMode` auto_id=`` class=`` rect=[-1404, 65, -1349, 131]
- name=`Option` auto_id=`` class=`` rect=[-1338, 65, -1296, 131]
- name=`Error Lookup` auto_id=`` class=`` rect=[-1296, 65, -1253, 131]
- name=`Report Config` auto_id=`` class=`` rect=[-1253, 65, -1211, 131]
- name=`Logout` auto_id=`` class=`` rect=[-1211, 65, -1169, 131]
- name=`Manual` auto_id=`` class=`` rect=[-1158, 65, -1115, 131]
- name=`Technical Support` auto_id=`` class=`` rect=[-1115, 65, -1064, 131]
- name=`About` auto_id=`` class=`` rect=[-25, 35, -1, 57]
- name=`Stop` auto_id=`1219` class=`Button` rect=[-1333, 348, -1248, 373]
- name=`Start` auto_id=`1218` class=`Button` rect=[-1467, 348, -1382, 373]
- name=`Stop` auto_id=`1231` class=`Button` rect=[-953, 348, -868, 373]
- name=`Start` auto_id=`1232` class=`Button` rect=[-1081, 348, -996, 373]
- name=`Open` auto_id=`DropDown` class=`` rect=[-991, 243, -976, 265]
- name=`Apply` auto_id=`1270` class=`Button` rect=[-611, 910, -479, 946]
- name=`Apply` auto_id=`1277` class=`Button` rect=[-611, 498, -479, 532]
- name=`Open` auto_id=`DropDown` class=`` rect=[-1253, 243, -1238, 265]
- name=`Open` auto_id=`DropDown` class=`` rect=[-557, 243, -542, 265]
- name=`Open` auto_id=`DropDown` class=`` rect=[-357, 243, -342, 265]
- name=`` auto_id=`` class=`Button` rect=[-22, 157, -7, 172]
- name=`Minimize` auto_id=`` class=`` rect=[-67, -1, -43, 23]
- name=`Restore` auto_id=`` class=`` rect=[-45, -1, -21, 23]
- name=`Close` auto_id=`` class=`` rect=[-23, -1, 1, 23]

### ComboBox
- name=`` auto_id=`1233` class=`ComboBox` rect=[-1137, 242, -975, 266]
- name=`` auto_id=`1241` class=`ComboBox` rect=[-1361, 242, -1237, 266]
- name=`Procedure Model Type :` auto_id=`1250` class=`ComboBox` rect=[-777, 242, -541, 266]
- name=`DLC : ` auto_id=`1235` class=`ComboBox` rect=[-437, 242, -341, 266]

### RadioButton
- name=`Wired-Rules` auto_id=`1273` class=`Button` rect=[-713, 386, -599, 408]
- name=`Wireless-Rules` auto_id=`1274` class=`Button` rect=[-713, 430, -579, 450]
- name=`OTA-estimatedRuntime` auto_id=`1275` class=`Button` rect=[-713, 470, -537, 490]

### CheckBox
- name=`For Cooperative ECU` auto_id=`1234` class=`Button` rect=[-329, 242, -171, 268]
- name=`Add Pre Procedure` auto_id=`1236` class=`Button` rect=[-713, 678, -479, 706]
- name=`Add Background-Transfer` auto_id=`1260` class=`Button` rect=[-713, 592, -461, 608]
- name=`Add Processing-Preperation(OBD EMS)` auto_id=`1261` class=`Button` rect=[-713, 636, -419, 652]
- name=`Add Processing` auto_id=`1262` class=`Button` rect=[-713, 730, -419, 746]
- name=`Add Activation` auto_id=`1263` class=`Button` rect=[-713, 774, -419, 790]
- name=`Add Rollback` auto_id=`1264` class=`Button` rect=[-713, 818, -419, 834]
- name=`Add Post Procedure` auto_id=`1240` class=`Button` rect=[-713, 858, -479, 886]
- name=`OEUK Vehicle` auto_id=`1224` class=`Button` rect=[-161, 246, -47, 266]
- name=`NFS` auto_id=`1225` class=`Button` rect=[-35, 246, 21, 266]

### Edit
- name=`Log Path :` auto_id=`33001` class=`Edit` rect=[-1832, 623, -224, 638]
- name=`` auto_id=`4` class=`Edit` rect=[-1863, 215, -1526, 239]

### List
- name=`` auto_id=`2` class=`ListBox` rect=[-1910, 646, -1524, 1015]
- name=`` auto_id=`1222` class=`SysListView32` rect=[-1081, 440, -799, 958]
- name=`Action Rule` auto_id=`1221` class=`SysListView32` rect=[-1467, 440, -1161, 958]

### Tab
- name=`` auto_id=`1` class=`BCGPTabWnd:2d0000:8:10003:10` rect=[-1913, 643, -1521, 1037]
- name=`` auto_id=`` class=`BCGPTabWnd:2d0000:8:10003:10` rect=[-1516, 153, 0, 1008]

### TabItem
- name=`Log` auto_id=`` class=`` rect=[-1913, 1017, -1882, 1035]
- name=`Home` auto_id=`` class=`` rect=[-1860, 33, -1802, 60]
- name=`Edit` auto_id=`` class=`` rect=[-1802, 33, -1754, 60]
- name=`Window` auto_id=`` class=`` rect=[-1754, 33, -1680, 60]
- name=`ReprogramConfig_0_1_1_114.hcfg-r` auto_id=`` class=`` rect=[-1511, 155, -1279, 176]

### MenuBar
- name=`System` auto_id=`MenuBar` class=`` rect=[-1920, 0, -1898, 22]

### MenuItem
- name=`System` auto_id=`` class=`` rect=[-1920, 0, -1898, 22]

### Pane
- name=`Output` auto_id=`` class=`BCGPMiniFrame:2d0000:8:10003:10` rect=[-1917, 588, -1517, 1017]
- name=`` auto_id=`` class=`BCGPShadowSideWnd` rect=[-1923, 1017, -1511, 1023]
- name=`` auto_id=`` class=`BCGPShadowSideWnd` rect=[-1517, 588, -1511, 1017]
- name=`` auto_id=`` class=`BCGPShadowSideWnd` rect=[-1923, 582, -1511, 588]
- name=`` auto_id=`` class=`BCGPShadowSideWnd` rect=[-1923, 588, -1917, 1017]
- name=`Output` auto_id=`32803` class=`BCGPControlBar:2d0000:8:10003:10` rect=[-1913, 619, -1521, 1013]
- name=`` auto_id=`59392` class=`BCGPToolBar:2d0000:8:10003:10` rect=[-1913, 619, -205, 643]
- name=`Properties` auto_id=`32860` class=`BCGPControlBar:2d0000:8:10003:10` rect=[-1920, 153, -1522, 1008]
- name=`PropertyListToolbar` auto_id=`59392` class=`BCGPToolBar:2d0000:8:10003:10` rect=[-1916, 215, -1867, 241]
- name=`` auto_id=`160` class=`BCGPToolBar:2d0000:8:10003:10` rect=[-1920, 188, -1522, 213]
- name=`Workspace` auto_id=`59648` class=`MDIClient` rect=[-1516, 153, 0, 1008]
- name=`` auto_id=`59648` class=`AfxFrameOrView140u` rect=[-1511, 180, -5, 1003]

### Group
- name=`Ribbon Tabs` auto_id=`` class=`` rect=[-1860, 33, -88, 60]
- name=`Action Rule` auto_id=`1239` class=`Button` rect=[-1103, 314, -351, 972]
- name=`Testcase Rule` auto_id=`1238` class=`Button` rect=[-1483, 314, -1143, 972]
- name=`Add Rule Units` auto_id=`1271` class=`Button` rect=[-743, 560, -379, 958]
- name=`Use Options` auto_id=`1272` class=`Button` rect=[-1483, 202, 35, 292]
- name=`Add Scenario Units` auto_id=`1276` class=`Button` rect=[-743, 342, -379, 546]

### Text
- name=`Log Path :` auto_id=`65535` class=`Static` rect=[-1912, 622, -204, 646]
- name=`` auto_id=`` class=`` rect=[3, 3, 142, 21]
- name=`SourceAddr : ` auto_id=`1269` class=`Static` rect=[-1231, 248, -1139, 264]
- name=`0.00%` auto_id=`1278` class=`Static` rect=[-841, 404, -785, 420]
- name=`0.00%` auto_id=`1279` class=`Static` rect=[-1219, 406, -1161, 422]
- name=`` auto_id=`` class=`` rect=[3, 3, 104, 21]
- name=`GatewayType : ` auto_id=`1280` class=`Static` rect=[-1475, 248, -1369, 264]
- name=`Procedure Model Type :` auto_id=`` class=`Static` rect=[-941, 248, -779, 264]
- name=`Procedure Model Type :` auto_id=`` class=`` rect=[3, 3, 216, 21]
- name=`DLC : ` auto_id=`1281` class=`Static` rect=[-483, 248, -443, 264]
- name=`DLC : ` auto_id=`` class=`` rect=[3, 3, 76, 21]

## ComboBox options observed

- `1233` auto_id=`1233` status=`expanded`
  - `CCU/CWC : 0x0F00`
  - `ERT : 0x0E81`
  - `HPVC : 0x0F10`
  - `260424_174345_decrypted`
  - `Codex`
  - `Custom Office Templates`
  - `My Data Sources`
  - `OTA`
  - `PlatformIO`
  - `WindowsPowerShell`
  - `CCS`
  - `INF`
  - `MEM`
  - `OWD`
  - `OWP`
  - `yulink files - Shortcut`
  - `yess.yura.co.kr:9000/idp/profile/Logout?execution=e1s1`
  - `[TEST] ME1 NA SOP OTA 검증 (HU OTA) - 제어기검증팀_베트남_협업공간(SYS_VIETNAM) - Confluence`
  - `HFS /Gradius(DLP)/`
  - `Note - 베트남SW연구소 - Confluence`
  - `Lesson 2: Prerequisites - ✉️ AI Email Employee · Agents In A Box`
  - `Claude`
  - `Edit - 2026-04-15 - 베트남SW연구소 - Confluence`
  - `Remove newline requirement from serial command processing | Claude Code`
  - `Analyze OLED buffer handling and serial input processing | Claude Code`
  - `Recycle Bin`
  - `Google Chrome`
  - `H-OTA Studio Updater`
  - `H-OTA Studio`
  - `Microsoft Edge`
  - `MiniTool ShadowMaker`
  - `PotPlayer 64 bit`
  - `Vector vTESTstudio Viewer 8`
  - `yulink`
- `1241` auto_id=`1241` status=`expanded`
  - `CCU2 : DoIP`
  - `CCU1 : EthDiag`
  - `260424_174345_decrypted`
  - `Codex`
  - `Custom Office Templates`
  - `My Data Sources`
  - `OTA`
  - `PlatformIO`
  - `WindowsPowerShell`
  - `CCS`
  - `INF`
  - `MEM`
  - `OWD`
  - `OWP`
  - `yulink files - Shortcut`
  - `yess.yura.co.kr:9000/idp/profile/Logout?execution=e1s1`
  - `[TEST] ME1 NA SOP OTA 검증 (HU OTA) - 제어기검증팀_베트남_협업공간(SYS_VIETNAM) - Confluence`
  - `HFS /Gradius(DLP)/`
  - `Note - 베트남SW연구소 - Confluence`
  - `Lesson 2: Prerequisites - ✉️ AI Email Employee · Agents In A Box`
  - `Claude`
  - `Edit - 2026-04-15 - 베트남SW연구소 - Confluence`
  - `Remove newline requirement from serial command processing | Claude Code`
  - `Analyze OLED buffer handling and serial input processing | Claude Code`
  - `Recycle Bin`
  - `Google Chrome`
  - `H-OTA Studio Updater`
  - `H-OTA Studio`
  - `Microsoft Edge`
  - `MiniTool ShadowMaker`
  - `PotPlayer 64 bit`
  - `Vector vTESTstudio Viewer 8`
  - `yulink`
- `Procedure Model Type :` auto_id=`1250` status=`expanded`
  - `GroupA(LX3,SW1,LQ2,JG1)`
  - `GroupB(GN7PE~)`
  - `260424_174345_decrypted`
  - `Codex`
  - `Custom Office Templates`
  - `My Data Sources`
  - `OTA`
  - `PlatformIO`
  - `WindowsPowerShell`
  - `CCS`
  - `INF`
  - `MEM`
  - `OWD`
  - `OWP`
  - `yulink files - Shortcut`
  - `yess.yura.co.kr:9000/idp/profile/Logout?execution=e1s1`
  - `[TEST] ME1 NA SOP OTA 검증 (HU OTA) - 제어기검증팀_베트남_협업공간(SYS_VIETNAM) - Confluence`
  - `HFS /Gradius(DLP)/`
  - `Note - 베트남SW연구소 - Confluence`
  - `Lesson 2: Prerequisites - ✉️ AI Email Employee · Agents In A Box`
  - `Claude`
  - `Edit - 2026-04-15 - 베트남SW연구소 - Confluence`
  - `Remove newline requirement from serial command processing | Claude Code`
  - `Analyze OLED buffer handling and serial input processing | Claude Code`
  - `Recycle Bin`
  - `Google Chrome`
  - `H-OTA Studio Updater`
  - `H-OTA Studio`
  - `Microsoft Edge`
  - `MiniTool ShadowMaker`
  - `PotPlayer 64 bit`
  - `Vector vTESTstudio Viewer 8`
  - `yulink`
- `DLC : ` auto_id=`1235` status=`failed`

## Existing Python runner model

- Parsed Python files: 33
- Common flow: launch shortcut, Ctrl+O load `*.hcfg-r`, click TesterMode, set Gateway Type, choose Wireless/Wired rule, Apply, Start, mirror Output pane, wait result popup, capture screenshots, close H-OTA, rename ASC logs.
- Existing runner selectors and send_keys calls are stored in `static_script_selectors.json`.

## Files in this documentation set

- `ui_tree_initial.json`
- `ui_tree_after_config_load.json`
- `ui_tree_after_tester_mode.json`
- `control_inventory.json`
- `combo_options.json`
- `static_script_selectors.json`
- `action_log.json`
- `top_windows.json`

## Automation guidance

- Prefer UIA selectors (`title`, `auto_id`, `control_type`) over coordinate clicks.
- Treat `Start` (`auto_id=1232`) as a destructive execution control; only invoke it inside a testcase runner with CANoe/AppState handling active.
- Keep the Output pane polling selector stable: `Output` pane `auto_id=32803`, child list `auto_id=2`.
- If a combo option cannot be selected by UIA `.select(...)`, existing runners fall back to focus + `send_keys("label{ENTER}")`.
- For ERT flows, set Gateway Type first, then SourceAddr (`auto_id=1233`) before selecting Wired-Rules.
