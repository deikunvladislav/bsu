@echo off
echo ============================================
echo    Phone Factory Project Build & Run
echo ============================================

echo Step 1: Cleaning bin directory...
if exist bin rmdir /s /q bin
mkdir bin

echo Step 2: Compiling source code...

echo Compiling model classes...
javac -d bin -cp "lib/json-20231013.jar" src/factory/model/*.java
if %errorlevel% neq 0 goto error

echo Compiling validation classes...
javac -d bin -cp "bin;lib/json-20231013.jar" src/factory/validation/*.java
if %errorlevel% neq 0 goto error

echo Compiling storage classes...
javac -d bin -cp "bin;lib/json-20231013.jar" src/factory/storage/*.java
if %errorlevel% neq 0 goto error

echo Compiling IO classes...
javac -d bin -cp "bin;lib/json-20231013.jar" src/factory/io/*.java
if %errorlevel% neq 0 goto error

echo Compiling menu classes...
javac -d bin -cp "bin;lib/json-20231013.jar" src/factory/menu/*.java
if %errorlevel% neq 0 goto error

echo Compiling main class...
javac -d bin -cp "bin;lib/json-20231013.jar" src/factory/Main.java
if %errorlevel% neq 0 goto error

echo Step 3: Compiling tests...
echo Compiling all test files...
dir /B tests\factory\tests\*.java > test_files.txt
echo Found test files:
type test_files.txt

javac -d bin -cp "bin;lib/json-20231013.jar;lib/junit-platform-console-standalone-1.9.2.jar" tests/factory/tests/*.java
if %errorlevel% neq 0 (
    echo WARNING: Test compilation had issues
    echo Checking which tests compiled...
    dir /B bin\tests\factory\tests\*.class > compiled_tests.txt
    echo Compiled test classes:
    type compiled_tests.txt
    del test_files.txt
    del compiled_tests.txt
    pause
) else (
    echo ✓ All tests compiled successfully!
    del test_files.txt
)

echo.
echo ============================================
echo    BUILD COMPLETED!
echo ============================================
echo.

:menu
echo What would you like to do?
echo 1. Run Application
echo 2. Run Tests
echo 3. Run Tests with Details
echo 4. Check Test Participation
echo 5. Recompile Tests Only
echo 6. Exit
echo.
set /p choice="Enter your choice (1-6): "

if "%choice%"=="1" goto runapp
if "%choice%"=="2" goto runtests
if "%choice%"=="3" goto runtests_detailed
if "%choice%"=="4" goto check_tests
if "%choice%"=="5" goto recompile_tests
if "%choice%"=="6" goto exit

echo Invalid choice, please try again.
echo.
goto menu

:runtests
echo.
echo ============================================
echo    RUNNING TESTS
echo ============================================
java -jar lib/junit-platform-console-standalone-1.9.2.jar --class-path "bin;lib/json-20231013.jar" --scan-class-path
echo.
set /p continue="Press Enter to return to menu..."
goto menu

:runtests_detailed
echo.
echo ============================================
echo    RUNNING TESTS WITH DETAILS
echo ============================================
java -jar lib/junit-platform-console-standalone-1.9.2.jar --class-path "bin;lib/json-20231013.jar" --scan-class-path --details tree
echo.
set /p continue="Press Enter to return to menu..."
goto menu

:runapp
echo.
echo ============================================
echo    RUNNING APPLICATION
echo ============================================
java -cp "bin;lib/json-20231013.jar" factory.Main
echo.
set /p continue="Press Enter to return to menu..."
goto menu

:check_tests
echo.
echo ============================================
echo    CHECKING TEST PARTICIPATION
echo ============================================
call check-tests.bat
goto menu

:recompile_tests
echo.
echo ============================================
echo    RECOMPILING TESTS
echo ============================================
javac -d bin -cp "bin;lib/json-20231013.jar;lib/junit-platform-console-standalone-1.9.2.jar" tests/factory/tests/*.java
if %errorlevel% neq 0 (
    echo Test recompilation failed!
) else (
    echo Test recompilation successful!
)
pause
goto menu

:rebuild
echo.
echo ============================================
echo    REBUILDING...
echo ============================================
call "%~f0"
goto :eof

:error
echo.
echo ============================================
echo    BUILD FAILED!
echo ============================================
echo Please check the errors above and try again.
echo.
echo Common issues:
echo 1. Make sure all Java files are in the correct locations
echo 2. Check that lib/json-20231013.jar exists
echo 3. Verify Java is installed and in PATH
echo.
pause
exit /b 1

:exit
echo Goodbye!
pause