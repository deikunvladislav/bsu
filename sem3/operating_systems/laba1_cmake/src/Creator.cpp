#include "../common/utils.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: Creator <filename> <count>\n";
        return 1;
    }
    string filename = argv[1];
    int count = stoi(argv[2]);
    vector<employee> data;
    for (int i = 0; i < count; ++i) {
        employee e;
        cout << "Enter employee #" << (i + 1) << " (num name hours): ";
        cin >> e.num >> e.name >> e.hours;
        data.push_back(e);
    }
    io::writeBinaryFile(filename, data);
    return 0;
}
