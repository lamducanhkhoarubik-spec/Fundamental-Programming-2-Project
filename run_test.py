import os
import subprocess
import glob

# Cấu hình đường dẫn
EXE_PATH = os.path.join("build", "Debug", "eniesLobby_bin.exe")
TESTCASE_DIR = "testcase"
SCORE_PER_TEST = 3.0  # Bạn muốn mỗi câu bao nhiêu điểm thì sửa ở đây nhé

# Định nghĩa màu sắc hiển thị trên Terminal
GREEN = "\033[92m"
RED = "\033[91m"
CYAN = "\033[96m"
YELLOW = "\033[93m"
RESET = "\033[0m"
BOLD = "\033[1m"

def run_all_tests():
    # Tìm các file input_*.txt
    search_path = os.path.join(TESTCASE_DIR, "input_*.txt")
    test_files = sorted(glob.glob(search_path))
    
    if not test_files:
        print(f"{RED}Không tìm thấy file testcase nào trong thư mục '{TESTCASE_DIR}'!{RESET}")
        return

    if not os.path.exists(EXE_PATH):
        print(f"{RED}Không tìm thấy file thực thi tại '{EXE_PATH}'. Bạn đã build code chưa?{RESET}")
        return

    print(f"\n{BOLD}{CYAN}=================================================={RESET}")
    print(f"{BOLD}{CYAN}          HỆ THỐNG CHẤM ĐIỂM ENIES LOBBY          {RESET}")
    print(f"{BOLD}{CYAN}=================================================={RESET}\n")

    total_tests = len(test_files)
    passed_tests = 0

    for index, file_path in enumerate(test_files, 1):
        file_name = os.path.basename(file_path)
        print(f"👉 [{index}/{total_tests}] Đang chạy: {BOLD}{file_name}{RESET}...", end="")
        
        try:
            # Chạy file exe và truyền đường dẫn file input vào
            result = subprocess.run(
                [EXE_PATH, file_path], 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE, 
                text=True, 
                timeout=5 # Giới hạn 5s phòng trường hợp code bị lặp vô hạn
            )
            
            # Kiểm tra xem chương trình chạy thành công (exit code = 0) hay bị lỗi
            if result.returncode == 0:
                print(f" [{GREEN}{BOLD}PASSED{RESET}] (+{SCORE_PER_TEST}đ)")
                passed_tests += 1
            else:
                print(f" [{RED}{BOLD}FAILED / CRASHED{RESET}]")
                print(f"{YELLOW}--- Lỗi chi tiết ---{RESET}\n{result.stderr.strip()}")
                
        except subprocess.TimeoutExpired:
            print(f" [{RED}{BOLD}TIMEOUT{RESET}] (Code chạy quá 5 giây)")
        except Exception as e:
            print(f" [{RED}{BOLD}ERROR{RESET}] ({str(e)})")

    # Tính toán tổng điểm
    total_score = passed_tests * SCORE_PER_TEST
    max_score = total_tests * SCORE_PER_TEST
    
    print(f"\n{BOLD}{CYAN}=================================================={RESET}")
    print(f"{BOLD}KẾT QUẢ CHUNG:{RESET}")
    print(f"✅ Số câu đúng: {GREEN}{passed_tests}/{total_tests}{RESET}")
    print(f"⭐ Tổng điểm đạt được: {YELLOW}{BOLD}{total_score:.1f} / {max_score:.1f}{RESET} điểm")
    print(f"{BOLD}{CYAN}=================================================={RESET}\n")

if __name__ == "__main__":
    # Đảm bảo terminal Windows hiển thị được màu sắc ANSI
    os.system("") 
    run_all_tests()