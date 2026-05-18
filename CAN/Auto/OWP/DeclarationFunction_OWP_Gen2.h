/*@!Encoding:65001*/
/*
 * ===================================================================================
 * FILE: DeclarationFunction_OWP_Gen2.h
 * ===================================================================================
 *
 * DESCRIPTION:
 *   Reusable CAPL helper function library for OWP (Parking Recording) automated
 *   test cases on Gen 2.0 Built-in Camera (BLTN_CAM) system.
 *
 *   This header provides a complete set of functions for controlling and verifying
 *   the dashcam recording system via CANoe panel GUI controls and CAN bus signals.
 *   It abstracts low-level CAN signal manipulation and panel system variable
 *   interaction into high-level, reusable test building blocks.
 *
 * KEYWORDS:
 *   CANoe, CAPL, Built-in Camera, BLTN_CAM, dashcam, blackbox camera,
 *   OWD (Continuous Driving Recording), OWP (Continuous Parking Recording),
 *   DEV (Driving Event Recording), PEV (Parking Event Recording),
 *   TML (Time-lapse Recording), MAR (Manual Recording), PMD (Parking Motion Detection),
 *   CAN bus, panel control, automated testing, signal verification, recording test,
 *   SMK (Smart Key / Terminal Control), ICU (Instrument Cluster Unit),
 *   HU (Head Unit), AVN (Audio Video Navigation), FRS (Factory Reset),
 *   VN1600 hardware interface, AIN voltage measurement
 *
 * SYSTEM CONTEXT:
 *   - Target: Automotive Built-in Camera (dashcam) ECU
 *   - Bus: CAN (Controller Area Network)
 *   - Tool: Vector CANoe with CAPL scripting
 *   - Hardware: VN1600 CAN interface with AIN (Analog Input) capability
 *   - Panel: CANoe panel GUI with buttons, switches, LED controls, and trackbars
 *
 * USAGE:
 *   In your .can test script, add:
 *     includes { #include "DeclarationFunction_OWP_Gen2.h" }
 *
 * REQUIRED VARIABLES (declare in your .can file's variables{} block):
 *   message HU_BLTN_CAM_PE_00_200ms _HU_BLTN_CAM_PE_00_200ms;
 *     -> CAN message for HU to BLTN_CAM periodic commands (UI mode, settings)
 *   message HU_BLTN_CAM_E_01        _HU_BLTN_CAM_E_01;
 *     -> CAN message for Manual Event Recording Request (MEV_Rec_Req)
 *   message HU_CLOCK_PE_02          _HU_CLOCK_PE_02;
 *     -> CAN message for clock/time setting commands
 *
 * CAN SIGNAL REFERENCE:
 *   SMK_TrmnlCtrlGrpStaBDC  : Terminal control (0x0=OFF, 0x1=ACC, 0x2=IGN)
 *   BLTN_CAM_HU_UI_Mode     : UI mode response (0x0=AVN, 0x1=Built-in Cam)
 *   BLTN_CAM_RecSet_*       : Recording setting status (0x1=OFF, 0x2=ON)
 *   BLTN_CAM_RecSta_*       : Recording active status (0x0=OFF, 0x1=Recording)
 *   BLTN_CAM_SenSet_DEV     : Driving event sensitivity level (0x1~0x5 = Lv.1~Lv.5)
 *   BLTN_CAM_SenSet_PEV     : Parking event sensitivity level (0x1~0x5 = Lv.1~Lv.5)
 *   BLTN_CAM_PMD_RanSet     : Parking motion detection range (0x1~0x5 = Lv.1~Lv.5)
 *   BLTN_CAM_Set_FR_Ver_Pos : Front camera vertical position (CAN enum)
 *   BLTN_CAM_Set_RR_Ver_Pos : Rear camera vertical position (CAN enum)
 *   BLTN_CAM_Set_PrkTime    : OWP parking recording time (0x1~0x14 = 1Hr~20Hr)
 *   BLTN_CAM_Set_EV_BfrTime : Event before-time raw value (actual = raw + 10 seconds)
 *   BLTN_CAM_Set_EV_AftTime : Event after-time raw value (actual = raw + 10 seconds)
 *   BLTN_CAM_Set_Drv_TML_Int: Time-lapse interval (0x1=20min, 0x2=40min, 0x5=100min)
 *   ICU_PowerAutoCutModSta  : Power auto-cut mode (0x0=Factory, 0x1=Dealer, 0x2=Customer)
 *   CF_AVN_BLTN_CAM_RecReset: SD card format/reset signal (0x1=Reset triggered)
 *   BLTN_CAM_SD_Format_Op_State: SD format operation state (0x1=Formatting)
 *   MEV_Rec_Req             : Manual Event Recording Request (0x0=Reset, 0x1=Trigger)
 *   BLTN_CAM_State           : Overall camera state
 *
 * PANEL SYSTEM VARIABLES (GUI controls in CANoe panel):
 *   @panel::Switch_CGWNM        : CGW Network Management switch (0=OFF, 1=ON)
 *   @panel::Button_USMReset      : USM (Update Storage Memory) reset button
 *   @panel::Button_RecReset      : Recording reset / SD card format button
 *   @panel::Button_Timelaps_Start: Time-lapse start button
 *   @panel::Button_Timelaps_Stop : Time-lapse stop button
 *   @panel::Button_SwitchEvt     : Event switch / MAR trigger button
 *   @panel::LED_Ctrl_SetOWD      : OWD recording ON/OFF toggle LED control
 *   @panel::LED_Ctrl_SetOWP      : OWP recording ON/OFF toggle LED control
 *   @panel::LED_Ctrl_SetDEV      : DEV recording ON/OFF toggle LED control
 *   @panel::LED_Ctrl_SetPEV      : PEV recording ON/OFF toggle LED control
 *   @panel::LED_Ctrl_SetPMD      : PMD recording ON/OFF toggle LED control
 *   @sysvar::panel::TrackBar_DEV_Sen      : DEV sensitivity trackbar (1~5)
 *   @sysvar::panel::TrackBar_PEV_Sen      : PEV sensitivity trackbar (1~5)
 *   @sysvar::panel::TrackBar_PMD_RanSet   : PMD range trackbar (1~5)
 *   @sysvar::panel::TrackBar_FR_Ver_Pos   : Front camera vertical position trackbar (1~11)
 *   @sysvar::panel::TrackBar_RR_Ver_Pos   : Rear camera vertical position trackbar (1~11)
 *   @sysvar::panel::TrackBar_OWP_TIME     : OWP parking time trackbar (1~20 hours)
 *   @sysvar::panel::TrackBar_EV_Time_Bfr  : Event before-time trackbar (1=20s, 2=15s, 3=10s)
 *   @sysvar::panel::TrackBar_EV_Time_Aft  : Event after-time trackbar (10, 15, or 20 seconds)
 *   @sysvar::panel::TrackBar_Timelaps_TIME: Time-lapse interval trackbar (1=20min, 2=40min, 3=100min)
 *   @sysvar::panel::EnableDrvInfo          : Driving info simulation switch (0=OFF, 1=ON)
 *   @sysvar::IO::VN1600_1::AIN            : VN1600 analog input voltage reading
 *
 * FUNCTION RETURN CONVENTIONS:
 *   - int return: 1 = PASS/SUCCESS, 0 = FAIL/TIMEOUT/ERROR
 *   - void return: fire-and-forget actions (no verification needed)
 *   - Set_* functions: include internal Check + retry loop (up to 15 attempts)
 *   - Check_* functions: instant verification, no retry
 *   - WaitFor_* functions: polling with configurable timeout_ms
 *
 * TABLE OF CONTENTS:
 *   Section  1: KEY / SMK CONTROL           - Ignition key ON/OFF, terminal state control
 *   Section  2: PANEL BUTTON ACTIONS        - GUI button clicks (USM Reset, Rec Reset, TML, Event)
 *   Section  3: BLTN_CAM MODE CONTROL       - Enter/Exit Built-in Camera mode (vs AVN mode)
 *   Section  4: RECORDING TOGGLE            - Turn ON/OFF each recording type via panel LED
 *   Section  5: RECORDING STATUS CHECK      - Instant check of RecSet (setting) and RecSta (status) signals
 *   Section  6: RECORDING STATUS WAIT       - Wait with timeout for recording to start (polling loop)
 *   Section  7: FACTORY / CUSTOMER MODE     - ICU PowerAutoCut mode control (Factory/Dealer/Customer)
 *   Section  8: FRS RESET DETECTION         - SD card format / recording data reset monitoring
 *   Section  9: AIN VOLTAGE                 - VN1600 analog input voltage measurement and threshold check
 *   Section 10: SCREENSHOT / CAPTURE        - CANoe test report screenshot capture
 *   Section 11: COUNTDOWN TIMER             - Wait countdown display (milliseconds and minutes)
 *   Section 12: SIGNAL LOGGING              - Dump all recording-related signal values to write window
 *   Section 13: DRIVING INFO SIMULATION     - Enable/Disable vehicle driving info simulation
 *   Section 14: COMMON INIT SEQUENCE        - Standard test setup and teardown procedures
 *   Section 15: BLTN_CAM SETTINGS           - Panel TrackBar controls for camera settings
 *               (DEV Sensitivity, PEV Sensitivity, PMD Range, FR/RR Camera VerPos,
 *                OWP Parking Time, EV Before/After Time, TML Interval)
 *
 * EXTRACTED FROM: Common patterns across OWD_001 ~ OWD_032 test files.
 * ===================================================================================
 */

// =============================================================================
// GEN 2.0 COMPATIBILITY: PMD / PMDR UNSUPPORTED SIGNALS
// =============================================================================
//
// The active Gen 2.0 HS DBC in this project does not contain the PMD/PMDR
// signals used by later copied tests. These accessors keep legacy OWP helpers
// compile-safe and make the missing feature explicit in the CANoe output.
// =============================================================================

