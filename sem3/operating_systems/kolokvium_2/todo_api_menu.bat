@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

color 0A
title Todo API Client

set "BASE_URL=http://localhost:8000"
set "CURL=curl.exe"
set "USERNAME="
set "PASSWORD="
set "TOKEN="
set "USER_ID="
set "RESPONSE_FILE=%temp%\todo_response_%random%.json"
set "CONFIG_FILE=todo_config.txt"

where curl >nul 2>&1
if errorlevel 1 (
    echo ERROR: curl not found in system!
    echo Install curl or add it to PATH
    pause
    exit /b 1
)

echo Checking server connection...
%CURL% -s -m 3 "%BASE_URL%/health/live" >nul 2>&1
if errorlevel 1 (
    echo.
    echo ERROR: Server unavailable
    echo Make sure server is running at %BASE_URL%
    echo.
    echo Start server with build_run_test.bat or python run.py
    echo.
    pause
    exit /b 1
)

echo Connection established!
timeout /t 1 /nobreak >nul

if exist "%CONFIG_FILE%" (
    for /f "tokens=1,2 delims==" %%a in (%CONFIG_FILE%) do (
        if "%%a"=="USERNAME" set "USERNAME=%%b"
        if "%%a"=="TOKEN" set "TOKEN=%%b"
        if "%%a"=="USER_ID" set "USER_ID=%%b"
    )
    if not "!TOKEN!"=="" (
        echo Found saved data for user: !USERNAME!
        set /p "LOAD=Use saved token? (Y/N): "
        if /i "!LOAD!"=="Y" goto MainMenu
    )
)

:LoginMenu
cls
echo ============================================
echo           Todo API - Login Menu
echo ============================================
echo [1] Login
echo [2] Register new user
echo [3] Initialize database
echo [4] Exit
echo ============================================
set /p "CHOICE=Select action: "

if "!CHOICE!"=="1" goto Login
if "!CHOICE!"=="2" goto Register
if "!CHOICE!"=="3" goto InitDB
if "!CHOICE!"=="4" goto End
goto LoginMenu

:InitDB
cls
echo ============================================
echo       Initialize Database
echo ============================================
echo.
echo Initializing database...
python init_db.py
echo.
pause
goto LoginMenu

:Login
cls
echo ============================================
echo                 Login
echo ============================================
echo Enter your credentials or type 'back' to return to menu
echo.
:LoginUsername
set /p "USERNAME=Username (or 'back' to return): "
if /i "!USERNAME!"=="back" goto LoginMenu

:LoginPassword
set /p "PASSWORD=Password (or 'back' to return): "
if /i "!PASSWORD!"=="back" goto LoginMenu

echo.
echo Logging in...
%CURL% -s -X POST "%BASE_URL%/auth/login" ^
    -H "Content-Type: application/json" ^
    -d "{\"username\":\"%USERNAME%\",\"password\":\"%PASSWORD%\"}" > "%RESPONSE_FILE%" 2>nul

echo.
if not exist "%RESPONSE_FILE%" (
    echo ERROR: No response from server
    goto LoginError
)

echo import json, sys > "%temp%\analyze_login.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\analyze_login.py"
echo     try: >> "%temp%\analyze_login.py"
echo         data = json.load(f) >> "%temp%\analyze_login.py"
echo         code = data.get('code', '') >> "%temp%\analyze_login.py"
echo         message = data.get('message', '') >> "%temp%\analyze_login.py"
echo         access_token = data.get('access_token', '') >> "%temp%\analyze_login.py"
echo         if 'user' in data: >> "%temp%\analyze_login.py"
echo             user_id = data['user'].get('id', '') >> "%temp%\analyze_login.py"
echo             username = data['user'].get('username', '') >> "%temp%\analyze_login.py"
echo         else: >> "%temp%\analyze_login.py"
echo             user_id = '' >> "%temp%\analyze_login.py"
echo             username = '' >> "%temp%\analyze_login.py"
echo         print(f'CODE={code}') >> "%temp%\analyze_login.py"
echo         print(f'MESSAGE={message}') >> "%temp%\analyze_login.py"
echo         print(f'ACCESS_TOKEN={access_token}') >> "%temp%\analyze_login.py"
echo         print(f'USER_ID={user_id}') >> "%temp%\analyze_login.py"
echo         print(f'USERNAME={username}') >> "%temp%\analyze_login.py"
echo     except Exception as e: >> "%temp%\analyze_login.py"
echo         print('ERROR=Invalid JSON response') >> "%temp%\analyze_login.py"
echo         print(f'RAW={str(e)}') >> "%temp%\analyze_login.py"

python "%temp%\analyze_login.py" "%RESPONSE_FILE%" > "%temp%\login_analysis.txt" 2>nul

set "LOGIN_CODE="
set "LOGIN_MESSAGE="
set "TOKEN="
set "USER_ID="
set "PARSED_USERNAME="

if exist "%temp%\login_analysis.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\login_analysis.txt"') do (
        if "%%a"=="CODE" set "LOGIN_CODE=%%b"
        if "%%a"=="MESSAGE" set "LOGIN_MESSAGE=%%b"
        if "%%a"=="ACCESS_TOKEN" set "TOKEN=%%b"
        if "%%a"=="USER_ID" set "USER_ID=%%b"
        if "%%a"=="USERNAME" set "PARSED_USERNAME=%%b"
    )
)

del "%temp%\analyze_login.py" 2>nul
del "%temp%\login_analysis.txt" 2>nul

if not "!LOGIN_CODE!"=="" (
    if "!LOGIN_CODE!"=="INVALID_CREDENTIALS" (
        echo LOGIN ERROR
        echo Invalid username or password
        del "%RESPONSE_FILE%" 2>nul
        echo.
        pause
        goto Login
    )
    
    if "!LOGIN_CODE!"=="LOGIN_ERROR" (
        echo LOGIN ERROR
        echo Server login error. Please try again
        del "%RESPONSE_FILE%" 2>nul
        echo.
        pause
        goto Login
    )
)

