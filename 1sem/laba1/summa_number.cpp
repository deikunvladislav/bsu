#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int a, sum{ 0 };
	cout << "введите число" << endl;
	cin >> a;
	a = abs(a);
	while (a % 10 != 0)
	{
		sum = sum + a % 10;
		a = a / 10;
	}
	cout << "сумма цифр числа = " << sum << endl;
}