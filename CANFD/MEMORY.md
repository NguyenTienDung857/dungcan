# Project Memory - CANFD Built-in Cam

This file stores durable project knowledge for future Codex sessions.
Update it when a session learns something that should survive beyond the
current task.

Categories:

- `preference` - how the user wants work done
- `decision` - past choices and reasoning
- `context` - project-specific knowledge
- `pattern` - recurring workflows
- `mistake` - things to avoid

Entry format:

```text
YYYY-MM-DD | category | short title
- Facts:
- Evidence:
- Use next time:
```

## preference

2026-05-15 | preference | OTA Python case filenames use three-digit numbering
- Facts: The user asked to rename older OTA Python case files such as
  `case_11.py` to the uniform `case_0xx.py` style.
- Evidence: `case_9.py`, `case_9_2.py`, `case_10.py`, `case_10_2.py`,
  `case_11.py`, `case_12.py`, `case_15.py`, and `case_19.py` were renamed to
  `case_009.py`, `case_009_2.py`, `case_010.py`, `case_010_2.py`,
  `case_011.py`, `case_012.py`, `case_015.py`, and `case_019.py`.
- Use next time: New OTA Python runner files should use `case_###.py`; for
  secondary runner variants, keep the padded main case number, for example
  `case_009_2.py`.

2026-05-08 | preference | Do not create wrapper helpers for header calls
- Facts: The user corrected `OWD_008` because local helpers were wrapping
  shared header helpers or hiding simple capture/logging behind test-specific
  functions.
- Evidence: User said `OWD_008` should use functions already in the header and
  not create a private function whose body only calls the header function.
- Use next time: Call `Auto/DeclarationFunction_Gen2.5.h` helpers directly
  inside testcase bodies. Do not add local wrapper functions that only delegate
  to header helpers or hide simple step evidence capture.

2026-05-12 | preference | Prefer testWaitForTimeout over Clock_Down_ms in tests
- Facts: The user corrected OWP_006 and then asked to remove remaining
  testcase uses of `Clock_Down_ms(...)`.
- Evidence: Existing `.can` call sites in OWP_005, OWP_023, OWP_025, and
  OWD_003 were replaced with `testWaitForTimeout(...)`.
- Use next time: Do not add new testcase calls to `Clock_Down_ms(...)`; use
  `testWaitForTimeout(...)` directly for explicit waits unless the user
  explicitly requests the countdown helper.

2026-05-05 | preference | Write requested CAPL steps sequentially
- Facts: When the user provides an explicit step order, they prefer the CAPL
  code to stay sequential inside the testcase body, calling existing helper
  functions directly. A longer file is acceptable if the flow is easy to read.
- Evidence: User said OWD_007 should not wrap the requested actions into a
  shared local flow helper like `Run_TML_DEV_And_Camera_Action()` because the
  work is just calling helper functions in order.
- Use next time: Do not collapse user-provided step flows into generic local
  `Run_*` helpers. Keep the action sequence visible in each testcase unless the
  user explicitly asks for a shared helper or abstraction.

2026-04-29 | preference | Keep test coding inside Auto
- Facts: The user wants Codex to write test cases in `Auto` only.
- Evidence: User explicitly said the goal is to write test cases in `Auto` and
  not touch other folders.
- Use next time: Treat `Auto/**` as the normal writable source area. Treat
  `CAPL/**`, project config, reports, and generated artifacts as read-only
  unless the user explicitly changes the scope.

2026-04-29 | preference | OWD and OWP are the canonical CAPL style
- Facts: The user considers the OWD and OWP test files the standard structure
  because they wrote those tests themselves.
- Evidence: User corrected the initial guidance and said the standard test
  structure should be like OWD or OWP.
- Use next time: Use `Auto/OWD` and `Auto/OWP` as the primary style reference
  for new test structure. Use the target module's samples only for signal logic
  and feature-specific details.

2026-04-29 | preference | Use header functions first
- Facts: The user emphasized that an important rule is to use the functions
  already available in the header.
- Evidence: User said "luat quan trong la dung cac ham co trong header ay".
- Use next time: Search `Auto/DeclarationFunction_Gen2.5.h` before coding any
  action, wait, check, capture, timer, COM marker, or signal manipulation. Do
  not duplicate helper logic when the header has a suitable function.

## decision

2026-04-29 | decision | Agent continuity files initialized at repo root
- Facts: Added repo-level `AGENTS.md`, `STATUS.md`, `MEMORY.md`, plus
  `Auto/AGENTS.md` for folder-specific CAPL rules.
- Evidence: These files encode scope boundaries, read order, memory categories,
  and Auto-only coding workflow.
- Use next time: Start by reading `STATUS.md`, `MEMORY.md`, root `AGENTS.md`,
  and `Auto/AGENTS.md`.

