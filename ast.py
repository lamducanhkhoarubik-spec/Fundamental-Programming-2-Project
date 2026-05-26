import re
import os

def parse_cpp_file(filepath):
    """Đọc file C++ và trích xuất tên Class cùng tên các Hàm/Methods"""
    if not os.path.exists(filepath):
        print(f"❌ Không tìm thấy file: {filepath}")
        return set(), set()

    with open(filepath, 'r', encoding='utf-8') as f:
        code = f.read()

    # 1. Xóa sạch các đoạn Comment (để tránh quét nhầm hàm nằm trong comment)
    code = re.sub(r'//.*', '', code)
    code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)

    # 2. Quét tìm tên các Class (ví dụ: class MyClass { )
    classes = set(re.findall(r'\bclass\s+(\w+)', code))

    # 3. Quét tìm tên tất cả các hàm/methods (Dựa vào dấu mở ngoặc đơn `(`)
    # Loại trừ các từ khóa điều khiển trong C++ như if, while, for...
    all_words_before_paren = re.findall(r'\b([a-zA-Z_]\w*)\s*\(', code)
    cpp_keywords = {'if', 'while', 'for', 'switch', 'catch', 'sizeof', 'main', 'return'}
    functions = {f for f in all_words_before_paren if f not in cpp_keywords}

    return classes, functions

def check_outline(file_cua_ban, file_frame):
    print(f"🔍 Đang tự động kiểm tra outline...")
    
    # Quét cấu trúc 2 file
    my_classes, my_functions = parse_cpp_file(file_cua_ban)
    frame_classes, frame_functions = parse_cpp_file(file_frame)

    # So sánh xem file của bạn có thiếu cái gì của file frame không
    missing_classes = frame_classes - my_classes
    missing_functions = frame_functions - my_functions

    # In kết quả kiểm tra
    error_found = False
    if missing_classes:
        print(f"❌ File của bạn đang THIẾU các Class sau: {', '.join(missing_classes)}")
        error_found = True
    if missing_functions:
        print(f"❌ File của bạn đang THIẾU các Hàm/Methods sau: {', '.join(missing_functions)}")
        error_found = True

    if not error_found:
        print("✅ Tuyệt vời! File của bạn đã đầy đủ tất cả các Class và Hàm giống như file Frame mẫu.")

# --- ĐỔI TÊN 2 FILE C++ CỦA BẠN VÀO ĐÂY ĐỂ CHẠY ---
file_1 = "eniesLobby.cpp" 
file_2 = "eniesLobby_frame.h"

check_outline(file_1, file_2)