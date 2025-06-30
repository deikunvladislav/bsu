#include <iostream>
#include <cstdlib>  
#include <ctime> 
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

// блок функций, базовые

float randmass(float* a, int n)
{
	for (int i = 0; i < n; ++i) {
		a[i] = (float)(rand() % 100) / 10;
	}
	return a[n];
}
float coutmass(float* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << "  ";
	}
	return a[n];
}

// функция для работы qsort

int compare(const void* a, const void* b)
{
	float A = *(const float*)a;
	float B = *(const float*)b;
	return ((A < B) - (A > B));
}

// функция для bsearch

int comp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

// функция хоара(сортировка)

void choarsort(float* a, int first, int last)
{

	int i = first, j = last;
	float tmp, c = a[(first + last) / 2];

	do {
		while (a[i] < c)
			i++;
		while (a[j] > c)
			j--;

		if (i <= j)
		{
			if (i < j)
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
			i++;
			j--;
		}
	} while (i <= j);

	if (i < last)
		choarsort(a, i, last);
	if (first < j)
		choarsort(a, first, j);
}

// сортировка подсчетом
float countsort(float * a, int n)
{
	int count[100]{ 0 };
	for (int i = 0; i < n; i++)
	{
		count[static_cast<int>(a[i] * 10)]++;
	}
	int i = 0;
	for (int k = 0; k < n; k++)
	{
		if (count[i] != 0)
		{
			a[k] = static_cast<float>(i / 10);
			count[i]--;
		}
		else
		{for (i; i < 100; i++)
			{
			if (count[i] != 0)
				{break;}
		    }
		    a[k] = static_cast<float>(i)/10;
			count[i]--;
		}
	}
	return a[n];
}

// блок основной функции

int main() {
	setlocale(LC_ALL, "RUS");
	srand(static_cast<unsigned int>(time(0)));
	int g, n;
	float f;
	cout << "введите число элементв массива" << endl;
	cin >> n;
	float* a = new float[n];
	randmass(a, n);
	int first{ 0 }, last{ n - 1 };
	cout << "выберите сортировку:" << endl << "1 - сортировка хоара" << endl << "2 - сортировка подсчетом" << endl <<
		"3 - сортировка qsort" << endl << "4 - сортировка по убыванию функцией sort " << endl <<
		"5 - сортировка по возрастанию функцией sort " << endl << "6 - поиск через функцию bsearch" << endl;
	cin >> g;
	cout << "исходный массив" << endl;
	coutmass(a, n);
	cout << endl;
	if (g < 1 and g>6)
	{
		cout << "неверные данные" << endl; return 0;
	}
	// обработка ( сортировка поиск)
	if (g == 6) // bsearch
	{
		sort(a, a + n);
		cout << "отсортированный массив" << endl;
		coutmass(a, n);
		cout << endl << "введите что искать" << endl;
		cin >> f;
		float* p = &a[0];
		float* s = (float*)bsearch(&f, a, n, sizeof(float), comp);
		if (!s) { cout << "элемент не найден" << endl; return 0; }
		else { cout << "элемент найден, индекс = " << s - a << endl; return 0; }
	}
	if (g == 4) // sort по убыванию
	{
		sort(a, a + n, [](float a, float b) {return a > b; });
	}
	if (g == 5)  // sort по возрастанию
	{
		sort(a, a + n);
	}
	if (g == 3) // qsort сортировка по убыванию
	{
		qsort(a, n, sizeof(float), compare);
	}
	if (g == 1)  // сортировка хоара
	{
		choarsort(a, first, last);
	}
	if (g == 2)  // сортировка подсчетом
	{
		countsort(a, n);
	}
	// вывод результата

	cout << " преобразованный массив" << endl;
	coutmass(a, n);

	delete[] a;
	return 0;
}