byte Gen20_PMD_Set_Value()
{
  write("  [GEN2.0] BLTN_CAM_Set_PMD is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMDEV_Set_Value()
{
  write("  [GEN2.0] BLTN_CAM_Set_PMDEV is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMDTime_Set_Value()
{
  write("  [GEN2.0] BLTN_CAM_Set_PMDTime is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMD_RecSta_Value()
{
  write("  [GEN2.0] BLTN_CAM_RecSta_PMD is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMD_RecSet_Value()
{
  write("  [GEN2.0] BLTN_CAM_RecSet_PMD is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMDR_RanSet_Value()
{
  write("  [GEN2.0] BLTN_CAM_RanSet_PMDR is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMDR_MaxRanSet_Value()
{
  write("  [GEN2.0] BLTN_CAM_MaxRanSet_PMDR is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMDR_NvalueSet_Value()
{
  write("  [GEN2.0] BLTN_CAM_Ran_PMDR_NvalueSet is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMD_Count_Value()
{
  write("  [GEN2.0] BLTN_CAM_PMD_Count is not available in the active DBC");
  return 0x0;
}

byte Gen20_PMD_Range_Value()
{
  write("  [GEN2.0] BLTN_CAM_PMD_RanSet is not available in the active DBC");
  return 0x0;
}

// =============================================================================
// SECTION 1: KEY / SMK CONTROL
// =============================================================================
//
// Purpose: Control the vehicle ignition key state via CAN signal.
// Keywords: ignition, key on, key off, SMK, terminal, ACC, IGN, power state,
//           CGW NM (Gateway Network Management), vehicle wake-up, BDC
//
// Signal: SMK_TrmnlCtrlGrpStaBDC (Smart Key Terminal Control Group Status)
//   0x0 = OFF  (key removed / power off)
//   0x1 = ACC  (accessory mode - limited electronics)
//   0x2 = IGN  (ignition on - full electronics, camera system active)
//
// Typical test flow: KEY_ON() -> [run test] -> KEY_OFF()
// For full initialization with CGW NM: Set_SMK_IGN() (enables network first)
// =============================================================================

/*
 * KEY_ON()
 * --------
 * Quick ignition ON: directly sets SMK terminal to IGN (0x2).
 * Does NOT enable CGW Network Management - use Set_SMK_IGN() for full init.
 *
 * Keywords: key on, ignition on, power on, start, wake up, SMK IGN
 * Signal modified: SMK_TrmnlCtrlGrpStaBDC = 0x2
 * Return: void (fire-and-forget, no verification)
 */
void KEY_ON()
{
  setSignal(SMK_TrmnlCtrlGrpStaBDC, 0x2);
}

/*
 * KEY_OFF()
 * ---------
 * Quick ignition OFF: directly sets SMK terminal to OFF (0x0).
 * Simulates removing the ignition key / turning off the vehicle.
 * The camera ECU should begin parking mode operations after KEY_OFF.
 *
 * Keywords: key off, ignition off, power off, shutdown, sleep, SMK OFF
 * Signal modified: SMK_TrmnlCtrlGrpStaBDC = 0x0
 * Return: void (fire-and-forget, no verification)
 */
void KEY_OFF()
{
  setSignal(SMK_TrmnlCtrlGrpStaBDC, 0x0);
}

/*
 * Set_SMK_IGN()
 * -------------
 * Full ignition ON procedure with CGW Network Management enable.
 * Steps:
 *   1. Enable CGW NM switch on panel (wakes up CAN network)
 *   2. Wait 2 seconds for network stabilization
 *   3. Set SMK terminal to IGN (0x2)
 *   4. Wait 3 seconds for ECU initialization
 *   5. Log current SMK value for confirmation
 *
 * Use this instead of KEY_ON() when starting a test from cold boot.
 *
 * Keywords: SMK IGN, full init, CGW NM, network management, ignition,
 *           panel switch, vehicle wake up, initialization
 * Panel variable: @panel::Switch_CGWNM = 1
 * Signal modified: SMK_TrmnlCtrlGrpStaBDC = 0x2
 * Return: void
 */
void Set_SMK_IGN()
{
  write("=== Setting SMK_Signal to IGN (0x2) via panel ===");
  @panel::Switch_CGWNM = 1;
  testWaitForTimeout(2000);
  setSignal(SMK_TrmnlCtrlGrpStaBDC, 0x2);
  testWaitForTimeout(3000);
  write("  SMK_TrmnlCtrlGrpStaBDC = 0x%X (IGN)", $SMK_TrmnlCtrlGrpStaBDC);
}

/*
 * Set_SMK_ACC()
 * -------------
 * Set SMK terminal to ACC (Accessory, 0x1).
 * ACC mode provides limited electronics power without full ignition.
 * Some camera features may be restricted in ACC mode.
 *
 * Keywords: SMK ACC, accessory mode, limited power, partial ignition
 * Signal modified: SMK_TrmnlCtrlGrpStaBDC = 0x1
 * Return: void
 */
void Set_SMK_ACC()
{
  write("=== Setting SMK_Signal to ACC (0x1) via panel ===");
  setSignal(SMK_TrmnlCtrlGrpStaBDC, 0x1);
  testWaitForTimeout(2000);
  write("  SMK_TrmnlCtrlGrpStaBDC = 0x%X (ACC)", $SMK_TrmnlCtrlGrpStaBDC);
}

/*
 * Get_SMK_Status_String(char result[])
 * -------------------------------------
 * Reads the current SMK terminal state and converts it to a human-readable
 * string: "IGN", "ACC", "OFF", or "Unknown".
 * Useful for logging and test report output.
 *
 * Keywords: SMK status, terminal state, read status, string conversion, debug
 * Signal read: SMK_TrmnlCtrlGrpStaBDC
 * Parameter: result[] - output char array (min 10 chars), will contain status string
 * Return: void (result written via output parameter)
 */
void Get_SMK_Status_String(char result[])
{
  byte smk_val;
  smk_val = $SMK_TrmnlCtrlGrpStaBDC;

  if (smk_val == 0x2)
    snprintf(result, 10, "%s", "IGN");
  else if (smk_val == 0x1)
    snprintf(result, 10, "%s", "ACC");
  else if (smk_val == 0x0)
    snprintf(result, 10, "%s", "OFF");
  else
    snprintf(result, 10, "%s", "Unknown");
}

// =============================================================================
// SECTION 2: PANEL BUTTON ACTIONS
// =============================================================================
//
// Purpose: Simulate user button clicks on the CANoe panel GUI.
//          These correspond to physical button interactions on the camera
//          settings interface.
// Keywords: panel, button, click, GUI interaction, USM reset, Rec reset,
//           SD card format, time-lapse start/stop, event switch, MAR trigger,
//           manual event recording, MEV request, CAN message send
//
// Panel buttons use toggle behavior: set to 0 then 1 to simulate a click.
// Some buttons only need setting to 1 (latching behavior).
// =============================================================================

/*
 * Click_USM_Reset()
 * -----------------
 * Clicks the USM (Update Storage Memory) Reset button on the panel.
 * This resets the USB storage/memory module, useful before starting tests
 * to ensure clean storage state.
 *
 * Keywords: USM reset, storage reset, memory reset, panel button, initialization
 * Panel variable: @panel::Button_USMReset = 1
 * Wait: 5000ms after click for reset to complete
 * Return: void
 */
void Click_USM_Reset()
{
  write("=== Clicking USM Reset button on panel ===");
  @panel::Button_USMReset = 1;
  testWaitForTimeout(5000);
  write("  USM Reset clicked");
}

/*
 * Click_Rec_Reset()
 * -----------------
 * Clicks the Recording Reset button on the panel.
 * Initiates SD card format / recording data reset.
 * Used to clear all recorded files and start fresh.
 *
 * Keywords: recording reset, SD card format, clear recordings, panel button,
 *           data reset, storage cleanup, initialization
 * Panel variable: @panel::Button_RecReset = 1
 * Wait: 2000ms after click
 * Return: void
 */
void Click_Rec_Reset()
{
  write("=== Clicking Rec Reset button on panel ===");
  @panel::Button_RecReset = 1;
  testWaitForTimeout(2000);
  write("  Rec Reset clicked");
}

/*
 * Click_TML_Start()
 * -----------------
 * Clicks the Time-lapse Start button on the panel.
 * Initiates time-lapse (TML) recording mode.
 * Uses toggle pattern: set to 0 first, then 1 (simulates button press).
 *
 * Keywords: time-lapse, TML, start recording, timelapse start, panel button
 * Panel variable: @panel::Button_Timelaps_Start (0 -> 1 toggle)
 * Wait: 500ms between toggle + 3000ms after click
 * Return: void
 */
void Click_TML_Start()
{
  write("=== Clicking TML Start button on panel ===");
  @panel::Button_Timelaps_Start = 0;
  testWaitForTimeout(500);
  @panel::Button_Timelaps_Start = 1;
  testWaitForTimeout(3000);
  write("  TML Start clicked");
}

/*
 * Click_TML_Stop()
 * ----------------
 * Clicks the Time-lapse Stop button on the panel.
 * Stops ongoing time-lapse (TML) recording.
 * Uses toggle pattern: set to 0 first, then 1 (simulates button press).
 *
 * Keywords: time-lapse, TML, stop recording, timelapse stop, panel button
 * Panel variable: @panel::Button_Timelaps_Stop (0 -> 1 toggle)
 * Wait: 500ms between toggle + 3000ms after click
 * Return: void
 */
void Click_TML_Stop()
{
  write("=== Clicking TML Stop button on panel ===");
  @panel::Button_Timelaps_Stop = 0;
  testWaitForTimeout(500);
  @panel::Button_Timelaps_Stop = 1;
  testWaitForTimeout(3000);
  write("  TML Stop clicked");
}

/*
 * Click_Evt_Switch()
 * ------------------
 * Clicks the Event Switch button on the panel.
 * Triggers a manual recording event (MAR - Manual Recording).
 * In real vehicle, this is equivalent to pressing the physical event button
 * on the dashcam to save the current moment.
 * Uses toggle pattern: set to 0 first, then 1 (simulates button press).
 *
 * Keywords: event switch, MAR, manual recording, manual event, trigger event,
 *           save moment, panel button, dashcam event button
 * Panel variable: @panel::Button_SwitchEvt (0 -> 1 toggle)
 * Wait: 500ms between toggle + 3000ms after click
 * Return: void
 */
void Click_Evt_Switch()
{
  write("=== Clicking Event Switch button on panel ===");
  @panel::Button_SwitchEvt = 0;
  testWaitForTimeout(500);
  @panel::Button_SwitchEvt = 1;
  testWaitForTimeout(3000);
  write("  Event Switch clicked");
}

/*
 * Send_MEV_Rec_Req()
 * ------------------
 * Sends a Manual Event Recording Request via CAN message.
 * This is the CAN-level method to trigger a manual recording event,
 * as opposed to panel button click.
 *
 * Process:
 *   1. Fill all 8 bytes of HU_BLTN_CAM_E_01 with 0xFF (default padding)
 *   2. Set MEV_Rec_Req signal to 0x1 (Trigger)
 *   3. Send the CAN message via output()
 *
 * Keywords: MEV, manual event recording, CAN message, trigger, HU_BLTN_CAM_E_01,
 *           MEV_Rec_Req, recording request, event trigger via CAN
 * CAN message: HU_BLTN_CAM_E_01
 * Signal: MEV_Rec_Req = 0x1 (Trigger)
 * Wait: 3000ms after sending
 * Return: void
 */
void Send_MEV_Rec_Req()
{
  int i;
  write("=== Sending MEV_Rec_Req = 0x1 via CAN ===");
  for (i = 0; i < 8; i++)
  {
    _HU_BLTN_CAM_E_01.byte(i) = 0xFF;
  }
  _HU_BLTN_CAM_E_01.MEV_Rec_Req = 0x1;
  output(_HU_BLTN_CAM_E_01);
  testWaitForTimeout(3000);
  write("  MEV_Rec_Req sent");
}

/*
 * Reset_MEV_Rec_Req()
 * --------------------
 * Resets the Manual Event Recording Request signal back to 0x0.
 * Must be called after Send_MEV_Rec_Req() to clear the request,
 * otherwise the signal stays asserted.
 *
 * Keywords: MEV reset, clear event request, MEV_Rec_Req clear, CAN message reset
 * CAN message: HU_BLTN_CAM_E_01
 * Signal: MEV_Rec_Req = 0x0 (Reset)
 * Wait: 1000ms after sending
 * Return: void
 */
void Reset_MEV_Rec_Req()
{
  int i;
  write("=== Resetting MEV_Rec_Req = 0x0 ===");
  for (i = 0; i < 8; i++)
  {
    _HU_BLTN_CAM_E_01.byte(i) = 0xFF;
  }
  _HU_BLTN_CAM_E_01.MEV_Rec_Req = 0x0;
  output(_HU_BLTN_CAM_E_01);
  testWaitForTimeout(1000);
}

// =============================================================================
// SECTION 3: BLTN_CAM MODE CONTROL (Enter / Exit Built-in Camera)
// =============================================================================
//
// Purpose: Switch the Head Unit (HU) display between AVN mode and Built-in
//          Camera mode. The camera settings UI is only accessible when in
//          Built-in Camera mode.
// Keywords: BLTN_CAM, built-in camera, UI mode, AVN mode, head unit display,
//           HU_BLTN_CAM_UI_Mode, enter camera, exit camera, mode switch,
//           HU_BLTN_CAM_PE_00_200ms, BLTN_CAM_HU_UI_Mode
//
// UI Mode values:
//   HU_BLTN_CAM_UI_Mode = 0x0 : AVN mode (normal Audio/Video/Navigation)
//   HU_BLTN_CAM_UI_Mode = 0x1 : Built-in Camera mode (dashcam settings visible)
//
// Response signal: BLTN_CAM_HU_UI_Mode (feedback from camera ECU)
//   0x0 = AVN mode confirmed
//   0x1 = Built-in Camera mode confirmed
// =============================================================================

/*
 * Enter_BLTN_CAM()
 * ----------------
 * Switches HU display to Built-in Camera mode by sending UI_Mode=0x1
 * via CAN message HU_BLTN_CAM_PE_00_200ms.
 * Polls BLTN_CAM_HU_UI_Mode up to 10 times (500ms interval = 5s total)
 * to confirm the mode switch was accepted.
 *
 * Keywords: enter camera mode, open camera UI, BLTN_CAM mode on,
 *           HU_BLTN_CAM_PE_00_200ms, switch to camera
 * CAN message: HU_BLTN_CAM_PE_00_200ms
 * Signal sent: HU_BLTN_CAM_UI_Mode = 0x1
 * Signal checked: BLTN_CAM_HU_UI_Mode == 0x1
 * Return: void (logs OK/NG)
 */
void Enter_BLTN_CAM()
{
  int i;
  write(">>> Enter BLTN_CAM mode <<<");
  _HU_BLTN_CAM_PE_00_200ms.HU_BLTN_CAM_UI_Mode = 0x1;
  output(_HU_BLTN_CAM_PE_00_200ms);

  for (i = 0; i < 10; i++)
  {
    testWaitForTimeout(500);
    if (getSignal(BLTN_CAM_HU_UI_Mode) == 0x1)
    {
      write(">>> [OK] BLTN_CAM mode confirmed <<<");
      return;
    }
  }
  write(">>> [NG] Timeout: BLTN_CAM_HU_UI_Mode = 0x%X <<<",
        getSignal(BLTN_CAM_HU_UI_Mode));
}

/*
 * Exit_BLTN_CAM()
 * ---------------
 * Switches HU display back to AVN (Audio/Video/Navigation) mode
 * by sending UI_Mode=0x0 via CAN message.
 * Polls BLTN_CAM_HU_UI_Mode up to 10 times (500ms interval = 5s total)
 * to confirm the mode switch was accepted.
 *
 * Keywords: exit camera mode, close camera UI, back to AVN, BLTN_CAM mode off,
 *           return to navigation, switch to AVN
 * CAN message: HU_BLTN_CAM_PE_00_200ms
 * Signal sent: HU_BLTN_CAM_UI_Mode = 0x0
 * Signal checked: BLTN_CAM_HU_UI_Mode == 0x0
 * Return: void (logs OK/NG)
 */
void Exit_BLTN_CAM()
{
  int i;
  write(">>> Exit to AVN mode <<<");
  _HU_BLTN_CAM_PE_00_200ms.HU_BLTN_CAM_UI_Mode = 0x0;
  output(_HU_BLTN_CAM_PE_00_200ms);

  for (i = 0; i < 10; i++)
  {
    testWaitForTimeout(500);
    if (getSignal(BLTN_CAM_HU_UI_Mode) == 0x0)
    {
      write(">>> [OK] AVN mode confirmed <<<");
      return;
    }
  }
  write(">>> [NG] Timeout: BLTN_CAM_HU_UI_Mode = 0x%X <<<",
        getSignal(BLTN_CAM_HU_UI_Mode));
}

/*
 * Wait_For_BLTN_CAM_Mode(long timeout_ms)
 * ----------------------------------------
 * Waits for the Built-in Camera mode to become active, with a configurable
 * timeout. Used when the mode change is triggered externally (e.g., by
 * the user or another test step) and we just need to wait for it.
 * Periodically logs the current state at ~1/6 of total timeout intervals.
 *
 * Keywords: wait for camera mode, polling, timeout, BLTN_CAM_HU_UI_Mode,
 *           mode detection, manual entry, wait for UI
 * Signal checked: BLTN_CAM_HU_UI_Mode == 0x1
 * Parameter: timeout_ms - maximum wait time in milliseconds
 * Return: 1 = camera mode detected, 0 = timeout
 */
int Wait_For_BLTN_CAM_Mode(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;
  byte ui_mode = 0;

  write("Waiting for Built-in Cam mode... (max %d seconds)", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 1000) * 1000;
  if (log_interval < 1000)
    log_interval = 1000;

  while (elapsed < timeout_ms)
  {
    ui_mode = $BLTN_CAM_HU_UI_Mode;

    if (ui_mode == 0x1)
    {
      write(">>> Now in Built-in Cam mode! <<<");
      return 1;
    }

    if (elapsed == 0 || elapsed % log_interval == 0)
    {
      write("  [%d s] UI_Mode=0x%X (0=AVN, 1=BLTN_CAM)", elapsed / 1000, ui_mode);
    }

    testWaitForTimeout(1000);
    elapsed += 1000;
  }

  write(">>> TIMEOUT: Not in Built-in Cam mode <<<");
  return 0;
}

/*
 * Check_BLTN_CAM_UI_Mode(byte expected_mode)
 * -------------------------------------------
 * Instantly checks whether the current BLTN_CAM_HU_UI_Mode matches the
 * expected value. Waits 2 seconds before reading for signal stabilization.
 *
 * Keywords: check UI mode, verify mode, BLTN_CAM_HU_UI_Mode, instant check,
 *           AVN mode check, camera mode check, mode verification
 * Signal checked: BLTN_CAM_HU_UI_Mode
 * Parameter: expected_mode - 0x0 (AVN) or 0x1 (Built-in Cam)
 * Return: 1 = PASS (mode matches), 0 = FAIL (mode mismatch)
 */
int Check_BLTN_CAM_UI_Mode(byte expected_mode)
{
  byte ui_mode = 0;

  testWaitForTimeout(2000);

  ui_mode = $BLTN_CAM_HU_UI_Mode;
  write("  BLTN_CAM_HU_UI_Mode = 0x%X", ui_mode);

  if (ui_mode == expected_mode)
  {
    if (expected_mode == 0x0)
      write(">>> PASS: BLTN_CAM_HU_UI_Mode = 0x0 (AVN Mode) <<<");
    else if (expected_mode == 0x1)
      write(">>> PASS: BLTN_CAM_HU_UI_Mode = 0x1 (Build-in Cam Mode) <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: BLTN_CAM_HU_UI_Mode != 0x%X <<<", expected_mode);
    return 0;
  }
}

// =============================================================================
// SECTION 4: RECORDING TOGGLE (Panel LED Controls)
// =============================================================================
//
// Purpose: Turn ON or OFF each recording type through the CANoe panel LED
//          toggle controls. Each function includes a retry loop to handle
//          cases where the ECU doesn't respond immediately.
// Keywords: recording toggle, turn on, turn off, LED control, panel,
//           OWD, OWP, DEV, PEV, PMD, continuous recording, event recording,
//           parking recording, driving recording, motion detection,
//           RecSet signal, retry loop, toggle button
//
// Recording types:
//   OWD = Continuous Driving Recording (records while driving)
//   OWP = Continuous Parking Recording (records while parked)
//   DEV = Driving Event Recording (saves clip on driving impact/event)
//   PEV = Parking Event Recording (saves clip on parking impact/event)
//   PMD = Parking Motion Detection (records on motion detected while parked)
//
// RecSet signal values (setting status, response from camera ECU):
//   0x1 = OFF (recording function disabled)
//   0x2 = ON  (recording function enabled)
//
// Note: These are toggle controls - clicking when ON turns OFF, and vice versa.
//       The retry loop checks the signal after each click attempt.
//       Turn_ON retries up to 10 times, Turn_OFF retries up to 15 times.
// =============================================================================

/*
 * Turn_ON_OWD()
 * -------------
 * Enables Continuous Driving Recording (OWD) via panel LED toggle.
 * Retries up to 10 times until BLTN_CAM_RecSet_OWD == 0x2 (ON).
 *
 * Keywords: OWD on, enable driving recording, continuous driving, LED toggle,
 *           BLTN_CAM_RecSet_OWD, panel LED_Ctrl_SetOWD
 * Panel variable: @panel::LED_Ctrl_SetOWD = 1 (toggle click)
 * Signal checked: BLTN_CAM_RecSet_OWD == 0x2 (ON)
 * Retry: up to 10 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_ON_OWD()
{
  int retry = 0;
  write("=== Turn ON OWD via panel ===");

  while ($BLTN_CAM_RecSet_OWD != 0x2 && retry < 10)
  {
    @panel::LED_Ctrl_SetOWD = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_OWD=0x%X", retry + 1, $BLTN_CAM_RecSet_OWD);
    retry++;
  }

  if ($BLTN_CAM_RecSet_OWD == 0x2)
    write(">>> SUCCESS: OWD = ON (0x2) <<<");
  else
    write(">>> FAILED: Could not set OWD to ON <<<");
}

/*
 * Turn_ON_OWP()
 * -------------
 * Enables Continuous Parking Recording (OWP) via panel LED toggle.
 * Retries up to 10 times until BLTN_CAM_RecSet_OWP == 0x2 (ON).
 *
 * Keywords: OWP on, enable parking recording, continuous parking, LED toggle,
 *           BLTN_CAM_RecSet_OWP, panel LED_Ctrl_SetOWP
 * Panel variable: @panel::LED_Ctrl_SetOWP = 1
 * Signal checked: BLTN_CAM_RecSet_OWP == 0x2 (ON)
 * Retry: up to 10 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_ON_OWP()
{
  int retry = 0;
  write("=== Turn ON OWP via panel ===");

  while ($BLTN_CAM_RecSet_OWP != 0x2 && retry < 10)
  {
    @panel::LED_Ctrl_SetOWP = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_OWP=0x%X", retry + 1, $BLTN_CAM_RecSet_OWP);
    retry++;
  }

  if ($BLTN_CAM_RecSet_OWP == 0x2)
    write(">>> SUCCESS: OWP = ON (0x2) <<<");
  else
    write(">>> FAILED: Could not set OWP to ON <<<");
}

/*
 * Turn_ON_DEV()
 * -------------
 * Enables Driving Event Recording (DEV) via panel LED toggle.
 * Retries up to 10 times until BLTN_CAM_RecSet_DEV == 0x2 (ON).
 *
 * Keywords: DEV on, enable driving event, event recording, impact detection,
 *           BLTN_CAM_RecSet_DEV, panel LED_Ctrl_SetDEV
 * Panel variable: @panel::LED_Ctrl_SetDEV = 1
 * Signal checked: BLTN_CAM_RecSet_DEV == 0x2 (ON)
 * Retry: up to 10 attempts, 3000ms between each (DEV needs longer)
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_ON_DEV()
{
  int retry = 0;
  write("=== Turn ON DEV via panel ===");

  while ($BLTN_CAM_RecSet_DEV != 0x2 && retry < 10)
  {
    @panel::LED_Ctrl_SetDEV = 1;
    testWaitForTimeout(3000);
    write("  Attempt %d: RecSet_DEV=0x%X", retry + 1, $BLTN_CAM_RecSet_DEV);
    retry++;
  }

  if ($BLTN_CAM_RecSet_DEV == 0x2)
    write(">>> SUCCESS: DEV = ON (0x2) <<<");
  else
    write(">>> FAILED: Could not set DEV to ON <<<");
}

/*
 * Turn_ON_PEV()
 * -------------
 * Enables Parking Event Recording (PEV) via panel LED toggle.
 * Retries up to 10 times until BLTN_CAM_RecSet_PEV == 0x2 (ON).
 *
 * Keywords: PEV on, enable parking event, parking impact, event recording,
 *           BLTN_CAM_RecSet_PEV, panel LED_Ctrl_SetPEV
 * Panel variable: @panel::LED_Ctrl_SetPEV = 1
 * Signal checked: BLTN_CAM_RecSet_PEV == 0x2 (ON)
 * Retry: up to 10 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_ON_PEV()
{
  int retry = 0;
  write("=== Turn ON PEV via panel ===");

  while ($BLTN_CAM_RecSet_PEV != 0x2 && retry < 10)
  {
    @panel::LED_Ctrl_SetPEV = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_PEV=0x%X", retry + 1, $BLTN_CAM_RecSet_PEV);
    retry++;
  }

  if ($BLTN_CAM_RecSet_PEV == 0x2)
    write(">>> SUCCESS: PEV = ON (0x2) <<<");
  else
    write(">>> FAILED: Could not set PEV to ON <<<");
}

/*
 * Turn_OFF_OWD()
 * --------------
 * Disables Continuous Driving Recording (OWD) via panel LED toggle.
 * Retries up to 15 times while BLTN_CAM_RecSet_OWD is still 0x2 (ON).
 *
 * Keywords: OWD off, disable driving recording, turn off continuous driving,
 *           BLTN_CAM_RecSet_OWD, panel LED_Ctrl_SetOWD
 * Panel variable: @panel::LED_Ctrl_SetOWD = 1 (toggle click)
 * Signal checked: BLTN_CAM_RecSet_OWD == 0x1 (OFF)
 * Retry: up to 15 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_OFF_OWD()
{
  int retry = 0;
  write("=== Turn OFF OWD via panel ===");

  while ($BLTN_CAM_RecSet_OWD == 0x2 && retry < 15)
  {
    @panel::LED_Ctrl_SetOWD = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_OWD=0x%X", retry + 1, $BLTN_CAM_RecSet_OWD);
    retry++;
  }

  if ($BLTN_CAM_RecSet_OWD == 0x1)
    write(">>> SUCCESS: OWD = OFF (0x1) <<<");
  else
    write(">>> FAILED: Could not set OWD to OFF <<<");
}

/*
 * Turn_OFF_OWP()
 * --------------
 * Disables Continuous Parking Recording (OWP) via panel LED toggle.
 * Retries up to 15 times while BLTN_CAM_RecSet_OWP is still 0x2 (ON).
 *
 * Keywords: OWP off, disable parking recording, turn off continuous parking,
 *           BLTN_CAM_RecSet_OWP, panel LED_Ctrl_SetOWP
 * Panel variable: @panel::LED_Ctrl_SetOWP = 1
 * Signal checked: BLTN_CAM_RecSet_OWP == 0x1 (OFF)
 * Retry: up to 15 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_OFF_OWP()
{
  int retry = 0;
  write("=== Turn OFF OWP via panel ===");

  while ($BLTN_CAM_RecSet_OWP == 0x2 && retry < 15)
  {
    @panel::LED_Ctrl_SetOWP = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_OWP=0x%X", retry + 1, $BLTN_CAM_RecSet_OWP);
    retry++;
  }

  if ($BLTN_CAM_RecSet_OWP == 0x1)
    write(">>> SUCCESS: OWP = OFF (0x1) <<<");
  else
    write(">>> FAILED: Could not set OWP to OFF <<<");
}

/*
 * Turn_OFF_DEV()
 * --------------
 * Disables Driving Event Recording (DEV) via panel LED toggle.
 * Retries up to 15 times while BLTN_CAM_RecSet_DEV is still 0x2 (ON).
 *
 * Keywords: DEV off, disable driving event, turn off event recording,
 *           BLTN_CAM_RecSet_DEV, panel LED_Ctrl_SetDEV
 * Panel variable: @panel::LED_Ctrl_SetDEV = 1
 * Signal checked: BLTN_CAM_RecSet_DEV == 0x1 (OFF)
 * Retry: up to 15 attempts, 3000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_OFF_DEV()
{
  int retry = 0;
  write("=== Turn OFF DEV via panel ===");

  while ($BLTN_CAM_RecSet_DEV == 0x2 && retry < 15)
  {
    @panel::LED_Ctrl_SetDEV = 1;
    testWaitForTimeout(3000);
    write("  Attempt %d: RecSet_DEV=0x%X", retry + 1, $BLTN_CAM_RecSet_DEV);
    retry++;
  }

  if ($BLTN_CAM_RecSet_DEV == 0x1)
    write(">>> SUCCESS: DEV = OFF (0x1) <<<");
  else
    write(">>> FAILED: Could not set DEV to OFF <<<");
}

/*
 * Turn_OFF_PEV()
 * --------------
 * Disables Parking Event Recording (PEV) via panel LED toggle.
 * Retries up to 15 times while BLTN_CAM_RecSet_PEV is still 0x2 (ON).
 *
 * Keywords: PEV off, disable parking event, turn off parking event recording,
 *           BLTN_CAM_RecSet_PEV, panel LED_Ctrl_SetPEV
 * Panel variable: @panel::LED_Ctrl_SetPEV = 1
 * Signal checked: BLTN_CAM_RecSet_PEV == 0x1 (OFF)
 * Retry: up to 15 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_OFF_PEV()
{
  int retry = 0;
  write("=== Turn OFF PEV via panel ===");

  while ($BLTN_CAM_RecSet_PEV == 0x2 && retry < 15)
  {
    @panel::LED_Ctrl_SetPEV = 1;
    testWaitForTimeout(2000);
    write("  Attempt %d: RecSet_PEV=0x%X", retry + 1, $BLTN_CAM_RecSet_PEV);
    retry++;
  }

  if ($BLTN_CAM_RecSet_PEV == 0x1)
    write(">>> SUCCESS: PEV = OFF (0x1) <<<");
  else
    write(">>> FAILED: Could not set PEV to OFF <<<");
}

/*
 * Turn_ON_PMD()
 * -------------
 * Enables Parking Motion Detection (PMD) via panel LED toggle.
 * PMD detects motion near the parked vehicle and starts recording.
 * Retries up to 10 times until BLTN_CAM_RecSet_PMD == 0x2 (ON).
 *
 * Keywords: PMD on, enable motion detection, parking motion, motion sensor,
 *           BLTN_CAM_RecSet_PMD, panel LED_Ctrl_SetPMD
 * Panel variable: @panel::LED_Ctrl_SetPMD = 1
 * Signal checked: BLTN_CAM_RecSet_PMD == 0x2 (ON)
 * Retry: up to 10 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_ON_PMD()
{
  write("=== Turn ON PMD via panel ===");
  write(">>> [GEN2.0 UNSUPPORTED] PMD panel/signal is not present in the active DBC <<<");
}

/*
 * Turn_OFF_PMD()
 * --------------
 * Disables Parking Motion Detection (PMD) via panel LED toggle.
 * Retries up to 15 times while BLTN_CAM_RecSet_PMD is still 0x2 (ON).
 *
 * Keywords: PMD off, disable motion detection, turn off parking motion,
 *           BLTN_CAM_RecSet_PMD, panel LED_Ctrl_SetPMD
 * Panel variable: @panel::LED_Ctrl_SetPMD = 1
 * Signal checked: BLTN_CAM_RecSet_PMD == 0x1 (OFF)
 * Retry: up to 15 attempts, 2000ms between each
 * Return: void (logs SUCCESS/FAILED)
 */
void Turn_OFF_PMD()
{
  write("=== Turn OFF PMD via panel ===");
  write(">>> [GEN2.0 UNSUPPORTED] PMD panel/signal is not present in the active DBC <<<");
}

// =============================================================================
// SECTION 5: RECORDING STATUS CHECK (Instant Verification)
// =============================================================================
//
// Purpose: Instantly read and verify recording-related CAN signals.
//          No retry or polling - just reads the current signal value once.
//          Used for test step verification (PASS/FAIL reporting).
// Keywords: check, verify, instant check, recording status, signal read,
//           RecSet, RecSta, PASS, FAIL, test verification, assertion
//
// Two signal categories:
//
//   RecSet (Recording Setting) - whether the recording function is configured:
//     BLTN_CAM_RecSet_OWD / OWP / DEV / PEV / PMD
//     Values: 0x1 = OFF (disabled), 0x2 = ON (enabled)
//     Functions: Check_RecSet_XXX_ON() / Check_RecSet_XXX_OFF()
//
//   RecSta (Recording Status) - whether recording is actually active:
//     BLTN_CAM_RecSta_OWD / OWP / DEV / TML / MAR / PEV / PMD
//     Values: 0x0 = OFF (not recording), 0x1 = Recording (actively recording)
//     Functions: Check_RecSta_XXX_Recording() / Check_RecSta_XXX_OFF()
//
// Return: 1 = PASS (signal matches expected), 0 = FAIL (mismatch)
// =============================================================================

// ---------------------------------------------------------------------------
// RecSet checks: verify recording SETTING status (0x1=OFF, 0x2=ON)
// Keywords: RecSet, recording setting, configuration check, ON/OFF setting
// ---------------------------------------------------------------------------

/*
 * Check_RecSet_OWD_ON() / Check_RecSet_OWD_OFF()
 * Check if OWD (Continuous Driving Recording) setting is ON (0x2) or OFF (0x1).
 * Signal: BLTN_CAM_RecSet_OWD
 * Keywords: OWD setting, driving recording setting, RecSet OWD
 */
int Check_RecSet_OWD_ON()
{
  byte val = $BLTN_CAM_RecSet_OWD;
  write("  BLTN_CAM_RecSet_OWD = 0x%X", val);
  if (val == 0x2)
  {
    write(">>> PASS: RecSet_OWD = ON <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_OWD is NOT ON <<<");
    return 0;
  }
}

int Check_RecSet_OWD_OFF()
{
  byte val = $BLTN_CAM_RecSet_OWD;
  write("  BLTN_CAM_RecSet_OWD = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSet_OWD = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_OWD is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSet_OWP_ON() / Check_RecSet_OWP_OFF()
 * Check if OWP (Continuous Parking Recording) setting is ON (0x2) or OFF (0x1).
 * Signal: BLTN_CAM_RecSet_OWP
 * Keywords: OWP setting, parking recording setting, RecSet OWP
 */
int Check_RecSet_OWP_ON()
{
  byte val = $BLTN_CAM_RecSet_OWP;
  write("  BLTN_CAM_RecSet_OWP = 0x%X", val);
  if (val == 0x2)
  {
    write(">>> PASS: RecSet_OWP = ON <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_OWP is NOT ON <<<");
    return 0;
  }
}

int Check_RecSet_OWP_OFF()
{
  byte val = $BLTN_CAM_RecSet_OWP;
  write("  BLTN_CAM_RecSet_OWP = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSet_OWP = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_OWP is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSet_DEV_ON() / Check_RecSet_DEV_OFF()
 * Check if DEV (Driving Event Recording) setting is ON (0x2) or OFF (0x1).
 * Signal: BLTN_CAM_RecSet_DEV
 * Keywords: DEV setting, driving event setting, RecSet DEV
 */
int Check_RecSet_DEV_ON()
{
  byte val = $BLTN_CAM_RecSet_DEV;
  write("  BLTN_CAM_RecSet_DEV = 0x%X", val);
  if (val == 0x2)
  {
    write(">>> PASS: RecSet_DEV = ON <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_DEV is NOT ON <<<");
    return 0;
  }
}

int Check_RecSet_DEV_OFF()
{
  byte val = $BLTN_CAM_RecSet_DEV;
  write("  BLTN_CAM_RecSet_DEV = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSet_DEV = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_DEV is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSet_PEV_ON() / Check_RecSet_PEV_OFF()
 * Check if PEV (Parking Event Recording) setting is ON (0x2) or OFF (0x1).
 * Signal: BLTN_CAM_RecSet_PEV
 * Keywords: PEV setting, parking event setting, RecSet PEV
 */
int Check_RecSet_PEV_ON()
{
  byte val = $BLTN_CAM_RecSet_PEV;
  write("  BLTN_CAM_RecSet_PEV = 0x%X", val);
  if (val == 0x2)
  {
    write(">>> PASS: RecSet_PEV = ON <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_PEV is NOT ON <<<");
    return 0;
  }
}

int Check_RecSet_PEV_OFF()
{
  byte val = $BLTN_CAM_RecSet_PEV;
  write("  BLTN_CAM_RecSet_PEV = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSet_PEV = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSet_PEV is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSet_PMD_ON() / Check_RecSet_PMD_OFF()
 * Check if PMD (Parking Motion Detection) setting is ON (0x2) or OFF (0x1).
 * Signal: BLTN_CAM_RecSet_PMD
 * Keywords: PMD setting, motion detection setting, RecSet PMD
 */
int Check_RecSet_PMD_ON()
{
  write("  [GEN2.0 UNSUPPORTED] BLTN_CAM_RecSet_PMD is not present in the active DBC");
  return 0;
}

int Check_RecSet_PMD_OFF()
{
  write("  [GEN2.0 UNSUPPORTED] BLTN_CAM_RecSet_PMD is not present in the active DBC");
  return 0;
}

// ---------------------------------------------------------------------------
// RecSta checks: verify recording ACTIVE status (0x0=OFF, 0x1=Recording)
// Keywords: RecSta, recording status, active recording, is recording,
//           recording in progress, actual recording state
// ---------------------------------------------------------------------------

/*
 * Check_RecSta_OWD_Recording() / Check_RecSta_OWD_OFF()
 * Check if OWD is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_OWD
 * Keywords: OWD status, driving recording active, OWD RecSta
 */
int Check_RecSta_OWD_Recording()
{
  byte val = $BLTN_CAM_RecSta_OWD;
  write("  BLTN_CAM_RecSta_OWD = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_OWD = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_OWD is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_OWD_OFF()
{
  byte val = $BLTN_CAM_RecSta_OWD;
  write("  BLTN_CAM_RecSta_OWD = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_OWD = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_OWD is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_OWP_Recording() / Check_RecSta_OWP_OFF()
 * Check if OWP is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_OWP
 * Keywords: OWP status, parking recording active, OWP RecSta
 */
int Check_RecSta_OWP_Recording()
{
  byte val = $BLTN_CAM_RecSta_OWP;
  write("  BLTN_CAM_RecSta_OWP = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_OWP = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_OWP is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_OWP_OFF()
{
  byte val = $BLTN_CAM_RecSta_OWP;
  write("  BLTN_CAM_RecSta_OWP = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_OWP = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_OWP is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_DEV_Recording() / Check_RecSta_DEV_OFF()
 * Check if DEV is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_DEV
 * Keywords: DEV status, driving event recording active, DEV RecSta
 */
int Check_RecSta_DEV_Recording()
{
  byte val = $BLTN_CAM_RecSta_DEV;
  write("  BLTN_CAM_RecSta_DEV = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_DEV = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_DEV is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_DEV_OFF()
{
  byte val = $BLTN_CAM_RecSta_DEV;
  write("  BLTN_CAM_RecSta_DEV = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_DEV = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_DEV is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_TML_Recording() / Check_RecSta_TML_OFF()
 * Check if TML (Time-lapse) is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_TML
 * Keywords: TML status, timelapse recording active, TML RecSta
 */
int Check_RecSta_TML_Recording()
{
  byte val = $BLTN_CAM_RecSta_TML;
  write("  BLTN_CAM_RecSta_TML = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_TML = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_TML is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_TML_OFF()
{
  byte val = $BLTN_CAM_RecSta_TML;
  write("  BLTN_CAM_RecSta_TML = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_TML = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_TML is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_MAR_Recording() / Check_RecSta_MAR_OFF()
 * Check if MAR (Manual Recording) is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_MAR
 * Keywords: MAR status, manual recording active, MAR RecSta, manual event
 */
int Check_RecSta_MAR_Recording()
{
  byte val = $BLTN_CAM_RecSta_MAR;
  write("  BLTN_CAM_RecSta_MAR = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_MAR = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_MAR is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_MAR_OFF()
{
  byte val = $BLTN_CAM_RecSta_MAR;
  write("  BLTN_CAM_RecSta_MAR = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_MAR = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_MAR is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_PEV_Recording() / Check_RecSta_PEV_OFF()
 * Check if PEV (Parking Event) is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_PEV
 * Keywords: PEV status, parking event recording active, PEV RecSta
 */
int Check_RecSta_PEV_Recording()
{
  byte val = $BLTN_CAM_RecSta_PEV;
  write("  BLTN_CAM_RecSta_PEV = 0x%X", val);
  if (val == 0x1)
  {
    write(">>> PASS: RecSta_PEV = Recording <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_PEV is NOT Recording <<<");
    return 0;
  }
}

int Check_RecSta_PEV_OFF()
{
  byte val = $BLTN_CAM_RecSta_PEV;
  write("  BLTN_CAM_RecSta_PEV = 0x%X", val);
  if (val == 0x0)
  {
    write(">>> PASS: RecSta_PEV = OFF <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RecSta_PEV is NOT OFF <<<");
    return 0;
  }
}

/*
 * Check_RecSta_PMD_Recording() / Check_RecSta_PMD_OFF()
 * Check if PMD (Parking Motion Detection) is actively recording (0x1) or stopped (0x0).
 * Signal: BLTN_CAM_RecSta_PMD
 * Keywords: PMD status, motion detection recording active, PMD RecSta
 */
int Check_RecSta_PMD_Recording()
{
  write("  [GEN2.0 UNSUPPORTED] BLTN_CAM_RecSta_PMD is not present in the active DBC");
  return 0;
}

int Check_RecSta_PMD_OFF()
{
  write("  [GEN2.0 UNSUPPORTED] BLTN_CAM_RecSta_PMD is not present in the active DBC");
  return 0;
}

/*
 * Check_All_Recording_Signals()
 * -----------------------------
 * Checks all 4 main recording status signals at once:
 *   OWD, TML, DEV, MAR - all must be 0x1 (Recording) to PASS.
 * Used for tests that enable multiple recordings simultaneously.
 *
 * Keywords: check all recordings, bulk check, all RecSta, multi-signal check,
 *           OWD TML DEV MAR, all recording active, combined verification
 * Signals checked: RecSta_OWD, RecSta_TML, RecSta_DEV, RecSta_MAR
 * Return: 1 = ALL are Recording, 0 = at least one is NOT Recording
 */
int Check_All_Recording_Signals()
{
  int result = 1;
  byte rec_owd, rec_tml, rec_dev, rec_mar;

  write("=== Checking all 4 recording signals ===");

  rec_owd = $BLTN_CAM_RecSta_OWD;
  rec_tml = $BLTN_CAM_RecSta_TML;
  rec_dev = $BLTN_CAM_RecSta_DEV;
  rec_mar = $BLTN_CAM_RecSta_MAR;

  write("  BLTN_CAM_RecSta_OWD = 0x%X", rec_owd);
  write("  BLTN_CAM_RecSta_TML = 0x%X", rec_tml);
  write("  BLTN_CAM_RecSta_DEV = 0x%X", rec_dev);
  write("  BLTN_CAM_RecSta_MAR = 0x%X", rec_mar);

  if (rec_owd != 0x1)
  {
    write(">>> FAIL: RecSta_OWD is NOT Recording <<<");
    result = 0;
  }
  if (rec_tml != 0x1)
  {
    write(">>> FAIL: RecSta_TML is NOT Recording <<<");
    result = 0;
  }
  if (rec_dev != 0x1)
  {
    write(">>> FAIL: RecSta_DEV is NOT Recording <<<");
    result = 0;
  }
  if (rec_mar != 0x1)
  {
    write(">>> FAIL: RecSta_MAR is NOT Recording <<<");
    result = 0;
  }

  if (result == 1)
    write(">>> PASS: All 4 recording signals = 0x1 (Recording) <<<");

  return result;
}

// =============================================================================
// SECTION 6: RECORDING STATUS WAIT (Polling with Timeout)
// =============================================================================
//
// Purpose: Wait for a specific recording to start (RecSta = 0x1) with a
//          configurable timeout. Uses polling loop with periodic status logging.
//          Unlike Section 5 (instant check), these functions keep trying until
//          timeout expires.
// Keywords: wait for recording, polling, timeout, RecSta, recording start,
//           wait until recording, long wait, recording detection, elapsed time
//
// All functions follow the same pattern:
//   - Poll the RecSta signal every 2 seconds
//   - Log current status at ~1/6 of total timeout interval
//   - Return 1 immediately when recording detected
//   - Return 0 if timeout reached without detection
//
// Parameter: timeout_ms - maximum wait time in milliseconds
// Return: 1 = recording detected within timeout, 0 = timeout (recording NOT detected)
// =============================================================================

/*
 * WaitFor_RecSta_OWD_Recording(long timeout_ms)
 * Wait for OWD (Continuous Driving) recording to start.
 * Signal: BLTN_CAM_RecSta_OWD == 0x1
 * Keywords: wait OWD, OWD recording start, driving recording wait
 */
int WaitFor_RecSta_OWD_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for OWD Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_OWD == 0x1)
    {
      write(">>> DETECTED: OWD Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_OWD=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_OWD);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: OWD Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_OWP_Recording(long timeout_ms)
 * Wait for OWP (Continuous Parking) recording to start.
 * Signal: BLTN_CAM_RecSta_OWP == 0x1
 * Keywords: wait OWP, OWP recording start, parking recording wait
 */
int WaitFor_RecSta_OWP_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for OWP Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_OWP == 0x1)
    {
      write(">>> DETECTED: OWP Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_OWP=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_OWP);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: OWP Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_DEV_Recording(long timeout_ms)
 * Wait for DEV (Driving Event) recording to start.
 * Signal: BLTN_CAM_RecSta_DEV == 0x1
 * Keywords: wait DEV, DEV recording start, driving event recording wait
 */
int WaitFor_RecSta_DEV_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for DEV Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_DEV == 0x1)
    {
      write(">>> DETECTED: DEV Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_DEV=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_DEV);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: DEV Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_MAR_Recording(long timeout_ms)
 * Wait for MAR (Manual Recording) recording to start.
 * Signal: BLTN_CAM_RecSta_MAR == 0x1
 * Keywords: wait MAR, MAR recording start, manual recording wait, manual event wait
 */
int WaitFor_RecSta_MAR_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for MAR Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_MAR == 0x1)
    {
      write(">>> DETECTED: MAR Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_MAR=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_MAR);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: MAR Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_TML_Recording(long timeout_ms)
 * Wait for TML (Time-lapse) recording to start.
 * Signal: BLTN_CAM_RecSta_TML == 0x1
 * Keywords: wait TML, TML recording start, timelapse recording wait
 */
int WaitFor_RecSta_TML_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for TML Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_TML == 0x1)
    {
      write(">>> DETECTED: TML Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_TML=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_TML);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: TML Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_PEV_Recording(long timeout_ms)
 * Wait for PEV (Parking Event) recording to start.
 * Signal: BLTN_CAM_RecSta_PEV == 0x1
 * Keywords: wait PEV, PEV recording start, parking event recording wait
 */
int WaitFor_RecSta_PEV_Recording(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;

  write("=== Waiting for PEV Recording (max %d seconds) ===", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 2000) * 2000;
  if (log_interval < 2000)
    log_interval = 2000;

  while (elapsed < timeout_ms)
  {
    if ($BLTN_CAM_RecSta_PEV == 0x1)
    {
      write(">>> DETECTED: PEV Recording ON after %d seconds <<<", elapsed / 1000);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] RecSta_PEV=0x%X", elapsed / 1000, $BLTN_CAM_RecSta_PEV);

    testWaitForTimeout(2000);
    elapsed += 2000;
  }
  write(">>> TIMEOUT: PEV Recording NOT detected <<<");
  return 0;
}

/*
 * WaitFor_RecSta_PMD_Recording(long timeout_ms)
 * Wait for PMD (Parking Motion Detection) recording to start.
 * Signal: BLTN_CAM_RecSta_PMD == 0x1
 * Keywords: wait PMD, PMD recording start, motion detection recording wait
 */
int WaitFor_RecSta_PMD_Recording(long timeout_ms)
{
  write("=== Waiting for PMD Recording skipped: Gen 2.0 active DBC has no BLTN_CAM_RecSta_PMD ===");
  return 0;
}

// =============================================================================
// SECTION 7: FACTORY / CUSTOMER MODE CONTROL (ICU PowerAutoCut)
// =============================================================================
//
// Purpose: Control the ICU (Instrument Cluster Unit) PowerAutoCut mode.
//          This determines the camera ECU's power management behavior:
//          - Factory Mode: camera stays powered for testing (no auto-cutoff)
//          - Dealer Mode: intermediate setting for dealer diagnostics
//          - Customer Mode: normal operation with power auto-cutoff to save battery
// Keywords: factory mode, customer mode, dealer mode, ICU, PowerAutoCut,
//           power management, battery protection, auto power off,
//           ICU_PowerAutoCutModSta, mode switch, production line, end of line
//
// Signal: ICU_PowerAutoCutModSta
//   0x0 = Factory Mode  (no auto power cut - for production testing)
//   0x1 = Dealer Mode   (for dealer service/diagnostics)
//   0x2 = Customer Mode (normal use - auto power cut enabled for battery protection)
//
// Typical test flow:
//   Set_Factory_Mode() -> [run test without power cut interference] -> Set_Customer_Mode()
// =============================================================================

/*
 * Set_PowerAutoCut_Mode(byte target_mode, long timeout_ms)
 * --------------------------------------------------------
 * Sets ICU_PowerAutoCutModSta to the specified mode with retry polling.
 * Keeps trying to set the signal and checks for confirmation within timeout.
 * Re-sends the setSignal command at each log interval to ensure delivery.
 *
 * Keywords: set power mode, PowerAutoCut, ICU mode, retry, confirmation,
 *           factory mode set, customer mode set, dealer mode set
 * Signal: ICU_PowerAutoCutModSta
 * Parameters:
 *   target_mode - 0x0 (Factory), 0x1 (Dealer), or 0x2 (Customer)
 *   timeout_ms  - maximum wait time for mode confirmation
 * Return: 1 = mode set successfully, 0 = timeout
 */
int Set_PowerAutoCut_Mode(byte target_mode, long timeout_ms)
{
  long elapsed = 0;
  long log_interval;
  byte current_mode;
  char mode_name[50];

  if (target_mode == 0)
    snprintf(mode_name, elcount(mode_name), "%s", "Factory Mode");
  else if (target_mode == 1)
    snprintf(mode_name, elcount(mode_name), "%s", "Dealer Mode");
  else if (target_mode == 2)
    snprintf(mode_name, elcount(mode_name), "%s", "Customer Mode");
  else
    snprintf(mode_name, elcount(mode_name), "%s", "Unknown Mode");

  write("Setting PowerAutoCutMode to 0x%X (%s)...", target_mode, mode_name);
  setSignal(ICU_PowerAutoCutModSta, target_mode);
  testWaitForTimeout(500);

  log_interval = (timeout_ms / 6 / 1000) * 1000;
  if (log_interval < 1000)
    log_interval = 1000;

  while (elapsed < timeout_ms)
  {
    current_mode = $ICU_PowerAutoCutModSta;
    if (current_mode == target_mode)
    {
      write(">>> SUCCESS: PowerAutoCutMode = 0x%X (%s) <<<", current_mode, mode_name);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
    {
      write("  [%d s] PowerAutoCutMode=0x%X (waiting for 0x%X)", elapsed / 1000, current_mode, target_mode);
      setSignal(ICU_PowerAutoCutModSta, target_mode);
    }
    testWaitForTimeout(1000);
    elapsed += 1000;
  }

  write(">>> TIMEOUT: Could not set PowerAutoCutMode to 0x%X <<<", target_mode);
  return 0;
}

/*
 * Verify_PowerAutoCut_Mode(byte expected_mode)
 * ---------------------------------------------
 * Instantly verifies that ICU_PowerAutoCutModSta matches the expected mode.
 * No retry - just reads and compares once.
 *
 * Keywords: verify mode, check power mode, PowerAutoCut verify, mode check,
 *           instant verification, factory mode check, customer mode check
 * Signal read: ICU_PowerAutoCutModSta
 * Parameter: expected_mode - 0x0 (Factory), 0x1 (Dealer), or 0x2 (Customer)
 * Return: 1 = PASS (matches), 0 = FAIL (mismatch)
 */
int Verify_PowerAutoCut_Mode(byte expected_mode)
{
  byte current_mode;
  char mode_name[50];

  current_mode = $ICU_PowerAutoCutModSta;

  if (expected_mode == 0)
    snprintf(mode_name, elcount(mode_name), "%s", "Factory Mode");
  else if (expected_mode == 1)
    snprintf(mode_name, elcount(mode_name), "%s", "Dealer Mode");
  else if (expected_mode == 2)
    snprintf(mode_name, elcount(mode_name), "%s", "Customer Mode");
  else
    snprintf(mode_name, elcount(mode_name), "%s", "Unknown Mode");

  write("Verifying PowerAutoCutMode: Current=0x%X, Expected=0x%X (%s)", current_mode, expected_mode, mode_name);

  if (current_mode == expected_mode)
  {
    write(">>> VERIFIED: PowerAutoCutMode = 0x%X (%s) <<<", current_mode, mode_name);
    return 1;
  }
  else
  {
    write(">>> FAILED: PowerAutoCutMode = 0x%X, Expected 0x%X <<<", current_mode, expected_mode);
    return 0;
  }
}

/*
 * Set_Factory_Mode()
 * ------------------
 * Shortcut to set ICU to Factory Mode (0x0) with 30-second timeout.
 * Factory mode disables auto power cutoff, keeping camera powered
 * indefinitely for test automation.
 *
 * Keywords: factory mode, production test, no power cut, keep alive,
 *           test mode, ICU factory, disable auto power off
 * Calls: Set_PowerAutoCut_Mode(0x0, 30000)
 * Return: 1 = success, 0 = timeout
 */
int Set_Factory_Mode()
{
  return Set_PowerAutoCut_Mode(0x0, 30000);
}

/*
 * Set_Customer_Mode()
 * -------------------
 * Shortcut to set ICU to Customer Mode (0x2) with 30-second timeout.
 * Customer mode enables auto power cutoff to protect vehicle battery
 * (normal end-user operation).
 *
 * Keywords: customer mode, normal mode, enable power cut, battery protection,
 *           end user mode, ICU customer, auto power off enabled
 * Calls: Set_PowerAutoCut_Mode(0x2, 30000)
 * Return: 1 = success, 0 = timeout
 */
int Set_Customer_Mode()
{
  return Set_PowerAutoCut_Mode(0x2, 30000);
}

// =============================================================================
// SECTION 8: FRS RESET DETECTION (SD Card Format / Recording Data Reset)
// =============================================================================
//
// Purpose: Monitor and trigger SD card formatting / recording data reset.
//          Used to clear all recorded data before starting a clean test.
// Keywords: FRS reset, SD card format, recording reset, data clear,
//           CF_AVN_BLTN_CAM_RecReset, BLTN_CAM_SD_Format_Op_State,
//           format SD card, clear recordings, storage reset, clean state
//
// Signals:
//   CF_AVN_BLTN_CAM_RecReset    : 0x1 = Record Data Reset triggered
//   BLTN_CAM_SD_Format_Op_State : 0x1 = SD card formatting in progress
// =============================================================================

/*
 * Wait_For_FRS_Reset(long timeout_ms)
 * ------------------------------------
 * Monitors CF_AVN_BLTN_CAM_RecReset signal and waits for it to become 0x1
 * (indicating recording data reset has been triggered).
 * Polls every 100ms for precise reset detection.
 *
 * Keywords: wait for reset, FRS reset detection, recording data reset,
 *           CF_AVN_BLTN_CAM_RecReset, reset monitoring, SD format wait
 * Signal checked: CF_AVN_BLTN_CAM_RecReset == 0x1
 * Parameter: timeout_ms - maximum wait time in milliseconds
 * Return: 1 = reset detected, 0 = timeout
 */
int Wait_For_FRS_Reset(long timeout_ms)
{
  long elapsed = 0;
  long log_interval;
  byte reset_signal = 0;

  write("Monitoring CF_AVN_BLTN_CAM_RecReset for reset detection...");
  write("Waiting for value = 0x1 (Record Data Reset) (max %d seconds)", timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 100) * 100;
  if (log_interval < 100)
    log_interval = 100;

  while (elapsed < timeout_ms)
  {
    reset_signal = $CF_AVN_BLTN_CAM_RecReset;

    if (reset_signal == 0x1)
    {
      write(">>> Reset detected! CF_AVN_BLTN_CAM_RecReset = 0x1 <<<");
      return 1;
    }

    if (elapsed == 0 || elapsed % log_interval == 0)
    {
      write("  [%d s] CF_AVN_BLTN_CAM_RecReset=0x%X",
            elapsed / 1000, reset_signal);
    }

    testWaitForTimeout(100);
    elapsed += 100;
  }

  write(">>> TIMEOUT: Reset not detected within %d seconds <<<", timeout_ms / 1000);
  return 0;
}

/*
 * Rec_Reset()
 * -----------
 * Performs SD card format by repeatedly clicking the RecReset button
 * until BLTN_CAM_SD_Format_Op_State becomes 0x1 (formatting started).
 * Uses infinite retry loop (no max limit) - will keep clicking until success.
 *
 * Keywords: SD card format, recording reset, format storage, Rec Reset click,
 *           BLTN_CAM_SD_Format_Op_State, force format, persistent reset
 * Panel variable: @panel::Button_RecReset = 1
 * Signal checked: BLTN_CAM_SD_Format_Op_State == 0x1
 * Return: void (completes when format operation begins)
 */
void Rec_Reset()
{
  write("=== Performing Recording Reset (SD Format) ===");
  while (getSignal(BLTN_CAM_SD_Format_Op_State) != 0x1)
  {
    @panel::Button_RecReset = 1;
    testWaitForTimeout(500);
  }
  write(">>> Recording Reset complete (SD_Format_Op_State = 0x1) <<<");
}

// =============================================================================
// SECTION 9: AIN VOLTAGE (VN1600 Hardware Analog Input)
// =============================================================================
//
// Purpose: Read and check analog input voltage from VN1600 hardware interface.
//          Used to verify power supply levels to the camera ECU.
//          Can detect if the camera is powered (>threshold) or unpowered (<threshold).
// Keywords: AIN, analog input, voltage, VN1600, hardware, power supply,
//           voltage measurement, power check, threshold, battery voltage,
//           ECU power, voltage monitoring, hardware interface
//
// System variable: @sysvar::IO::VN1600_1::AIN (float, unit: Volts)
// =============================================================================

/*
 * Get_AIN_Voltage()
 * -----------------
 * Reads the current AIN (Analog Input) voltage from VN1600 hardware.
 * Returns the voltage as a float value in Volts.
 *
 * Keywords: read voltage, AIN voltage, VN1600 AIN, analog input read,
 *           power supply voltage, measure voltage
 * System variable read: @sysvar::IO::VN1600_1::AIN
 * Return: float - current voltage in Volts (e.g., 12.55)
 */
float Get_AIN_Voltage()
{
  float ain_value = 0.0;
  ain_value = @sysvar::IO::VN1600_1::AIN;
  write("  AIN Voltage: %.2f V", ain_value);
  return ain_value;
}

/*
 * Check_AIN_Below_Threshold(float threshold, long timeout_ms)
 * ------------------------------------------------------------
 * Waits for AIN voltage to drop below the specified threshold.
 * Used to verify that the camera ECU has been powered off (e.g., < 1.0V).
 * Polls every 1 second until voltage drops below threshold or timeout.
 *
 * Keywords: voltage below, power off check, low voltage, ECU off verification,
 *           AIN threshold, voltage drop, power cut check
 * Parameters:
 *   threshold  - voltage threshold in Volts (e.g., 1.0)
 *   timeout_ms - maximum wait time in milliseconds
 * Return: 1 = voltage below threshold (PASS), 0 = timeout (still above)
 */
int Check_AIN_Below_Threshold(float threshold, long timeout_ms)
{
  long elapsed = 0;
  long log_interval;
  float ain_voltage = 0.0;

  write("Checking AIN voltage below %.1f V... (max %d seconds)", threshold, timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 1000) * 1000;
  if (log_interval < 1000)
    log_interval = 1000;

  while (elapsed < timeout_ms)
  {
    ain_voltage = Get_AIN_Voltage();
    if (ain_voltage < threshold)
    {
      write(">>> PASS: AIN = %.2f V < %.1f V <<<", ain_voltage, threshold);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] AIN=%.2f V (waiting for < %.1f V)", elapsed / 1000, ain_voltage, threshold);

    testWaitForTimeout(1000);
    elapsed += 1000;
  }
  write(">>> TIMEOUT: AIN = %.2f V still >= %.1f V <<<", ain_voltage, threshold);
  return 0;
}

/*
 * Check_AIN_Above_Threshold(float threshold, long timeout_ms)
 * ------------------------------------------------------------
 * Waits for AIN voltage to rise above the specified threshold.
 * Used to verify that the camera ECU has been powered on (e.g., > 11.0V).
 * Polls every 1 second until voltage rises above threshold or timeout.
 *
 * Keywords: voltage above, power on check, high voltage, ECU on verification,
 *           AIN threshold, voltage rise, power supply check
 * Parameters:
 *   threshold  - voltage threshold in Volts (e.g., 11.0)
 *   timeout_ms - maximum wait time in milliseconds
 * Return: 1 = voltage above threshold (PASS), 0 = timeout (still below)
 */
int Check_AIN_Above_Threshold(float threshold, long timeout_ms)
{
  long elapsed = 0;
  long log_interval;
  float ain_voltage = 0.0;

  write("Checking AIN voltage above %.1f V... (max %d seconds)", threshold, timeout_ms / 1000);

  log_interval = (timeout_ms / 6 / 1000) * 1000;
  if (log_interval < 1000)
    log_interval = 1000;

  while (elapsed < timeout_ms)
  {
    ain_voltage = Get_AIN_Voltage();
    if (ain_voltage > threshold)
    {
      write(">>> PASS: AIN = %.2f V > %.1f V <<<", ain_voltage, threshold);
      return 1;
    }
    if (elapsed == 0 || elapsed % log_interval == 0)
      write("  [%d s] AIN=%.2f V (waiting for > %.1f V)", elapsed / 1000, ain_voltage, threshold);

    testWaitForTimeout(1000);
    elapsed += 1000;
  }
  write(">>> TIMEOUT: AIN = %.2f V still <= %.1f V <<<", ain_voltage, threshold);
  return 0;
}

// =============================================================================
// SECTION 10: SCREENSHOT / CAPTURE (CANoe Test Report)
// =============================================================================
//
// Purpose: Capture screenshots of the "OWP" CANoe panel window for test reports.
//          Adds the captured image with signal annotations to the test report.
// Keywords: screenshot, capture, test report, panel capture, OWP window,
//           testReportAddWindowCapture, evidence, visual verification
// =============================================================================

/*
 * CaptureGraphics(char signalList[])
 * -----------------------------------
 * Captures a screenshot of the "OWP" panel window and adds it to the
 * CANoe test report with the specified signal list annotation.
 * Waits 3 seconds before capture to ensure the panel is fully rendered.
 *
 * Keywords: screenshot, capture panel, test report image, OWP window capture,
 *           visual evidence, testReportAddWindowCapture, signal annotation
 * Parameter: signalList[] - comma-separated string of signal names to annotate
 *            in the report (e.g., "BLTN_CAM_RecSta_OWP,BLTN_CAM_RecSet_OWP")
 * Return: void
 */
void CaptureGraphics(char signalList[])
{
  testWaitForTimeout(3000);
  testReportAddWindowCapture("OWP", signalList, "Screenshot");
  write(">>> Screenshot captured <<<");
}

// =============================================================================
// SECTION 11: COUNTDOWN TIMER (Wait with Visual Countdown)
// =============================================================================
//
// Purpose: Wait for a specified duration while displaying a countdown in the
//          write window. Useful for visualizing long wait periods in test
//          execution (e.g., waiting for parking time to expire).
// Keywords: countdown, timer, wait, delay, clock, remaining time, display,
//           visual countdown, progress indicator, MM:SS format
// =============================================================================

/*
 * Clock_Down_ms(long CD_time)
 * ---------------------------
 * Countdown timer with millisecond input. Displays remaining time in MM:SS
 * format every second. Total wait = CD_time milliseconds.
 *
 * Keywords: countdown milliseconds, timer ms, wait with countdown,
 *           remaining time display, second-by-second countdown
 * Parameter: CD_time - total countdown time in milliseconds (e.g., 60000 for 1 min)
 * Return: void (blocks until countdown complete)
 */
void Clock_Down_ms(long CD_time)
{
  write("=== COUNT DOWN: %d seconds ===", CD_time / 1000);
  while (CD_time > 0)
  {
    write("  Remaining: %02d:%02d", CD_time / (1000 * 60), (CD_time / 1000) % 60);
    testWaitForTimeout(1000);
    CD_time -= 1000;
  }
  write("=== COUNT DOWN COMPLETE ===");
}

/*
 * Clock_Down_min(long CD_minutes)
 * --------------------------------
 * Countdown timer with minute input. Displays remaining time in MM:00
 * format once per minute. Total wait = CD_minutes * 60 seconds.
 * More efficient for long waits (hours) - less write window spam.
 *
 * Keywords: countdown minutes, timer minutes, long wait countdown,
 *           minute-by-minute countdown, hour countdown
 * Parameter: CD_minutes - total countdown time in minutes (e.g., 120 for 2 hours)
 * Return: void (blocks until countdown complete)
 */
void Clock_Down_min(long CD_minutes)
{
  write("=== COUNT DOWN: %d minutes ===", CD_minutes);
  while (CD_minutes > 0)
  {
    write("  Remaining: %02d:00", CD_minutes);
    testWaitForTimeout(60000);
    CD_minutes -= 1;
  }
  write("=== COUNT DOWN COMPLETE ===");
}

// =============================================================================
// SECTION 12: SIGNAL LOGGING (Debug / Status Dump)
// =============================================================================
//
// Purpose: Dump all recording-related signal values to the write window.
//          Useful for debugging, test evidence, and understanding the
//          current state of all camera recording signals at a glance.
// Keywords: signal logging, debug dump, status dump, print all signals,
//           recording status dump, signal snapshot, diagnostic output,
//           state inspection, current values
// =============================================================================

/*
 * Log_All_RecSignals()
 * --------------------
 * Logs the current values of ALL recording-related CAN signals to the
 * write window in a formatted table. Includes:
 *   - RecSet signals (OWD, OWP, DEV, PEV)
 *   - RecSta signals (OWD, OWP, DEV, MAR, TML, PEV)
 *   - Sensitivity settings (DEV, PEV)
 *   - Parking time, Camera state, UI mode
 *   - SMK terminal status, ICU PowerAutoCut mode
 *
 * Keywords: log all signals, dump recording signals, print status,
 *           signal snapshot, debug all, comprehensive log, full status
 * Signals read: BLTN_CAM_RecSet_*, BLTN_CAM_RecSta_*, SenSet_*, Set_PrkTime,
 *               BLTN_CAM_State, BLTN_CAM_HU_UI_Mode, SMK_TrmnlCtrlGrpStaBDC,
 *               ICU_PowerAutoCutModSta
 * Return: void
 */
void Log_All_RecSignals()
{
  write("");
  write("=== Recording Signal Status ===");
  write("  BLTN_CAM_RecSet_OWD  = 0x%X", $BLTN_CAM_RecSet_OWD);
  write("  BLTN_CAM_RecSet_OWP  = 0x%X", $BLTN_CAM_RecSet_OWP);
  write("  BLTN_CAM_RecSet_DEV  = 0x%X", $BLTN_CAM_RecSet_DEV);
  write("  BLTN_CAM_RecSet_PEV  = 0x%X", $BLTN_CAM_RecSet_PEV);
  write("  BLTN_CAM_RecSta_OWD  = 0x%X", $BLTN_CAM_RecSta_OWD);
  write("  BLTN_CAM_RecSta_OWP  = 0x%X", $BLTN_CAM_RecSta_OWP);
  write("  BLTN_CAM_RecSta_DEV  = 0x%X", $BLTN_CAM_RecSta_DEV);
  write("  BLTN_CAM_RecSta_MAR  = 0x%X", $BLTN_CAM_RecSta_MAR);
  write("  BLTN_CAM_RecSta_TML  = 0x%X", $BLTN_CAM_RecSta_TML);
  write("  BLTN_CAM_RecSta_PEV  = 0x%X", $BLTN_CAM_RecSta_PEV);
  write("  BLTN_CAM_SenSet_DEV  = 0x%X", $BLTN_CAM_SenSet_DEV);
  write("  BLTN_CAM_SenSet_PEV  = 0x%X", $BLTN_CAM_SenSet_PEV);
  write("  BLTN_CAM_Set_PrkTime = 0x%X", $BLTN_CAM_Set_PrkTime);
  write("  BLTN_CAM_State       = 0x%X", $BLTN_CAM_State);
  write("  BLTN_CAM_HU_UI_Mode  = 0x%X", $BLTN_CAM_HU_UI_Mode);
  write("  SMK_TrmnlCtrlGrpStaBDC = 0x%X", $SMK_TrmnlCtrlGrpStaBDC);
  write("  ICU_PowerAutoCutModSta = 0x%X", $ICU_PowerAutoCutModSta);
  write("========================================");
  write("");
}

// =============================================================================
// SECTION 13: DRIVING INFO SIMULATION (Vehicle Speed / GPS Simulation)
// =============================================================================
//
// Purpose: Enable or disable driving information simulation via panel sysvar.
//          When enabled, the system simulates vehicle driving data (speed, GPS, etc.)
//          which triggers the camera ECU to enter "driving" mode.
//          Required for testing OWD (driving recording) and DEV (driving event).
// Keywords: driving info, driving simulation, vehicle speed, GPS simulation,
//           EnableDrvInfo, panel sysvar, driving mode, vehicle motion,
//           simulate driving, simulate parking, driving state
// =============================================================================

/*
 * Enable_DrivingInfo()
 * --------------------
 * Enables driving info simulation (vehicle appears to be driving).
 * Camera ECU will treat this as vehicle-in-motion state.
 *
 * Keywords: enable driving, start driving simulation, vehicle moving,
 *           driving mode on, EnableDrvInfo on
 * System variable: @sysvar::panel::EnableDrvInfo = 1
 * Wait: 3000ms for state to take effect
 * Return: void
 */
void Enable_DrivingInfo()
{
  write("=== Enabling Driving Info Simulation ===");
  @sysvar::panel::EnableDrvInfo = 1;
  testWaitForTimeout(3000);
  write("  EnableDrvInfo = ON");
}

/*
 * Disable_DrivingInfo()
 * ---------------------
 * Disables driving info simulation (vehicle appears to be parked/stationary).
 * Camera ECU will treat this as vehicle-parked state.
 *
 * Keywords: disable driving, stop driving simulation, vehicle parked,
 *           driving mode off, EnableDrvInfo off, simulate parked
 * System variable: @sysvar::panel::EnableDrvInfo = 0
 * Wait: 3000ms for state to take effect
 * Return: void
 */
void Disable_DrivingInfo()
{
  write("=== Disabling Driving Info Simulation ===");
  @sysvar::panel::EnableDrvInfo = 0;
  testWaitForTimeout(3000);
  write("  EnableDrvInfo = OFF");
}

// =============================================================================
// SECTION 14: COMMON INIT SEQUENCE (Standard Test Setup / Teardown)
// =============================================================================
//
// Purpose: Provide standard initialization and cleanup procedures that most
//          test cases share. Reduces boilerplate code in individual test files.
// Keywords: initialization, init, setup, cleanup, teardown, test preparation,
//           standard init, test start, test end, CGW NM, SMK, USM Reset,
//           Rec Reset, boilerplate, common setup
//
// Standard_Init flow:
//   1. Enable CGW Network Management (wake up CAN bus)
//   2. Set SMK to IGN (ignition on)
//   3. Click USM Reset (reset storage)
//   4. Click Rec Reset (format SD card)
// =============================================================================

/*
 * Standard_Init()
 * ---------------
 * Performs standard test initialization sequence:
 *   1. @panel::Switch_CGWNM = 1 (enable CGW NM for CAN communication)
 *   2. Set_SMK_IGN() (set ignition to ON)
 *   3. Click_USM_Reset() (reset update storage memory)
 *   4. Click_Rec_Reset() (format SD card / reset recordings)
 *
 * Call this at the beginning of each test case for a clean starting state.
 *
 * Keywords: standard init, test initialization, setup procedure, test start,
 *           CGW NM enable, ignition on, USM reset, SD format, clean state
 * Return: void
 */
void Standard_Init()
{
  write("=== Standard Test Initialization ===");
  @panel::Switch_CGWNM = 1;
  testWaitForTimeout(3000);

  Set_SMK_IGN();
  Click_USM_Reset();
  Click_Rec_Reset();

  write("=== Initialization Complete ===");
}

/*
 * Standard_Cleanup()
 * ------------------
 * Performs standard test cleanup:
 *   1. @panel::Switch_CGWNM = 0 (disable CGW NM, release CAN bus)
 *
 * Call this at the end of each test case.
 *
 * Keywords: standard cleanup, test teardown, test end, CGW NM disable,
 *           release bus, finalize test
 * Return: void
 */
void Standard_Cleanup()
{
  write("=== Standard Test Cleanup ===");
  @panel::Switch_CGWNM = 0;
  write("=== Cleanup Complete ===");
}

// =============================================================================
// SECTION 15: BLTN_CAM SETTINGS (Panel TrackBar Controls)
// =============================================================================
//
// Purpose: Configure camera recording parameters through CANoe panel trackbar
//          (slider) controls. Each setting has a Check function (instant verify)
//          and a Set function (set value + internal check + retry up to 15 times).
// Keywords: trackbar, slider, panel settings, camera configuration,
//           sensitivity, camera angle, parking time, event time, timelapse,
//           DEV sensitivity, PEV sensitivity, PMD range, camera vertical position,
//           front camera, rear camera, OWP parking time, event before time,
//           event after time, TML interval, Set with check, retry logic
//
// Pattern for all Set_* functions in this section:
//   1. Validate input range
//   2. Set the trackbar value via @sysvar::panel::TrackBar_XXX
//   3. Wait 3000ms for CAN feedback
//   4. Call Check_XXX() to verify
//   5. If check fails, retry up to 15 times (re-set trackbar each time)
//   6. Final confirmation check, return 1 (PASS) or 0 (FAIL)
//
// All Check_* functions: read CAN feedback signal, compare to expected, return 1/0
// All Set_* functions: set + check + retry, return 1 (success) or 0 (failed after retries)
// =============================================================================

// ---------------------------------------------------------------------------
// DEV Sensitivity (Driving Event Sensitivity)
// ---------------------------------------------------------------------------
// Controls sensitivity level for driving event (impact) detection.
// Higher level = more sensitive (triggers on smaller impacts).
//
// TrackBar: @sysvar::panel::TrackBar_DEV_Sen
// Range: 1~5 (Lv.1=least sensitive ~ Lv.5=most sensitive)
// Feedback signal: BLTN_CAM_SenSet_DEV (0x1~0x5, direct mapping)
// Keywords: DEV sensitivity, driving event sensitivity, impact sensitivity,
//           G-sensor sensitivity, collision detection level, BLTN_CAM_SenSet_DEV
// ---------------------------------------------------------------------------

/*
 * Check_DEV_Sensitivity(int expected_level)
 * Instantly checks if DEV sensitivity matches expected level (1~5).
 * Signal: BLTN_CAM_SenSet_DEV
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_DEV_Sensitivity(int expected_level)
{
  byte val = $BLTN_CAM_SenSet_DEV;
  write("  BLTN_CAM_SenSet_DEV = 0x%X", val);
  if (val == expected_level)
  {
    write(">>> PASS: DEV Sensitivity = Lv.%d <<<", expected_level);
    return 1;
  }
  else
  {
    write(">>> FAIL: DEV Sensitivity = 0x%X, expected Lv.%d <<<", val, expected_level);
    return 0;
  }
}

/*
 * Set_DEV_Sensitivity(int level)
 * Sets DEV sensitivity to specified level (1~5) with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_DEV_Sen
 * Signal: BLTN_CAM_SenSet_DEV
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_DEV_Sensitivity(int level)
{
  int retry = 0;
  write("=== Setting DEV Sensitivity to Lv.%d ===", level);
  if (level < 1 || level > 5)
  {
    write(">>> ERROR: Invalid level %d (must be 1~5) <<<", level);
    return 0;
  }
  @sysvar::panel::TrackBar_DEV_Sen = level;
  testWaitForTimeout(3000);

  while (Check_DEV_Sensitivity(level) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting DEV Sensitivity...", retry + 1);
    @sysvar::panel::TrackBar_DEV_Sen = level;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_SenSet_DEV == level)
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set DEV Sensitivity after %d retries <<<", retry);
    return 0;
  }
}

// ---------------------------------------------------------------------------
// PEV Sensitivity (Parking Event Sensitivity)
// ---------------------------------------------------------------------------
// Controls sensitivity level for parking event (impact) detection.
// Higher level = more sensitive (triggers on smaller impacts while parked).
//
// TrackBar: @sysvar::panel::TrackBar_PEV_Sen
// Range: 1~5 (Lv.1=least sensitive ~ Lv.5=most sensitive)
// Feedback signal: BLTN_CAM_SenSet_PEV (0x1~0x5, direct mapping)
// Keywords: PEV sensitivity, parking event sensitivity, parking impact,
//           parking G-sensor, parking collision level, BLTN_CAM_SenSet_PEV
// ---------------------------------------------------------------------------

/*
 * Check_PEV_Sensitivity(int expected_level)
 * Instantly checks if PEV sensitivity matches expected level (1~5).
 * Signal: BLTN_CAM_SenSet_PEV
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_PEV_Sensitivity(int expected_level)
{
  byte val = $BLTN_CAM_SenSet_PEV;
  write("  BLTN_CAM_SenSet_PEV = 0x%X", val);
  if (val == expected_level)
  {
    write(">>> PASS: PEV Sensitivity = Lv.%d <<<", expected_level);
    return 1;
  }
  else
  {
    write(">>> FAIL: PEV Sensitivity = 0x%X, expected Lv.%d <<<", val, expected_level);
    return 0;
  }
}

/*
 * Set_PEV_Sensitivity(int level)
 * Sets PEV sensitivity to specified level (1~5) with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_PEV_Sen
 * Signal: BLTN_CAM_SenSet_PEV
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_PEV_Sensitivity(int level)
{
  int retry = 0;
  write("=== Setting PEV Sensitivity to Lv.%d ===", level);
  if (level < 1 || level > 5)
  {
    write(">>> ERROR: Invalid level %d (must be 1~5) <<<", level);
    return 0;
  }
  @sysvar::panel::TrackBar_PEV_Sen = level;
  testWaitForTimeout(3000);

  while (Check_PEV_Sensitivity(level) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting PEV Sensitivity...", retry + 1);
    @sysvar::panel::TrackBar_PEV_Sen = level;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_SenSet_PEV == level)
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set PEV Sensitivity after %d retries <<<", retry);
    return 0;
  }
}

// ---------------------------------------------------------------------------
// PMD Range (Parking Motion Detection Range)
// ---------------------------------------------------------------------------
// Controls the detection range for parking motion detection.
// Higher level = wider detection range.
//
// TrackBar: @sysvar::panel::TrackBar_PMD_RanSet
// Range: 1~5 (Lv.1=shortest range ~ Lv.5=widest range)
// Feedback signal: BLTN_CAM_PMD_RanSet (0x1~0x5, direct mapping)
// Keywords: PMD range, motion detection range, parking motion range,
//           detection distance, sensor range, BLTN_CAM_PMD_RanSet
// ---------------------------------------------------------------------------

/*
 * Check_PMD_Range(int expected_level)
 * Instantly checks if PMD range matches expected level (1~5).
 * Signal: BLTN_CAM_PMD_RanSet
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_PMD_Range(int expected_level)
{
  write("  [GEN2.0 UNSUPPORTED] BLTN_CAM_PMD_RanSet is not present in the active DBC; expected Lv.%d", expected_level);
  return 0;
}

/*
 * Set_PMD_Range(int level)
 * Sets PMD detection range to specified level (1~5) with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_PMD_RanSet
 * Signal: BLTN_CAM_PMD_RanSet
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_PMD_Range(int level)
{
  write("=== Setting PMD Range to Lv.%d ===", level);
  if (level < 1 || level > 5)
  {
    write(">>> ERROR: Invalid level %d (must be 1~5) <<<", level);
    return 0;
  }
  write(">>> [GEN2.0 UNSUPPORTED] PMD range control is not present in the active DBC <<<");
  return 0;
}

// ---------------------------------------------------------------------------
// Front Camera Vertical Position (FR Camera Angle Adjustment)
// ---------------------------------------------------------------------------
// Adjusts the vertical viewing angle of the front camera.
// TrackBar range 1~11 maps to angle offset -5 to +5 degrees.
//
// TrackBar: @sysvar::panel::TrackBar_FR_Ver_Pos
// Range: 1~11
//   Trackbar  1  2  3  4  5  6  7  8  9 10 11
//   Angle    -5 -4 -3 -2 -1  0 +1 +2 +3 +4 +5
//   CAN    0x15 0x14 0x13 0x12 0x11 0x0 0x1 0x2 0x3 0x4 0x5
//
// Feedback signal: BLTN_CAM_Set_FR_Ver_Pos (CAN enum value)
// Keywords: front camera, vertical position, camera angle, FR camera,
//           tilt angle, viewing angle, BLTN_CAM_Set_FR_Ver_Pos, FR_Ver_Pos
//
// Note: Uses FR_RR_Trackbar_To_CAN() helper for trackbar-to-CAN conversion.
//       Same conversion formula shared with Rear Camera (RR).
// ---------------------------------------------------------------------------

/*
 * FR_RR_Trackbar_To_CAN(int trackbar_val)
 * ----------------------------------------
 * Helper function: converts trackbar value (1~11) to CAN enum value.
 * Used by both Front Camera and Rear Camera position functions.
 *
 * Mapping logic:
 *   trackbar 1~5  -> negative angles: CAN = 0x16 - trackbar (0x15..0x11)
 *   trackbar 6    -> center (0 degrees): CAN = 0x0
 *   trackbar 7~11 -> positive angles: CAN = trackbar - 6 (0x1..0x5)
 *
 * Keywords: trackbar to CAN, angle conversion, camera position mapping,
 *           enum conversion, FR RR helper, vertical position mapping
 * Parameter: trackbar_val - trackbar position (1~11)
 * Return: CAN enum value, or -1 if invalid input
 */
int FR_RR_Trackbar_To_CAN(int trackbar_val)
{
  // 1->0x15(-5), 2->0x14(-4), ..., 5->0x11(-1), 6->0x0(0), 7->0x1(+1), ..., 11->0x5(+5)
  if (trackbar_val >= 1 && trackbar_val <= 5)
    return 0x16 - trackbar_val;
  else if (trackbar_val == 6)
    return 0x0;
  else if (trackbar_val >= 7 && trackbar_val <= 11)
    return trackbar_val - 6;
  return -1;
}

/*
 * Check_FR_Cam_VerPos(int expected_trackbar_val)
 * Checks if front camera vertical position matches expected trackbar value.
 * Converts trackbar to CAN value for comparison.
 * Signal: BLTN_CAM_Set_FR_Ver_Pos
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_FR_Cam_VerPos(int expected_trackbar_val)
{
  int expected_can = FR_RR_Trackbar_To_CAN(expected_trackbar_val);
  byte val = $BLTN_CAM_Set_FR_Ver_Pos;
  write("  BLTN_CAM_Set_FR_Ver_Pos = 0x%X (expected CAN=0x%X, trackbar=%d)", val, expected_can, expected_trackbar_val);
  if (val == expected_can)
  {
    write(">>> PASS: FR Camera VerPos matched <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: FR Camera VerPos mismatch <<<");
    return 0;
  }
}

/*
 * Set_FR_Cam_VerPos(int trackbar_val)
 * Sets front camera vertical position with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_FR_Ver_Pos
 * Signal: BLTN_CAM_Set_FR_Ver_Pos
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_FR_Cam_VerPos(int trackbar_val)
{
  int retry = 0;
  write("=== Setting Front Camera Vertical Position = %d ===", trackbar_val);
  if (trackbar_val < 1 || trackbar_val > 11)
  {
    write(">>> ERROR: Invalid value %d (must be 1~11) <<<", trackbar_val);
    return 0;
  }
  @sysvar::panel::TrackBar_FR_Ver_Pos = trackbar_val;
  testWaitForTimeout(3000);

  while (Check_FR_Cam_VerPos(trackbar_val) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting FR Camera VerPos...", retry + 1);
    @sysvar::panel::TrackBar_FR_Ver_Pos = trackbar_val;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_Set_FR_Ver_Pos == FR_RR_Trackbar_To_CAN(trackbar_val))
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set FR Camera VerPos after %d retries <<<", retry);
    return 0;
  }
}

// ---------------------------------------------------------------------------
// Rear Camera Vertical Position (RR Camera Angle Adjustment)
// ---------------------------------------------------------------------------
// Adjusts the vertical viewing angle of the rear camera.
// Same trackbar-to-CAN mapping as Front Camera (uses FR_RR_Trackbar_To_CAN).
//
// TrackBar: @sysvar::panel::TrackBar_RR_Ver_Pos
// Range: 1~11 (same mapping as Front Camera above)
// Feedback signal: BLTN_CAM_Set_RR_Ver_Pos (CAN enum value)
// Keywords: rear camera, vertical position, camera angle, RR camera,
//           tilt angle, back camera, BLTN_CAM_Set_RR_Ver_Pos, RR_Ver_Pos
// ---------------------------------------------------------------------------

/*
 * Check_RR_Cam_VerPos(int expected_trackbar_val)
 * Checks if rear camera vertical position matches expected trackbar value.
 * Signal: BLTN_CAM_Set_RR_Ver_Pos
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_RR_Cam_VerPos(int expected_trackbar_val)
{
  int expected_can = FR_RR_Trackbar_To_CAN(expected_trackbar_val);
  byte val = $BLTN_CAM_Set_RR_Ver_Pos;
  write("  BLTN_CAM_Set_RR_Ver_Pos = 0x%X (expected CAN=0x%X, trackbar=%d)", val, expected_can, expected_trackbar_val);
  if (val == expected_can)
  {
    write(">>> PASS: RR Camera VerPos matched <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: RR Camera VerPos mismatch <<<");
    return 0;
  }
}

/*
 * Set_RR_Cam_VerPos(int trackbar_val)
 * Sets rear camera vertical position with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_RR_Ver_Pos
 * Signal: BLTN_CAM_Set_RR_Ver_Pos
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_RR_Cam_VerPos(int trackbar_val)
{
  int retry = 0;
  write("=== Setting Rear Camera Vertical Position = %d ===", trackbar_val);
  if (trackbar_val < 1 || trackbar_val > 11)
  {
    write(">>> ERROR: Invalid value %d (must be 1~11) <<<", trackbar_val);
    return 0;
  }
  @sysvar::panel::TrackBar_RR_Ver_Pos = trackbar_val;
  testWaitForTimeout(3000);

  while (Check_RR_Cam_VerPos(trackbar_val) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting RR Camera VerPos...", retry + 1);
    @sysvar::panel::TrackBar_RR_Ver_Pos = trackbar_val;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_Set_RR_Ver_Pos == FR_RR_Trackbar_To_CAN(trackbar_val))
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set RR Camera VerPos after %d retries <<<", retry);
    return 0;
  }
}

// ---------------------------------------------------------------------------
// OWP Parking Time (Continuous Parking Recording Duration)
// ---------------------------------------------------------------------------
// Sets how long the camera continues recording after the vehicle is parked
// (key OFF). After this time expires, OWP recording stops to save battery.
//
// TrackBar: @sysvar::panel::TrackBar_OWP_TIME
// Range: 1~20 (hours) - trackbar value = hours directly
// Feedback signal: BLTN_CAM_Set_PrkTime (0x1~0x14 = 1Hr~20Hr, direct mapping)
// Keywords: OWP parking time, parking duration, recording time, hours,
//           PrkTime, parking recording duration, battery time,
//           BLTN_CAM_Set_PrkTime, OWP time setting, how long to record
// ---------------------------------------------------------------------------

/*
 * Check_OWP_ParkingTime(int expected_hours)
 * Checks if OWP parking time matches expected hours (1~20).
 * Signal: BLTN_CAM_Set_PrkTime
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_OWP_ParkingTime(int expected_hours)
{
  byte val = $BLTN_CAM_Set_PrkTime;
  write("  BLTN_CAM_Set_PrkTime = 0x%X", val);
  if (val == expected_hours)
  {
    write(">>> PASS: PrkTime = 0x%X (%dHr) <<<", val, expected_hours);
    return 1;
  }
  else
  {
    write(">>> FAIL: PrkTime = 0x%X, expected 0x%X (%dHr) <<<", val, expected_hours, expected_hours);
    return 0;
  }
}

/*
 * Check_OWP_ParkingTime_InRange()
 * Checks if OWP parking time is in valid range 0x1~0x14 (1Hr~20Hr).
 * Does NOT check specific value, only validates range.
 * Signal: BLTN_CAM_Set_PrkTime
 * Keywords: parking time range check, valid range, PrkTime in range
 * Return: 1 = in range (PASS), 0 = out of range (FAIL)
 */
int Check_OWP_ParkingTime_InRange()
{
  byte val = $BLTN_CAM_Set_PrkTime;
  write("  BLTN_CAM_Set_PrkTime = 0x%X", val);
  if (val >= 0x1 && val <= 0x14)
  {
    write(">>> PASS: PrkTime = 0x%X in range 1~20Hr <<<", val);
    return 1;
  }
  else
  {
    write(">>> FAIL: PrkTime = 0x%X out of range <<<", val);
    return 0;
  }
}

/*
 * Set_OWP_ParkingTime(int hours)
 * Sets OWP parking time to specified hours (1~20) with internal check and retry.
 * TrackBar: @sysvar::panel::TrackBar_OWP_TIME
 * Signal: BLTN_CAM_Set_PrkTime
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_OWP_ParkingTime(int hours)
{
  int retry = 0;
  write("=== Setting OWP Parking Time = %d Hr ===", hours);
  if (hours < 1 || hours > 20)
  {
    write(">>> ERROR: Invalid value %d (must be 1~20) <<<", hours);
    return 0;
  }
  @sysvar::panel::TrackBar_OWP_TIME = hours;
  testWaitForTimeout(3000);

  while (Check_OWP_ParkingTime(hours) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting OWP Parking Time...", retry + 1);
    @sysvar::panel::TrackBar_OWP_TIME = hours;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_Set_PrkTime == hours)
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set OWP Parking Time after %d retries <<<", retry);
    return 0;
  }
}

// ---------------------------------------------------------------------------
// Event Before Time (Pre-event Buffer Duration)
// ---------------------------------------------------------------------------
// Controls how many seconds of video BEFORE the event are saved.
// When an event (impact) is detected, the camera saves a clip that includes
// footage from X seconds before the event occurred.
//
// TrackBar: @sysvar::panel::TrackBar_EV_Time_Bfr
// Range: 1~3 (INVERSE mapping - higher trackbar = shorter time!)
//   Trackbar 1 -> 20 seconds before event (CAN BfrTime_NvalueSet = 0xA)
//   Trackbar 2 -> 15 seconds before event (CAN BfrTime_NvalueSet = 0x5)
//   Trackbar 3 -> 10 seconds before event (CAN BfrTime_NvalueSet = 0x0)
//
// Feedback signal: BLTN_CAM_Set_EV_BfrTime
//   Actual seconds = BLTN_CAM_Set_EV_BfrTime + 10
//   (raw 0xA -> 10+10=20s, raw 0x5 -> 5+10=15s, raw 0x0 -> 0+10=10s)
//
// Keywords: event before time, pre-event buffer, before event seconds,
//           BfrTime, pre-recording, pre-event video, event clip before,
//           BLTN_CAM_Set_EV_BfrTime, EV_Time_Bfr
// ---------------------------------------------------------------------------

/*
 * Check_EV_BeforeTime(int expected_trackbar)
 * Checks if event before-time matches the expected trackbar setting.
 * Converts trackbar (1=20s, 2=15s, 3=10s) to seconds for comparison.
 * Signal: BLTN_CAM_Set_EV_BfrTime (actual = raw + 10)
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_EV_BeforeTime(int expected_trackbar)
{
  int expected_seconds;
  int actual_seconds;

  if (expected_trackbar == 1)
    expected_seconds = 20;
  else if (expected_trackbar == 2)
    expected_seconds = 15;
  else
    expected_seconds = 10;

  actual_seconds = $BLTN_CAM_Set_EV_BfrTime + 10;
  write("  EV BfrTime = %ds (expected %ds)", actual_seconds, expected_seconds);

  if (actual_seconds == expected_seconds)
  {
    write(">>> PASS: EV BfrTime = %ds <<<", actual_seconds);
    return 1;
  }
  else
  {
    write(">>> FAIL: EV BfrTime = %ds, expected %ds <<<", actual_seconds, expected_seconds);
    return 0;
  }
}

/*
 * Set_EV_BeforeTime(int trackbar_val)
 * Sets event before-time via trackbar (1=20s, 2=15s, 3=10s) with retry.
 * TrackBar: @sysvar::panel::TrackBar_EV_Time_Bfr
 * Signal: BLTN_CAM_Set_EV_BfrTime
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_EV_BeforeTime(int trackbar_val)
{
  int retry = 0;
  write("=== Setting Event Before Time (Trackbar=%d) ===", trackbar_val);
  if (trackbar_val < 1 || trackbar_val > 3)
  {
    write(">>> ERROR: Invalid value %d (must be 1~3) <<<", trackbar_val);
    return 0;
  }
  @sysvar::panel::TrackBar_EV_Time_Bfr = trackbar_val;
  testWaitForTimeout(3000);

  while (Check_EV_BeforeTime(trackbar_val) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting EV Before Time...", retry + 1);
    @sysvar::panel::TrackBar_EV_Time_Bfr = trackbar_val;
    testWaitForTimeout(3000);
    retry++;
  }

  {
    int expected_seconds;
    int actual_seconds;
    if (trackbar_val == 1)
      expected_seconds = 20;
    else if (trackbar_val == 2)
      expected_seconds = 15;
    else
      expected_seconds = 10;
    actual_seconds = $BLTN_CAM_Set_EV_BfrTime + 10;
    if (actual_seconds == expected_seconds)
    {
      return 1;
    }
    else
    {
      write(">>> FAILED: Could not set EV Before Time after %d retries <<<", retry);
      return 0;
    }
  }
}

// ---------------------------------------------------------------------------
// Event After Time (Post-event Buffer Duration)
// ---------------------------------------------------------------------------
// Controls how many seconds of video AFTER the event are saved.
// When an event (impact) is detected, the camera continues recording for
// X more seconds after the event.
//
// TrackBar: @sysvar::panel::TrackBar_EV_Time_Aft
// Values: 10, 15, or 20 (seconds - direct input, NOT trackbar index!)
//   10 seconds -> CAN AftTime_NvalueSet = 0x0
//   15 seconds -> CAN AftTime_NvalueSet = 0x5
//   20 seconds -> CAN AftTime_NvalueSet = 0xA
//
// Feedback signal: BLTN_CAM_Set_EV_AftTime
//   Actual seconds = BLTN_CAM_Set_EV_AftTime + 10
//   (raw 0x0 -> 0+10=10s, raw 0x5 -> 5+10=15s, raw 0xA -> 10+10=20s)
//
// Keywords: event after time, post-event buffer, after event seconds,
//           AftTime, post-recording, post-event video, event clip after,
//           BLTN_CAM_Set_EV_AftTime, EV_Time_Aft
// ---------------------------------------------------------------------------

/*
 * Check_EV_AfterTime(int expected_seconds)
 * Checks if event after-time matches expected seconds (10, 15, or 20).
 * Signal: BLTN_CAM_Set_EV_AftTime (actual = raw + 10)
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_EV_AfterTime(int expected_seconds)
{
  int actual_seconds;
  actual_seconds = $BLTN_CAM_Set_EV_AftTime + 10;
  write("  EV AftTime = %ds (expected %ds)", actual_seconds, expected_seconds);

  if (actual_seconds == expected_seconds)
  {
    write(">>> PASS: EV AftTime = %ds <<<", actual_seconds);
    return 1;
  }
  else
  {
    write(">>> FAIL: EV AftTime = %ds, expected %ds <<<", actual_seconds, expected_seconds);
    return 0;
  }
}

/*
 * Set_EV_AfterTime(int seconds)
 * Sets event after-time in seconds (10, 15, or 20) with retry.
 * TrackBar: @sysvar::panel::TrackBar_EV_Time_Aft
 * Signal: BLTN_CAM_Set_EV_AftTime
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_EV_AfterTime(int seconds)
{
  int retry = 0;
  write("=== Setting Event After Time = %ds ===", seconds);
  if (seconds != 10 && seconds != 15 && seconds != 20)
  {
    write(">>> ERROR: Invalid value %d (must be 10, 15, or 20) <<<", seconds);
    return 0;
  }
  @sysvar::panel::TrackBar_EV_Time_Aft = seconds;
  testWaitForTimeout(3000);

  while (Check_EV_AfterTime(seconds) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting EV After Time...", retry + 1);
    @sysvar::panel::TrackBar_EV_Time_Aft = seconds;
    testWaitForTimeout(3000);
    retry++;
  }

  {
    int actual_seconds;
    actual_seconds = $BLTN_CAM_Set_EV_AftTime + 10;
    if (actual_seconds == seconds)
    {
      return 1;
    }
    else
    {
      write(">>> FAILED: Could not set EV After Time after %d retries <<<", retry);
      return 0;
    }
  }
}

// ---------------------------------------------------------------------------
// Time-lapse Interval (TML Recording Interval)
// ---------------------------------------------------------------------------
// Controls the interval between time-lapse photo captures.
// Time-lapse mode takes periodic snapshots to create a sped-up video.
//
// TrackBar: @sysvar::panel::TrackBar_Timelaps_TIME
// Range: 1~3 (NON-LINEAR mapping!)
//   Trackbar 1 -> 20 minutes  (CAN TML_Int = 0x1)
//   Trackbar 2 -> 40 minutes  (CAN TML_Int = 0x2)
//   Trackbar 3 -> 100 minutes (CAN TML_Int = 0x5)
//
// Feedback signal: BLTN_CAM_Set_Drv_TML_Int (CAN enum value)
// Keywords: time-lapse interval, TML interval, timelapse time, capture interval,
//           snapshot interval, BLTN_CAM_Set_Drv_TML_Int, Timelaps_TIME,
//           20min 40min 100min
// ---------------------------------------------------------------------------

/*
 * Check_TML_Interval(int expected_trackbar)
 * Checks if TML interval matches expected trackbar (1=20min, 2=40min, 3=100min).
 * Converts trackbar to CAN value for comparison.
 * Signal: BLTN_CAM_Set_Drv_TML_Int
 * Return: 1 = PASS, 0 = FAIL
 */
int Check_TML_Interval(int expected_trackbar)
{
  int expected_can;
  byte val;

  if (expected_trackbar == 1)
    expected_can = 0x1;
  else if (expected_trackbar == 2)
    expected_can = 0x2;
  else
    expected_can = 0x5;

  val = $BLTN_CAM_Set_Drv_TML_Int;
  write("  BLTN_CAM_Set_Drv_TML_Int = 0x%X (expected 0x%X)", val, expected_can);

  if (val == expected_can)
  {
    write(">>> PASS: TML Interval matched <<<");
    return 1;
  }
  else
  {
    write(">>> FAIL: TML Interval mismatch <<<");
    return 0;
  }
}

/*
 * Set_TML_Interval(int trackbar_val)
 * Sets TML interval via trackbar (1=20min, 2=40min, 3=100min) with retry.
 * TrackBar: @sysvar::panel::TrackBar_Timelaps_TIME
 * Signal: BLTN_CAM_Set_Drv_TML_Int
 * Retry: up to 15 attempts
 * Return: 1 = success, 0 = failed after retries
 */
int Set_TML_Interval(int trackbar_val)
{
  int retry = 0;
  int expected_can;
  char desc[20];

  if (trackbar_val == 1)
  {
    snprintf(desc, 20, "%s", "20min");
    expected_can = 0x1;
  }
  else if (trackbar_val == 2)
  {
    snprintf(desc, 20, "%s", "40min");
    expected_can = 0x2;
  }
  else if (trackbar_val == 3)
  {
    snprintf(desc, 20, "%s", "100min");
    expected_can = 0x5;
  }
  else
  {
    write(">>> ERROR: Invalid value %d (must be 1, 2, or 3) <<<", trackbar_val);
    return 0;
  }

  write("=== Setting Time-lapse Interval = %s (Trackbar=%d) ===", desc, trackbar_val);
  @sysvar::panel::TrackBar_Timelaps_TIME = trackbar_val;
  testWaitForTimeout(3000);

  while (Check_TML_Interval(trackbar_val) == 0 && retry < 15)
  {
    write("  Retry %d/15: Re-setting TML Interval...", retry + 1);
    @sysvar::panel::TrackBar_Timelaps_TIME = trackbar_val;
    testWaitForTimeout(3000);
    retry++;
  }

  if ($BLTN_CAM_Set_Drv_TML_Int == expected_can)
  {
    return 1;
  }
  else
  {
    write(">>> FAILED: Could not set TML Interval after %d retries <<<", retry);
    return 0;
  }
}

// ===========================================================================
// SECTION 16: SIGNAL POLLING FUNCTIONS (Direct Signal Read)
// ===========================================================================
//
// Purpose: Wait for specific recording status signals to reach expected values
//          by directly reading the signal value ($signal) in a polling loop.
//          This avoids using testWaitForSignalChange() and instead checks the
//          actual signal value each iteration for more predictable behavior.
//
// All functions share the same pattern:
//   - Poll every 1 second (1000 ms)
//   - timeout_s parameter = maximum wait time in seconds
//   - Return 1 = signal reached expected value before timeout
//   - Return 0 = timeout expired, signal did NOT reach expected value
//
// Available functions:
//   WaitFor_RecSta_OWP_ON(timeout_s)  - BLTN_CAM_RecSta_OWP == 0x1
//   WaitFor_RecSta_MAR_ON(timeout_s)  - BLTN_CAM_RecSta_MAR == 0x1
//   WaitFor_RecSta_PEV_ON(timeout_s)  - BLTN_CAM_RecSta_PEV == 0x1
//   WaitFor_RecSta_PEV_OFF(timeout_s) - BLTN_CAM_RecSta_PEV == 0x0
//   WaitFor_OWP_ON_60s()              - wrapper: WaitFor_RecSta_OWP_ON(60)
// ===========================================================================

// ---------------------------------------------------------------------------
// WaitFor_RecSta_OWP_ON(int timeout_s)
// ---------------------------------------------------------------------------
// Polls BLTN_CAM_RecSta_OWP every 1 second until it equals 0x1 (ON).
// Used after KEY OFF to detect when OWP parking recording auto-starts.
//
// Signal: BLTN_CAM_RecSta_OWP (0x0 = OFF, 0x1 = ON/Recording)
// Parameter: timeout_s = max wait time in seconds
// Return: 1 = RecSta_OWP became 0x1 (ON), 0 = timeout
//
// Example:
//   KEY_OFF();
//   if (WaitFor_RecSta_OWP_ON(100) == 1) {
//       write("OWP recording started");
//   }
// ---------------------------------------------------------------------------
int WaitFor_RecSta_OWP_ON(int timeout_s)
{
  int i;
  write("  Waiting for BLTN_CAM_RecSta_OWP = 0x1 (ON) (max %ds)...", timeout_s);
  for (i = 0; i < timeout_s; i++)
  {
    if ($BLTN_CAM_RecSta_OWP == 0x1)
    {
      write(">>> [OK] BLTN_CAM_RecSta_OWP = 0x1 (ON) after %ds <<<", i);
      return 1;
    }
    testWaitForTimeout(1000);
  }
  write(">>> [NG] BLTN_CAM_RecSta_OWP = 0x%X after %ds <<<", $BLTN_CAM_RecSta_OWP, timeout_s);
  return 0;
}

// ---------------------------------------------------------------------------
// WaitFor_RecSta_MAR_ON(int timeout_s)
// ---------------------------------------------------------------------------
// Polls BLTN_CAM_RecSta_MAR every 1 second until it equals 0x1 (ON).
// Used after Click_Evt_Switch() to detect when Manual Event (MAR) recording
// starts automatically.
//
// Signal: BLTN_CAM_RecSta_MAR (0x0 = OFF, 0x1 = ON/Recording)
// Parameter: timeout_s = max wait time in seconds
// Return: 1 = RecSta_MAR became 0x1 (ON), 0 = timeout
//
// Example:
//   Click_Evt_Switch();
//   if (WaitFor_RecSta_MAR_ON(10) == 1) {
//       write("MAR recording started");
//   }
// ---------------------------------------------------------------------------
int WaitFor_RecSta_MAR_ON(int timeout_s)
{
  int i;
  write("  Waiting for BLTN_CAM_RecSta_MAR = 0x1 (ON) (max %ds)...", timeout_s);
  for (i = 0; i < timeout_s; i++)
  {
    if ($BLTN_CAM_RecSta_MAR == 0x1)
    {
      write(">>> [OK] BLTN_CAM_RecSta_MAR = 0x1 (ON) after %ds <<<", i);
      return 1;
    }
    testWaitForTimeout(1000);
  }
  write(">>> [NG] BLTN_CAM_RecSta_MAR = 0x%X after %ds <<<", $BLTN_CAM_RecSta_MAR, timeout_s);
  return 0;
}

// ---------------------------------------------------------------------------
// WaitFor_RecSta_PEV_ON(int timeout_s)
// ---------------------------------------------------------------------------
// Polls BLTN_CAM_RecSta_PEV every 1 second until it equals 0x1 (ON).
// Used after physical PEV trigger (bump/shake vehicle) or Click_Evt_Switch()
// to detect when Parking Event (PEV) recording starts.
//
// Signal: BLTN_CAM_RecSta_PEV (0x0 = OFF, 0x1 = ON/Recording)
// Parameter: timeout_s = max wait time in seconds
// Return: 1 = RecSta_PEV became 0x1 (ON), 0 = timeout
//
// Example:
//   // After physical PEV trigger
//   if (WaitFor_RecSta_PEV_ON(10) == 1) {
//       write("PEV recording started");
//   }
// ---------------------------------------------------------------------------
int WaitFor_RecSta_PEV_ON(int timeout_s)
{
  int i;
  write("  Waiting for BLTN_CAM_RecSta_PEV = 0x1 (ON) (max %ds)...", timeout_s);
  for (i = 0; i < timeout_s; i++)
  {
    if ($BLTN_CAM_RecSta_PEV == 0x1)
    {
      write(">>> [OK] BLTN_CAM_RecSta_PEV = 0x1 (ON) after %ds <<<", i);
      return 1;
    }
    testWaitForTimeout(1000);
  }
  write(">>> [NG] BLTN_CAM_RecSta_PEV = 0x%X after %ds <<<", $BLTN_CAM_RecSta_PEV, timeout_s);
  return 0;
}

// ---------------------------------------------------------------------------
// WaitFor_RecSta_PEV_OFF(int timeout_s)
// ---------------------------------------------------------------------------
// Polls BLTN_CAM_RecSta_PEV every 1 second until it equals 0x0 (OFF).
// Used after PEV recording starts to detect when PEV recording stops.
// This allows measuring the PEV recording duration.
//
// Signal: BLTN_CAM_RecSta_PEV (0x0 = OFF/Stopped, 0x1 = ON/Recording)
// Parameter: timeout_s = max wait time in seconds
// Return: 1 = RecSta_PEV became 0x0 (OFF), 0 = timeout
//
// Example:
//   // PEV already recording
//   if (WaitFor_RecSta_PEV_OFF(30) == 1) {
//       write("PEV recording stopped");
//   }
// ---------------------------------------------------------------------------
int WaitFor_RecSta_PEV_OFF(int timeout_s)
{
  int i;
  write("  Waiting for BLTN_CAM_RecSta_PEV = 0x0 (OFF) (max %ds)...", timeout_s);
  for (i = 0; i < timeout_s; i++)
  {
    if ($BLTN_CAM_RecSta_PEV == 0x0)
    {
      write(">>> [OK] BLTN_CAM_RecSta_PEV = 0x0 (OFF) after %ds <<<", i);
      return 1;
    }
    testWaitForTimeout(1000);
  }
  write(">>> [NG] BLTN_CAM_RecSta_PEV = 0x%X after %ds <<<", $BLTN_CAM_RecSta_PEV, timeout_s);
  return 0;
}

// ---------------------------------------------------------------------------
// WaitFor_OWP_ON_60s() - Wrapper function
// ---------------------------------------------------------------------------
// Convenience wrapper that calls WaitFor_RecSta_OWP_ON(60).
// Provides backward compatibility for test cases that use the fixed 60s timeout.
//
// Return: 1 = RecSta_OWP became 0x1 (ON) within 60s, 0 = timeout
// ---------------------------------------------------------------------------
int WaitFor_OWP_ON_60s()
{
  return WaitFor_RecSta_OWP_ON(60);
}