2026-04-29 | decision | Shared helper header is default read/use target
- Facts: `Auto/DeclarationFunction_Gen2.5.h` is the primary helper library for
  new tests.
- Evidence: User called out the header at the top of `Auto`; inspection shows it
  contains key/power, mode, recording, waiting, checking, capture, timer, COM,
  and panel helper functions.
- Use next time: Search this header before writing implementation code. Use its
  helpers by default. Edit it only when explicitly requested or when a shared
  helper is clearly needed.

## context

2026-05-18 | context | H-OTA UI documentation and scan artifacts
- Facts: H-OTA Studio runtime and installed-file documentation now lives under
  `Auto/OTA/hota_docs`. The latest successful elevated runtime scan is
  `Auto/OTA/hota_docs/scan_20260518_105845`; the latest disconnected exhaustive
  click probe is `Auto/OTA/hota_docs/exhaustive_probe_20260518_111521`.
- Evidence: `scan_hota_ui.py` loaded
  `Auto/OTA/0/ReprogramConfig_0_1_1_114.hcfg-r`, clicked `TesterMode`, dumped
  UI trees/screenshots, expanded visible combo boxes, and invoked 11 safe
  utility/dialog buttons. `exhaustive_hota_probe.py` clicked Manual, Technical
  Support, EditMode, TesterMode, option/report/help dialogs, rule checkboxes,
  radio buttons, Apply buttons, and the enabled Testcase Rule Start button in a
  disconnected session. Testcase Rule Start opened the expected no-selected-
  testcase popup, while Action Rule Start and Stop were disabled; `SAVE` and
  `Logout` were skipped. `build_hota_docs.py` generated Markdown/JSON docs plus
  `hota_full_ui_dump.txt` and `hota_exhaustive_click_dump.txt`.
- Use next time: Start H-OTA automation work by reading
  `Auto/OTA/hota_docs/README.md`, `automation_selectors.md`, and
  `runtime_ui_scan.md`, then use `hota_full_ui_dump.txt` for the combined
  selector/layout/click evidence before adding coordinate clicks.

2026-05-18 | context | H-OTA scanner needs elevated Python
- Facts: H-OTA Studio runs as a 32-bit elevated GUI app in this workspace.
  Non-admin Python can see only limited UIA data and cannot reliably send
  Ctrl+O/clicks. Elevated Python completed the scanner, though pywinauto still
  warns that 32-bit Python is preferred for full Win32 automation.
- Evidence: Non-admin `scan_hota_ui.py` failed to open the file dialog; the
  elevated run created `scan_20260518_105845` with `load_config`,
  `click TesterMode`, and four ComboBox probes in `action_log.json`.
- Use next time: Run UI discovery from an Administrator shell and prefer
  selector-based UIA/Win32 access. Do not click `Start` during normal discovery;
  only use the exhaustive click probe when the user explicitly confirms a
  disconnected/safe session. Keep `SAVE` and `Logout` skipped unless the user
  explicitly asks to change config or login/session state.

2026-05-16 | context | BAS 2.0 runs should be fully prompt-free
- Facts: `BAS_016`, `BAS_017`, and `BAS_018` 2.0 automation should not ask for
  recorded-file deletion confirmation, ACC/IGN confirmation, test ID, or
  battery type.
- Evidence: The 2026-05-16 BAS cleanup removed `testWaitForMessageBox` from
  the three BAS 2.0 files and changed local Gen2 `Set_Battery()` to auto-use
  eAGM/SOC `ICU_BS2SoC = 0x50`.
- Use next time: Keep BAS 2.0 pass/fail based on CAN-visible mode, key,
  parking recording, battery, profile, and setting signals. Do not restore
  operator prompts for video deletion or battery selection.

2026-05-15 | context | OTA case 010/012 AppState handoff waits for SecurityAccess
- Facts: `Auto/OTA/case_010.py` and `Auto/OTA/case_012.py` send their
  mid-run `PythonCom::AppState` only after detecting `activate-rule`, then a
  later `securityAccess` output line, then waiting 5 seconds.
- Evidence: The prior `routineControl` trigger was replaced by
  `securityAccess` detection for AppState 10 and AppState 12 handoffs.
- Use next time: For these OTA cases, preserve the
  `activate-rule -> securityAccess -> 5s -> AppState` ordering unless the user
  asks for a different H-OTA synchronization point.

2026-05-15 | context | OTA CAPL captures include AppState
- Facts: OTA `.can` screenshot signal lists now include
  `PythonCom::AppState` for direct `CaptureGraphics("OTA", "...")` calls and
  the local `gCaptureSignals` variables in OTA_009, OTA_010, and OTA_011.
