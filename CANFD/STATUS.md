# Project Status - CANFD Built-in Cam

Last updated: 2026-05-16

## Current Goal

Implement and maintain requested Auto CAPL/OTA automated test flows, currently
the OTA `.can` COM/capture instrumentation cleanup.

Primary implementation target for future work:

- `Auto/**`

Primary references for future work:

- `Auto/DeclarationFunction_Gen2.5.h`
- canonical structure samples under `Auto/OWD` and `Auto/OWP`
- nearby `.can` samples under the target `Auto/<module>` folder for
  feature-specific signal logic
- `CAPL/**` as read-only reference
- `system_space.txt` for BLTN_CAM ECU behavior

## Active Constraints

- Write test code only in `Auto`.
- Use functions from `Auto/DeclarationFunction_Gen2.5.h` first. Do not
  duplicate helper logic or write direct signal manipulation when the header has
  a suitable function.
- Do not modify `CAPL` unless the user explicitly asks.
- Do not modify project configuration, database, environment, panel, report, or
  generated CANoe artifacts unless explicitly asked.
- Prefer existing helper functions from `Auto/DeclarationFunction_Gen2.5.h`.
- Follow OWD/OWP as the canonical test structure. Use other module samples only
  for feature-specific signal logic and expected values.
- Add `write()` logs and report pass/fail details for debug visibility.
- Read and apply relevant `system_space.txt` sections before encoding expected
  ECU behavior.

## Initial Repo Notes

- `Auto` contains test modules including `BAS`, `BAT`, `CCS`, `DIA`, `HU`,
  `INF`, `INF_2`, `MEM`, `OTA`, `OWD`, and `OWP`.
- Root `Auto/DeclarationFunction_Gen2.5.h` is a large shared helper library for
  Gen 2.5 BLTN_CAM tests.
- `CAPL` contains simulation/reference CAPL files and should normally be
  read-only for Codex.
- `system_space.txt` is a condensed BLTN_CAM behavior/spec reference in
  Vietnamese.
- `rg.exe` was blocked by Windows in this folder during initialization; use
  PowerShell `Get-ChildItem` / `Select-String` if `rg` is unavailable.
- `git status` already showed many pre-existing modified/deleted/generated files
  before agent initialization. Do not assume those changes were made by Codex.

## Files Added For Agent Continuity

- `AGENTS.md`
- `Auto/AGENTS.md`
- `MEMORY.md`
- `STATUS.md`

No CAPL test implementation was changed during initialization.

## Next Session Checklist

1. Read `STATUS.md`, `MEMORY.md`, `AGENTS.md`, and `Auto/AGENTS.md`.
2. Identify requested test ID and target module folder under `Auto`.
3. Read OWD/OWP samples for the canonical structure.
4. Read the nearest existing `.can` samples in the target folder for domain
   details.
5. Search `Auto/DeclarationFunction_Gen2.5.h` for existing helpers and plan to
   use those helpers before writing direct signal logic.
6. Read the relevant `system_space.txt` section for expected ECU behavior.
7. Implement only the requested `.can` changes under `Auto`.
8. Verify include paths, helper names, CAPL syntax style, and no out-of-scope
   file edits.
9. Update `STATUS.md` and add durable lessons to `MEMORY.md` when useful.

## Current Blockers

- No active blocker.
- CANoe compile/run was not attempted during initialization.

## Recent Work

2026-05-16:

- Fixed `OTA_004.can` AppState event wait handling.
  - Removed blocking wait logic from `on sysvar_update PythonCom::AppState`;
    the event now only latches AppState 12 and performs `KEY_OFF()`.
  - Added testcase-side AppState 12 wait and PEV check flow.
  - After AppState 12, the testcase waits 5 seconds, then calls
    `WaitFor_RecSta_PEV_Recording(10)` up to 5 times with a 12-second delay
    between calls. The helper sends COM8 internally.
  - This avoids CANoe runtime warning `09-0020 Wait call ignored, because this
    procedure cannot be suspended`.
  - Static verification only:
    - Confirmed no `testWaitForTimeout(...)` remains in the AppState sysvar
      event procedure.
    - Confirmed no direct `Send_Value_To_COM(8)` remains in `OTA_004.can`.
    - Ran `git diff --check -- Auto/OTA/OTA_004.can`; no whitespace errors,
      only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not attempted from terminal.

- Removed `NM_State_BLTN_CAM_FD` from OTA CAPL screenshot signal lists.
  - Updated `Auto/OTA/OTA_002.can`, `Auto/OTA/OTA_003.can`,
    `Auto/OTA/OTA_004.can`, `Auto/OTA/OTA_005.can`,
    `Auto/OTA/OTA_007.can`, and `Auto/OTA/OTA_008.can`.
  - Preserved the remaining capture context signals, including
    `PythonCom::AppState`.
  - Static verification only:
    - Confirmed `NM_State_BLTN_CAM_FD` no longer appears in
      `Auto/OTA/*.can`.
    - Confirmed OTA `CaptureGraphics(...)` calls and local
      `gCaptureSignals` definitions still include `PythonCom::AppState`.
    - Ran `git diff --check` on the touched OTA `.can` files; no whitespace
      errors, only the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

- Removed `ICU_PowerAutoCutModSta` from OTA CAPL screenshot signal lists.
  - Updated shared `gCaptureSignals` in `Auto/OTA/OTA_009.can` and
    `Auto/OTA/OTA_010.can`.
  - In `Auto/OTA`, this signal was only present in those shared capture
    lists, not in OTA pass/fail logic.
  - Static verification only:
    - Confirmed `ICU_PowerAutoCutModSta` no longer appears in
      `Auto/OTA/*.can`.
    - Confirmed the edited shared capture lists still include
      `PythonCom::AppState`.
  - CANoe compile/run was not attempted from terminal.

- Removed manual user prompts from the BAS 2.0 automated flows.
  - Updated `Auto/BAS/BAS_016 - 2.0 generation.can`,
    `Auto/BAS/BAS_017 - 2.0 generation.can`, and
    `Auto/BAS/BAS_018 - 2.0 generation.can`.
  - Removed recorded-file deletion/retention message-box confirmations from
    pass/fail logic.
  - BAS_016/BAS_017 now pass on CAN-visible parking recording and
    setting-reset/setting-retention checks only.
  - BAS_018 now replaces file confirmation prompts with automatic
    default/Guest setting comparisons and replaces ACC/IGN prompts with
    direct key/ACC signal control.
  - Updated `Auto/BAS/DeclarationFunction_Gen2.h` `Set_Battery()` so BAS 2.0
    uses automatic eAGM/SOC setup (`ICU_BS2SoC = 0x50`) instead of asking for
    PLBM/EAGM input.
  - BAS 2.0 `CaptureGraphics("BAS", ...)` calls now include the related
    battery signal `ICU_BS2SoC`.
  - Static verification only:
    - Confirmed no `testWaitForMessageBox`, `testWaitForStringInput`,
      `testGetStringInput`, `TestWaitForValueInput`, or `TestGetValueInput`
      remains in the three BAS 2.0 files or `DeclarationFunction_Gen2.h`.
    - Confirmed all BAS 2.0 captures include `ICU_BS2SoC`.
    - Checked brace counts for the three files and Gen2 header.
    - Ran `git diff --check` on touched BAS files; no whitespace errors, only
      the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

2026-05-15:

- Updated OTA Python AppState handoff timing in `Auto/OTA/case_010.py` and
  `Auto/OTA/case_012.py`.
  - Changed the post-`activate-rule` trigger from waiting for a later
    `routineControl` / `routine control` line to waiting for a later
    `securityAccess` / `security access` line.
  - Added a 5-second delay after `securityAccess` detection before sending
    `PythonCom::AppState = 10` or `PythonCom::AppState = 12`.
  - Updated top TEST FLOW comments and runtime logs to match the new trigger.
  - Static verification only:
    - Parsed both edited Python files with Python `ast.parse`; no syntax
      errors.
    - Confirmed no `routineControl` wait wording remains in the two files.
    - Checked both edited Python files for trailing whitespace; no matches.
    - Ran `git diff --check -- STATUS.md MEMORY.md`; no whitespace errors,
      only the repo's usual LF-to-CRLF warnings.
  - H-OTA/CANoe execution was not attempted from terminal.

