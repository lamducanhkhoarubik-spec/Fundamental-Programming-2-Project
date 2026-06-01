@echo off
:: Di chuyển vào thư mục chứa script
cd /d "%~dp0"

:: 1. Tạo thư mục build nếu chưa có
if not exist build (
    mkdir build
)

:: 2. Khởi tạo cấu hình CMake (chạy ngầm cấu hình hệ thống)
cmake -S . -B build >nul 2>&1

:: 3. Kích hoạt trình biên dịch (Giữ nguyên log để Python bắt lỗi nếu có)
cmake --build build --config Debug

:: Thoát kịch bản thành công
exit /b 0