if "!TOKEN!"=="" (
    :LoginError
    echo LOGIN ERROR
    echo.
    if exist "%RESPONSE_FILE%" (
        echo Server response:
        type "%RESPONSE_FILE%"
        echo.
        del "%RESPONSE_FILE%" 2>nul
    ) else (
        echo No response received from server
    )
    echo.
    pause
    goto Login
)

if "!PARSED_USERNAME!"=="" set "PARSED_USERNAME=!USERNAME!"

echo Login successful! User ID: !USER_ID!
del "%RESPONSE_FILE%" 2>nul

echo USERNAME=!PARSED_USERNAME! > "%CONFIG_FILE%"
echo TOKEN=!TOKEN! >> "%CONFIG_FILE%"
echo USER_ID=!USER_ID! >> "%CONFIG_FILE%"
echo Last login: %date% %time% >> "%CONFIG_FILE%"

timeout /t 2 /nobreak >nul
goto MainMenu

:Register
cls
echo ============================================
echo              Registration
echo ============================================
echo Enter new user details or type 'back' to return to menu
echo.
:RegisterUsername
set /p "NEWUSER=New username (or 'back' to return): "
if /i "!NEWUSER!"=="back" goto LoginMenu
if "!NEWUSER!"=="" (
    echo Username cannot be empty
    goto RegisterUsername
)

:RegisterPassword
set /p "NEWPASS=New password (min 6 characters, or 'back' to return): "
if /i "!NEWPASS!"=="back" goto LoginMenu
if "!NEWPASS!"=="" (
    echo Password cannot be empty
    goto RegisterPassword
)

if "!NEWPASS:~0,6!"=="!NEWPASS!" if not "!NEWPASS!"=="!NEWPASS:~0,6!" (
    echo.
    echo Password must be at least 6 characters
    pause
    goto RegisterPassword
)

:RegisterConfirm
set /p "CONFIRM=Confirm password (or 'back' to return): "
if /i "!CONFIRM!"=="back" goto LoginMenu
if "!CONFIRM!"=="" (
    echo Please confirm password
    goto RegisterConfirm
)

if not "!NEWPASS!"=="!CONFIRM!" (
    echo.
    echo Passwords do not match
    pause
    goto RegisterConfirm
)

echo.
echo Registering...
%CURL% -s -X POST "%BASE_URL%/auth/register" ^
    -H "Content-Type: application/json" ^
    -d "{\"username\":\"%NEWUSER%\",\"password\":\"%NEWPASS%\"}" > "%RESPONSE_FILE%" 2>nul

echo.
if not exist "%RESPONSE_FILE%" (
    echo ERROR: No response from server
    goto RegisterError
)

echo import json, sys > "%temp%\analyze_response.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\analyze_response.py"
echo     try: >> "%temp%\analyze_response.py"
echo         data = json.load(f) >> "%temp%\analyze_response.py"
echo         code = data.get('code', '') >> "%temp%\analyze_response.py"
echo         message = data.get('message', '') >> "%temp%\analyze_response.py"
echo         print(f'CODE={code}') >> "%temp%\analyze_response.py"
echo         print(f'MESSAGE={message}') >> "%temp%\analyze_response.py"
echo         if 'user_id' in data: >> "%temp%\analyze_response.py"
echo             print(f'USER_ID={data["user_id"]}') >> "%temp%\analyze_response.py"
echo         if 'username' in data: >> "%temp%\analyze_response.py"
echo             print(f'USERNAME={data["username"]}') >> "%temp%\analyze_response.py"
echo     except Exception as e: >> "%temp%\analyze_response.py"
echo         print('ERROR=Invalid JSON response') >> "%temp%\analyze_response.py"
echo         print(f'RAW={str(e)}') >> "%temp%\analyze_response.py"

python "%temp%\analyze_response.py" "%RESPONSE_FILE%" > "%temp%\response_analysis.txt" 2>nul

set "RESPONSE_CODE="
set "RESPONSE_MESSAGE="
set "REG_USER_ID="
set "REG_USERNAME="

if exist "%temp%\response_analysis.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\response_analysis.txt"') do (
        if "%%a"=="CODE" set "RESPONSE_CODE=%%b"
        if "%%a"=="MESSAGE" set "RESPONSE_MESSAGE=%%b"
        if "%%a"=="USER_ID" set "REG_USER_ID=%%b"
        if "%%a"=="USERNAME" set "REG_USERNAME=%%b"
    )
)

del "%temp%\analyze_response.py" 2>nul
del "%temp%\response_analysis.txt" 2>nul

if not "!RESPONSE_CODE!"=="" (
    if "!RESPONSE_CODE!"=="USERNAME_EXISTS" (
        echo ERROR: Username "!NEWUSER!" already exists
        echo Please choose a different username
        del "%RESPONSE_FILE%" 2>nul
        echo.
        pause
        goto RegisterUsername
    )
    
    if "!RESPONSE_CODE!"=="VALIDATION_ERROR" (
        echo ERROR: Validation error
        echo Username must contain only letters and numbers
        del "%RESPONSE_FILE%" 2>nul
        echo.
        pause
        goto RegisterUsername
    )
    
    if "!RESPONSE_CODE!"=="REGISTRATION_ERROR" (
        echo ERROR: Registration error
        echo Check if database is initialized
        del "%RESPONSE_FILE%" 2>nul
        echo.
        pause
        goto LoginMenu
    )
)

if not "!REG_USER_ID!"=="" (
    echo Registration successful
    echo User ID: !REG_USER_ID!
    echo Now login with new credentials
    del "%RESPONSE_FILE%" 2>nul
    echo.
    pause
    set "USERNAME=!NEWUSER!"
    set "PASSWORD=!NEWPASS!"
    goto Login
)

:RegisterError
echo Unknown registration error
echo.
echo Server response:
type "%RESPONSE_FILE%" 2>nul
echo.
if exist "%RESPONSE_FILE%" del "%RESPONSE_FILE%" 2>nul
echo.
pause
goto RegisterUsername