- Added `PythonCom::AppState` to OTA CAPL screenshot signal lists.
  - Updated every direct `CaptureGraphics("OTA", "...")` signal list in
    `Auto/OTA/OTA_001.can` through `OTA_024.can` where an OTA `.can` file
    exists.
  - Updated shared `gCaptureSignals` values in `Auto/OTA/OTA_009.can`,
    `Auto/OTA/OTA_010.can`, and `Auto/OTA/OTA_011.can`.
  - Static verification only:
    - Confirmed no direct OTA `CaptureGraphics(...)` line is missing
      `PythonCom::AppState`.
    - Confirmed no OTA `gCaptureSignals` definition is missing
      `PythonCom::AppState`.
    - Ran `git diff --check -- Auto/OTA/*.can`; no whitespace errors, only
      the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

- Added immediate half-time OTA screenshots after AppState handoff in
  `Auto/OTA/case_009.py`, `case_010.py`, `case_011.py`, and `case_012.py`.
  - Extended each script's existing `capture_hota_and_monitor_screenshots(...)`
    helper with an optional filename suffix.
  - After successfully sending `PythonCom::AppState` values `9`, `10`, `11`,
    or `12`, each script now immediately captures the same H-OTA/window monitor
    evidence into `Auto/OTA/Report`.
  - Half-time capture names now insert `_half` immediately after the timestamp,
    for example `<script_name>_YYYYMMDD_HHMMSS_half_hota.png` and
    `<script_name>_YYYYMMDD_HHMMSS_half_screen1.png`.
  - Static verification only:
    - Parsed the four edited Python files with Python `ast.parse`; no syntax
      errors.
    - Checked the four edited files for trailing whitespace; no matches.
  - H-OTA/CANoe execution was not attempted from terminal.

- Added OTA Python run logging to `Auto/OTA/Log_python`.
  - Added shared helper `Auto/OTA/python_run_log.py`.
  - Created `Auto/OTA/Log_python` as the log destination.
  - Updated all 31 OTA Python runner scripts to import
    `start_python_log(...)` and call it at the start of
    `open_hota_and_file()`.
  - Each run now creates a log file named
    `<script_name>_YYYYMMDD_HHMMSS.log` under `Auto/OTA/Log_python`.
  - The logger tees both `stdout` and `stderr`, so console/popup output remains
    visible while the same output and Python tracebacks are written to disk.
  - Static verification only:
    - Parsed all 33 `Auto/OTA/*.py` files with Python `ast.parse`; no syntax
      errors.
    - Confirmed all 31 OTA runner files have the `start_python_log(...)`
      import and startup call.
    - Checked `Auto/OTA/*.py` for trailing whitespace with `Select-String`; no
      matches.
  - H-OTA/CANoe execution was not attempted from terminal.

- Updated `Auto/OTA/case_012.py` AppState handoff timing.
  - Changed the Output-pane wait logic so `PythonCom::AppState = 12` is sent
    only after an `activate-rule` line has been detected and a later
    `routineControl` / `routine control` line appears.
  - Added explicit logs for the two-stage detection:
    `activate-rule` detected, then `routineControl` detected before the COM
    notification is sent.
  - Static verification only:
    - Parsed `Auto/OTA/case_012.py` with Python `ast.parse`; no syntax errors.
    - Checked edited files for trailing whitespace with `Select-String`; no
      matches.
    - Ran `git diff --check -- STATUS.md`; no whitespace errors, only the
      repo's usual LF-to-CRLF warning.
  - H-OTA/CANoe execution was not attempted from terminal.

- Updated `Auto/OTA/case_010.py` AppState handoff timing with the same
  two-stage detection.
  - Changed the early-return path so `PythonCom::AppState = 10` is sent only
    after an `activate-rule` line has been detected and a later
    `routineControl` / `routine control` line appears.
  - Preserved the original `case_010.py` behavior after the handoff: close
    H-OTA Studio and return immediately.
  - Static verification only:
    - Parsed `Auto/OTA/case_010.py` with Python `ast.parse`; no syntax errors.
    - Checked `Auto/OTA/case_010.py` for trailing whitespace with
      `Select-String`; no matches.
  - H-OTA/CANoe execution was not attempted from terminal.

- Reworked OTA Python H-OTA config selection to avoid hardcoding the
  `.hcfg-r` filename.
  - Added shared helper `Auto/OTA/hota_config.py`.
  - The helper searches the requested config folder for files ending in
    `.hcfg-r` and returns the path only when exactly one match exists.
  - Updated all 31 OTA Python runner scripts to import
    `find_hota_config_file(...)` and call it with the existing folder number
    (`0`, `1`, `2`, or `3`) instead of embedding
    `ReprogramConfig_0_1_1_114.hcfg-r`.
  - Updated the top TEST FLOW comment in each runner so it describes loading
    the only `*.hcfg-r` file from the selected config folder.
  - Static verification only:
    - Confirmed no hardcoded `ReprogramConfig_0_1_1_*` filename remains in
      `Auto/OTA/*.py`.
    - Confirmed 31 imports of `find_hota_config_file(...)` and 31
      `file_path = find_hota_config_file(script_dir, "...")` call sites.
    - Confirmed the helper resolves the current `.hcfg-r` file in folders
      `Auto/OTA/0`, `Auto/OTA/1`, `Auto/OTA/2`, and `Auto/OTA/3`.
    - Parsed all 32 `Auto/OTA/*.py` files with Python `ast.parse`; no syntax
      errors.
    - Ran `git diff --check -- Auto/OTA/*.py`; no whitespace errors, only
      the repo's usual LF-to-CRLF warnings.
  - H-OTA execution was not attempted from terminal.

- Updated OTA Python H-OTA config references across all 31 `Auto/OTA/*.py`
  scripts.
  - Changed every `ReprogramConfig_0_1_1_113.hcfg-r` reference to
    `ReprogramConfig_0_1_1_114.hcfg-r`.
  - Updated both visible step comments and `file_path` assignments.
  - Confirmed the new config exists under `Auto/OTA/0`, `Auto/OTA/1`,
    `Auto/OTA/2`, and `Auto/OTA/3`.
  - Static verification only:
    - Confirmed no `ReprogramConfig_0_1_1_113.hcfg-r` reference remains in
      `Auto/OTA/*.py`.
    - Confirmed 62 references to `ReprogramConfig_0_1_1_114.hcfg-r`
      remain, matching 31 scripts with 2 references each.
    - Parsed all 31 `Auto/OTA/*.py` files with Python `ast.parse`; no syntax
      errors.
    - Ran `git diff --check -- Auto/OTA/*.py`; no whitespace errors, only
      the repo's usual LF-to-CRLF warnings.
  - H-OTA execution was not attempted from terminal.

- Narrowed OTA CAPL screenshot capture signal lists per user request.
  - Removed these signals from every `CaptureGraphics(...)` list in
    `Auto/OTA/OTA_*.can`: `BLTN_CAM_HU_UI_Mode`, `BLTN_CAM_RecSet_OWD`,
    `BLTN_CAM_RecSet_OWP`, `BLTN_CAM_RecSet_PEV`,
    `BLTN_CAM_Set_PEV_AppNoti`, `BLTN_CAM_Set_PMD`, and
    `BLTN_CAM_State`.
  - Remaining OTA capture signals are now focused on key/version/result/NM
    evidence: `SMK_TrmnlCtrlGrpStaBDC`, `BLTN_CAM_SWVerMinor3`,
    `BLTN_CAM_RecSta_OWD`, `BLTN_CAM_RecSta_OWP`,
    `BLTN_CAM_RecSta_PEV`, `BLTN_CAM_RecSta_PMD`,
    `BLTN_CAM_PEV_Recorded`, `NM_State_BLTN_CAM_FD`, and
    `ICU_PowerAutoCutModSta`.
  - Static verification only:
    - Confirmed none of the removed signals remains in OTA capture lists.
    - Ran `git diff --check -- Auto\OTA\*.can STATUS.md MEMORY.md`; no
      whitespace errors, only the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

