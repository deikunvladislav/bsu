#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

static void waitAndClose(PROCESS_INFORMATION& pi) {
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

static bool runChildMutable(string cmdLine) {
    STARTUPINFOA si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {};
    vector<char> buf(cmdLine.begin(), cmdLine.end());
    buf.push_back('\0');

    if (!CreateProcessA(
            nullptr,
            buf.data(),
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi)) {
        cerr << "Failed to start: " << cmdLine << "\n";
        return false;
    }
    waitAndClose(pi);
    return true;
}

template<typename T>
static void printBinaryFile(const string& filename) {
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

static void printTextFile(const string& filename) {
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

static bool fileExists(const string& path) {
    DWORD attrs = GetFileAttributesA(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

static void touchEmptyBinary(const string& path) {
    ofstream(path, ios::binary).close();
}

static void touchEmptyText(const string& path) {
    ofstream(path).close();
}

int main() {
    string binFile;
    cout << "Enter binary file name: ";
    getline(cin, binFile);

    int count = 0;
    cout << "Enter number of records: ";
    if (!(cin >> count)) {
        cin.clear();
    }
    if (count < 0) count = 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (count == 0) {
        touchEmptyBinary(binFile);
    }
    {
        string cmd1 = "Creator.exe " + string("\"") + binFile + "\" " + to_string(count);
        bool okCreator = runChildMutable(cmd1);
        if (!okCreator && !fileExists(binFile)) {
            touchEmptyBinary(binFile);
        }
    }
    struct employee { int num; char name[10]; double hours; };
    printBinaryFile<employee>(binFile);
    string reportFile;
    cout << "Enter report file name: ";
    getline(cin, reportFile);
    double rate = 0.0;
    cout << "Enter rate per hour: ";
    if (!(cin >> rate)) {
        cin.clear();
        rate = 0.0;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (rate < 0.0) rate = 0.0;
    touchEmptyText(reportFile);
    {
        string cmd2 = "Reporter.exe " + string("\"") + binFile + "\" \"" + reportFile + "\" " + to_string(rate);
        bool okReporter = runChildMutable(cmd2);
        if (!okReporter && !fileExists(reportFile)) {
            touchEmptyText(reportFile);
        }
    }
    printTextFile(reportFile);
    return 0;
}