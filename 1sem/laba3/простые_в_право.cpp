#include <iostream>
#include <cmath>
#include <ctime>
#include <iomanip>
using namespace std;
int transfer_to_end(int a[], int j,int d)
{
	int temp;
	temp = a[j];
	a[j] = a[d];
	a[d] = temp;
	return a[j], a[d];
}
bool simp_check(int a[], int j)
{
	for (int k = 2; k <= ceil(sqrt(a[j])); k++)
	{
		if (a[j] % k == 0)
		{
			return false;
		}
	}
	return true;
}
int simp_counter(int a[], int n)
{
	int g{ 0 };
	for (int j = 0; j <= n - 1; j++)
	{
		if (simp_check(a, j) == true)
		{
			g = g + 1;
		}
	}
	return g;
}
int randmass(int a[], int n)
{
	for (int k = 0; k <= n - 1; k++)
	{
		a[k] = rand() % 100 + 1;
	}
	return a[n];
}
int coutmass(int a[], int n)
{
	for (int j = 0; j <= n - 1; j++)
	{
		cout << setw(4) << a[j];
	}
	cout << endl;
}
int main()
{
	setlocale(LC_ALL,  "RUS");
	int a[100], n, g, d;
	srand(static_cast<unsigned int> (time(0)));
	cout << "введите число элементов массива" << endl;
	cin >> n;
	d = n - 1;
	randmass(a, n);
	// этот выввод нужен для удобства проверки работы программы
	//тут начало проверки
	coutmass(a, n);
	// тут конец 
	g = simp_counter(a, n);
	// обращу ваше внимание на то что цикл работает не с начала массива а с конца
	// на мой взгляд так можно решить эту задачу изящнее и быстрее
	for (int j = n-1; j >= 0; j--)
	{
		if (simp_check(a, j) == false)
		{
			for (int k = 0; k < n-g; k++)
			{
				if (simp_check(a, k) == true)
				{
					transfer_to_end(a, k, j);
					break;
				}
			}
		}
	}
	coutmass(a, n);
	return 0;
}