# Auto Folder Instructions

`Auto` is the only normal source-code area for Codex work in this project.
This folder contains CANoe/CAPL automated test cases and shared helper headers.

## Primary Header

Use the shared Gen 2.5 helper header first:

- `Auto/DeclarationFunction_Gen2.5.h`

This header is the primary API for test code. Search and use it before writing
manual signal logic.

Typical include paths:

- From `Auto/<module>/<test>.can`:
  `#include "..\DeclarationFunction_Gen2.5.h"`
- From `Auto/<test>.can`:
  `#include "DeclarationFunction_Gen2.5.h"`

Some modules also contain local headers, for example `Auto/BAS` or `Auto/INF`.
When editing a module with a local header, copy the include style used by nearby
tests in that same folder.

## Header-First Rule

Use existing functions from `Auto/DeclarationFunction_Gen2.5.h` as the default
implementation path. This is more important than inventing new logic.

Before coding any action or check, search the header for relevant helpers:

- key and power: `KEY_ON`, `KEY_OFF`, `Set_SMK_IGN`, `Set_BPlus_...`
- reset and panel buttons: `Click_USM_Reset`, `Click_Rec_Reset`,
  `Click_Evt_Switch`, `Click_TML_Start`, `Click_TML_Stop`
- BLTN_CAM mode: `Enter_BLTN_CAM`, `Exit_BLTN_CAM`,
  `Wait_For_BLTN_CAM_Mode`
- recording setup: `Turn_ON_OWD`, `Turn_OFF_OWD`, `Turn_ON_OWP`,
  `Turn_OFF_OWP`, `Turn_ON_DEV`, `Turn_ON_PEV`, `Turn_ON_PMD`,
  `Turn_ON_PMDEV`, `Turn_ON_PMDTime`
- status checks and waits: `Check_...`, `WaitFor_...`, `Verify_...`,
  `Monitor_...`
- configuration: `Set_PEV_Sensitivity`, `Set_PMD_RanSet`,
  `Set_OWP_ParkingTime`, `Set_EV_BeforeTime`, `Set_EV_AfterTime`,
  `Set_PEV_AppNoti`
- evidence and reporting: `CaptureGraphics`, `StartTestTimer`,
  `StopTestTimer`, `SetTestStep`, `Send_Value_To_COM`, `Log_Error_Details`

Do not duplicate helper behavior or write direct signal assignments if the
header already provides a function. If no helper exists, follow the OWD/OWP
style and keep any new helper local to the test file.

## Reference Samples

Before writing a new test, use OWD/OWP as the structural standard. The user
wrote these and considers them the cleanest examples in this project:

- `Auto/OWD/OWD_027 - 2.5 generation.can`
- `Auto/OWP/OWP_001 - 2.5 generation.can`

Then inspect at least one similar `.can` file in the target module for
feature-specific signals, expected values, COM marker ranges, and screenshots.
Useful secondary references include:

- `Auto/CCS/CCS_005 - 2.5 generation.can`
- `Auto/CCS/CCS_020 - 2.5 generation.can`
- `Auto/OTA/OTA_001.can`
- `Auto/INF/INF_001 - 2.5 generation.can`

Match OWD/OWP for step layout, banners, debug logs, result tracking, timer
usage, pass/fail handling, and screenshots. Match the target module only for
domain-specific signal logic.

## OWD/OWP-Style Test Template

Use this as the default shape for new tests. Adjust names, COM marker values,
signals, waits, and captures to match the requested test case.

```capl
/*@!Encoding:65001*/

includes
{
  #include "..\DeclarationFunction_Gen2.5.h"
}

/*
  Test Case: <ID>
  Title: <short behavior>

  Verification items:
    - Step 1: <action and expected result>
    - Step 2: <action and expected result>

  Preconditions:
    - <state>

  NOTE: Uses 2-5s delay for signal propagation after each state transition.
*/

variables
{
  char failReason[512];
  int step1_passed = 0;
  byte actualValue = 0x0;
}

// ============================================================================
// Main Test Case
// ============================================================================

testcase <ID>_Init()
{
  Send_Value_To_COM(<init_marker>);
  write("========================================");
  write("=== START TEST <ID> ===");
  write("========================================");
  testStep("Notification", "Start test <ID>");
  Send_Value_To_COM(44);
}

testcase <ID>_1()
{
  int result = 0;

  //============================================================================
  // STEP 1: <short step intent>
  //============================================================================
  Send_Value_To_COM(<step_marker>);
  SetTestStep("Buoc 1 - <ID>");
  StartTestTimer();

  write("");
  write("=================================================================");
  write("=== STEP 1: <short step banner>                              ===");
  write("=================================================================");
  testStep("Step1", "<human-readable action and expected result>");

  write("[1.1] Setting SMK_Signal to IGN...");
  Set_SMK_IGN();
  testWaitForTimeout(3000);

  write("[1.2] Clicking USM Reset...");
  Click_USM_Reset();
  testWaitForTimeout(2000);

  write("[1.3] Clicking Rec Reset...");
  Click_Rec_Reset();
  testWaitForTimeout(2000);

  write("[1.4] Checking <SignalName>...");
  result = <Check_Or_Wait_Helper>();
  actualValue = $<SignalName>;
  write("<SignalName> = 0x%X, expected 0x%X", actualValue, <expected>);

  if(result == 1)
  {
    step1_passed = 1;
    write(">>> Step 1: PASS - <SignalName> reached expected value <<<");
    testStepPass("Step1", "Step 1 PASS");
  }
  else
  {
    step1_passed = 0;
    write(">>> Step 1: FAIL - <SignalName> did not reach expected value <<<");
    snprintf(failReason, elcount(failReason),
             "Step 1 FAIL: <SignalName>=0x%X expected 0x%X.",
             actualValue, <expected>);
    Log_Error_Details();
    testStepFail("Step1", failReason);
  }

  write("[1.5] Capturing screenshot...");
  CaptureGraphics("<module>", "<SignalName>;<OtherSignal>");

  write("=== FINISH STEP 1 ===");
  StopTestTimer();
  Send_Value_To_COM(44);
}
```

## CAPL Rules

- Keep code CAPL-compatible. Avoid C++/modern C patterns that CANoe CAPL may
  not support.
- Use header functions first. Manual signal writes are a fallback, not the
  default.
- Prefer `byte`, `int`, `long`, `char[]`, `message`, and `msTimer` as seen in
  existing files.
- Use `testWaitForTimeout(...)` for explicit waits unless the local sample uses
  a better helper.
- Use existing `WaitFor_...` and `Check_...` helpers for polling and assertions.
- Keep helper functions local to the `.can` file unless the user asks to update
  the shared header.
- Do not edit `.cbf`, `.dbg`, reports, or generated output files.

## Logging Checklist

Each implemented step should include:

- step banner with `write("=== STEP ... ===")`
- action logs before setting signals or pressing panel controls
- wait logs before long waits
- actual value logs before assertion
- pass/fail logs with expected and actual values
- screenshot/capture log when `CaptureGraphics(...)` is called

## Common Built-in Cam Signals and Modes

Use `system_space.txt` and the helper header as the source of truth. Common
terms in this project:

- `OWD`: On While Driving recording
- `OWP`: On While Parked recording
- `DEV`: Driving Event recording
- `PEV`: Parked Event recording
- `PMD`: Parking Motion Detection
- `MAR`: Manual recording
- `TML`: Timelapse
- `BLTN_CAM`: Built-in Cam ECU
- `HU` / `AVN`: Head Unit
- `SMK_TrmnlCtrlGrpStaBDC`: key/terminal state