:MainMenu
cls
echo ============================================
echo      Todo API - Main Menu
echo ============================================
if defined USERNAME echo User: !USERNAME!
if defined USER_ID echo ID: !USER_ID!
echo ============================================
echo [1]  Create task
echo [2]  List all tasks
echo [3]  Get task by ID
echo [4]  Update task (PATCH)
echo [5]  Replace task (PUT)
echo [6]  Delete task
echo [7]  Task statistics
echo [8]  Search tasks
echo [9]  Filter tasks by status
echo ---
echo [10] User profile
echo [11] Change password
echo [12] Server health check
echo [13] Server metrics
echo [14] Delete account/data
echo [15] Logout
echo [0]  Exit program
echo ============================================
set /p "CHOICE=Select action: "

if "!CHOICE!"=="1" goto CreateTask
if "!CHOICE!"=="2" goto ListTasks
if "!CHOICE!"=="3" goto GetTask
if "!CHOICE!"=="4" goto UpdateTask
if "!CHOICE!"=="5" goto ReplaceTask
if "!CHOICE!"=="6" goto DeleteTask
if "!CHOICE!"=="7" goto Stats
if "!CHOICE!"=="8" goto Search
if "!CHOICE!"=="9" goto Filter
if "!CHOICE!"=="10" goto Profile
if "!CHOICE!"=="11" goto ChangePassword
if "!CHOICE!"=="12" goto Health
if "!CHOICE!"=="13" goto Metrics
if "!CHOICE!"=="14" goto DeleteAccountMenu
if "!CHOICE!"=="15" goto Logout
if "!CHOICE!"=="0" goto End
goto MainMenu

:CreateTask
cls
echo ============================================
echo           Create New Task
echo ============================================
echo.
:CreateTaskTitle
set /p "TITLE=Task title (or 'back' to return): "
if /i "!TITLE!"=="back" goto MainMenu
if "!TITLE!"=="" (
    echo Title cannot be empty
    goto CreateTaskTitle
)

set /p "DESC=Task description (Enter to skip, 'back' to return): "
if /i "!DESC!"=="back" goto MainMenu

:CreateTaskStatus
echo.
echo Available statuses: todo, in_progress, done
set /p "STATUS=Status (default: todo, or 'back' to return): "
if /i "!STATUS!"=="back" goto MainMenu
if "!STATUS!"=="" set "STATUS=todo"

if not "!STATUS!"=="todo" if not "!STATUS!"=="in_progress" if not "!STATUS!"=="done" (
    echo Invalid status! Must be: todo, in_progress, or done
    goto CreateTaskStatus
)

echo.
echo Creating task...
if "!DESC!"=="" (
    %CURL% -s -X POST "%BASE_URL%/tasks" ^
        -H "Authorization: Bearer %TOKEN%" ^
        -H "Content-Type: application/json" ^
        -d "{\"title\":\"%TITLE%\",\"status\":\"%STATUS%\"}"
) else (
    %CURL% -s -X POST "%BASE_URL%/tasks" ^
        -H "Authorization: Bearer %TOKEN%" ^
        -H "Content-Type: application/json" ^
        -d "{\"title\":\"%TITLE%\",\"description\":\"%DESC%\",\"status\":\"%STATUS%\"}"
)

echo.
echo.
pause
goto MainMenu

:ListTasks
cls
echo ============================================
echo           All Tasks
echo ============================================
echo.
%CURL% -s -X GET "%BASE_URL%/tasks" ^
    -H "Authorization: Bearer %TOKEN%" ^
    | python -m json.tool 2>nul || %CURL% -s -X GET "%BASE_URL%/tasks" -H "Authorization: Bearer %TOKEN%"

echo.
pause
goto MainMenu

:GetTask
cls
echo ============================================
echo           Get Task by ID
echo ============================================
echo.
:GetTaskID
set /p "ID=Enter task ID (or 'back' to return): "
if /i "!ID!"=="back" goto MainMenu
if "!ID!"=="" (
    echo Task ID cannot be empty
    goto GetTaskID
)

echo.
echo Getting task...
%CURL% -s -X GET "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" > "%RESPONSE_FILE%" 2>nul

echo.
if not exist "%RESPONSE_FILE%" (
    echo ERROR: Could not retrieve task
    goto GetTaskError
)

echo import json, sys > "%temp%\check_get_task.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\check_get_task.py"
echo     try: >> "%temp%\check_get_task.py"
echo         data = json.load(f) >> "%temp%\check_get_task.py"
echo         if 'code' in data and data['code'] == 'TASK_NOT_FOUND': >> "%temp%\check_get_task.py"
echo             print('ERROR=TASK_NOT_FOUND') >> "%temp%\check_get_task.py"
echo         elif 'id' in data: >> "%temp%\check_get_task.py"
echo             print('OK') >> "%temp%\check_get_task.py"
echo         else: >> "%temp%\check_get_task.py"
echo             print('ERROR=UNKNOWN') >> "%temp%\check_get_task.py"
echo     except Exception as e: >> "%temp%\check_get_task.py"
echo         print('ERROR=INVALID_JSON') >> "%temp%\check_get_task.py"

python "%temp%\check_get_task.py" "%RESPONSE_FILE%" > "%temp%\get_check.txt" 2>nul

set "TASK_CHECK_RESULT="

if exist "%temp%\get_check.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\get_check.txt"') do (
        if "%%a"=="ERROR" set "TASK_CHECK_RESULT=%%b"
        if "%%a"=="OK" set "TASK_CHECK_RESULT=OK"
    )
)

del "%temp%\check_get_task.py" 2>nul
del "%temp%\get_check.txt" 2>nul