- Expanded OTA CAPL screenshot capture signal lists across all 20
  `Auto/OTA/OTA_*.can` test files.
  - Each `CaptureGraphics("OTA", ...)` now captures common context signals:
    `SMK_TrmnlCtrlGrpStaBDC`, `BLTN_CAM_SWVerMinor3`, `BLTN_CAM_State`, and
    `BLTN_CAM_HU_UI_Mode`.
  - OWD-related files also capture `BLTN_CAM_RecSet_OWD` and
    `BLTN_CAM_RecSta_OWD`.
  - OWP-related files also capture `BLTN_CAM_RecSet_OWP`,
    `BLTN_CAM_RecSta_OWP`, and `NM_State_BLTN_CAM_FD`.
  - PMD-related files also capture `BLTN_CAM_Set_PMD`,
    `BLTN_CAM_RecSta_PMD`, and where network sleep is part of the flow,
    `NM_State_BLTN_CAM_FD`.
  - `OTA_004.can` captures its OWP, PEV, PEV recorded, AppNoti, and NM
    evidence signals.
  - `OTA_009.can` and `OTA_010.can` also capture
    `ICU_PowerAutoCutModSta` as power-cut context for their B+ flows.
  - Static verification only:
    - Listed every OTA `.can` capture list and confirmed no old
      three-signal `CaptureGraphics` list remains.
    - Ran `git diff --check -- Auto\OTA\*.can STATUS.md MEMORY.md`; no
      whitespace errors, only the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

- Updated OTA CAPL COM markers across all 20 `Auto/OTA/OTA_*.can` test files.
  - Added one test-start marker in each primary OTA flow using the requested
    formula `Send_Value_To_COM(1xx00)`, where `xx` is the OTA file number:
    `OTA_008` -> `10800`, `OTA_021` -> `12100`.
  - `OTA_012.can` has no `testcase OTA_012()`, so its start/end markers were
    added in `MainTest()`.
  - Added `Send_Value_To_COM(44)` before every early `return;` in the primary
    OTA flow and at the final end of the flow.
  - Capture signal inventory for all OTA `.can` `CaptureGraphics(...)` calls:
    `SMK_TrmnlCtrlGrpStaBDC`, `BLTN_CAM_RecSta_OWD`,
    `BLTN_CAM_SWVerMinor3`.
  - Static verification only:
    - Confirmed each of the 20 OTA `.can` files has exactly one matching
      `1xx00` start marker.
    - Confirmed no `return;` in `Auto/OTA/*.can` is missing an immediately
      preceding `Send_Value_To_COM(44);`.
    - Confirmed brace counts are balanced in all 20 OTA `.can` files.
    - Ran `git diff --check -- Auto\OTA\*.can`; no whitespace errors, only
      the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not attempted from terminal.

- Updated OTA Python final artifact flow across all 31 `Auto/OTA/*.py` scripts.
  - Added/reused `terminate_current_hota_process()` in every OTA Python
    runner.
  - After final popup result handling and screenshot capture, each script now
    closes H-OTA Studio before calling `rename_recent_asc_files(...)`.
  - The close call is outside the popup/screenshot `try` block, so H-OTA is
    still closed before ASC processing if popup text parsing or screenshot
    capture raises an exception.
  - The intended order is now:
    `capture screenshot` -> `close H-OTA Studio` -> `wait 30s` ->
    `scan/rename current-run LOGASC/*.asc`.
  - Static verification only:
    - Parsed all 31 `Auto/OTA/*.py` files with Python `ast.parse`; no syntax
      errors.
    - Confirmed each script has one post-popup close message and still uses
      `wait_seconds=30` for ASC scanning.
    - Confirmed close order check passes for all scripts: capture path before
      close, and ASC rename call after close.
    - Ran `git diff --check` on `Auto/OTA/*.py`, `STATUS.md`, and `MEMORY.md`;
      no whitespace errors, only the repo's usual LF-to-CRLF warnings.
  - CANoe/H-OTA execution was not attempted from terminal.

- Updated OTA Python artifact naming and LOGASC rename behavior across 31
  scripts in `Auto/OTA/*.py`.
  - Screenshot filenames already used the running Python script stem through
    `script_name`; this pattern is now documented in each script header.
  - Replaced `rename_recent_baocao_file(...)` with
    `rename_recent_asc_files(...)`.
  - The LOGASC logic now waits 30 seconds after screenshot capture before
    scanning.
  - It scans all current-run `LOGASC/*.asc` files instead of only
    `LOGASC/baocao.asc`.
  - Current-run detection accepts `.asc` files whose create time or modified
    time is after the run start, with the existing 5-second tolerance.
  - Renamed `.asc` files now preserve the original base name and include the
    running script stem:
    `<original>_<script_name>_YYYYMMDD_HHMMSS.asc`.
  - Static verification only:
    - Parsed all 31 `Auto/OTA/*.py` files with Python `ast.parse`; no syntax
      errors.
    - Confirmed all 31 scripts contain `rename_recent_asc_files(...)` with
      `wait_seconds=30`.
    - Confirmed no `rename_recent_baocao_file`, `baocao`, or old
      `wait_seconds=3` rename call remains in `Auto/OTA/*.py`.
    - Confirmed screenshot filename templates still include
      `script_name` and `screenshot_ts`.
    - Ran `git diff --check` on `Auto/OTA/*.py`, `STATUS.md`, and `MEMORY.md`;
      no whitespace errors, only the repo's usual LF-to-CRLF warnings.
  - CANoe/H-OTA execution was not attempted from terminal.

- Standardized OTA Python case filenames to three-digit numbering:
  - Renamed:
    - `Auto/OTA/case_9.py` -> `Auto/OTA/case_009.py`.
    - `Auto/OTA/case_9_2.py` -> `Auto/OTA/case_009_2.py`.
    - `Auto/OTA/case_10.py` -> `Auto/OTA/case_010.py`.
    - `Auto/OTA/case_10_2.py` -> `Auto/OTA/case_010_2.py`.
    - `Auto/OTA/case_11.py` -> `Auto/OTA/case_011.py`.
    - `Auto/OTA/case_12.py` -> `Auto/OTA/case_012.py`.
    - `Auto/OTA/case_15.py` -> `Auto/OTA/case_015.py`.
    - `Auto/OTA/case_19.py` -> `Auto/OTA/case_019.py`.
  - Updated call sites and visible log/comment strings in
    `Auto/OTA/OTA_009.can`, `OTA_010.can`, `OTA_011.can`, `OTA_012.can`,
    `OTA_015.can`, and `OTA_019.can`.
  - Updated the top filename comments inside the renamed Python scripts.
  - Static verification only:
    - Listed all `Auto/OTA/case_*.py` files and confirmed the Python case names
      now use three-digit numbering.
    - Scanned `Auto/OTA/*.can` and `Auto/OTA/*.py`; no unpadded
      `case_<1-2 digit>.py` references remain in source files.
    - CANoe compile/run was not available from terminal.

- Updated OTA_021-OTA_024 main ERT Python call split:
  - Created exact copies:
    - `Auto/OTA/case_021.py` from `Auto/OTA/update_to_00_ERT.py`.
    - `Auto/OTA/case_022.py` from `Auto/OTA/update_to_01_ERT.py`.
    - `Auto/OTA/case_023.py` from `Auto/OTA/update_to_02_ERT.py`.
    - `Auto/OTA/case_024.py` from `Auto/OTA/update_to_03_ERT.py`.
  - Updated `Auto/OTA/OTA_021.can` through `Auto/OTA/OTA_024.can` so their
    main Step 3 Python syscall uses `case_021.py` through `case_024.py`.
  - Static verification only:
    - Compared SHA-256 hashes for each original ERT script and new case script;
      all four pairs match.
    - Confirmed the four `.can` files now reference only the new `case_02x.py`
      scripts for the main ERT flow and no longer reference the old
      `update_to_0x_ERT.py` names.
    - CANoe compile/run was not available from terminal.

