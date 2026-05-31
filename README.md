# ⚓ Writing a Grading System for the Project: One Piece Arc Water 7 - Enies Lobby (Part 2)

Welcome to the automated grading framework and project workspace repository for the **Programming-fundamental-project-2** assignment! This document contains the full project specifications along with setup guides for the multi-platform automated evaluation engine.

---

## 📖 1. Read the Assignment Description
- The file **252_CO1027_Assignment_2_VI_V1.0.pdf** is your main guide.  
- 👉 Carefully read it to understand the requirements, tasks, and instructions before coding.

---

## 🧩 2. Understand `.h` and `.cpp` Files
- **`.h` files** ✍️: Used to **declare** classes, functions, and features.  
- **`.cpp` files** 💻: Where you actually **implement** the code declared in `.h`.

Example:
- `eniesLobby.h` → contains class and method declarations.  
- `eniesLobby.cpp` → contains the actual implementation of those methods.

⚠️ **NOTES**: 
- **Read carefully the requirement of each file, don't change any parameters as well as available libraries.**
- **Only submit `eniesLobby.h` and `eniesLobby.cpp`. Do NOT modify `main.h`.**
- **All sources of this assignment are protected until the deadline. (Plagiarism)**

---

## 🚀 3. Your Missions

### Task 0: Read Input Data
- Parse configuration file to initialize `BattleContext`, characters, and buildings.
- Handle `CONTEXT`, `STRAW_HAT`, `CP9`, `BUILDING` lines.

### Task 1: Implement Base Classes
- `Character` (abstract class) with all attributes and methods.
- `BattleContext` with turn counter and battle state variables.
- `Building` (abstract class) with durability system.

### Task 2: Implement Straw Hat Pirates
- `StrawHat` base class with bounty attribute.
- **Luffy** - Gear Second, damage scales with missing HP.
- **Zoro** - Bonus damage to low HP targets.
- **Sanji** - Speed-based damage, defense reduction.
- **Nami** - Armor penetration, weather control.
- **Chopper** - Healing abilities.
- **Usopp (Sogeking)** - Speed reduction, escape support.
- **Franky** - Heavy weapons, building destruction.

### Task 3: Implement CP9 Agents
- `CP9Agent` base class with doriki power level.
- **Lucci** - High burst damage with armor bypass.
- **Kaku** - Multi-hit combo attacks.
- **Jabra** - Bonus damage when low on HP.
- **Blueno** - Damage varies based on HP threshold.
- **Kalifa** - Morale reduction and speed debuff.
- **Kumadori** - Doriki-scaling damage.
- **Fukurou** - Extra damage to lowest HP target.

### Task 4: Implement Buildings
- `MainGate` - First barrier, unlocks rescue progress when destroyed.
- `Courthouse` - Increases alarm level each turn.
- `TowerOfJustice` - Advances rescue progress.
- `BridgeOfHesitation` - Enables escape route.
- `BusterCallShip` - Countdown timer to bad ending.

### Task 5: Implement Turn Order Management
- Build linked list (`TurnNode`) with Straw Hats first, then CP9 agents.
- Rotate list after each turn (move current node to end).

### Task 6: Implement Target Selection Logic
- **Straw Hats**: Priority order (MainGate → Courthouse → BusterCallShip → CP9 → BridgeOfHesitation)
- **Chopper special**: Heal lowest HP Straw Hat if energy available.
- **CP9**: Always attack first alive Straw Hat.
- **Fukurou special**: Track if targeting lowest HP enemy.

### Task 7: Implement Battle Flow
- `runBattle()` - Main battle loop until end condition or max turns.
- `processTurn()` - Execute attack/special skill based on energy.
- `processBuildings()` - Apply building effects after each turn.
- `checkEndCondition()` - Victory/defeat conditions.

