#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int a, m{ 0 }, k{ 0 }, h{ 0 };
	cout << "введите число" << endl;
	cin >> a;
	a = abs(a);
	h = abs(a);
	while (a > 0)
	{
		k = a % 10;
		m = m * 10 + k;
		a = a / 10;

	}
	cout << m << endl;
	if (h == m)
	{
		cout << "введенное число палиндром" << endl;
	}
	else { cout << "введенное число не палиндром" << endl; }
	return 0;
}