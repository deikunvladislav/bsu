@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title Todo API - Complete Setup
color 0A

echo ============================================
echo    Todo API - System Setup
echo ============================================
echo.

echo [1] Checking Python...
python --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Python not installed
    pause
    exit /b 1
)

echo [2] Cleaning port 8000...
for /f "tokens=5" %%a in ('netstat -aon ^| findstr :8000 ^| findstr LISTENING 2^>nul') do (
    echo Stopping process on port 8000 (PID: %%a)
    taskkill /F /PID %%a >nul 2>&1
)
timeout /t 1 /nobreak >nul

echo [3] Installing dependencies...
pip install -r requirements.txt >nul 2>&1
if errorlevel 1 (
    echo Could not install dependencies
    echo Trying to install main packages...
    pip install Flask==2.3.3 Flask-SQLAlchemy==3.1.1 Flask-JWT-Extended==4.5.2 >nul 2>&1
)
echo Dependencies checked

echo [4] Initializing database...
echo Note: Database will be automatically created on first server start.
echo If manual initialization needed, run: python init_db.py
timeout /t 1 /nobreak >nul

echo [5] Running tests...
echo Running tests with new test structure...
pytest tests/ -v --tb=short 2>&1 | findstr /C:"PASSED" /C:"FAILED" /C:"ERROR" /C:"test_" || echo Tests completed

echo.
echo [6] Starting server...
echo ============================================
echo Server starting at http://localhost:8000
echo ============================================
echo.
echo Use Ctrl+C to stop
echo.
echo To use the API, open a new terminal window
echo and run todo_api_menu.bat
echo.

python run.py

echo.
echo ============================================
echo Server stopped
echo ============================================
echo.
pause
endlocal