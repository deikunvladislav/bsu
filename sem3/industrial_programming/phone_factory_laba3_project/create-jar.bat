@echo off
echo ========================================
echo Creating Phone Factory JAR File
echo ========================================

if exist "temp" rmdir /s /q temp
mkdir temp

xcopy /s /e /i "build\classes\*" "temp\"

echo Main-Class: factory.Main > manifest.txt
echo Class-Path: lib/json-20231013.jar >> manifest.txt
echo Built-By: Phone Factory Lab 5 >> manifest.txt
echo Created-By: JDK 17 >> manifest.txt

jar cvfm PhoneFactory.jar manifest.txt -C temp .

rmdir /s /q temp
del manifest.txt

echo JAR file created: PhoneFactory.jar
echo To run: java -jar PhoneFactory.jar
pause