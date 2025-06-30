#include <iostream>
#include <cmath>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	double x, n;
	cout << "введите число х" << endl;
	cin >> x;
	cout << "введите степень" << endl;
	cin >> n;
	if(n==0 and x==0|| n<0 and x==0||x<0 and n<1 and n>0)
	{
		cout << "введены некоректные данные" << endl;
	}
	else { cout << "X^n = " << pow(x, n); }
	return 0;
}
