import os
import shutil
import subprocess
import csv
import glob
import sys

# Try to import colorama for beautiful terminal styling
try:
    from colorama import init, Fore, Back, Style
    init(autoreset=True)
except ImportError:
    # Fallback class if colorama is not installed
    class Fore: color = "" ; RED = ""; GREEN = ""; YELLOW = ""; CYAN = ""; WHITE = ""; RESET = ""
    class Back: BLACK = ""; RED = ""; GREEN = ""
    class Style: BRIGHT = ""; RESET = ""

# =================================================================
# ⚙️ PATH CONFIGURATION
# =================================================================
SUBMIT_DIR = "submit"
TARGET_FILE = "eniesLobby_1.cpp" 
BAT_FILE = "runt.bat"             
EXE_PATH = r"build\Debug\eniesLobby_bin.exe" 
TESTCASE_DIR = "testcase"
CSV_OUTPUT = "grading_results.csv"
POINTS_PER_TEST = 3.0
FAKE_INPUT_FILE = "input_1.txt" 

def parse_testcase(file_path):
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    content_upper = content.upper()
    separator = None
    
    if "EXPECTED_OUTPUT:" in content_upper:
        idx = content_upper.find("EXPECTED_OUTPUT:")
        separator = content[idx:idx+16]
    elif "EXPECTED_OUTPUT" in content_upper:
        idx = content_upper.find("EXPECTED_OUTPUT")
        separator = content[idx:idx+15]
    elif "EXPECTED OUTPUT:" in content_upper:
        idx = content_upper.find("EXPECTED OUTPUT:")
        separator = content[idx:idx+16]
    elif "EXPECTED OUTPUT" in content_upper:
        idx = content_upper.find("EXPECTED OUTPUT")
        separator = content[idx:idx+15]

    if separator and separator in content:
        parts = content.split(separator)
        return parts[0].strip(), parts[1].strip()
        
    return content.strip(), ""

def print_header():
    os.system('cls' if os.name == 'nt' else 'clear')
    print(Fore.CYAN + Style.BRIGHT + "╔══════════════════════════════════════════════════════════════╗")
    print(Fore.CYAN + Style.BRIGHT + "║              ENIES LOBBY AUTOMATED GRADING SYSTEM            ║")
    print(Fore.CYAN + Style.BRIGHT + "╚══════════════════════════════════════════════════════════════╝")