- Updated OTA Python top-of-file flow comments:
  - Replaced stale copied header comments in `Auto/OTA/*.py` with ASCII flow
    summaries that match each script's current logic.
  - Covered wireless, wired ERT, activation, rollback, OEUK Vehicle, and
    AppState marker variants:
    - `case_9.py`: CommunicationControl/Activate-Rule -> AppState 9, close
      H-OTA, return.
    - `case_10.py`: activate-rule -> AppState 10, close H-OTA, return.
    - `case_11.py`: CommunicationControl/Activate-Rule -> AppState 11, keep
      waiting for popup.
    - `case_12.py` and `case_002`-`case_005`: AppState 12 marker handling.
    - `case_15.py` and `case_19.py`: OEUK Vehicle selection.
  - Static verification only:
    - Scanned headers against actual code markers for OEUK, ERT SourceAddr,
      Add Activation/Rollback, and AppState 9/10/11/12; no mismatches found.
    - Compared tracked script bodies from `import subprocess` onward against
      `HEAD`; only top comments changed.
    - Ran `git diff --check -- Auto\OTA\*.py`; no whitespace errors, only the
      repo's usual LF-to-CRLF warnings.

- Updated OTA_020 main Python call split:
  - Created `Auto/OTA/case_020.py` as an exact copy of
    `Auto/OTA/update_to_00.py`.
  - Updated `Auto/OTA/OTA_020.can` so the setup flow still uses
    `update_to_01.py` when it needs to bring SW version to 1, while the main
    version-1-to-0 testcase run now calls `case_020.py`.
  - Static verification only:
    - Compared SHA-256 hashes for `update_to_00.py` and `case_020.py`; they
      match.
    - Confirmed `OTA_020.can` uses `gCase020Cmd` and no longer references
      `update_to_00.py`.
    - CANoe compile/run was not available from terminal.

- Updated OTA_016 main Python call split:
  - Created `Auto/OTA/case_016.py` as an exact copy of
    `Auto/OTA/Rollback_to_00.py`.
  - Updated `Auto/OTA/OTA_016.can` so setup still uses
    `update_to_00.py` and `update_to_01.py`, while the main rollback flow now
    calls `case_016.py`.
  - Static verification only:
    - Compared SHA-256 hashes for `Rollback_to_00.py` and `case_016.py`; they
      match.
    - Confirmed `OTA_016.can` uses `gCase016Cmd` and no longer references
      `Rollback_to_00.py`.
    - CANoe compile/run was not available from terminal.

- Updated OTA_006 main Python call split:
  - Created `Auto/OTA/case_006.py` as an exact copy of
    `Auto/OTA/update_to_02.py`.
  - Updated `Auto/OTA/OTA_006.can` so the setup flow still uses
    `update_to_00.py` when it needs to bring SW version back to 0, while the
    main testcase run now calls `case_006.py`.
  - Static verification only:
    - Compared SHA-256 hashes for `update_to_02.py` and `case_006.py`; they
      match.
    - Confirmed `OTA_006.can` uses `gCase006Cmd` and no longer references
      `update_to_02.py` in the main run.
    - CANoe compile/run was not available from terminal.

- Updated OTA_001 main Python call split:
  - Created `Auto/OTA/case_001.py` as an exact copy of
    `Auto/OTA/update_to_00.py`.
  - Updated `Auto/OTA/OTA_001.can` so the setup flow still calls
    `update_to_00.py` when it needs to bring SW version back to 0, while the
    main testcase run now calls `case_001.py`.
  - Static verification only:
    - Compared SHA-256 hashes for `update_to_00.py` and `case_001.py`; they
      match.
    - Confirmed `Run_UpdateTo00_Python(...)` is still used for setup and
      `Run_Case001_Python(...)` is used for the main run.
    - CANoe compile/run was not available from terminal.

- Created `Auto/OTA/OTA_010.can` from the `Auto/OTA/OTA_009.can` logic.
  - Updated testcase/log/report names from `OTA_009` to `OTA_010`.
  - Updated the intermediate trigger from `AppState=9` to `AppState=10`.
  - Updated Python command calls from `case_9.py` / `case_9_2.py` to
    `case_10.py` / `case_10_2.py`.
  - Kept the final result logic unchanged: wait for AppState 3/4 and pass only
    when AppState is 3 and `Check_SWVerMinor3(1)` passes.
  - Static verification only:
    - Confirmed no `OTA_009`, `case_9`, or `AppState=9` references remain in
      `OTA_010.can`.
    - CANoe compile/run was not available from terminal.

- Created `Auto/OTA/case_10_2.py` as an exact copy of
  `Auto/OTA/case_9_2.py`.
- Static verification only:
  - Compared SHA-256 hashes for both files; they match.
  - No CANoe compile/run was attempted because this was a file copy request.

2026-05-13:

- Updated BAS 2.0 CAPL files to capture signal evidence and run sequentially:
  - `Auto/BAS/BAS_016 - 2.0 generation.can`
  - `Auto/BAS/BAS_017 - 2.0 generation.can`
  - `Auto/BAS/BAS_018 - 2.0 generation.can`
  - Removed `TestWaitForValueInput(...)` / `TestGetValueInput()` case
    selection from all three `MainTest()` functions.
  - `MainTest()` now runs all testcases in order:
    - BAS_016: `BAS_016_1()` -> `BAS_016_2()` -> `BAS_016_3()`.
    - BAS_017: `BAS_017_1()` -> `BAS_017_4()` -> `BAS_017_7()` ->
      `BAS_017_8()` -> `BAS_017_11()` -> `BAS_017_14()`.
    - BAS_018: `BAS_018_1()` through `BAS_018_8()`.
  - Added `CaptureGraphics("BAS", "...")` evidence captures for related BAS
    signals, including `BLTN_CAM_RecSta_OWP`, `ICU_PowerAutoCutModSta`,
    `SMK_TrmnlCtrlGrpStaBDC`, recording setting signals, profile signal, and
    `ICU_BS2SoC` where BAS_017 uses the battery timer.
  - Updated `Auto/BAS/DeclarationFunction_Gen2.h` with the same
    `CaptureGraphics(char window[], char signalList[])` helper shape as
    `Auto/DeclarationFunction_Gen2.5.h`; BAS 2.0 files include this local
    header and cannot safely include the Gen2.5 header directly because common
    functions such as `KEY_ON`, `KEY_OFF`, and `Init` overlap.
  - Updated BAS local `Init()` to reset `flag`, `flag_RecSet_error`, and
    `OWP_memvar`, which is needed now that the files run multiple testcases in
    one sequence.
  - Static verification only:
    - Confirmed no `TestWaitForValueInput`, `TestGetValueInput`, or Graphics 2
      capture calls remain in the three BAS files.
    - Confirmed all signal evidence captures use window name `BAS`.
    - Ran `git diff --check` on the touched BAS files and header; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

2026-05-13:

- Added detailed flow comments at the top of BAS 2.0 CAPL files:
  - `Auto/BAS/BAS_016 - 2.0 generation.can`
  - `Auto/BAS/BAS_017 - 2.0 generation.can`
  - `Auto/BAS/BAS_018 - 2.0 generation.can`
  - The comments describe each file's purpose, common setup, testcase mapping,
    Factory/Dealer/Customer mode paths, expected setting/file behavior, and
    `MainTest()` selection flow.
  - No testcase logic was changed.
  - Used ASCII Vietnamese-without-diacritics comments because these files
    declare `/*@!Encoding:1252*/`.
  - Static verification only:
    - Inspected the new top-of-file comments.
    - Ran `git diff --check` on the three BAS `.can` files; no whitespace
      errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

2026-05-13:

- Updated `Auto/DeclarationFunction_Gen2.5.h`.
  - Added shared helper `Set_Dealer_Mode()`.
  - The helper sets `ICU_PowerAutoCutModSta` to Dealer Mode by calling
    `Set_PowerAutoCut_Mode(0x1, 30000)`, matching the existing
    `Set_Factory_Mode()` / `Set_Customer_Mode()` shortcut style.
  - Static verification only:
    - Confirmed `Set_Dealer_Mode()` and
      `Set_PowerAutoCut_Mode(0x1, 30000)` are present in the header.
    - Ran `git diff --check -- Auto/DeclarationFunction_Gen2.5.h`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

