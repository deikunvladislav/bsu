#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "RU");
	int n, l, i{ 1 }, k{ 1 };
	cout << "введите количество строчек" << endl;
	cin >> n;
	cout << "введите количество '*' в первой строке " << endl;
	cin >> l;
	if (n < 0 || l < 0)
	{
		cout << " неверные данные";
		return 0;
	}
	for (int i{ 1 }; i <= n; i++)
	{
		for (int k{ 1 }; k <= l; k++)
		{
			cout << "*";
		}
		cout << endl;
		l = l + 1;
	}
	return 0;
}