if "!TASK_CHECK_RESULT!"=="TASK_NOT_FOUND" (
    echo ERROR: Task not found!
    echo Task ID !ID! does not exist or you don't have permission to access it.
    if exist "%RESPONSE_FILE%" (
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

if not "!TASK_CHECK_RESULT!"=="OK" (
    echo ERROR: Could not retrieve task
    if exist "%RESPONSE_FILE%" (
        echo Server response:
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

%CURL% -s -X GET "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" ^
    | python -m json.tool 2>nul || %CURL% -s -X GET "%BASE_URL%/tasks/%ID%" -H "Authorization: Bearer %TOKEN%"

del "%RESPONSE_FILE%" 2>nul
echo.
pause
goto MainMenu

:GetTaskError
echo ERROR: Could not connect to server or retrieve task
echo.
pause
goto MainMenu

:UpdateTask
cls
echo ============================================
echo           Update Task (PATCH)
echo ============================================
echo.
:UpdateTaskID
set /p "ID=Task ID to update (or 'back' to return): "
if /i "!ID!"=="back" goto MainMenu
if "!ID!"=="" (
    echo Task ID cannot be empty
    goto UpdateTaskID
)

echo.
echo Getting current task...
%CURL% -s -X GET "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" > "%RESPONSE_FILE%" 2>nul

echo.
if not exist "%RESPONSE_FILE%" (
    echo ERROR: Could not retrieve task
    goto UpdateTaskError
)

echo import json, sys > "%temp%\check_task.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\check_task.py"
echo     try: >> "%temp%\check_task.py"
echo         data = json.load(f) >> "%temp%\check_task.py"
echo         if 'code' in data and data['code'] == 'TASK_NOT_FOUND': >> "%temp%\check_task.py"
echo             print('ERROR=TASK_NOT_FOUND') >> "%temp%\check_task.py"
echo         elif 'id' in data: >> "%temp%\check_task.py"
echo             print('OK') >> "%temp%\check_task.py"
echo             print(f'TITLE={data.get("title", "")}') >> "%temp%\check_task.py"
echo             print(f'DESCRIPTION={data.get("description", "")}') >> "%temp%\check_task.py"
echo             print(f'STATUS={data.get("status", "")}') >> "%temp%\check_task.py"
echo         else: >> "%temp%\check_task.py"
echo             print('ERROR=UNKNOWN') >> "%temp%\check_task.py"
echo     except Exception as e: >> "%temp%\check_task.py"
echo         print('ERROR=INVALID_JSON') >> "%temp%\check_task.py"

python "%temp%\check_task.py" "%RESPONSE_FILE%" > "%temp%\task_check.txt" 2>nul

set "TASK_CHECK_RESULT="
set "TASK_TITLE="
set "TASK_DESC="
set "TASK_STATUS="

if exist "%temp%\task_check.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\task_check.txt"') do (
        if "%%a"=="ERROR" set "TASK_CHECK_RESULT=%%b"
        if "%%a"=="OK" set "TASK_CHECK_RESULT=OK"
        if "%%a"=="TITLE" set "TASK_TITLE=%%b"
        if "%%a"=="DESCRIPTION" set "TASK_DESC=%%b"
        if "%%a"=="STATUS" set "TASK_STATUS=%%b"
    )
)

del "%temp%\check_task.py" 2>nul
del "%temp%\task_check.txt" 2>nul

if "!TASK_CHECK_RESULT!"=="TASK_NOT_FOUND" (
    echo ERROR: Task not found!
    echo Task ID !ID! does not exist or you don't have permission to access it.
    if exist "%RESPONSE_FILE%" (
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

if not "!TASK_CHECK_RESULT!"=="OK" (
    echo ERROR: Could not retrieve task
    if exist "%RESPONSE_FILE%" (
        echo Server response:
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

echo Current task:
type "%RESPONSE_FILE%" 2>nul
del "%RESPONSE_FILE%" 2>nul

echo.
echo.
echo --- Enter new values (press Enter to keep current) ---
echo.
if "!TASK_TITLE!"=="" (
    set "TASK_TITLE=Unknown"
)
echo Current title: !TASK_TITLE!
:UpdateTaskTitle
set /p "NEW_TITLE=New TITLE [leave blank to keep current]: "
if /i "!NEW_TITLE!"=="back" goto MainMenu

if "!TASK_DESC!"=="" (
    set "TASK_DESC=None"
)
echo Current description: !TASK_DESC!
:UpdateTaskDesc
set /p "NEW_DESC=New DESCRIPTION [leave blank to keep current]: "
if /i "!NEW_DESC!"=="back" goto MainMenu

if "!TASK_STATUS!"=="" (
    set "TASK_STATUS=todo"
)
echo Current status: !TASK_STATUS!
echo Available statuses: todo, in_progress, done
:UpdateTaskStatus
set /p "NEW_STATUS=New STATUS [leave blank to keep current]: "
if /i "!NEW_STATUS!"=="back" goto MainMenu

if not "!NEW_STATUS!"=="" (
    if not "!NEW_STATUS!"=="todo" if not "!NEW_STATUS!"=="in_progress" if not "!NEW_STATUS!"=="done" (
        echo Invalid status! Must be: todo, in_progress, or done
        echo.
        goto UpdateTaskStatus
    )
)

set "JSON_DATA={"
if not "!NEW_TITLE!"=="" (
    set "JSON_DATA=!JSON_DATA!\"title\":\"!NEW_TITLE!\""
)

if not "!NEW_DESC!"=="" (
    if "!JSON_DATA:~-1!"=="{" (
        set "JSON_DATA=!JSON_DATA!\"description\":\"!NEW_DESC!\""
    ) else (
        set "JSON_DATA=!JSON_DATA!,\"description\":\"!NEW_DESC!\""
    )
)

if not "!NEW_STATUS!"=="" (
    if "!JSON_DATA:~-1!"=="{" (
        set "JSON_DATA=!JSON_DATA!\"status\":\"!NEW_STATUS!\""
    ) else (
        set "JSON_DATA=!JSON_DATA!,\"status\":\"!NEW_STATUS!\""
    )
)

set "JSON_DATA=!JSON_DATA!}"

if "!JSON_DATA!"=="{}" (
    echo.
    echo No fields to update
    pause
    goto UpdateTask
)

echo.
echo Updating task...
echo JSON being sent: !JSON_DATA!
%CURL% -s -X PATCH "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" ^
    -H "Content-Type: application/json" ^
    -d "!JSON_DATA!"

echo.
echo.
pause
goto MainMenu

:UpdateTaskError
echo ERROR: Could not connect to server or retrieve task
echo.
pause
goto MainMenu

:ReplaceTask
cls
echo ============================================
echo           Replace Task (PUT)
echo ============================================
echo.
:ReplaceTaskID
set /p "ID=Task ID to replace (or 'back' to return): "
if /i "!ID!"=="back" goto MainMenu
if "!ID!"=="" (
    echo Task ID cannot be empty
    goto ReplaceTaskID
)

echo.
echo Getting current task...
%CURL% -s -X GET "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" > "%RESPONSE_FILE%" 2>nul

echo import json, sys > "%temp%\check_replace_task.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\check_replace_task.py"
echo     try: >> "%temp%\check_replace_task.py"
echo         data = json.load(f) >> "%temp%\check_replace_task.py"
echo         if 'code' in data and data['code'] == 'TASK_NOT_FOUND': >> "%temp%\check_replace_task.py"
echo             print('ERROR=TASK_NOT_FOUND') >> "%temp%\check_replace_task.py"
echo         elif 'id' in data: >> "%temp%\check_replace_task.py"
echo             print('OK') >> "%temp%\check_replace_task.py"
echo             print(f'TITLE={data.get("title", "")}') >> "%temp%\check_replace_task.py"
echo         else: >> "%temp%\check_replace_task.py"
echo             print('ERROR=UNKNOWN') >> "%temp%\check_replace_task.py"
echo     except Exception as e: >> "%temp%\check_replace_task.py"
echo         print('ERROR=INVALID_JSON') >> "%temp%\check_replace_task.py"

python "%temp%\check_replace_task.py" "%RESPONSE_FILE%" > "%temp%\replace_check.txt" 2>nul

set "TASK_CHECK_RESULT="
set "TASK_TITLE="

if exist "%temp%\replace_check.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\replace_check.txt"') do (
        if "%%a"=="ERROR" set "TASK_CHECK_RESULT=%%b"
        if "%%a"=="OK" set "TASK_CHECK_RESULT=OK"
        if "%%a"=="TITLE" set "TASK_TITLE=%%b"
    )
)

del "%temp%\check_replace_task.py" 2>nul
del "%temp%\replace_check.txt" 2>nul

if "!TASK_CHECK_RESULT!"=="TASK_NOT_FOUND" (
    echo ERROR: Task not found!
    echo Task ID !ID! does not exist or you don't have permission to access it.
    if exist "%RESPONSE_FILE%" (
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

if not "!TASK_CHECK_RESULT!"=="OK" (
    echo ERROR: Could not retrieve task
    if exist "%RESPONSE_FILE%" (
        echo Server response:
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

echo Current task:
type "%RESPONSE_FILE%" 2>nul
del "%RESPONSE_FILE%" 2>nul

echo.
echo.
echo --- Enter COMPLETE new task data ---
echo.
:ReplaceTaskTitle
set /p "NEW_TITLE=New TITLE (required): "
if /i "!NEW_TITLE!"=="back" goto MainMenu
if "!NEW_TITLE!"=="" (
    echo Title cannot be empty
    goto ReplaceTaskTitle
)

set /p "NEW_DESC=New DESCRIPTION [Enter for empty]: "
if /i "!NEW_DESC!"=="back" goto MainMenu

:ReplaceTaskStatus
echo Available statuses: todo, in_progress, done
set /p "NEW_STATUS=Status (default: todo): "
if /i "!NEW_STATUS!"=="back" goto MainMenu
if "!NEW_STATUS!"=="" set "NEW_STATUS=todo"

if not "!NEW_STATUS!"=="todo" if not "!NEW_STATUS!"=="in_progress" if not "!NEW_STATUS!"=="done" (
    echo Invalid status! Must be: todo, in_progress, or done
    goto ReplaceTaskStatus
)

echo.
echo Replacing task...
if "!NEW_DESC!"=="" (
    %CURL% -s -X PUT "%BASE_URL%/tasks/%ID%" ^
        -H "Authorization: Bearer %TOKEN%" ^
        -H "Content-Type: application/json" ^
        -d "{\"title\":\"%NEW_TITLE%\",\"description\":\"\",\"status\":\"%NEW_STATUS%\"}"
) else (
    %CURL% -s -X PUT "%BASE_URL%/tasks/%ID%" ^
        -H "Authorization: Bearer %TOKEN%" ^
        -H "Content-Type: application/json" ^
        -d "{\"title\":\"%NEW_TITLE%\",\"description\":\"%NEW_DESC%\",\"status\":\"%NEW_STATUS%\"}"
)

echo.
echo.
pause
goto MainMenu

:DeleteTask
cls
echo ============================================
echo           Delete Task
echo ============================================
echo.
:DeleteTaskID
set /p "ID=Task ID to delete (or 'back' to return): "
if /i "!ID!"=="back" goto MainMenu
if "!ID!"=="" (
    echo Task ID cannot be empty
    goto DeleteTaskID
)

echo.
echo Checking task existence...
echo.
%CURL% -s -X GET "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%" > "%RESPONSE_FILE%" 2>nul

if not exist "%RESPONSE_FILE%" (
    echo ERROR: Could not check task
    goto DeleteTaskError
)

echo import json, sys > "%temp%\check_task_del.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\check_task_del.py"
echo     try: >> "%temp%\check_task_del.py"
echo         data = json.load(f) >> "%temp%\check_task_del.py"
echo         if 'code' in data and data['code'] == 'TASK_NOT_FOUND': >> "%temp%\check_task_del.py"
echo             print('ERROR=TASK_NOT_FOUND') >> "%temp%\check_task_del.py"
echo         elif 'id' in data: >> "%temp%\check_task_del.py"
echo             print('OK') >> "%temp%\check_task_del.py"
echo             print(f'TITLE={data.get("title", "")}') >> "%temp%\check_task_del.py"
echo         else: >> "%temp%\check_task_del.py"
echo             print('ERROR=UNKNOWN') >> "%temp%\check_task_del.py"
echo     except Exception as e: >> "%temp%\check_task_del.py"
echo         print('ERROR=INVALID_JSON') >> "%temp%\check_task_del.py"

python "%temp%\check_task_del.py" "%RESPONSE_FILE%" > "%temp%\task_del_check.txt" 2>nul

set "TASK_CHECK_RESULT="
set "TASK_DEL_TITLE="

if exist "%temp%\task_del_check.txt" (
    for /f "tokens=1,2 delims==" %%a in ('type "%temp%\task_del_check.txt"') do (
        if "%%a"=="ERROR" set "TASK_CHECK_RESULT=%%b"
        if "%%a"=="OK" set "TASK_CHECK_RESULT=OK"
        if "%%a"=="TITLE" set "TASK_DEL_TITLE=%%b"
    )
)

del "%temp%\check_task_del.py" 2>nul
del "%temp%\task_del_check.txt" 2>nul

if "!TASK_CHECK_RESULT!"=="TASK_NOT_FOUND" (
    echo ERROR: Task not found!
    echo Task ID !ID! does not exist or you don't have permission to access it.
    if exist "%RESPONSE_FILE%" (
        echo Server response:
        type "%RESPONSE_FILE%" 2>nul
        del "%RESPONSE_FILE%" 2>nul
    )
    echo.
    pause
    goto MainMenu
)

if not "!TASK_CHECK_RESULT!"=="OK" (
    echo ERROR: Could not retrieve task
    del "%RESPONSE_FILE%" 2>nul
    echo.
    pause
    goto MainMenu
)

echo Task found:
echo Title: !TASK_DEL_TITLE!
echo ID: !ID!
del "%RESPONSE_FILE%" 2>nul

echo.
set /p "CONFIRM=Are you sure you want to delete task %ID%? (Y/N): "
if /i not "!CONFIRM!"=="Y" goto MainMenu

echo.
echo Deleting task...
%CURL% -s -X DELETE "%BASE_URL%/tasks/%ID%" ^
    -H "Authorization: Bearer %TOKEN%"

echo.
echo Task deleted
pause
goto MainMenu

:DeleteTaskError
echo ERROR: Could not connect to server or check task
echo.
pause
goto MainMenu

:Stats
cls
echo ============================================
echo           Task Statistics
echo ============================================
echo.
%CURL% -s -X GET "%BASE_URL%/tasks/stats" ^
    -H "Authorization: Bearer %TOKEN%" ^
    | python -m json.tool 2>nul || %CURL% -s -X GET "%BASE_URL%/tasks/stats" -H "Authorization: Bearer %TOKEN%"

echo.
pause
goto MainMenu

:Search
cls
echo ============================================
echo           Search Tasks
echo ============================================
echo.
:SearchQuery
set /p "QUERY=Search query (in title and description, or 'back' to return): "
if /i "!QUERY!"=="back" goto MainMenu
if "!QUERY!"=="" (
    echo Search query cannot be empty
    goto SearchQuery
)

echo.
echo Searching tasks...
%CURL% -s -X GET "%BASE_URL%/tasks/search?query=%QUERY%" ^
    -H "Authorization: Bearer %TOKEN%" ^
    | python -m json.tool 2>nul || %CURL% -s -X GET "%BASE_URL%/tasks/search?query=%QUERY%" -H "Authorization: Bearer %TOKEN%"

echo.
pause
goto MainMenu

:Filter
cls
echo ============================================
echo        Filter Tasks by Status
echo ============================================
echo.
echo Select status for filtering (or '0' to return to main menu):
echo [1] todo
echo [2] in_progress
echo [3] done
echo [4] all statuses
echo [0] Back to main menu
echo.
:FilterChoice
set /p "STATUS_CHOICE=Your choice: "

if "!STATUS_CHOICE!"=="0" goto MainMenu
if "!STATUS_CHOICE!"=="1" set "FILTER_STATUS=todo"
if "!STATUS_CHOICE!"=="2" set "FILTER_STATUS=in_progress"
if "!STATUS_CHOICE!"=="3" set "FILTER_STATUS=done"
if "!STATUS_CHOICE!"=="4" set "FILTER_STATUS="

if not "!STATUS_CHOICE!"=="1" if not "!STATUS_CHOICE!"=="2" if not "!STATUS_CHOICE!"=="3" if not "!STATUS_CHOICE!"=="4" (
    echo Invalid choice
    goto FilterChoice
)

if "!FILTER_STATUS!"=="" (
    %CURL% -s -X GET "%BASE_URL%/tasks" ^
        -H "Authorization: Bearer %TOKEN%"
) else (
    %CURL% -s -X GET "%BASE_URL%/tasks?status=%FILTER_STATUS%" ^
        -H "Authorization: Bearer %TOKEN%"
)

echo.
pause
goto MainMenu

:Profile
cls
echo ============================================
echo           User Profile
echo ============================================
echo.
%CURL% -s -X GET "%BASE_URL%/auth/profile" ^
    -H "Authorization: Bearer %TOKEN%" ^
    | python -m json.tool 2>nul || %CURL% -s -X GET "%BASE_URL%/auth/profile" -H "Authorization: Bearer %TOKEN%"

echo.
pause
goto MainMenu

:ChangePassword
cls
echo ============================================
echo           Change Password
echo ============================================
echo.
:ChangeOldPass
set /p "OLD_PASS=Current password (or 'back' to return): "
if /i "!OLD_PASS!"=="back" goto MainMenu
if "!OLD_PASS!"=="" (
    echo Current password cannot be empty
    goto ChangeOldPass
)

:ChangeNewPass
set /p "NEW_PASS=New password (or 'back' to return): "
if /i "!NEW_PASS!"=="back" goto MainMenu
if "!NEW_PASS!"=="" (
    echo New password cannot be empty
    goto ChangeNewPass
)

:ChangeConfirm
set /p "CONFIRM_NEW=Confirm new password (or 'back' to return): "
if /i "!CONFIRM_NEW!"=="back" goto MainMenu
if "!CONFIRM_NEW!"=="" (
    echo Please confirm new password
    goto ChangeConfirm
)

if not "!NEW_PASS!"=="!CONFIRM_NEW!" (
    echo Passwords do not match
    pause
    goto ChangeConfirm
)

echo.
echo Changing password...
%CURL% -s -X POST "%BASE_URL%/auth/change-password" ^
    -H "Authorization: Bearer %TOKEN%" ^
    -H "Content-Type: application/json" ^
    -d "{\"old_password\":\"%OLD_PASS%\",\"new_password\":\"%NEW_PASS%\"}"

echo.
echo.
pause
goto MainMenu

:Health
cls
echo ============================================
echo       Server Health Check
echo ============================================
echo.
echo Liveness:
%CURL% -s "%BASE_URL%/health/live"
echo.
echo Readiness:
%CURL% -s "%BASE_URL%/health/ready"
echo.
pause
goto MainMenu

:Metrics
cls
echo ============================================
echo            Server Metrics
echo ============================================
echo.
set "FIRST_LINE=1"
for /f "delims=" %%i in ('%CURL% -s "%BASE_URL%/metrics" 2^>nul') do (
    if "!FIRST_LINE!"=="1" set "FIRST_LINE=0"
    echo %%i
)

if "!FIRST_LINE!"=="1" (
    echo ERROR: Could not retrieve metrics
    echo Make sure server is running and /metrics endpoint is available
)

echo.
echo Press any key to continue...
pause >nul
goto MainMenu

:DeleteAccountMenu
cls
echo ============================================
echo           Account Management
echo ============================================
if defined USERNAME echo User: !USERNAME!
if defined USER_ID echo ID: !USER_ID!
echo ============================================
echo [1] Delete all my tasks
echo [2] Delete my account (and all tasks)
echo [3] Clear saved login data only
echo [0] Back to main menu
echo ============================================
set /p "DELETE_CHOICE=Select action: "

if "!DELETE_CHOICE!"=="1" goto DeleteAllTasks
if "!DELETE_CHOICE!"=="2" goto DeleteAccount
if "!DELETE_CHOICE!"=="3" goto ClearConfig
if "!DELETE_CHOICE!"=="0" goto MainMenu
goto DeleteAccountMenu

:DeleteAllTasks
cls
echo ============================================
echo        Delete All Tasks
echo ============================================
echo.
if defined USERNAME echo WARNING: This will delete ALL tasks for user: !USERNAME!
echo This action cannot be undone
echo.
set /p "CONFIRM=Are you absolutely sure? (Type 'DELETE' to confirm): "
if /i not "!CONFIRM!"=="DELETE" (
    echo Action cancelled
    pause
    goto DeleteAccountMenu
)

echo.
echo Deleting all tasks...
echo Getting all tasks first...
%CURL% -s -X GET "%BASE_URL%/tasks" ^
    -H "Authorization: Bearer %TOKEN%" > "%RESPONSE_FILE%" 2>nul

if not exist "%RESPONSE_FILE%" (
    echo ERROR: Could not retrieve tasks
    goto DeleteAccountMenuError
)

echo import json, sys > "%temp%\analyze_tasks.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\analyze_tasks.py"
echo     try: >> "%temp%\analyze_tasks.py"
echo         data = json.load(f) >> "%temp%\analyze_tasks.py"
echo         if isinstance(data, list): >> "%temp%\analyze_tasks.py"
echo             task_ids = [str(item['id']) for item in data if 'id' in item] >> "%temp%\analyze_tasks.py"
echo             print(','.join(task_ids)) >> "%temp%\analyze_tasks.py"
echo         else: >> "%temp%\analyze_tasks.py"
echo             print('') >> "%temp%\analyze_tasks.py"
echo     except: >> "%temp%\analyze_tasks.py"
echo         print('') >> "%temp%\analyze_tasks.py"

python "%temp%\analyze_tasks.py" "%RESPONSE_FILE%" > "%temp%\task_ids.txt" 2>nul

set "TASK_IDS="
if exist "%temp%\task_ids.txt" (
    set /p TASK_IDS=<"%temp%\task_ids.txt" 2>nul
)

del "%temp%\analyze_tasks.py" 2>nul
del "%temp%\task_ids.txt" 2>nul
del "%RESPONSE_FILE%" 2>nul

if "!TASK_IDS!"=="" (
    echo No tasks found to delete
    pause
    goto DeleteAccountMenu
)

set TASK_COUNT=0
for %%i in (!TASK_IDS!) do set /a TASK_COUNT+=1

echo Found !TASK_COUNT! tasks to delete
set /p "CONFIRM2=Delete all !TASK_COUNT! tasks? (Y/N): "
if /i not "!CONFIRM2!"=="Y" (
    echo Action cancelled
    pause
    goto DeleteAccountMenu
)

echo Deleting tasks...
set DELETED_COUNT=0
for %%i in (!TASK_IDS!) do (
    echo Deleting task ID: %%i
    %CURL% -s -X DELETE "%BASE_URL%/tasks/%%i" ^
        -H "Authorization: Bearer %TOKEN%" >nul 2>&1
    set /a DELETED_COUNT+=1
)

echo.
echo Successfully deleted !DELETED_COUNT! tasks
pause
goto DeleteAccountMenu

:DeleteAccountMenuError
if exist "%RESPONSE_FILE%" del "%RESPONSE_FILE%" 2>nul
echo.
pause
goto DeleteAccountMenu

:DeleteAccount
cls
echo ============================================
echo        Delete Account
echo ============================================
echo.
echo DANGER ZONE
echo.
echo WARNING: This will PERMANENTLY delete:
if defined USERNAME echo 1. Your account: !USERNAME! ID: !USER_ID!
if defined USER_ID echo 2. ALL your tasks 
echo 3. All your data
echo.
echo This action cannot be undone
echo.
echo To proceed, you must confirm with your password
:DeleteAccountPassword
set /p "DELETE_PASSWORD=Enter your password to confirm (or 'cancel' to abort): "
if /i "!DELETE_PASSWORD!"=="cancel" (
    echo Account deletion cancelled
    pause
    goto DeleteAccountMenu
)

if "!DELETE_PASSWORD!"=="" (
    echo Password cannot be empty
    goto DeleteAccountPassword
)

echo.
echo Step 1: Checking if delete endpoint exists...
%CURL% -s -X OPTIONS "%BASE_URL%/auth/delete-account" >nul 2>&1
if errorlevel 1 (
    echo ERROR: Delete account endpoint not available
    echo Please make sure you have the latest version of the API
    pause
    goto DeleteAccountMenu
)

echo.
echo Verifying password...
%CURL% -s -X POST "%BASE_URL%/auth/login" ^
    -H "Content-Type: application/json" ^
    -d "{\"username\":\"%USERNAME%\",\"password\":\"%DELETE_PASSWORD%\"}" > "%RESPONSE_FILE%" 2>nul

echo import json, sys > "%temp%\verify_password.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\verify_password.py"
echo     try: >> "%temp%\verify_password.py"
echo         data = json.load(f) >> "%temp%\verify_password.py"
echo         access_token = data.get('access_token', '') >> "%temp%\verify_password.py"
echo         if access_token: >> "%temp%\verify_password.py"
echo             print('VALID=1') >> "%temp%\verify_password.py"
echo         else: >> "%temp%\verify_password.py"
echo             print('VALID=0') >> "%temp%\verify_password.py"
echo     except: >> "%temp%\verify_password.py"
echo         print('VALID=0') >> "%temp%\verify_password.py"

python "%temp%\verify_password.py" "%RESPONSE_FILE%" > "%temp%\password_check.txt" 2>nul

set "PASSWORD_VALID="
if exist "%temp%\password_check.txt" (
    set /p PASSWORD_VALID=<"%temp%\password_check.txt" 2>nul
)

del "%temp%\verify_password.py" 2>nul
del "%temp%\password_check.txt" 2>nul
del "%RESPONSE_FILE%" 2>nul

if not "!PASSWORD_VALID!"=="VALID=1" (
    echo.
    echo INCORRECT PASSWORD
    echo Account deletion cancelled
    pause
    goto DeleteAccountMenu
)

echo Password verified
echo.
set /p "FINAL_CONFIRM=LAST CHANCE: Type 'DELETE MY ACCOUNT' to permanently delete: "
if /i not "!FINAL_CONFIRM!"=="DELETE MY ACCOUNT" (
    echo Account deletion cancelled
    pause
    goto DeleteAccountMenu
)

echo.
echo Step 2: Attempting to delete account from database...
%CURL% -s -X DELETE "%BASE_URL%/auth/delete-account" ^
    -H "Authorization: Bearer %TOKEN%" ^
    -H "Content-Type: application/json" > "%RESPONSE_FILE%" 2>nul

if not exist "%RESPONSE_FILE%" (
    echo ERROR: No response from server
    goto DeleteLocalOnly
)

echo import json, sys > "%temp%\analyze_delete.py"
echo with open(sys.argv[1], 'r', encoding='utf-8') as f: >> "%temp%\analyze_delete.py"
echo     try: >> "%temp%\analyze_delete.py"
echo         data = json.load(f) >> "%temp%\analyze_delete.py"
echo         message = data.get('message', '') >> "%temp%\analyze_delete.py"
echo         if 'deleted successfully' in message.lower(): >> "%temp%\analyze_delete.py"
echo             print('SUCCESS=1') >> "%temp%\analyze_delete.py"
echo         else: >> "%temp%\analyze_delete.py"
echo             print('SUCCESS=0') >> "%temp%\analyze_delete.py"
echo     except: >> "%temp%\analyze_delete.py"
echo         print('SUCCESS=0') >> "%temp%\analyze_delete.py"

python "%temp%\analyze_delete.py" "%RESPONSE_FILE%" > "%temp%\delete_result.txt" 2>nul

set "DELETE_SUCCESS="
if exist "%temp%\delete_result.txt" (
    set /p DELETE_SUCCESS=<"%temp%\delete_result.txt" 2>nul
)

del "%temp%\analyze_delete.py" 2>nul
del "%temp%\delete_result.txt" 2>nul

if not "!DELETE_SUCCESS!"=="SUCCESS=1" (
    echo.
    echo WARNING: Could not delete account from database
    echo Server response:
    type "%RESPONSE_FILE%"
    echo.
    echo Falling back to local data deletion only...
    del "%RESPONSE_FILE%" 2>nul
    goto DeleteLocalOnly
)

echo Account successfully deleted from database
del "%RESPONSE_FILE%" 2>nul

:DeleteLocalOnly
echo.
echo Step 3: Clearing all local data...
set "TOKEN="
set "OLD_USERNAME=!USERNAME!"
set "USERNAME="
set "PASSWORD="
set "OLD_USER_ID=!USER_ID!"
set "USER_ID="

if exist "%CONFIG_FILE%" (
    del "%CONFIG_FILE%" 2>nul
    if not exist "%CONFIG_FILE%" (
        echo Configuration file deleted
    ) else (
        echo WARNING: Could not delete config file
    )
) else (
    echo No config file found
)

echo.
echo ============================================
echo    ACCOUNT DELETION COMPLETE
echo ============================================
echo.
if defined OLD_USERNAME (
    echo Account: !OLD_USERNAME! ID: !OLD_USER_ID!
) else (
    echo Account information cleared
)
echo.
echo All data has been permanently removed
echo.
echo You will be returned to the login screen
echo.
pause
goto LoginMenu

:ClearConfig
cls
echo ============================================
echo       Clear Saved Login Data
echo ============================================
echo.
echo This will:
echo 1. Delete saved login credentials
echo 2. Clear your session
echo 3. Return to login screen
echo.
echo Your account and tasks in the database will NOT be affected
echo.
set /p "CONFIRM=Clear saved login data? (Y/N): "
if /i not "!CONFIRM!"=="Y" (
    echo Action cancelled
    pause
    goto DeleteAccountMenu
)

set "TOKEN="
set "USERNAME="
set "PASSWORD="
set "USER_ID="

if exist "%CONFIG_FILE%" (
    del "%CONFIG_FILE%" 2>nul
    echo Saved data deleted
) else (
    echo No saved data found
)

echo.
echo Returning to login screen...
timeout /t 2 /nobreak >nul
goto LoginMenu

:Logout
cls
echo ============================================
echo                 Logout
echo ============================================
echo.
if defined USERNAME echo Logging out user: !USERNAME!
echo.
set "TOKEN="
set "USERNAME="
set "PASSWORD="
set "USER_ID="

if exist "%CONFIG_FILE%" del "%CONFIG_FILE%" 2>nul

echo Logout successful
echo All local data cleared
pause
goto LoginMenu

:End
cls
echo ============================================
echo    Todo API - Goodbye
echo ============================================
echo.
echo Thank you for using the application
echo.
timeout /t 2 /nobreak >nul
endlocal
exit /b