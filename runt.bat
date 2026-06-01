@off
:: Di chuyển vào thư mục chứa script
cd /d "%~dp0"

:: 1. Tạo thư mục build nếu chưa có
if not exist build (
    mkdir build
)

:: 2. Khởi tạo cấu hình CMake (chỉ chạy ngầm, giấu log lỗi linh tinh)
cmake -S . -B build >nul 2>&1

:: 3. Kích hoạt trình biên dịch Build ra file thực thi (.exe)
cmake --build build --config Debug >nul 2>&1

:: Thoát kịch bản thành công
exit /b 0