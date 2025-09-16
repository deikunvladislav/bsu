#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

namespace LabTests
{
    TEST_CLASS(LabIntegrationTests)
    {
    public:

        TEST_METHOD(TestBinaryWriteRead)
        {
            const char* filename = "test_employees.dat";
            {
                ofstream ofs(filename, ios::binary);
                Assert::IsTrue(ofs.is_open());
                employee e1 = { 101, "Ivan", 40.5 };
                employee e2 = { 102, "Olga", 35.0 };
                ofs.write(reinterpret_cast<const char*>(&e1), sizeof(e1));
                ofs.write(reinterpret_cast<const char*>(&e2), sizeof(e2));
            }
            {
                ifstream ifs(filename, ios::binary);
                Assert::IsTrue(ifs.is_open());
                employee r1, r2;
                ifs.read(reinterpret_cast<char*>(&r1), sizeof(r1));
                ifs.read(reinterpret_cast<char*>(&r2), sizeof(r2));
                Assert::AreEqual(101, r1.num);
                Assert::AreEqual(string("Ivan"), string(r1.name));
                Assert::AreEqual(40.5, r1.hours, 0.001);
                Assert::AreEqual(102, r2.num);
                Assert::AreEqual(string("Olga"), string(r2.name));
                Assert::AreEqual(35.0, r2.hours, 0.001);
            }
            remove(filename);
        }

        TEST_METHOD(TestSalaryCalculation)
        {
            employee e = { 103, "Max", 20.0 };
            double rate = 15.0;
            double expectedSalary = e.hours * rate;
            double actualSalary = e.hours * rate;
            Assert::AreEqual(expectedSalary, actualSalary, 0.001);
        }

        TEST_METHOD(TestReporterOutput)
        {
            const char* binFile = "test_employees.dat";
            const char* reportFile = "test_report.txt";
            double      rate = 10.0;
            {
                ofstream ofs(binFile, ios::binary);
                Assert::IsTrue(ofs.is_open());
                employee e = { 201, "Test", 10.0 };
                ofs.write(reinterpret_cast<const char*>(&e), sizeof(e));
            }
            string cmdLine = "Reporter.exe " + string(binFile) + " " + reportFile + " " + to_string(rate);
            STARTUPINFOA si{ sizeof(si) };
            PROCESS_INFORMATION pi;
            vector<char> reporterCmd(cmdLine.begin(), cmdLine.end());
            reporterCmd.push_back('\0');
            BOOL success = CreateProcessA(
                nullptr,
                reporterCmd.data(),
                nullptr,
                nullptr,
                FALSE,
                0,
                nullptr,
                nullptr,
                &si,
                &pi
            );
            Assert::IsTrue(success);
            Assert::AreEqual(WAIT_OBJECT_0, WaitForSingleObject(pi.hProcess, 5000));
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);
            Assert::AreEqual((DWORD)0, exitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            ifstream ifs(reportFile);
            Assert::IsTrue(ifs.is_open());
            string line;
            bool foundSalary = false;
            while (getline(ifs, line)) {
                if (line.find("201") != string::npos && line.find("100.00") != string::npos) {
                    foundSalary = true;
                    break;
                }
            }
            Assert::IsTrue(foundSalary);
            remove(binFile);
            remove(reportFile);
        }

        TEST_METHOD(TestMainExecution)
        {
            const string binFile = "test_main_employees.dat";
            const string reportFile = "test_main_report.txt";

            // prepare the sequence of console inputs:
            //   1) binary filename
            //   2) record count = 0
            //   3) report filename
            //   4) rate per hour = 0.0
            string consoleInput =
                binFile + "\n" +
                "0\n" +
                reportFile + "\n" +
                "0.0\n";

            SECURITY_ATTRIBUTES sa{ sizeof(sa), nullptr, TRUE };
            HANDLE hReadStdin, hWriteStdin;
            Assert::IsTrue(CreatePipe(&hReadStdin, &hWriteStdin, &sa, 0));
            Assert::IsTrue(SetHandleInformation(hWriteStdin, HANDLE_FLAG_INHERIT, 1));

            STARTUPINFOA si{ sizeof(si) };
            si.dwFlags = STARTF_USESTDHANDLES;
            si.hStdInput = hReadStdin;
            si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
            si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

            PROCESS_INFORMATION pi;
            string cmd = "Main.exe";
            BOOL success = CreateProcessA(
                nullptr,
                (LPSTR)cmd.data(),
                nullptr,
                nullptr,
                TRUE,
                CREATE_NO_WINDOW,
                nullptr,
                nullptr,
                &si,
                &pi
            );
            Assert::IsTrue(success);

            CloseHandle(hReadStdin);
            DWORD written = 0;
            Assert::IsTrue(WriteFile(hWriteStdin, consoleInput.c_str(), (DWORD)consoleInput.size(), &written, nullptr));
            CloseHandle(hWriteStdin);

            Assert::AreEqual(WAIT_OBJECT_0, WaitForSingleObject(pi.hProcess, 10000));
            DWORD exitCode = UINT_MAX;
            GetExitCodeProcess(pi.hProcess, &exitCode);
            Assert::AreEqual((DWORD)0, exitCode);

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            // check that Main.exe created both files, even if empty
            ifstream binFs(binFile, ios::binary);
            Assert::IsTrue(binFs.is_open());
            binFs.close();

            ifstream repFs(reportFile);
            Assert::IsTrue(repFs.is_open());
            repFs.close();

            remove(binFile.c_str());
            remove(reportFile.c_str());
        }
    };
}
