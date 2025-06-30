#include <iostream>
#include <fstream>
#include <regex>
#include <string>
using namespace std;

int main() {
	ifstream input("MAC_adress.txt");
	string str;
	regex rgx("^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}");
	while (getline(input, str))
	{
		if (regex_match(str, rgx) == 1)
			cout << str << " : this is MAC-adress " << endl;
		else
			cout << str << " : this is not MAC-adress " << endl;
	}
	return 0;
}