- Evidence: Static scan found no direct OTA `CaptureGraphics(...)` list or
  OTA `gCaptureSignals` definition missing `PythonCom::AppState`.
- Use next time: Preserve `PythonCom::AppState` when changing OTA CAPL capture
  signal lists so reports show the Python handoff/result state.

2026-05-15 | context | OTA Python runners tee console output to Log_python
- Facts: OTA Python runners now import `start_python_log(...)` from
  `Auto/OTA/python_run_log.py` and call it at the start of
  `open_hota_and_file()`.
- Evidence: Each run writes stdout/stderr to
  `Auto/OTA/Log_python/<script_name>_YYYYMMDD_HHMMSS.log` while keeping the
  popup/console output visible.
- Use next time: For new OTA Python runners, include the same
  `start_python_log(script_dir, script_name)` startup call so run logs stay
  consistent.

2026-05-15 | context | OTA Python auto-discovers H-OTA config filename
- Facts: OTA Python runners should not hardcode a specific
  `ReprogramConfig_*.hcfg-r` filename. Each runner keeps only the selected
  config folder number (`0`, `1`, `2`, or `3`) and calls
  `find_hota_config_file(...)` from `Auto/OTA/hota_config.py`.
- Evidence: Added `Auto/OTA/hota_config.py` and updated all 31
  `Auto/OTA/*.py` runner scripts. Static scan found no hardcoded
  `ReprogramConfig_0_1_1_*` filename in Python and 31 helper call sites.
- Use next time: For OTA Python H-OTA file selection, keep using the helper so
  future `.hcfg-r` filename changes do not require editing every runner.

2026-05-15 | context | OTA_021-OTA_024 ERT scripts split to case files
- Facts: `Auto/OTA/case_021.py` through `case_024.py` are exact copies of
  `update_to_00_ERT.py` through `update_to_03_ERT.py` respectively. The matching
  `OTA_021.can` through `OTA_024.can` main Step 3 flows call the new case files.
- Evidence: SHA-256 hashes matched for all four source/copy pairs, and a static
  scan showed the `.can` files reference `case_021.py` through `case_024.py`.
- Use next time: For later OTA ERT case splits, keep setup update scripts
  unchanged and only redirect the main testcase Python syscall to the new
  `case_xxx.py` copy requested by the user.

2026-05-13 | context | BAS 2.0 sequential run and BAS window captures
- Facts: `Auto/BAS/BAS_016 - 2.0 generation.can`,
  `Auto/BAS/BAS_017 - 2.0 generation.can`, and
  `Auto/BAS/BAS_018 - 2.0 generation.can` now run all cases sequentially from
  `MainTest()` instead of prompting for a case ID. BAS signal screenshots use
  `CaptureGraphics("BAS", "...")`.
- Evidence: The user corrected that the graphic window name for screenshots is
  `BAS`, and requested the case-selection prompt be removed.
- Use next time: For BAS 2.0 files, capture related signals on window `BAS`.
  These files include `Auto/BAS/DeclarationFunction_Gen2.h`, so keep the
  Gen2.5-compatible `CaptureGraphics(char window[], char signalList[])` helper
  available in the BAS local header instead of including both headers with
  overlapping common function names.

2026-05-13 | context | Dealer PowerAutoCut shortcut helper
- Facts: `Auto/DeclarationFunction_Gen2.5.h` now has `Set_Dealer_Mode()`.
  It calls `Set_PowerAutoCut_Mode(0x1, 30000)` to set
  `ICU_PowerAutoCutModSta` to Dealer Mode with the same 30-second timeout style
  as `Set_Factory_Mode()` and `Set_Customer_Mode()`.
- Evidence: User requested a helper based on `Set_Factory_Mode()` for
  `ICU_PowerAutoCutMode = 0x1 (Dealer mode)`.
- Use next time: Call `Set_Dealer_Mode()` from tests instead of directly
  calling `Set_PowerAutoCut_Mode(0x1, 30000)`.

2026-05-12 | context | OWP_006 OWP/PEV/PMD B+ interruption rewrite
- Facts: `Auto/OWP/OWP_006 - 2.5 generation.can` now implements the user's
  requested 7-step flow using direct shared-header helper calls. Steps 1-5
  cover OWP Recording around B+ OFF/ON, PEV/event switch, and rear camera
  detach/attach variants. Steps 6-7 cover PMD/PEV Recording around B+ OFF/ON.
  The file uses `testWaitForTimeout(...)` for explicit waits and Steps 2-5
  include a 3-second delay between `KEY_ON()` and `KEY_OFF()`. Wait helper
  timeouts are passed directly as literal values instead of stored in local
  timeout variables.
