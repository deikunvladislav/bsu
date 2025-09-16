#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: Reporter <input_bin> <output_txt> <rate_per_hour>\n";
        return 1;
    }
    const char* inFile = argv[1];
    const char* outFile = argv[2];
    double rate = atof(argv[3]);
    if (rate < 0) {
        cerr << "Rate must be non-negative\n";
        return 1;
    }
    ifstream ifs(inFile, ios::binary);
    if (!ifs) {
        cerr << "Cannot open binary file: " << inFile << "\n";
        return 1;
    }
    ofstream ofs(outFile);
    if (!ofs) {
        cerr << "Cannot open report file: " << outFile << "\n";
        return 1;
    }
    ofs << "Report for file \"" << inFile << "\"\n";
    ofs << left << setw(10) << "Num" << setw(12) << "Name" << setw(10) << "Hours" << setw(12) << "Salary" << "\n";
    employee e;
    while (ifs.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        double salary = e.hours * rate;
        ofs << left
            << setw(10) << e.num
            << setw(12) << e.name
            << setw(10) << fixed << setprecision(2) << e.hours
            << setw(12) << fixed << setprecision(2) << salary
            << "\n";
    }
    return 0;
}