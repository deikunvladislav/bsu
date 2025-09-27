#include "../common/utils.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string binFile, reportFile;
    int count;
    double rate;
    cout << "Enter binary filename and record count: ";
    cin >> binFile >> count;
    io::runProcess("Creator.exe", binFile + " " + to_string(count));
    auto data = io::readBinaryFile(binFile);
    for (const auto& e : data) {
        cout << e.to_string(0) << '\n';
    }
    cout << "Enter report filename and hourly rate: ";
    cin >> reportFile >> rate;
    io::runProcess("Reporter.exe", binFile + " " + reportFile + " " + to_string(rate));
    ifstream ifs(reportFile);
    if (ifs) {
        cout << "\nReport:\n" << ifs.rdbuf();
    }
    else {
        cout << "File error.\n";
        return 1;
    }
    return 0;
}