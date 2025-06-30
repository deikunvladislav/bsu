#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int a, i{ 1 }, result{ 1 };
	cout << "введите число" << endl;
	cin >> a;
	if (a < 0)
	{
		cout << "введены некоректные данные" << endl;
		return 0;
	}
	for (int i{ 1 }; a >= i; i++)
	{
		result = result * i;

	}
	cout << a<<"! = " <<result;
	return 0;
}