2026-05-12:

- Updated `Auto/OWP/OWP_006 - 2.5 generation.can`.
  - Replaced all `Clock_Down_ms(...)` waits with `testWaitForTimeout(...)`.
  - Added a 3-second delay after `KEY_ON()` before `KEY_OFF()` in Steps 2-5,
    matching the user's request to avoid immediate KEY ON/OFF transitions.
  - Updated the Step 2-5 `testStep(...)` descriptions to mention the new
    `wait 3s` action.
  - Removed local timeout variables such as `wait_owp_timeout_ms`,
    `wait_pmd_timeout_ms`, and `wait_pev_timeout_s`; wait helpers now receive
    literal timeout values directly, for example
    `WaitFor_RecSta_OWP_Recording(60000)`.
  - Static verification only:
    - Confirmed no `Clock_Down_ms` or local timeout-variable references remain
      in `OWP_006`.
    - Ran `git diff --check -- Auto/OWP/OWP_006 - 2.5 generation.can`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

- Replaced remaining `Clock_Down_ms(...)` testcase calls in `Auto`.
  - Updated:
    - `Auto/OWP/OWP_005 - 2.5 generation.can`
    - `Auto/OWP/OWP_023 - 2.5 generation.can`
    - `Auto/OWP/OWP_025 - 2.5 generation.can`
    - `Auto/OWD/OWD_003 - 2.5 generation.can`
  - Left `Clock_Down_ms()` definition/index entries in
    `Auto/DeclarationFunction_Gen2.5.h` untouched because those are helper
    definitions, not testcase call sites.
  - Static verification only:
    - `rg -n "Clock_Down_ms" Auto` now reports only the shared header index
      and helper definition.
    - Ran `git diff --check` on touched `.can` files and continuity docs; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

- Updated `Auto/DeclarationFunction_Gen2.5.h` `Click_USM_Reset()`.
  - Preserved the existing single BLTN_CAM enter/exit flow.
  - Added inline EV before/after recording time setup before exiting
    BLTN_CAM.
  - EV before-time is set to 10 seconds using
    `@sysvar::panel::TrackBar_EV_Time_Bfr = 3`, matching the header's
    inverse mapping.
  - EV after-time is set to 10 seconds using
    `@sysvar::panel::TrackBar_EV_Time_Aft = 10`.
  - The new logic copies the core retry/check behavior from
    `Set_EV_BeforeTime()` and `Set_EV_AfterTime()` but does not call those
    helpers because they perform their own BLTN_CAM enter/exit transitions.
  - Static verification only:
    - Inspected the edited `Click_USM_Reset()` body.
    - Ran `git diff --check -- Auto/DeclarationFunction_Gen2.5.h`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

- Rewrote `Auto/OWP/OWP_006 - 2.5 generation.can`.
  - Replaced the old repeated MAR/PEV manual-notification flow with the user's
    new 7-step OWP/PEV/PMD B+ interruption flow.
  - Step 1 now initializes with `Set_SMK_IGN()`, `Click_USM_Reset()`,
    `Click_Rec_Reset()`, then KEY OFF, waits for OWP Recording, waits 10s,
    cycles B+ OFF for 3s, then B+ ON.
  - Steps 2-5 now follow the requested KEY ON/OFF, OWP wait, PEV/event/rear
    detach variants, and B+ OFF/ON sequence.
  - Steps 6-7 now use `Turn_ON_PMD()` / `WaitFor_RecSta_PMD_Recording()` and
    the requested PMD/PEV B+ OFF/ON sequence.
  - Kept each step sequential inside the testcase body and called shared header
    helpers directly instead of adding OWP_006-specific wrapper helpers.
  - Static verification only:
    - Confirmed helper names exist in `Auto/DeclarationFunction_Gen2.5.h`.
    - Confirmed one exact `void MainTest()`, 8 OWP_006 testcase definitions
      including Init, and balanced braces.
    - Ran `git diff --check -- Auto/OWP/OWP_006 - 2.5 generation.can`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

- Updated `Auto/OWP/OWP_011 - 2.5 generation.can`.
  - Corrected the expected OFF/default value for `BLTN_CAM_PEV_Recorded` from
    `0x1` to `0x0` in Step 5 and Step 18.
  - Updated matching testcase comments, `testStep()` text, `write()` logs,
    pass conditions, fail expectations, and pass report text.
  - Checked `Auto/OWD/OWD_011 - 2.5 generation.can`; it does not use
    `BLTN_CAM_PEV_Recorded`, so no OWD_011 change was needed.
  - Static verification only:
    - Re-scanned `OWP_011` for stale `PEV_Recorded` OFF/`0x1` text.
    - Ran `git diff --check -- Auto/OWP/OWP_011 - 2.5 generation.can`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

- Updated `Auto/DeclarationFunction_Gen2.5.h` `Click_USM_Reset()`.
  - Preserved the single BLTN_CAM entry/exit flow.
  - Added inline DEV and PEV sensitivity setting to level 1 before exiting
    BLTN_CAM.
  - The new blocks reuse only the core logic from `Set_DEV_Sensitivity(1)` and
    `Set_PEV_Sensitivity(1)`: write the trackbar sysvar, wait, retry with
    `Check_DEV_Sensitivity(1)` / `Check_PEV_Sensitivity(1)`, and verify
    `BLTN_CAM_SenSet_DEV` / `BLTN_CAM_SenSet_PEV`.
  - Did not call the full sensitivity helpers because those helpers contain
    their own `Enter_BLTN_CAM()` / `Exit_BLTN_CAM()` transitions.
  - Static verification only:
    - Inspected the edited `Click_USM_Reset()` body.
    - Ran `git diff --check -- Auto/DeclarationFunction_Gen2.5.h`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

2026-05-11:

- Updated `Auto/DeclarationFunction_Gen2.5.h` `Click_USM_Reset()`.
  - After clicking `@panel::Button_USMReset`, the helper now manually enters
    BLTN_CAM mode with up to 10 attempts.
  - While staying inside BLTN_CAM, it turns ON OWD, OWP, PEV, then DEV by
    directly toggling the panel LED controls and checking the corresponding
    `BLTN_CAM_RecSet_*` signal, with up to 10 attempts per setting.
  - It then manually exits back to AVN with up to 10 attempts.
  - This intentionally avoids calling `Enter_BLTN_CAM()`, `Exit_BLTN_CAM()`,
    or `Turn_ON_OWD/OWP/PEV/DEV()` because those helpers have their own
    enter/exit behavior.
  - Static verification only:
    - Confirmed the edited helper uses the existing panel controls and
      `Check_RecSet_*_ON()` helpers.
    - Ran `git diff --check -- Auto/DeclarationFunction_Gen2.5.h`; no
      whitespace errors, only the repo's usual LF-to-CRLF warning.
  - CANoe compile/run was not available from terminal.

2026-05-09:

