# Codex Project Instructions - CANFD Built-in Cam

This repository is a Vector CANoe/CAPL project for Built-in Cam CAN FD tests.
The normal Codex task in this repo is to write or maintain automated test
cases under `Auto`.

## Read First

At the start of each session, read these files in order:

1. `STATUS.md` - current task state, last touched areas, active constraints.
2. `MEMORY.md` - long-term project preferences, decisions, context, patterns,
   and mistakes to avoid.
3. `Auto/AGENTS.md` - CAPL test coding rules for the writable test area.
4. `system_space.txt` - ECU/product behavior reference. Read only the relevant
   sections for the feature being tested.
5. `Auto/DeclarationFunction_Gen2.5.h` - primary helper API. Search this before
   writing any CAPL logic.
6. Canonical structure samples in `Auto/OWD` or `Auto/OWP`, especially when
   creating a new test shape.
7. Existing samples in `Auto/<module>` that match the target test family for
   feature-specific details.
8. Any module-local header used by the sample tests.

## Scope Boundaries

Default writable area:

- `Auto/**` for test-case implementation.
- `AGENTS.md`, `Auto/AGENTS.md`, `STATUS.md`, and `MEMORY.md` for agent
  continuity updates.

Default read/reference area:

- `Auto/**` for existing test files and helper headers.
- `CAPL/**` for read-only reference examples and simulation logic.
- `system_space.txt` for Built-in Cam ECU behavior.

Do not modify unless the user explicitly asks:

- `CAPL/**`
- `.vector/**`
- `.vscode/**`
- `Environment/**`
- `TestReports/**`
- `DB_CAN/**`, `DB_LIN/**`, `cdd/**`, `Panel/**`, `image/**`,
  `system variables/**`
- project configuration files such as `.cfg`, `.stcfg`, `.tse`,
  `.vtesettings`, `.yaml`
- generated/binary artifacts such as `.cbf`, `.dbg`, `.vtestreport`,
  `*-shm`, `*-wal`, `.zip`

There are many generated and local CANoe artifacts in this checkout. Do not
clean, revert, regenerate, or delete them as part of normal test coding.

## Coding Objective

Write CAPL automated test cases in `Auto` only. Test code should primarily use
the helper functions in `Auto/DeclarationFunction_Gen2.5.h`; if a module has a
local helper header, follow the include pattern used by nearby tests.

Treat `CAPL` files as examples to understand simulation behavior, signal
driving, timers, and panel interactions. They are not the implementation target.

## Header-First Rule

This is a hard rule: use the functions already available in
`Auto/DeclarationFunction_Gen2.5.h` whenever possible.

Before adding code for key state, reset, mode switching, recording settings,
recording status checks, waits, polling, screenshots, COM markers, timers,
panel buttons, burglar alarm, PMD, OWD, OWP, DEV, PEV, MAR, TML, network
management, power, or diagnostics, search the header first.

Do not duplicate helper logic or directly set signals when a suitable header
function exists. Direct signal writes and new local helpers are acceptable only
when:

- the header has no suitable function;
- a nearby OWD/OWP-style sample does it that way; or
- the user explicitly asks for low-level custom logic.

When a new local helper is necessary, keep it small and local to the `.can`
file unless the user asks to extend the shared header.

## CAPL Test Style

Use `Auto/OWD` and `Auto/OWP` as the canonical test structure. These folders
contain the preferred human-written style for this project. When the target
module is not OWD/OWP, still inspect the nearest `.can` sample in that module
for feature-specific signals and steps, but keep the overall structure close to
OWD/OWP.

Canonical OWD/OWP-style structure:

- `/*@!Encoding:65001*/`
- `includes { ... }`
- test-case description block with title, verification items, preconditions,
  and notes
- `variables { ... }`
- result-tracking variables such as `step1_passed`, `step2_passed`, or focused
  flags like `step1_tml_passed`
- optional `<ID>_Init()` testcase that prints a clear start banner and sends
  COM start/end markers
- one testcase per step: `<ID>_1()`, `<ID>_2()`, etc.
- each step starts with `Send_Value_To_COM(...)`, `SetTestStep(...)`, and
  `StartTestTimer()`
- step comment banner using `//====` blocks
- visible `write()` separator/banner logs before the step body
- numbered action logs such as `[1.1] ...` or `Step 1.1: ...`
- `testStep()`, `testStepPass()`, and `testStepFail()` for report output
- explicit helper calls, waits, actual signal logs, and `Log_Error_Details()`
  on failures where useful
- `CaptureGraphics(...)` for signal evidence
- each step ends with `StopTestTimer()` and `Send_Value_To_COM(44)`

Prefer existing helpers such as:

- key and power helpers: `KEY_ON()`, `KEY_OFF()`, `Set_SMK_IGN()`
- reset helpers: `Click_USM_Reset()`, `Click_Rec_Reset()`
- mode helpers: `Enter_BLTN_CAM()`, `Exit_BLTN_CAM()`
- recording helpers: `Turn_ON_OWD()`, `Turn_ON_OWP()`, `Turn_ON_PEV()`,
  `Turn_ON_PMD()`, and matching OFF/status/wait helpers
- wait/check helpers: `WaitFor_...`, `Check_...`, `Verify_...`
- utility helpers: `StartTestTimer()`, `StopTestTimer()`,
  `CaptureGraphics()`, `Send_Value_To_COM()`

If the helper header already has a function, use it instead of duplicating the
logic. Only edit shared helper headers when the user specifically asks or when a
small shared helper is clearly necessary for multiple tests.

## Debug Logging Rule

Every test step should log enough information to debug from CANoe output:

- announce the step start
- log each external action, signal set, wait, and expected value
- log actual signal values before pass/fail decisions
- include explicit fail reasons with expected and actual values

## Verification

When CANoe cannot be run from the terminal, verify by static inspection:

- syntax consistency with nearby `.can` files
- include path correctness
- helper function names exist in headers
- CAPL-compatible types and functions
- no accidental edits outside the allowed area

If a CANoe compile/run is available, prefer the smallest relevant test run.

## Continuity Rules

Use `STATUS.md` and `MEMORY.md` to avoid losing context between sessions:

- Update `STATUS.md` after meaningful work with current state, touched files,
  what was verified, blockers, and next steps.
- Add durable lessons to `MEMORY.md` under one of these categories:
  `preference`, `decision`, `context`, `pattern`, `mistake`.
- Keep memory entries short, dated, and factual.
- Do not store secrets, credentials, or machine-specific private data unless the
  user explicitly asks.
