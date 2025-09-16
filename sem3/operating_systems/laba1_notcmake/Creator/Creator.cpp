#include "../laba1_tester/pch.h"

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: Creator <binary_file> <record_count>\n";
        return 1;
    }
    const char* filename = argv[1];
    int count = atoi(argv[2]);
    if (count <= 0) {
        cerr << "Record count must be positive\n";
        return 1;
    }
    ofstream ofs(filename, ios::binary);
    if (!ofs) {
        cerr << "Cannot open file for writing: " << filename << "\n";
        return 1;
    }
    for (int i = 0; i < count; ++i) {
        employee e;
        cout << "Enter record #" << (i + 1) << ":\n";
        cout << "  Num: ";
        cin >> e.num;
        cout << "  Name: ";
        cin >> ws;
        string tmp;
        getline(cin, tmp);
        strncpy(e.name, tmp.c_str(), sizeof(e.name) - 1);
        e.name[sizeof(e.name) - 1] = '\0';
        cout << "  Hours: ";
        cin >> e.hours;
        ofs.write(reinterpret_cast<const char*>(&e), sizeof(e));
    }
    ofs.close();
    return 0;
}