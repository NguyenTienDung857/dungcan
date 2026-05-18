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

2026-05-13 | pattern | Gen 2.5 Auto copied into Gen 2.0 CAN project
- Facts: In `D:\project\CANoe\CAN`, the active Gen 2.0 DBC is
  `DB_CAN/20250226_STD_DB_CAR_2021_HS_B2_25-02-01.dbc`. Copied Gen 2.5 Auto
  files need these message mappings: `HU_BLTN_CAM_02_200ms` ->
  `HU_BLTN_CAM_PE_00_200ms`, `HU_BLTN_CAM_01_00ms` -> `HU_BLTN_CAM_E_01`,
  `HU_CLOCK_01_1000ms` -> `HU_CLOCK_PE_02`, `BDC_FD_SMK_03_200ms` ->
  `SMK_13_200ms`, `PDC_FD_10_200ms` -> `ICU_08_200ms`,
  `PDC_FD_11_200ms` -> `ICU_10_200ms`, and `IAU_FD_04_200ms` ->
  `IAU_16_200ms`.
- Facts: BAS setting NvalueSet signals in Gen 2.0 are on `HU_BLTN_CAM_E_00`;
  FR/RR vertical position, PEV app notification, MEV, LVDS, and TML requests
  remain on `HU_BLTN_CAM_E_01`.
- Facts: PMD/PMDR Gen 2.5 signals are not in the active Gen 2.0 DBC. Do not
  map them to unrelated signals; make helper behavior explicit unsupported or
  remove PMD-specific test expectations if the user wants a pure Gen 2.0 suite.
- Use next time: Verify copied Auto against the active DBC/LDF, not against all
  DB files in the checkout.
