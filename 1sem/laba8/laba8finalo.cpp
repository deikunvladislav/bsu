#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void massmaker(char * mass, int *& arr, int masslength, int &lexemcounter)
{
	char** lexems = new char* [masslength + 1];
	char* lexem = strtok(mass, " ");
	int i{ 0 };
	while(lexem!=NULL and i < masslength + 1)
	{
		lexems[i] = new char[strlen(lexem) + 1];
		strcpy(lexems[i], lexem);
		lexem = strtok(NULL, " ");
		i++;
		lexemcounter++;
	}
	string str;
	int f = 0;
	for (int i = 0; i < lexemcounter; i++)
	{
		if (isdigit(lexems[i][0]))
		{
			str.assign(lexems[i]);
			arr[f] = stoi(str);
			str = "";
			f++;
		}
	}
	lexemcounter = f;
	for (int j = 0; j < lexemcounter; j++)
	{
		delete[] lexems[j];
	}
	delete[] lexems;
}

void keymaker(string &xline, int &key)
{
	xline.erase(0, 2);
	key = stoi(xline);
}

int xfinder(int* arr, int lexemcounter, int key, int& xcounter, int& index)
{
		if (index < lexemcounter)
		{   if (arr[index] == key) { xcounter++; index++; xfinder(arr, lexemcounter, key, xcounter, index); }
			else { index++; xfinder(arr, lexemcounter, key, xcounter, index); }
		}
		else { return xcounter; }
}
int main()
{
	setlocale(LC_ALL, "RUS");
	string massline;
	string xline;
	int key;
	ifstream input("input.txt");
	if (!input) { cout << "файл не найден" << endl; return 0; }
	ofstream output("output.txt");
	if (!output) { cout << "файл не найден" << endl; return 0; }
	// cout << "введите массив" << endl;
	getline(input, massline);
	getline(input, xline);
	//cout << "введите ключевой элемент" << endl;
	char* mass = new char[massline.length() + 1];
	strcpy(mass, massline.c_str());
	int lexemcounter{ 0 }, masslength = int(strlen(mass) + 1);
	int* arr = new int[100];
	massmaker(mass, arr, masslength, lexemcounter);
	keymaker(xline, key);
	int xcounter{ 0 }, index{ 0 };
	xfinder(arr, lexemcounter, key, xcounter, index);
	output<<"количество вхождений элемента Х = "<< key<<", равно : "<< xcounter<<endl;


	input.close();
	output.close();
	delete[] arr;
	delete[] mass;
	return 0;
}