- Evidence: User provided the exact OWP_006 step sequence on 2026-05-12 and
  asked to overwrite the old OWP_006 file while using helpers from
  `Auto/DeclarationFunction_Gen2.5.h`; later corrected the file to avoid
  `Clock_Down_ms(...)` and add a small KEY ON/OFF delay.
- Use next time: Preserve the visible sequential order and direct helper calls.
  Do not restore `Clock_Down_ms(...)`, local timeout variables, or the previous
  repeated MAR/PEV manual-notification flow unless the user explicitly asks.

2026-05-12 | context | PEV_Recorded OFF/default is 0x0
- Facts: In `OWP_011`, the OFF/default expectation for
  `BLTN_CAM_PEV_Recorded` is `0x0`, not `0x1`.
- Evidence: The user corrected Step 5 and Step 18 where the long wait after
  `BLTN_CAM_PEV_Recorded = 0x2` should check the signal returning to `0x0`.
- Use next time: When checking `BLTN_CAM_PEV_Recorded` after a PEV event has
  ended, use `0x0` for OFF/default. Do not label `0x1` as OFF for this signal.

2026-05-12 | context | Click_USM_Reset sets sensitivity inline
- Facts: `Click_USM_Reset()` now sets DEV and PEV sensitivity to level 1 while
  it is already inside the single BLTN_CAM flow.
- Evidence: The user asked to use only the core of `Set_DEV_Sensitivity` and
  `Set_PEV_Sensitivity`, skipping their BLTN_CAM enter/exit steps because
  `Click_USM_Reset()` enters once to set everything.
- Use next time: Do not call the full sensitivity helpers from this reset flow.
  Inline the trackbar assignment, wait, retry/check, and final signal
  verification when sensitivity must be changed before the single BLTN_CAM
  exit.

2026-05-12 | context | Click_USM_Reset sets event time inline
- Facts: `Click_USM_Reset()` now sets EV before/after recording time to
  10 seconds while it is already inside the single BLTN_CAM flow.
- Evidence: The user asked to copy only the core logic from
  `Set_EV_BeforeTime()` and `Set_EV_AfterTime()` because those full helpers
  enter BLTN_CAM themselves.
- Use next time: Keep EV before-time as `TrackBar_EV_Time_Bfr = 3` for 10s and
  EV after-time as `TrackBar_EV_Time_Aft = 10`; do not call the full EV time
  helpers from `Click_USM_Reset()`.

2026-05-11 | context | Click_USM_Reset manually applies core recording settings
- Facts: `Auto/DeclarationFunction_Gen2.5.h` `Click_USM_Reset()` now clicks
  USM Reset, manually enters BLTN_CAM with up to 10 attempts, turns ON OWD,
  OWP, PEV, then DEV while staying inside BLTN_CAM, and manually exits AVN with
  up to 10 attempts.
- Evidence: User requested not to call the existing `Turn_ON_*` helpers because
  they contain their own BLTN_CAM enter/exit behavior.
- Use next time: Preserve this single-enter/single-exit manual flow inside
  `Click_USM_Reset()` unless the user asks to restore the old simple reset or
  refactor the shared helper behavior.

2026-05-06 | context | OWP_025 PMD OFF radar-only camera detach automation
- Facts: `Auto/OWP/OWP_025 - 2.5 generation.can` implements the requested
  steps `1, 2, 3, 5, 7, 8, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20` as
  sequential testcase bodies. It uses 8-minute network sleep waits, several
  minute signal waits, and a 5-second delay before final screenshots.
  `Set_Detach_Front` maps to the existing header helper `Set_Detach_Font()`.
- Evidence: User requested OWP_025 on 2026-05-06 with attach/detach front/rear
  camera steps, PMD OFF checks, PEV Recording checks, and final evidence
  screenshots for each step.
- Use next time: Preserve the requested visible step order. For Step 18 and
  Step 20, treat `BLTN_CAM_RecSta_PMD == 0x2` as the requested radar-only
  acceptance check; `Check_RecSta_PMD_Recording()` may be logged separately
  because the shared helper checks `0x1`.

2026-05-05 | context | OWP_023 PMD/PEV network-sleep automation
- Facts: `Auto/OWP/OWP_023 - 2.5 generation.can` implements the user's
  requested steps `1, 2, 4, 6, 9, 11, 15, 16, 17, 19, 20` as sequential
  testcase bodies. All network sleep waits are capped at 8 minutes, final
  screenshots wait 5 seconds first, and PMD Recording waits use the shared
  `WaitFor_RecSta_PMD_Recording(max_signal_wait_ms)` helper.
- Evidence: User requested OWP_023 on 2026-05-05 with explicit step action
  order, 8-minute sleep waits, several-minute signal waits, screenshots at the
  end of each step, and later clarified that PMD wait logic must call
  `WaitFor_RecSta_PMD_Recording()` because its internal PEV trigger is intended.
