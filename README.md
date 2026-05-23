# ⚓ Programming Fundamental Project 2: ONE PIECE Arc Water 7 - Enies Lobby (Part 2)

Welcome to the **Programming-fundamental-project-2** repository!  
This repo contains necessary files and folders for this assignment. Follow to make most of it.

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

## ⚙️ 4. How to Build and Run the Program

This project uses **CMake** for configuration and building, and includes a **Python script** for automatic testcase grading with a clean terminal layout.

### Configuration & Compilation (via CMake)

Open your terminal at the root directory of the project and run the following commands:

```bash
# 1. Create a build directory and enter it
mkdir build && cd build

# 2. Configure the project using CMake
cmake ..

# 3. Compile the source code into an executable
cmake --build . --config Debug
### Terminal
### Running the Auto-Grader System

To test all `input_*.txt` files inside the `testcase/` directory and calculate your final score automatically:

1. Navigate back to the **root directory** of the project.
2. Execute the python script:

```bash
python run_test.py

==================================================
            ENIES LOBBY GRADING SYSTEM            
==================================================

👉 [1/3] Running: input_1.txt... [PASSED] (+3.0 pts)
👉 [2/3] Running: input_2.txt... [PASSED] (+3.0 pts)
👉 [3/3] Running: input_3.txt... [PASSED] (+3.0 pts)

==================================================
SUMMARY REPORT:
✅ Total Passed: 3/3
⭐ Total Score: 9.0 / 9.0 points
==================================================