### Task 8: Implement End Conditions (Priority Order)
| Priority | Condition | Result Code |
|----------|-----------|-------------|
| 1 | robinRescued && escapeProgress >= 100 | `STRAW_HAT_WIN` |
| 2 | busterCallTimer <= 0 | `BUSTER_CALL` |
| 3 | All Straw Hats defeated | `CP9_WIN` |
| 4 | All CP9 defeated | `STRAW_HAT_WIN_BY_DEFEAT_CP9` |
| 5 | turnCount >= maxTurns | `TIME_OUT` |

---

## 🛠️ 4. Automation Architecture & Scripts

To automate batch-testing across multiple student submissions, this grading framework orchestrates cross-platform compilation toolchains with an execution monitor script:

* **`runt.bat` (Windows Command Script):** Targets environments using the Microsoft Visual Studio compiler toolchain. It silently handles working directory routing and invokes CMake (`cmake --build build --config Debug`) to assemble source code cleanly into `build\Debug\eniesLobby_bin.exe`.
* **`runt.sh` (Linux/macOS Shell Script):** Handles the equivalent UNIX environment routing, executing local automated compilation routines via standard POSIX `make` or `ninja` utilities.
* **`cham_diem.py` (Python Core Engine):** The evaluation conductor. It scans the student folder, iteratively overwrites target templates, executes backend compiler triggers, manages file spoofing mechanisms, and formats final evaluations into a spreadsheet log.

### 🌟 Core Grading Engine Features
* **Dynamic File Spoofing Manipulation:** Since submissions use physical file readers (`ifstream infile("input_1.txt")`), the Python script overrides system limitations by dynamically drafting a spoofed temporary `input_1.txt` at the root directory before launching execution, erasing it smoothly afterward.
* **Tokenized Content Validation:** Strips away absolute string mismatches stemming from styling frames (`=== RUNNING BATTLE ===`), trailing newlines, or whitespace indents. Evaluation is token-based; if the ground truth block matches the parsed execution pattern anywhere within the data flow, a pass is recorded.
* **Colorized Terminal Layout Dashboard:** Integrates ANSI styling variables to cleanly structure grading logs, track processing compilation states dynamically, and issue explicit comparison feedback when checks fail.

---

## ⚙️ 5. How to Set Up and Run the Grader System

Follow these instructions to configure your local compilation environment and initialize batch grading:

### Step 1: Install Interface Dependencies
Ensure that your development terminal has Python available. Run the following command to install the required interactive coloring library:
```bash
pip install colorama
```
### Step 2: Run the Auto-Grader Sequence
* **Insert all students' submission to folder `./submit`
* **Open a command prompt or terminal shell at the project root directory and trigger the test sequence block:

```bash
python cham_diem.py
```
🖥️ Live Terminal Presentation Layout Example
Plaintext
╔══════════════════════════════════════════════════════════════╗
║              ENIES LOBBY AUTOMATED GRADING SYSTEM            ║
╚══════════════════════════════════════════════════════════════╝
[*] Found 2 submission(s) and 8 testcase(s).
[+] Starting core engine...

┌─── [1/2] ───────────────────────────────────────────────────┐
│ STUDENT ID: khoa                                             │
└─────────────────────────────────────────────────────────────┘
 🔨 Compilation Phase: SUCCESSFUL!                                   
 📑 Executing test pipeline:
   ├─ PASSED ── input_1.txt
   ├─ PASSED ── input_2.txt
   ├─ FAILED ── input_3.txt
   │   ├── [Expected Key]: STRAW_HAT_WIN_BY_DEFEAT_CP9 6 20 50 50
   │   └── [Student Out ] : TIME_OUT 0 0 0 0...
   ├─ PASSED ── input_4.txt
   ...
   └── 📊 FINAL METRICS: Passed 7/8 cases ──>  SCORE: 21.0 

🎉 GRADING ENGINE PIPELINE TERMINATED! Results exported to: grading_results.csv
Step 4: Review Grade Book Records
Once grading processes terminate completely, an aggregated data spreadsheet called grading_results.csv will be generated inside the project root folder, providing an optimized record of all student outcomes.