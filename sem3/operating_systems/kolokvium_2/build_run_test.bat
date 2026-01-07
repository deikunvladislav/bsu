@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

title Todo API + Web Interface - Complete Setup
color 0A

echo ============================================
echo    Todo API + Web Interface - System Setup
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
timeout /t 2 /nobreak >nul

echo [3] Installing dependencies...
echo Installing Flask and dependencies...
pip install -r requirements.txt --upgrade >nul 2>&1
if errorlevel 1 (
    echo Could not install dependencies
    echo Trying to install main packages...
    pip install Flask==2.3.3 Flask-SQLAlchemy==3.1.1 Flask-JWT-Extended==4.5.2 python-dotenv==1.0.1 >nul 2>&1
)
echo Dependencies checked

echo [4] Creating necessary directories...
if not exist "app\static\css" mkdir "app\static\css"
if not exist "app\static\js" mkdir "app\static\js"
if not exist "app\templates" mkdir "app\templates"
if not exist "app\routes" mkdir "app\routes"

echo [5] Creating basic CSS and JS files...
REM CSS files will be created by Python script

echo [6] Initializing database...
echo Note: Database will be automatically created on first server start.
echo If manual initialization needed, run: python init_db.py
timeout /t 1 /nobreak >nul

echo [7] Running tests...
if exist "tests\" (
    echo Running tests...
    pytest tests/ -v --tb=short 2>&1 | findstr /C:"PASSED" /C:"FAILED" /C:"ERROR" /C:"test_" || echo Tests completed
) else (
    echo No tests directory found, skipping tests
)

echo.
echo [8] Starting Web Server...
echo ============================================
echo Web Server starting at http://localhost:8000
echo ============================================
echo.
echo Available Endpoints:
echo - Web Interface: http://localhost:8000/
echo - Login Page: http://localhost:8000/login
echo - Registration: http://localhost:8000/register
echo - Dashboard: http://localhost:8000/dashboard
echo - Tasks: http://localhost:8000/tasks
echo - API Health: http://localhost:8000/api/health/live
echo - API Metrics: http://localhost:8000/api/metrics
echo.
echo Test Users:
echo - Use the web interface to register a new account
echo - Or run: flask create-user (in another terminal)
echo.
echo Use Ctrl+C to stop
echo.

python run.py

echo.
echo ============================================
echo Server stopped
echo ============================================
echo.
pause
endlocal