- Use next time: Preserve the visible step order, the 8-minute sleep cap, and
  the shared PMD Recording wait helper unless the user asks to change the
  timing.

2026-05-05 | context | OWP_005 one-file sequential rewrite
- Facts: `Auto/OWP/OWP_005 - 2.5 generation.can` replaces the old split
  `OWP_005_A` and `OWP_005_B` source files. It follows the user's new OWP_005
  step order in one file and keeps the helper calls sequential inside each
  testcase. PMD Recording waits use `WaitFor_RecSta_PMD_Recording(360000)`.
- Evidence: User corrected the implementation direction on 2026-05-05: merge
  OWP_005 into one file, rewrite using the new logic, and remove the old flow.
- Use next time: Preserve one OWP_005 source file and do not restore the
  A/B split or older manual/action-log flow unless the user asks.

2026-05-04 | context | CCS_012 SD card park-operation terminate automation
- Facts: `Auto/CCS/CCS_012 - 2.5 generation.can` now follows the user's
  helper-based 4-step flow for OWP parking operation around SD card actions and
  network sleep. Step 2 repeats SD card + OWP wait + PrkOp `0x5` checks up to
  3 attempts; Step 3 checks PrkOp `0x4` within 40 seconds; Step 4 calls
  `Set_SDcard()` 9 times and checks that PrkOp was `0x5` within 2 minutes.
  Since there is no shared header helper for `BLTN_CAM_PrkOp_Termiate_Info`,
  the file polls that signal locally with `testWaitForTimeout(...)`.
- Evidence: User provided the exact 4-step CCS_012 flow on 2026-05-04, and the
  file was reworked with COM markers `31200-31204` plus screenshots including
  `BLTN_CAM_SD_Card_State`.
- Use next time: Preserve this 4-step SD/network-sleep flow and do not restore
  the older CCS_012 10-step SoC/NM/PMD sequence unless the user asks.

2026-05-04 | context | OWD_012 OWD/DEV mode and power-cycle automation
- Facts: `Auto/OWD/OWD_012 - 2.5 generation.can` now follows the user's
  helper-based 5-step flow: setup with KEY OFF/ON, SMK IGN, USM/Rec reset,
  OWD/DEV waits, repeated BLTN_CAM enter/exit cycles, KEY OFF, then KEY ON and
  B+ OFF/ON. Pass/fail is based on requested OWD/DEV wait helpers and UI mode
  checks after enter/exit helpers.
- Evidence: User provided the exact step list on 2026-05-04 and the existing
  OWD_012 file was reworked with COM markers `11200-11205`.
- Use next time: Preserve the requested action order and do not restore the
  old OWD_012 multiple-event/log-only flow unless the user explicitly asks.

2026-05-04 | context | OWD_008 B+ OFF/ON interruption automation
- Facts: `Auto/OWD/OWD_008 - 2.5 generation.can` implements the user's
  helper-based B+ OFF/ON flow for steps 1-6. The test uses B+ helpers around
  OWD, DEV, TML, event switch, and rear camera detach actions. Pass/fail is
  based on the requested OWD/DEV wait helpers before each B+ interruption.
- Evidence: User provided the exact step list on 2026-05-04 and the new test
  file was created under `Auto/OWD` with COM markers `10800-10806`.
- Use next time: Preserve the exact B+ action order unless the user asks to add
  signal checks after B+ restore or rear-camera state validation.

2026-05-04 | context | OWD_007 camera detach/attach automation
- Facts: `Auto/OWD/OWD_007 - 2.5 generation.can` now follows the user's
  helper-based camera detach/attach flow for steps 1-6, 8-10, and 12-15. Step
  13 repeats the Step 12 flow 10 times and Step 15 repeats the Step 14 flow 10
  times. The requested action order is now written sequentially inside each
  testcase; do not reintroduce local flow wrappers such as
  `Run_TML_DEV_And_Camera_Action()`, `Run_Step12_Flow()`, or
  `Run_Step14_Flow()`. Step 7 and Step 11 are reserved/no-action because the
  requested flow skips those numbers.
- Evidence: User provided the exact step list on 2026-05-04 and later corrected
  on 2026-05-05 that OWD_007 should not use a shared flow helper; the file was
  reworked to call the shared helper functions directly inside each testcase.
- Use next time: Preserve this sequential helper-call OWD_007 flow and only
  fill Step 7 or Step 11 if the user provides missing actions for those
  numbers.

2026-05-04 | context | COM helper values for camera and SD card controls
- Facts: Shared helpers in `Auto/DeclarationFunction_Gen2.5.h` map front/rear
  camera and SD card actions to `Send_Value_To_COM` values: attach font/front
  = 3, detach font/front = 4, attach rear = 5, detach rear = 6, SD card = 7
  followed by a 5-second wait.
