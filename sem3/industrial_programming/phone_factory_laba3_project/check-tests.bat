@echo off
echo ============================================
echo    CHECKING TEST COMPILATION AND PARTICIPATION
echo ============================================

echo Step 1: Checking if all test files exist...
if exist tests\factory\tests\PhoneFileHandlerTest.java (
    echo PhoneFileHandlerTest.java exists: yes
) else (
    echo PhoneFileHandlerTest.java exists: no
)

if exist tests\factory\tests\PhoneValidatorTest.java (
    echo PhoneValidatorTest.java exists: yes
) else (
    echo PhoneValidatorTest.java exists: no
)

if exist tests\factory\tests\PhoneXmlFileHandlerTest.java (
    echo PhoneXmlFileHandlerTest.java exists: yes
) else (
    echo PhoneXmlFileHandlerTest.java exists: no
)

if exist tests\factory\tests\PhoneTest.java (
    echo PhoneTest.java exists: yes
) else (
    echo PhoneTest.java exists: no
)

if exist tests\factory\tests\EncryptionUtilTest.java (
    echo EncryptionUtilTest.java exists: yes
) else (
    echo EncryptionUtilTest.java exists: no
)

if exist tests\factory\tests\PhoneMapStorageTest.java (
    echo PhoneMapStorageTest.java exists: yes
) else (
    echo PhoneMapStorageTest.java exists: no
)

if exist tests\factory\tests\PhoneListStorageTest.java (
    echo PhoneListStorageTest.java exists: yes
) else (
    echo PhoneListStorageTest.java exists: no
)

if exist tests\factory\tests\PhoneJsonFileHandlerTest.java (
    echo PhoneJsonFileHandlerTest.java exists: yes
) else (
    echo PhoneJsonFileHandlerTest.java exists: no
)

if exist tests\factory\tests\ArchiveUtilTest.java (
    echo ArchiveUtilTest.java exists: yes
) else (
    echo ArchiveUtilTest.java exists: no
)

if exist tests\factory\tests\PhoneBuilderTest.java (
    echo PhoneBuilderTest.java exists: yes
) else (
    echo PhoneBuilderTest.java exists: no
)

if exist tests\factory\tests\DataWriterDecoratorTest.java (
    echo DataWriterDecoratorTest.java exists: yes
) else (
    echo DataWriterDecoratorTest.java exists: no
)

echo.
echo Step 2: Checking compiled test classes...
if exist bin\tests\factory\tests\PhoneFileHandlerTest.class (
    echo PhoneFileHandlerTest compiled: yes
) else (
    echo PhoneFileHandlerTest compiled: no
)

if exist bin\tests\factory\tests\PhoneValidatorTest.class (
    echo PhoneValidatorTest compiled: yes
) else (
    echo PhoneValidatorTest compiled: no
)

if exist bin\tests\factory\tests\PhoneXmlFileHandlerTest.class (
    echo PhoneXmlFileHandlerTest compiled: yes
) else (
    echo PhoneXmlFileHandlerTest compiled: no
)

if exist bin\tests\factory\tests\PhoneTest.class (
    echo PhoneTest compiled: yes
) else (
    echo PhoneTest compiled: no
)

if exist bin\tests\factory\tests\EncryptionUtilTest.class (
    echo EncryptionUtilTest compiled: yes
) else (
    echo EncryptionUtilTest compiled: no
)

if exist bin\tests\factory\tests\PhoneMapStorageTest.class (
    echo PhoneMapStorageTest compiled: yes
) else (
    echo PhoneMapStorageTest compiled: no
)

if exist bin\tests\factory\tests\PhoneListStorageTest.class (
    echo PhoneListStorageTest compiled: yes
) else (
    echo PhoneListStorageTest compiled: no
)

if exist bin\tests\factory\tests\PhoneJsonFileHandlerTest.class (
    echo PhoneJsonFileHandlerTest compiled: yes
) else (
    echo PhoneJsonFileHandlerTest compiled: no
)

if exist bin\tests\factory\tests\ArchiveUtilTest.class (
    echo ArchiveUtilTest compiled: yes
) else (
    echo ArchiveUtilTest compiled: no
)

if exist bin\tests\factory\tests\PhoneBuilderTest.class (
    echo PhoneBuilderTest compiled: yes
) else (
    echo PhoneBuilderTest compiled: no
)

if exist bin\tests\factory\tests\DataWriterDecoratorTest.class (
    echo DataWriterDecoratorTest compiled: yes
) else (
    echo DataWriterDecoratorTest compiled: no
)

echo.
echo Step 3: Running tests to verify participation...
java -jar lib/junit-platform-console-standalone-1.9.2.jar --class-path "bin;lib/json-20231013.jar" --scan-class-path --details tree

echo.
echo ============================================
echo    TEST CHECK COMPLETED
echo ============================================
pause