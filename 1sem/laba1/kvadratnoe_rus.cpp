#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	double a, b, c, x1, x2, x, d;
	cout << "введите коэффициенты" << endl;
	cin >> a >> b >> c;
	d = b * b - 4 * a * c;
	if (a == 0 and b == 0 and c == 0)
	{
		cout << "любое число";
		return 0;
	}
	if (a == 0 and b == 0 and c != 0 || d < 0)
	{
		cout << "нет решений";
		return 0;
	}
	
	else if (d >= 0)
	{
		x1 = (-b + sqrt(d)) / 2 * a;
		x2 = (-b - sqrt(d)) / 2 * a;
		cout << "x1 = " << x1 << endl;
		cout << "x2 = " << x2;
		return 0;
	}

}