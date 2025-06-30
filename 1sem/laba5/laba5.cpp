#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;
void randmass (char** a, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < m; k++)
		{ 
			if (i == 0 || i % 2 == 0)
				a[i][k] = 'a' + rand() % 26;
			if (i % 2 == 1)
				a[i][k] = 'A' + rand() % 26;
		}
	}
}
void coutmass (char** a, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < m; k++)
		{
			printf("%c ", a[i][k]);
		}
        printf("\n");
	}
}
void perevernut(char** a, int n, int m)
{
	int st = 0, line = n-1;
	for (int k = 0; k < n; k++)
	{
		for(int l=0; l<m; l++)
		{
			if(line<0)
		    {break;}
			printf("%c ", a[line][st]);
			line--;
		}
		line = n - 1;
		st++;
		printf("\n");
	}
}
int main()
{
	setlocale(LC_ALL, "RUS");
	srand(static_cast<unsigned int>(time(0)));
	int n, m;
	printf("введите количество строк матрицы\n");
	cin >> n;
	printf("введите количество столбцов матрицы\n");
	cin >> m;
	char ** a = new char * [n];
	for (int i = 0; i < n; i++)
		 a[i] = new char[m];
	randmass(a, n, m);
	printf("исходный массив :\n");
	coutmass(a, n, m);
	cout << endl;
	printf("результат переворота\n");
	perevernut(a, n, m);
	for (int i = 0; i < n; i++)
		delete[] a[i];
	delete[] a;
	return 0;
}