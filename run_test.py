import os
import subprocess
import glob

# Configuration
EXE_PATH = os.path.join("build", "Debug", "eniesLobby_bin.exe")
TESTCASE_DIR = "testcase"
SCORE_PER_TEST = 3.0  # Modify this value to change points per test

# Terminal ANSI escape colors for styling
GREEN = "\033[92m"
RED = "\033[91m"
CYAN = "\033[96m"
YELLOW = "\033[93m"
RESET = "\033[0m"
BOLD = "\033[1m"

def get_final_result(output_text):
    """Extracts the actual execution result after the '=== FINAL RESULT ===' marker"""
    marker = "=== FINAL RESULT ==="
    if marker in output_text:
        return output_text.split(marker)[-1].strip()
    return output_text.strip()

def extract_expected_output(file_path):
    """Reads the input file and extracts the expected answer string from the bottom"""
    try:
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()
        
        # Look for the EXPECTED_OUTPUT: token to extract the trailing string
        marker = "EXPECTED_OUTPUT:"
        if marker in content:
            return content.split(marker)[-1].strip()
    except Exception as e:
        pass
    return None

def run_all_tests():
    # Find all input_*.txt files in the testcase folder
    search_path = os.path.join(TESTCASE_DIR, "input_*.txt")
    test_files = sorted(glob.glob(search_path))
    
    if not test_files:
        print(f"{RED}Error: No 'input_*.txt' testcase files found in '{TESTCASE_DIR}'!{RESET}")
        return

    print(f"\n{BOLD}{CYAN}=================================================={RESET}")
    print(f"{BOLD}{CYAN}            ENIES LOBBY GRADING SYSTEM            {RESET}")
    print(f"{BOLD}{CYAN}=================================================={RESET}\n")

    total_tests = len(test_files)
    passed_tests = 0

    for index, input_path in enumerate(test_files, 1):
        file_name = os.path.basename(input_path)
        print(f"👉 [{index}/{total_tests}] Grading: {BOLD}{file_name}{RESET}...", end="")
        
        # 1. Parse the expected answer embedded at the end of the input file
        expected_ans = extract_expected_output(input_path)
        
        if expected_ans is None:
            print(f" [{YELLOW}SKIPPED{RESET}] (Missing 'EXPECTED_OUTPUT:' at the end of the file)")
            continue

        try:
            # 2. Execute the compiled C++ binary
            result = subprocess.run(
                [EXE_PATH, input_path], 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE, 
                text=True, 
                timeout=5
            )
            
            if result.returncode != 0:
                print(f" [{RED}{BOLD}CRASHED{RESET}]")
                continue
                
            # 3. Retrieve the output string printed by your program
            user_ans = get_final_result(result.stdout)
            
            # 4. Compare output strings to determine the score
            if user_ans == expected_ans:
                print(f" [{GREEN}{BOLD}PASSED{RESET}] (+{SCORE_PER_TEST} pts)")
                passed_tests += 1
            else:
                print(f" [{RED}{BOLD}WRONG ANSWER{RESET}]")
                print(f"   {CYAN}↳ Your Output:    {RESET} {user_ans}")
                print(f"   {YELLOW}↳ Expected Answer:{RESET} {expected_ans}")
                print("-" * 50)
                
        except subprocess.TimeoutExpired:
            print(f" [{RED}{BOLD}TIMEOUT{RESET}]")

    # Final Summary Report Calculation
    total_score = passed_tests * SCORE_PER_TEST
    max_score = total_tests * SCORE_PER_TEST
    
    print(f"\n{BOLD}{CYAN}=================================================={RESET}")
    print(f"{BOLD}SUMMARY REPORT:{RESET}")
    print(f"✅ Total Passed: {GREEN}{passed_tests}/{total_tests}{RESET}")
    print(f"⭐ Total Score:  {YELLOW}{BOLD}{total_score:.1f} / {max_score:.1f}{RESET} points")
    print(f"{BOLD}{CYAN}=================================================={RESET}\n")

if __name__ == "__main__":
    # Force Windows Terminal to support ANSI color coding escape tags
    os.system("") 
    run_all_tests()