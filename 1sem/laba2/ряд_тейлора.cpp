#include <iostream>
#include <iomanip>
#include <cmath>
#include <time.h>
using namespace std;
void prostoe_check(int a[k])
{
	for (int i = 2; i - sqrt(a); i++)
	{
		if (a % i == 0)
		{
			t = false;
		}
	}
}
int main()
{
	setlocale(LC_ALL, "Russian");
	int a[100];
	int n, temp;
	cout << "введите количество элементов массива"<<endl;
	cin >> n;
	// заполнение массива
	for (int k = 0; k <= n - 1; k++)
	{
		a[k] = srand % 1000;
	}
	g = n - 1;
	// сортируем массив
	for (int k = 0; k <= n - 1; k++)
	{
		if (prostoe_check(a[k]) == 0)
		{
			temp = a[g];
			a[g] = a[k];
			a[k] = temp;
		}
	}

}