def main():
    print_header()
    
    test_files = glob.glob(os.path.join(TESTCASE_DIR, "input_*.txt"))
    student_files = [f for f in os.listdir(SUBMIT_DIR) if f.endswith('.cpp')]
    
    if not test_files:
        print(Fore.RED + f"[-] Error: No testcase files matching 'input_*.txt' found in '{TESTCASE_DIR}'!")
        return
    if not student_files:
        print(Fore.YELLOW + f"[!] Warning: The '{SUBMIT_DIR}' directory is completely empty!")
        return

    print(Fore.WHITE + f"[*] Found {Fore.CYAN}{len(student_files)}{Fore.WHITE} submission(s) and {Fore.CYAN}{len(test_files)}{Fore.WHITE} testcase(s).")
    print(Fore.GREEN + "[+] Starting core engine...\n")
    
    score_board = []

    for idx, file_name in enumerate(student_files, 1):
        student_id = file_name.split('_')[0]
        
        # Student Dashboard Header
        print(Fore.WHITE + Style.BRIGHT + f"┌─── [{idx}/{len(student_files)}] ───────────────────────────────────────────────────┐")
        print(Fore.WHITE + Style.BRIGHT + f"│ " + Fore.YELLOW + f"STUDENT ID: {student_id:<50}" + Fore.WHITE + Style.BRIGHT + " │")
        print(Fore.WHITE + Style.BRIGHT + f"└──────────────────────────────────────────────────────────────────┘")

        # 1. File Replacement
        shutil.copy(os.path.join(SUBMIT_DIR, file_name), TARGET_FILE)

        # 2. Compilation
        print(Fore.CYAN + " 🔨 Compiling source code via runt.bat...", end="", flush=True)
        res = subprocess.run(BAT_FILE, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        
        if not os.path.exists(EXE_PATH):
            print(Fore.RED + "\r 🔨 Compilation Phase: FAILED! (Binary missing)                     ")
            print(Fore.RED + Style.BRIGHT + " └── ❌ STATUS: SKIPPED DUE TO COMPILATION ERROR\n")
            score_board.append({"StudentID": student_id, "Status": "Compilation Error", "Score": 0})
            continue
        else:
            print(Fore.GREEN + "\r 🔨 Compilation Phase: SUCCESSFUL!                                   ")

        passed_count = 0
        total_tests = len(test_files)

        print(Fore.WHITE + " 📑 Executing test pipeline:")
        
        # 3. Running Testcases
        for test_path in sorted(test_files):
            t_name = os.path.basename(test_path)
            input_str, expected_str = parse_testcase(test_path)
            
            if not expected_str:
                total_tests -= 1
                continue

            with open(FAKE_INPUT_FILE, "w", encoding="utf-8") as tmp_f:
                tmp_f.write(input_str)

            try:
                proc = subprocess.run(
                    EXE_PATH, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                    text=True, timeout=3, encoding='utf-8', errors='ignore'
                )
                
                student_output_clean = " ".join(proc.stdout.strip().split())
                expected_output_clean = " ".join(expected_str.strip().split())

                if expected_output_clean in student_output_clean:
                    passed_count += 1
                    print(f"   ├─ {Fore.GREEN}PASSED{Fore.RESET} ── {t_name}")
                else:
                    print(f"   ├─ {Fore.RED}FAILED{Fore.RESET} ── {t_name}")
                    print(Fore.MAGENTA + f"   │   ├── [Expected Key]: {expected_str.strip()}")
                    student_raw = proc.stdout.strip().replace('\n', ' ')
                    short_output = student_raw if len(student_raw) < 60 else student_raw[:57] + "..."
                    print(Fore.MAGENTA + f"   │   └── [Student Out] : {short_output if short_output else '(No stdout output)'}")

            except subprocess.TimeoutExpired:
                print(f"   ├─ {Fore.RED}TIMEOUT{Fore.RESET} ── {t_name} {Fore.YELLOW}(Time Limit Exceeded - 3s)")
            except Exception as e:
                print(f"   ├─ {Fore.RED}ERROR{Fore.RESET} ── {t_name} ({str(e)})")
            finally:
                if os.path.exists(FAKE_INPUT_FILE):
                    os.remove(FAKE_INPUT_FILE)

        # 4. Finalizing Summary for Student
        total_score = passed_count * POINTS_PER_TEST
        print(Fore.WHITE + "   │")
        
        # Background coloring for the score panel
        if total_score >= 7.0:
            score_color = Back.GREEN + Fore.BLACK
        elif total_score >= 5.0:
            score_color = Back.CYAN + Fore.BLACK
        else:
            score_color = Back.RED + Fore.WHITE
            
        print(Fore.WHITE + f"   └── 📊 {Style.BRIGHT}FINAL METRICS: Passed {passed_count}/{total_tests} cases ──> " + score_color + f" SCORE: {total_score} " + Style.RESET_ALL + "\n")
        score_board.append({"StudentID": student_id, "Status": f"Passed {passed_count}/{total_tests}", "Score": total_score})

    # 5. Exporting CSV
    with open(CSV_OUTPUT, mode='w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=["StudentID", "Status", "Score"])
        writer.writeheader()
        writer.writerows(score_board)
        
    print(Fore.CYAN + Style.BRIGHT + "====================================================================")
    print(Fore.GREEN + Style.BRIGHT + f"🎉 GRADING ENGINE PIPELINE TERMINATED! Results exported to: {CSV_OUTPUT}")
    print(Fore.CYAN + Style.BRIGHT + "====================================================================")

if __name__ == "__main__":
    main()