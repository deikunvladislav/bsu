#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int a;
	bool t;
	cout << "введите число";
	cin >> a;
	if (a == 1)
	{
		cout << "не простое и не составное" << endl;
		return 0;
	}
	for (int i = 2; i - sqrt(a); i++)
	{
		if (a % i == 0)
		{
			cout << "составное" << endl;
			t = false;
			break;
		}
		else
		{
			cout << "простое" << endl;
			return 0;
		}
	}
}