- Evidence: User requested these exact helper names and COM values.
- Use next time: Call `Set_Attach_Font()`, `Set_Detach_Font()`,
  `Set_Attach_Rear()`, `Set_Detach_Rear()`, and `Set_SDcard()` from tests
  instead of sending those COM values directly.

2026-04-29 | context | Project focus
- Facts: This is a CANoe/CAPL Built-in Cam CAN FD project for Gen 2.5 style test
  automation.
- Evidence: Root contains Vector/CANoe files, `Auto` test scripts, `CAPL`
  simulation/reference scripts, and `system_space.txt`; helper header references
  Gen 2.5 BLTN_CAM CAN FD messages.
- Use next time: Assume test logic targets BLTN_CAM behavior unless the user
  specifies another ECU.

2026-04-29 | context | System behavior reference
- Facts: `system_space.txt` summarizes Built-in Cam behavior: OWD, OWP, DEV,
  PEV, PMD, MAR, TML, parking mode, network sleep, SD, HU, Wi-Fi, LVDS, and
  event behavior.
- Evidence: The file is a condensed Vietnamese system spec.
- Use next time: Read the relevant section before writing expected behavior,
  especially for parking, event recording, notification, sleep, and storage
  logic.

2026-04-29 | context | Reference folders
- Facts: `Auto` contains module folders such as `BAS`, `BAT`, `CCS`, `DIA`,
  `HU`, `INF`, `INF_2`, `MEM`, `OTA`, `OWD`, and `OWP`. `CAPL` contains
  simulation/reference `.can` files such as BLTN_CAM, power supply, driving
  info, panel, and LIN examples.
- Evidence: Initial directory inspection on 2026-04-29.
- Use next time: Choose the target module from the requested test ID, then read
  nearby tests in that module before editing.

## pattern

2026-05-15 | pattern | OTA CAN COM start/end markers
- Facts: OTA `.can` primary test flows use one start marker
  `Send_Value_To_COM(1xx00)`, where `xx` comes from the OTA file number, and
  close with `Send_Value_To_COM(44)`.
- Evidence: `Auto/OTA/OTA_001.can` through `OTA_024.can` now use markers such
  as `10100`, `10800`, `12100`, and `12400`; `OTA_012.can` places the markers
  in `MainTest()` because it has no `testcase OTA_012()`.
- Use next time: For new or rewritten OTA `.can` files, add the start marker
  after local declarations in the primary flow, and add `Send_Value_To_COM(44)`
  before every early `return;` plus the final end of that flow.

2026-05-15 | pattern | OTA CAN captures use related signal groups
- Facts: OTA `.can` screenshot captures should include common context signals
  (`SMK_TrmnlCtrlGrpStaBDC`, `BLTN_CAM_SWVerMinor3`) plus the file's
  feature-specific result evidence signals.
- Evidence: `Auto/OTA/OTA_001.can` through `OTA_024.can` capture lists were
  narrowed after the user asked to remove `BLTN_CAM_HU_UI_Mode`,
  `BLTN_CAM_State`, OWD/OWP/PEV RecSet signals, PMD Set, and PEV AppNoti from
  screenshots. On 2026-05-16, `NM_State_BLTN_CAM_FD` was also removed from OTA
  `.can` screenshot lists. OWD files keep OWD RecSta, OWP files keep OWP
  RecSta, PMD files keep PMD RecSta, `OTA_004` keeps OWP/PEV RecSta plus PEV
  recorded evidence. On 2026-05-16, `ICU_PowerAutoCutModSta` was also removed
  from the OTA shared capture lists because the OTA cases do not use it for
  pass/fail logic.
- Use next time: When copying or creating an OTA `.can` file, update the
  `CaptureGraphics("OTA", ...)` signal list from the actual helpers and direct
  result signals used by that test. Do not add UI mode, BLTN_CAM_State, RecSet,
  PMD Set, PEV AppNoti, `NM_State_BLTN_CAM_FD`, or
  `ICU_PowerAutoCutModSta` to OTA screenshot lists unless the user asks for
  them.

2026-05-15 | pattern | OTA Python artifacts include script source
- Facts: OTA Python screenshots and LOGASC outputs should carry the running
  Python script stem in the artifact filename so the source script is visible
  from the generated file name. After final screenshot capture, H-OTA Studio
  should be closed before ASC scanning so the log file is flushed to disk.
- Evidence: All `Auto/OTA/*.py` scripts use `script_name` for screenshot names
  and now rename current-run `LOGASC/*.asc` files to
  `<original>_<script_name>_YYYYMMDD_HHMMSS.asc` after closing H-OTA Studio and
  waiting 30 seconds.
