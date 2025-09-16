#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

void waitAndClose(PROCESS_INFORMATION& pi) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool runChild(const string& cmdLine) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessA(nullptr, (LPSTR)cmdLine.c_str(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        cerr << "Failed to start: " << cmdLine << "\n";
        return false;
    }
    waitAndClose(pi);
    return true;
}

template<typename T>
void printBinaryFile(const string& filename) {
    ifstream ifs(filename, ios::binary);
    if (!ifs) {
        cerr << "Cannot open binary file: " << filename << "\n";
        return;
    }
    cout << "\nContents of " << filename << ":\n";
    T rec;
    while (ifs.read(reinterpret_cast<char*>(&rec), sizeof(rec))) {
        cout << " Num=" << rec.num
            << " Name=" << rec.name
            << " Hours=" << rec.hours << "\n";
    }
}

void printTextFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs) {
        cerr << "Cannot open report: " << filename << "\n";
        return;
    }
    cout << "\n---- Report ----\n";
    string line;
    while (getline(ifs, line)) {
        cout << line << "\n";
    }
}

int main() {
    string binFile;
    int count;
    cout << "Enter binary file name: ";
    getline(cin, binFile);
    cout << "Enter number of records: ";
    cin >> count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string cmd1 = "Creator.exe " + binFile + " " + to_string(count);
    if (!runChild(cmd1)) return 1;
    struct employee { int num; char name[10]; double hours; };
    printBinaryFile<employee>(binFile);
    string reportFile;
    double rate;
    cout << "Enter report file name: ";
    getline(cin, reportFile);
    cout << "Enter rate per hour: ";
    cin >> rate;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string cmd2 = "Reporter.exe " + binFile + " " + reportFile + " " + to_string(rate);
    if (!runChild(cmd2)) return 1;
    printTextFile(reportFile);
    return 0;
}