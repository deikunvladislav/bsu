@echo off
echo ========================================
echo Running Phone Factory Lab 5 (GUI)
echo ========================================

if not exist "build\classes\factory\Main.class" (
    echo ERROR: Classes are not compiled
    echo Run compile.bat first
    pause
    exit /b 1
)

echo Starting application...
java -cp "build\classes;lib\json-20231013.jar" factory.Main

pause