- Added missing end-of-step screenshot evidence in `Auto/OWD` and `Auto/OWP`.
  - Inserted 51 final capture blocks across 17 `.can` files:
    - `Auto/OWD/OWD_003 - 2.5 generation.can`
    - `Auto/OWD/OWD_005 - 2.5 generation.can`
    - `Auto/OWD/OWD_006 - 2.5 generation.can`
    - `Auto/OWD/OWD_007 - 2.5 generation.can`
    - `Auto/OWD/OWD_018 - 2.5 generation.can`
    - `Auto/OWD/OWD_019 - 2.5 generation.can`
    - `Auto/OWD/OWD_027 - 2.5 generation.can`
    - `Auto/OWD/OWD_030 - 2.5 generation.can`
    - `Auto/OWP/OWP_001 - 2.5 generation.can`
    - `Auto/OWP/OWP_002 - 2.5 generation.can`
    - `Auto/OWP/OWP_003 - 2.5 generation.can`
    - `Auto/OWP/OWP_004 - 2.5 generation.can`
    - `Auto/OWP/OWP_007 - 2.5 generation.can`
    - `Auto/OWP/OWP_008 - 2.5 generation.can`
    - `Auto/OWP/OWP_014 - 2.5 generation.can`
    - `Auto/OWP/OWP_016 - 2.5 generation.can`
    - `Auto/OWP/OWP_017 - 2.5 generation.can`
  - Each inserted block uses the direct helper pattern:
    `write("[Capture] Wait 5 seconds before screenshot");`,
    `testWaitForTimeout(5000);`, then `CaptureGraphics(...)`.
  - Confirmed `OWD_007_8` now captures
    `BLTN_CAM_RecSet_OWD`, `BLTN_CAM_RecSta_OWD`, and
    `BLTN_CAM_State` before `StopTestTimer()`.
  - Static verification only:
    - Scanned all `Auto/OWD/*.can` and `Auto/OWP/*.can`; 372 step
      testcases with `StopTestTimer()` now have either `CaptureGraphics(...)`
      or an existing capture helper call.
    - Confirmed one exact `void MainTest()` and balanced braces in all 17
      touched `.can` files.
    - Ran `git diff --check` on the 17 touched `.can` files; no whitespace
      errors remained, only the repo's usual LF-to-CRLF warnings.
  - CANoe compile/run was not available from terminal.

2026-05-08:

- Fixed the immediate CANoe parse errors in
  `Auto/OWD/OWD_004 - 2.5 generation.can` after manual edits.
  - Root cause: two helper calls were missing trailing semicolons:
    `WaitFor_RecSta_OWP_Recording(20000);` and
    `WaitFor_RecSta_OWD_Recording(20000);`.
  - Also removed trailing whitespace reported by `git diff --check`.
  - Static verification only:
    - Confirmed no remaining simple helper-call lines matching the same
      missing-semicolon pattern.
    - Confirmed brace/parenthesis counts are balanced and there is exactly one
      `void MainTest()`.
    - `git diff --check` reports no whitespace errors for this file; Git only
      warns about LF-to-CRLF conversion.
  - CANoe compile/run was not available from terminal.

2026-05-08:

- Cleaned up `Auto/OWD/OWD_008 - 2.5 generation.can` after the user corrected
  the helper style.
  - Removed local wrapper/helper functions:
    - `Log_OWD_008_Signals()`.
    - `Wait_OWD_Recording_For_OWD_008()`.
    - `Capture_OWD_008_Evidence()`.
  - Each testcase now calls shared header helpers directly, including
    `WaitFor_RecSta_OWD_Recording(...)`, `WaitFor_RecSta_DEV_Recording(...)`,
    `CaptureGraphics(...)`, `Set_BPlus_Off()`, `Set_BPlus_On()`,
    `Set_Detach_Rear()`, and `Set_Attach_Rear()`.
  - Kept signal snapshots inline inside the testcase bodies instead of hiding
    them behind OWD_008-specific local helper functions.
  - Static verification only:
    - Confirmed no removed helper names remain in the file.
    - Confirmed the only remaining top-level function is exact
      `void MainTest()`.
    - Confirmed brace/parenthesis balance and one exact `MainTest()`.
    - Confirmed called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - `git diff --check` reported no whitespace errors for the file.
  - CANoe compile/run was not available from terminal.

2026-05-06:

- Corrected key-state logic in `Auto/OWP/OWP_023 - 2.5 generation.can` and
  `Auto/OWP/OWP_025 - 2.5 generation.can`.
  - Step 1 in both files now performs setup while KEY ON, then explicitly
    calls `KEY_OFF()` before waiting for PMD.
  - Added explicit `KEY_OFF()` at the start of `OWP_025` Step 8 and Step 11 so
    each step independently enters PMD OFF waits from KEY OFF.
  - Confirmed every `WaitFor_RecSta_PMD_OFF(...)` and
    `WaitFor_RecSta_PMD_Recording(...)` call in these two files has KEY OFF as
    the latest key state inside the testcase before the wait.
  - Removed `BLTN_CAM_PopUp_Req` from screenshots in both files, following the
    prior user correction to avoid that signal.
  - Static verification only:
    - Confirmed no `BLTN_CAM_PopUp_Req` or raw `testWaitForSignalInRange`
      remains in either file.
    - Checked brace balance and exact `MainTest()` count for both files.
    - Ran `git diff --check` for both files; only Git's LF-to-CRLF warning was
      reported for `OWP_023`.
  - CANoe compile/run was not available from terminal.

2026-05-06:

- Updated `Auto/OWP/OWP_005 - 2.5 generation.can` after user correction.
  - Removed every `BLTN_CAM_PopUp_Req` usage from screenshots, logs, pass/fail
    logic, and Step 23 popup monitoring.
  - Step 23 now uses `WaitFor_RecSta_PMD_OFF(60000)` for the PMD OFF wait
    instead of polling popup or calling direct PMD check logic.
  - Removed extra `Check_RecSta_PMD_Recording()` calls that had been added
    after `WaitFor_RecSta_PMD_Recording(...)`; the wait helper already handles
    the PMD Recording check internally.
  - Static verification only:
    - Confirmed no `BLTN_CAM_PopUp_Req`, `Check_RecSta_PMD_Recording`,
      `Check_RecSta_PMD_OFF`, or raw `testWaitForSignalInRange` remains in the
      file.
    - Checked brace balance and exact `MainTest()` count.
    - Ran `git diff --check` for the edited CAPL file.
  - CANoe compile/run was not available from terminal.

2026-05-06:

- Added `Auto/OWP/OWP_025 - 2.5 generation.can`.
  - Implements the requested PMD OFF/radar-only camera detach flow for steps
    `1, 2, 3, 5, 7, 8, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20`.
  - Keeps the user-provided action order visible inside each testcase body.
  - Uses shared helpers for key state, SMK IGN, USM/Rec reset, PMD setup,
    camera attach/detach, PMD OFF waits/checks, network sleep, PEV recording,
    `Check_Set_PMD_OFF()`, timers, COM markers, and screenshots.
  - Maps the requested `Set_Detach_Front` action to the existing header helper
    `Set_Detach_Font()`.
  - Caps `WaitForNetworkSleep(...)` at 8 minutes and uses several-minute
    signal waits. Each step waits 5 seconds before `CaptureGraphics(...)`.
  - Step 18 and Step 20 check `BLTN_CAM_RecSta_PMD == 0x2` and
    `BLTN_CAM_PEV_Recorded == 0x2`; `Check_RecSta_PMD_Recording()` is called
    for log evidence because that helper checks `0x1`, while the requested
    acceptance value for these steps is `0x2`.
  - Static verification only so far:
    - Confirmed the file has no direct `testWaitForSignalInRange` call.
    - Checked brace balance and exact `MainTest()` count.
    - Ran `git diff --check` for the new CAPL file.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Follow-up cleanup for files written/reworked by Codex:
  - `Auto/OWP/OWP_023 - 2.5 generation.can` and
    `Auto/OWP/OWP_005 - 2.5 generation.can` now use
    `WaitFor_RecSta_PMD_Recording(...)` for PMD Recording waits.
  - `Auto/CCS/CCS_012 - 2.5 generation.can` now polls
    `BLTN_CAM_PrkOp_Termiate_Info` locally with `testWaitForTimeout(...)`
    because no shared header helper exists for that signal.
  - Added detailed step-flow comments at the top of `OWP_005` and confirmed
    `OWP_023` / `CCS_012` also document their step flows.
  - Static verification only:
    - Confirmed the three files no longer call raw CAPL signal-range waits.
    - Confirmed PMD Recording waits call `WaitFor_RecSta_PMD_Recording(...)`.
    - Checked brace/parenthesis balance and exact `MainTest()` count for the
      three touched files.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Reworked `Auto/OWP/OWP_023 - 2.5 generation.can` for the requested
  OWP_023 PMD/PEV/network-sleep flow.
  - Implemented the provided steps as sequential testcase bodies:
    `1, 2, 4, 6, 9, 11, 15, 16, 17, 19, 20`.
  - Used 8-minute maximum waits for all `WaitForNetworkSleep(...)` calls.
  - Used several-minute signal waits. PMD Recording waits use the shared
    `WaitFor_RecSta_PMD_Recording(max_signal_wait_ms)` helper because that
    helper intentionally triggers PEV first, then waits for PMD Recording.
  - Called existing header helpers for key, reset, PMD setup, network
    management, PEV/DEV waits, camera vertical position, front-camera
    detach/attach, SD card action, BLTN_CAM entry, timer, COM, and capture.
  - Step 9 maps requested display viewing angle `-5/-5` to header trackbar
    value `1` via `Set_FR_Cam_VerPos(1)` and `Set_RR_Cam_VerPos(1)`.
  - Step 20 uses the existing direct signal-setting pattern for SOC because no
    shared SOC helper exists: `ICU_BS2SoC` and `BAT_SOC_for_BLTN_CAM` are set
    to `0x00`, then restored to `0x64`.
  - Each step waits 5 seconds before the final `CaptureGraphics(...)` call and
    captures the related RecSta/key/NM/camera/SOC/SD signals for evidence.
  - Static verification only:
    - Confirmed one `MainTest()` and 11 `OWP_023_*` testcases including init.
    - Confirmed 11 final capture points with explicit 5-second pre-capture
      waits.
    - Confirmed all called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - Checked brace/parenthesis balance and non-ASCII count for the new file.
    - `git diff --check` reported only Git's LF-to-CRLF working-copy warning.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Reworked `Auto/OWP/OWP_005 - 2.5 generation.can` after the user requested
  OWP_005 be merged into one `.can` file and rewritten from the new step logic.
  - Deleted the old split source files:
    - `Auto/OWP/OWP_005_A - 2.5 generation.can`.
    - `Auto/OWP/OWP_005_B - 2.5 generation.can`.
  - Created one sequential OWP_005 test module with the requested steps:
    `1, 2, 3, 4, 6, 8, 10, 12, 13, 14, 15, 16, 17, 20, 21, 23, 24, 25`.
  - Kept action order visible inside each testcase and did not add local
    `Run_*` flow wrappers.
  - Used 6-minute maximum waits for OWP/OWD/PEV/sleep checks. PMD Recording
    waits use the shared `WaitFor_RecSta_PMD_Recording(360000)` helper.
  - Step 23 monitors `BLTN_CAM_PopUp_Req == 0x8` for 60 seconds as the CAN-side
    evidence for the requested "Parking motion recording will not work" popup.
  - Static verification only:
    - Confirmed old OWP_005 split `.can` files are gone and the new `.can` file
      has exactly one `MainTest()`.
    - Confirmed called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - Checked brace/parenthesis balance and non-ASCII count for the new file.
    - `git diff --check` reported no whitespace errors for the touched OWP_005
      source paths.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Follow-up on `Auto/OWP/OWP_005 - 2.5 generation.can`:
  - Added a direct 5-second delay before the final `CaptureGraphics(...)` call
    in each OWP_005 step testcase.
  - Kept the captures inline in the testcase body and used the existing header
    `CaptureGraphics()` helper only.
  - Static verification only:
    - Confirmed 18 `CaptureGraphics(...)` calls and 18 matching
      "Wait 5 seconds before screenshot" logs.
    - Checked brace/parenthesis balance and exact `MainTest()` count.
    - `git diff --check` reported no whitespace errors for OWP_005.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Reworked `Auto/OWD/OWD_007 - 2.5 generation.can` after the user requested
  the OWD_007 actions be written sequentially instead of wrapped in shared
  local flow helpers.
  - Removed the local flow wrappers:
    - `Run_TML_DEV_And_Camera_Action()`.
    - `Run_Step12_Flow()`.
    - `Run_Step14_Flow()`.
  - Inlined the action order directly in the relevant testcases:
    - Steps 1-3 now explicitly call OWD wait, TML start, event switch, DEV
      wait, camera detach/attach helpers, and waits in sequence.
    - Step 12 and Step 14 now explicitly call their requested helper sequence
      inside the testcase body.
    - Step 13 and Step 15 keep the 10-loop automation but now repeat the
      helper calls directly inside each loop instead of calling a flow helper.
  - Preserved the existing `MainTest()` execution order and kept Step 13 /
    Step 15 commented out as they were in the current file.
  - Static verification only:
    - Confirmed no remaining references to the removed `Run_*` helpers.
    - Confirmed all called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - Checked brace/parenthesis balance, exact `MainTest()` count, and
      non-ASCII count for OWD_007.
    - `git diff --check` reported only Git's LF-to-CRLF working-copy warning.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Scanned all `Auto/**/*.can` for CANoe test-module entrypoint and leading BOM
  issues after OWD_031 was confirmed fixed.
  - Found and fixed remaining non-OWD entrypoint problems:
    - `Auto/BAT/BAT_011 for P-LBM battery.can`: `void Maintest()` ->
      `void MainTest()`.
    - `Auto/BAT/BAT_032 for eAGM battery.can`: `void Maintest()` ->
      `void MainTest()`.
    - `Auto/HU/HU_002 - 2.0 generation.can`: `void Maintest()` ->
      `void MainTest()`.
    - `Auto/HU/HU_002 - 2.5 generation.can`: `void Maintest()` ->
      `void MainTest()`.
    - `Auto/BAT/BAT_040 for eAGM battery.can`: added missing
      `void MainTest()` with selectable cases for BAT_040_1 through
      BAT_040_5 and an all-steps option.
  - Verified:
    - No `Auto/**/*.can` file with testcases is missing exact
      `void MainTest()`.
    - No `Auto/**/*.can` file still has `void Maintest()`.
    - No `Auto/**/*.can` file has more than one leading UTF-8 BOM.
    - The five newly edited BAT/HU files have balanced braces and exactly one
      `MainTest()`.
    - `git diff --check` reports only Git's LF-to-CRLF working-copy warnings.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Fixed `Auto/OWD/OWD_031 - 2.5 generation.can` after CANoe reported:
  `Test module 'OWD_031 - 2.5 generation': The test module is not assigned,
  invalid or MainTest() is missing`.
  - `void MainTest()` already existed, so this was treated as the "invalid"
    branch of CANoe's generic error.
  - Found the file had double UTF-8 BOM bytes before `/*@!Encoding:65001*/`
    (`EF BB BF EF BB BF`), which can prevent CAPL from being parsed as a valid
    test module.
  - Rewrote the file as UTF-8 without BOM and changed `SetTestStep("Bước ...")`
    to ASCII `SetTestStep("Buoc ...")`.
  - Applied the same double-BOM cleanup to nearby OWD files with the same
    pattern: `OWD_020` and `OWD_030`. `OWD_030` also had `Bước` changed to
    `Buoc`.
  - Verified:
    - No `Auto/OWD/*.can` file has more than one leading BOM now.
    - All OWD files still have exactly one case-sensitive `void MainTest()`.
    - OWD_031 helper calls exist in `Auto/DeclarationFunction_Gen2.5.h`.
    - Brace/parenthesis balance and trailing whitespace checks pass for the
      edited files.
    - `git diff --check` reports only Git's LF-to-CRLF working-copy warnings.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Fixed `Auto/OWD/OWD_025 - 2.5 generation.can`.
  - Root cause: the file had `void Maintest()` with a lowercase `t`, so CANoe
    may not recognize the test module entrypoint.
  - Changed it to exact `void MainTest()`.
  - Scanned all 32 `Auto/OWD/*.can` files case-sensitively; all now have
    exactly one `void MainTest()`, and no `void Maintest()` remains in OWD.
  - Static verification only:
    - Checked brace/parenthesis balance for `OWD_025`.
    - Ran `git diff --check`; only Git's LF-to-CRLF working-copy warning was
      reported.
  - Broader scan of `Auto/**/*.can` found similar entrypoint issues outside
    OWD in BAT/HU files, but those were not changed because the current request
    targeted OWD.
  - CANoe compile/run was not available from terminal.