- Use next time: When adding or copying OTA Python runners, keep screenshot
  names as `<script_name>_<timestamp>_...png`, scan all current-run `.asc`
  files rather than only `baocao.asc`, close H-OTA after final screenshots, and
  use `<original>_<script_name>_<timestamp>.asc` for renamed ASC logs.

2026-05-15 | pattern | OTA Python header comments must match script logic
- Facts: Several OTA Python scripts had copied top comments that did not match
  the actual UI automation flow, for example `case_15.py` selected OEUK
  Vehicle but its header did not say so.
- Evidence: `Auto/OTA/*.py` headers were refreshed to describe wireless vs
  wired ERT, activation, rollback, OEUK Vehicle, and AppState marker behavior
  such as case 9/10/11/12.
- Use next time: After copying or splitting an OTA Python runner, update the
  top-of-file flow comments from the actual code markers, not only from the
  source script name.

2026-05-15 | pattern | OTA main Python case split
- Facts: For OTA testcase files, the setup/update prerequisite script can stay
  named by the update direction, while the main testcase Python runner should
  be copied to a case-specific `case_<ID>.py` file when the user asks for a
  case split.
- Evidence: `OTA_001`, `OTA_006`, `OTA_016`, and `OTA_020` now keep setup
  scripts separate from their main `case_<ID>.py` runner.
- Use next time: For similar OTA requests, create the requested `case_<ID>.py`
  as an exact copy of the named source script, then update only the main-flow
  command/log/testStep strings in the matching `.can` file.

2026-04-29 | pattern | New test workflow
- Facts: Reliable workflow is read status/memory, inspect target module samples,
  inspect `Auto/DeclarationFunction_Gen2.5.h`, inspect relevant `system_space`
  section, then implement only the requested test.
- Evidence: Current repo has strong existing patterns and a large shared helper
  header.
- Use next time: Do not start from a blank CAPL style. Copy the structure and
  idioms from OWD/OWP first, then adapt feature details from nearby tests.

2026-04-29 | pattern | Test reporting and debug output
- Facts: Existing tests commonly use `Send_Value_To_COM`, `SetTestStep`,
  `StartTestTimer`, `StopTestTimer`, `testStep`, `testStepPass`,
  `testStepFail`, `write`, and `CaptureGraphics`.
- Evidence: `Auto/OWD/OWD_027 - 2.5 generation.can` and
  `Auto/OWP/OWP_001 - 2.5 generation.can`.
- Use next time: Include those calls when the surrounding test family uses them.

## mistake

2026-05-16 | mistake | Do not wait inside OTA AppState sysvar events
- Facts: CANoe reports `09-0020 Wait call ignored, because this procedure
  cannot be suspended` when `testWaitForTimeout(...)` or helpers containing
  waits run inside `on sysvar_update PythonCom::AppState`.
- Evidence: `Auto/OTA/OTA_004.can` AppState 12 handling used
  `testWaitForTimeout(...)` inside the sysvar event after `KEY_OFF()`, so the
  wait was ignored and the COM8 retry loop ran immediately. The fix keeps the
  sysvar event to AppState latch + `KEY_OFF()` only; the testcase then calls
  `WaitFor_RecSta_PEV_Recording(10)` and waits 12 seconds between retries.
- Use next time: Keep sysvar events non-blocking. For AppState-triggered OTA
  delays/retries, use `msTimer` state machines or move the blocking wait logic
  into a testcase.

2026-05-06 | mistake | PMD and OWP waits must start from KEY OFF
- Facts: The user corrected `OWP_023` and `OWP_025` because a setup flow had
  `KEY_OFF()` then `KEY_ON()` before a PMD wait. For OWP/PMD wait logic, the
  final key state before waiting must be KEY OFF.
- Evidence: User correction on 2026-05-06 for
  `Auto/OWP/OWP_023 - 2.5 generation.can` and
  `Auto/OWP/OWP_025 - 2.5 generation.can`.
- Use next time: When a step says `KEY ON/OFF` before OWP/PMD waits, implement
  `KEY_ON()` then `KEY_OFF()` and call the `WaitFor_RecSta_*` helper only after
  KEY OFF. If a testcase starts directly with a PMD wait, add an explicit
  `KEY_OFF()` first unless the user clearly intends it to inherit state.

2026-05-06 | mistake | Do not use BLTN_CAM_PopUp_Req for OWP PMD checks
- Facts: The user corrected `OWP_005` because it used `BLTN_CAM_PopUp_Req` for
  parking-motion popup evidence and included it in screenshots. They also
  clarified that `WaitFor_RecSta_PMD_Recording()` already performs the PMD
  Recording check, and any logic waiting for PMD OFF must use
  `WaitFor_RecSta_PMD_OFF()`.
