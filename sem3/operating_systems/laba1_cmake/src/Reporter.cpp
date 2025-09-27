#include "../common/utils.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: Reporter <binFile> <reportFile> <rate>\n";
        return 1;
    }
    string binFile = argv[1];
    string reportFile = argv[2];
    double rate = stod(argv[3]);
    io::writeReport(binFile, reportFile, rate);
    return 0;
}