2026-05-05:

- Fixed `Auto/TurnON.can` after CANoe reported:
  `Test module 'TurnON': The test module is not assigned, invalid or MainTest() is missing`.
  - Root cause in the file: `testcase TurnON_Init()` and `testcase TurnON_1()`
    existed, but `void MainTest()` was missing.
  - Added `MainTest()` with `testReportFileName("TestReports\\TurnON_report")`,
    `TurnON_Init()`, and `TurnON_1()`.
  - Added per-step COM markers: `Send_Value_To_COM(9111)` at the start of
    `TurnON_1()` and `Send_Value_To_COM(44)` at step end.
- Fixed the same structural issue in `Auto/TurnOFF.can`.
  - Added `MainTest()` with `testReportFileName("TestReports\\TurnOFF_report")`,
    `TurnOFF_Init()`, and `TurnOFF_1()`.
  - Added per-step COM markers: `Send_Value_To_COM(9101)` at the start of
    `TurnOFF_1()` and `Send_Value_To_COM(44)` at step end.
- Static verification only:
  - Confirmed brace/parenthesis balance, non-ASCII count, and trailing
    whitespace for both files.
  - Confirmed all called Turn_ON/Turn_OFF/Check helpers exist in
    `Auto/DeclarationFunction_Gen2.5.h`.
  - `git diff --check` reported no whitespace issues for both files.
- CANoe compile/run was not available from terminal.

2026-05-04:

- Reworked `Auto/CCS/CCS_012 - 2.5 generation.can`.
  - Replaced the previous 10-step CCS_012 flow with the requested 4-step SD
    card / OWP / network-sleep flow.
  - Step 1 performs KEY OFF/ON, `Set_SMK_IGN()`, reset helpers,
    `Turn_ON_OWP()`, `Set_SDcard()`, KEY OFF, `WaitForNetworkSleep(360)`,
    screenshots including `BLTN_CAM_SD_Card_State`, then waits 10 seconds.
  - Step 2 repeats up to 3 attempts of `Set_SDcard()` +
    `WaitFor_RecSta_OWP_Recording(30000)` + check
    `BLTN_CAM_PrkOp_Termiate_Info == 0x5` within 30 seconds, then waits for
    network sleep and captures evidence.
  - Step 3 checks `BLTN_CAM_PrkOp_Termiate_Info == 0x4` within 40 seconds
    after `Set_SDcard()`, then waits for network sleep and captures evidence.
  - Step 4 calls `Set_SDcard()` 9 times, checks that
    `BLTN_CAM_PrkOp_Termiate_Info` has been `0x5` within 2 minutes, then waits
    for network sleep and captures evidence.
  - Uses `31200-31204` COM markers and
    `TestReports\\CCS_012 - 2.5 generation_report`.
  - Static verification only:
    - Confirmed helper names exist in `Auto/DeclarationFunction_Gen2.5.h`.
    - Confirmed `BLTN_CAM_SD_Card_State` and
      `BLTN_CAM_PrkOp_Termiate_Info` exist in DBC files.
    - Checked brace/parenthesis balance, non-ASCII count, and trailing
      whitespace.
    - Ran `git diff --check`; only Git's LF-to-CRLF working-copy warning was
      reported.
  - CANoe compile/run was not available from terminal.

2026-05-04:

- Reworked `Auto/OWD/OWD_012 - 2.5 generation.can`.
  - Implements the requested steps 1-5 for OWD/DEV continuity while
    entering/exiting Built-in Cam mode, cycling KEY OFF/ON, and cycling B+.
  - Uses shared helpers: `KEY_OFF()`, `KEY_ON()`, `Set_SMK_IGN()`,
    `Click_USM_Reset()`, `Click_Rec_Reset()`,
    `WaitFor_RecSta_OWD_Recording()`, `WaitFor_RecSta_DEV_Recording()`,
    `Enter_BLTN_CAM()`, `Exit_BLTN_CAM()`, `Check_BLTN_CAM_UI_Mode()`,
    `Set_BPlus_Off()`, and `Set_BPlus_On()`.
  - Keeps `11200-11205` COM markers and
    `TestReports\\OWD_012 - 2.5 generation_report`.
  - Static verification only:
    - Confirmed all called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - Checked brace/parenthesis balance, non-ASCII count, and trailing
      whitespace for the CAPL file.
    - Ran `git diff --check` for the edited CAPL file; only Git's LF-to-CRLF
      working-copy warning was reported.
  - CANoe compile/run was not available from terminal.

2026-05-04:

- Added `Auto/OWD/OWD_008 - 2.5 generation.can`.
  - Implements the requested steps 1-6 for B+ OFF/ON interruption during OWD,
    DEV, TML, event switch, and rear camera detach flows.
  - Uses shared helpers: `KEY_OFF()`, `KEY_ON()`, `Set_SMK_IGN()`,
    `Click_USM_Reset()`, `Click_Rec_Reset()`,
    `WaitFor_RecSta_OWD_Recording()`, `WaitFor_RecSta_DEV_Recording()`,
    `Click_TML_Start()`, `Click_Evt_Switch()`, `Set_Detach_Rear()`,
    `Set_BPlus_Off()`, and `Set_BPlus_On()`.
  - Uses `10800-10806` COM markers and `TestReports\\OWD_008 - 2.5 generation_report`.
  - Static verification only:
    - Confirmed all called shared helper names exist in
      `Auto/DeclarationFunction_Gen2.5.h`.
    - Checked brace/parenthesis balance and non-ASCII count for the new CAPL
      file.
    - Ran `git diff --check` for the new CAPL file.
  - CANoe compile/run was not available from terminal.

2026-05-04:

- Reworked `Auto/OWD/OWD_007 - 2.5 generation.can`.
  - Implemented the requested steps 1-6, 8-10, and 12-15.
  - Uses `KEY_OFF()`, `KEY_ON()`, `Set_SMK_IGN()`, reset helpers, OWD/OWP/DEV
    wait helpers, `Click_TML_Start()`, `Click_Evt_Switch()`, and the shared
    front/rear attach/detach COM helpers.
  - Step 13 now runs Step 12 flow in a real 10-loop automation.
  - Step 15 now runs Step 14 flow in a real 10-loop automation.
  - Step 7 and Step 11 remain reserved/no-action testcases because the current
    user flow intentionally jumps from 6 to 8 and from 10 to 12.
- Static verification only:
  - Confirmed all called shared helper names exist in
    `Auto/DeclarationFunction_Gen2.5.h`.
  - Checked brace/parenthesis balance and non-ASCII count for the edited CAPL
    file.
  - Ran `git diff --check` for the edited CAPL file.
- CANoe compile/run was not available from terminal.

2026-05-04:

- Added shared helpers in `Auto/DeclarationFunction_Gen2.5.h`:
  - `Set_Attach_Font()` sends `Send_Value_To_COM(3)`.
  - `Set_Detach_Font()` sends `Send_Value_To_COM(4)`.
  - `Set_Attach_Rear()` sends `Send_Value_To_COM(5)`.
  - `Set_Detach_Rear()` sends `Send_Value_To_COM(6)`.
  - `Set_SDcard()` sends `Send_Value_To_COM(7)` and waits 5 seconds.
- Static verification only; CANoe compile/run was not available from terminal.

2026-05-04:

- Added `Auto/TurnOFF.can`.
  - Runs `KEY_ON()`, `Turn_OFF_OWD()`, `Turn_OFF_OWP()`, `Turn_OFF_DEV()`,
    `Turn_OFF_PEV()`, and `Turn_OFF_PMD()`.
  - Verifies SMK is IGN and OWD/OWP/DEV/PEV/PMD setting signals are OFF.
- Added `Auto/TurnON.can`.
  - Runs `KEY_ON()`, `Turn_ON_OWD()`, `Turn_ON_OWP()`, `Turn_ON_DEV()`, and
    `Turn_ON_PEV()`.
  - Verifies SMK is IGN and OWD/OWP/DEV/PEV setting signals are ON.
- Static verification only; CANoe compile/run was not available from terminal.
