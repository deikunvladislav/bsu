@echo off
echo ========================================
echo Compiling Phone Factory Lab 5 (GUI)
echo ========================================

where javac >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ERROR: JDK is not installed or not in PATH
    echo Install JDK 17 or higher
    pause
    exit /b 1
)

if not exist "build" mkdir build
if not exist "build\classes" mkdir build\classes
if not exist "build\tests" mkdir build\tests

if not exist "lib\json-20231013.jar" (
    echo WARNING: json-20231013.jar not found
    echo Download from: https://repo1.maven.org/maven2/org/json/json/20231013/json-20231013.jar
    echo and place in the lib folder
    pause
)

echo Compiling main code...
javac -cp "lib\json-20231013.jar" -d build\classes ^
    src\factory\Main.java ^
    src\factory\gui\MainFrame.java ^
    src\factory\gui\PhoneTableModel.java ^
    src\factory\gui\AddEditPhoneDialog.java ^
    src\factory\gui\FileWorkerThread.java ^
    src\factory\gui\BuilderPatternDialog.java ^
    src\factory\gui\components\ButtonPanel.java ^
    src\factory\gui\components\StatusBar.java ^
    src\factory\io\AbstractFileHandler.java ^
    src\factory\io\PhoneFileHandler.java ^
    src\factory\io\PhoneXmlFileHandler.java ^
    src\factory\io\PhoneJsonFileHandler.java ^
    src\factory\io\EncryptionUtil.java ^
    src\factory\io\ArchiveUtil.java ^
    src\factory\io\DataWriter.java ^
    src\factory\io\SimpleDataWriter.java ^
    src\factory\io\AbstractDataWriterDecorator.java ^
    src\factory\io\EncryptionDataWriterDecorator.java ^
    src\factory\io\FileHandlerAdapter.java ^
    src\factory\menu\Menu.java ^
    src\factory\model\AbstractEntity.java ^
    src\factory\model\Phone.java ^
    src\factory\model\PhoneBuilder.java ^
    src\factory\storage\AbstractStorage.java ^
    src\factory\storage\PhoneListStorage.java ^
    src\factory\storage\PhoneMapStorage.java ^
    src\factory\validation\PhoneValidator.java

if %ERRORLEVEL% neq 0 (
    echo COMPILATION ERROR!
    pause
    exit /b 1
)

echo Compilation completed successfully!
echo Classes created in folder build\classes
pause