- Evidence: User correction on 2026-05-06 while editing
  `Auto/OWP/OWP_005 - 2.5 generation.can`.
- Use next time: Do not add `BLTN_CAM_PopUp_Req` to OWP capture/check logic.
  Use `WaitFor_RecSta_PMD_Recording()` for PMD Recording waits and
  `WaitFor_RecSta_PMD_OFF()` for PMD OFF waits; do not add redundant
  `Check_RecSta_PMD_Recording()` immediately after the wait helper.

2026-05-05 | mistake | PMD wait helper intentionally triggers PEV first
- Facts: `WaitFor_RecSta_PMD_Recording()` intentionally calls PEV logic first;
  in this bench flow PEV completes and then PMD Recording rises.
- Evidence: User clarified on 2026-05-05 that any logic waiting for PMD must
  call `WaitFor_RecSta_PMD_Recording()` because the internal PEV trigger is
  required.
- Use next time: Do not replace PMD Recording waits with direct signal-range
  waits or instant `Check_RecSta_PMD_Recording()` checks. Use the shared PMD
  wait helper for PMD wait steps, and reserve checks for final snapshots only.

2026-05-05 | mistake | Scan all Auto modules for MainTest, not only OWD
- Facts: After OWD_031 was fixed, an all-`Auto/**/*.can` scan found the same
  CANoe entrypoint class outside OWD: `BAT_011`, `BAT_032`, `HU_002` 2.0, and
  `HU_002` 2.5 had `void Maintest()`; `BAT_040` had testcases but no
  `MainTest()` at all.
- Evidence: Case-sensitive scan checked exact `void MainTest(`,
  `void Maintest(`, testcase count, and leading BOM count across `Auto`.
- Use next time: For CANoe's invalid/missing-MainTest error, scan the whole
  `Auto` tree, not just the module the user names.

2026-05-05 | mistake | Utility CAPL tests still need MainTest
- Facts: `Auto/TurnON.can` and `Auto/TurnOFF.can` were originally created with
  `*_Init()` and `*_1()` testcases but no `void MainTest()`, causing CANoe to
  report the test module as invalid or missing `MainTest()`.
- Evidence: User reported the CANoe error for `TurnON` on 2026-05-05; file
  inspection confirmed no `MainTest()` before the fix.
- Use next time: Even for small utility tests at `Auto` root, add
  `MainTest()` with `testReportFileName(...)`, init testcase, step testcase(s),
  and COM start/end markers.

2026-05-05 | mistake | MainTest name is case-sensitive for CANoe entrypoint
- Facts: `Auto/OWD/OWD_025 - 2.5 generation.can` had `void Maintest()` instead
  of exact `void MainTest()`. The OWD folder now has 32 exact `MainTest()`
  definitions across 32 `.can` files after fixing OWD_025.
- Evidence: User asked to check OWD files after the TurnON missing-MainTest
  error; case-sensitive scan found only OWD_025 with `Maintest`.
- Use next time: Scan for exact `^void MainTest(` case-sensitively, not just a
  case-insensitive `MainTest` match. Treat `Maintest` as invalid.

2026-05-05 | mistake | Double UTF-8 BOM can make CAPL test module invalid
- Facts: `Auto/OWD/OWD_031 - 2.5 generation.can` already had exact
  `void MainTest()`, but CANoe still reported the module as invalid or missing
  `MainTest()`. The file had double BOM bytes before `/*@!Encoding:65001*/`.
  The same double-BOM pattern was also present in `OWD_020` and `OWD_030`.
- Evidence: User reported the OWD_031 CANoe error on 2026-05-05; byte scan
  showed `EF BB BF EF BB BF` at the start of OWD_020/030/031.
- Use next time: For CANoe's generic invalid/missing-MainTest error, check both
  exact `MainTest()` and leading BOM count. Remove duplicate BOMs and prefer
  ASCII `SetTestStep("Buoc ...")` in newly touched files.

2026-04-29 | mistake | Do not touch generated CANoe artifacts
- Facts: The checkout contains many modified `.cbf`, `.dbg`, `.vtestreport`,
  report sidecars, and CANoe config files.
- Evidence: `git status` showed many pre-existing modifications and deletions.
- Use next time: Never clean, regenerate, revert, or edit generated artifacts as
  part of normal test coding. Avoid accidental broad formatting or delete
  commands.

2026-04-29 | mistake | Do not invent helper logic before checking headers
- Facts: The primary header already contains many helper functions for key
  state, recording toggles, waits, checks, capture, panel control, and COM
  markers.
- Evidence: `Auto/DeclarationFunction_Gen2.5.h` table of contents and forward
  declarations.
- Use next time: Search the header first and use existing functions. Duplicate
  only when there is no suitable helper and keep fallback logic local.
