#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int a, b, t;
	cout << "введите числа" << endl;
	cin >> a >> b;
	t = a * b;
	if (a <= 0 || b <= 0)
	{
		cout << "введены некоректные данные" << endl;
		return 0;
	}
	else
	{
		while (a != b) {
			if (b > a)
				b = b - a;
			if (a > b)
				a = a - b;
		}
	}
	cout << "NOD = " << a << endl;
	cout << "NOK = " << t / a << endl;
	return 0;
}