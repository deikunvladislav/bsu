/*коментарий призван пояснить работу кода, в частности нескольких его функций:
1) функция cleaner используется для удаления части строки str1 (обращу внимание если ставить более
   одного разделителя то она будет отрезать некоректное число символов и следующие две функции сработают некореткно)
2) функция uniquewordssearch она ищет уникальные слова в строке str1 не используя массив лексем
   она построена на базе принципа разделения строк на лексемы и поочередного сравнения лексемы из
   str1 и всех лексем из str2, потом из строки str1 удаляется проверенная лексема и цикл повторяется
3) функция findnumbers10ss и numbers_tobegining_letters_toend эти две функции выполняют задачу поиска 
   чисел в 10с\c причем благодаря функции numbers_tobegining_letters_toend которая обрабатывает каждую 
   лексему строки strr( она содержит труды работы предыдущей функции) мы получаем все цифры ( в прямом смысле все)
   например: дано(5s4g654f65gd) после работы функции получим следующее(5465465sgfgd) а после работы функции findnumbers10ss
   получим следущее (5465465)
4) функции choice_sort и sorting_str1_according_to_aphabet сортируют строку str1 методом вставками, для работы 
   используется массив лексем
5) функция duplicate_all_words_delete_second_number_in_str1 обрабатывает строку типа string в отличае от предыдущих 
   функций ( они работают со строками типа char) первоначально функция отрезает от строки str1s часть именуемую token
   далее проверяет является ли эта лексема цифрой ( функция судит по первой букве важно заметить что лексема вида "4sg5s6dg45d5g"
   является числом(во время этой проверки мы считаем числа, чтобы удалит второе число из строки) а вот лексема вида "s44d5f4s4f98ds"
   напротив является словом, надеюсь понятно обьяснил. Далее токен складывается в строку result два раза через пробелы 
   (методом конкатенации строк), последние строки кода проверяют то чтобы строка str1s опустела а значит все лексемы были обработаны.*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

void cleaner(string& str1s, char* word1, int a)
{
	str1s.erase(a, strlen(word1) + 1);
}

void uniquewordssearch( char*str1, char*str2, char*str3, char* strr,  int a, string str2s, string str1s )
{
	bool t = false;
	char* word1 = strtok(str1, str3);
	while (word1 != NULL)
	{
		char* word2 = strtok(str2, str3);
		while (word2 != NULL)
		{
			if (strcmp(word1, word2) != 0)
			{
				word2 = strtok(NULL, str3);
			}
			else
			{
				t = true;
				break;
			}
		}
		strcpy(str2, str2s.c_str());
		if (t == false)
		{
			strcat(strr, word1);
			strcat(strr, " ");
		}
		t = false;
		cleaner(str1s, word1, a);
		strcpy(str1, "");
		strcpy(str1, str1s.c_str());
		word1 = strtok(str1, str3);
	}
}

void findnumbers10ss(char*str3, char* strr, int a, char*numbers)
{
	string sstrr;
	sstrr.assign(strr);
	char* word1;
	word1 = strtok(strr, str3);
	int digitindex{ 0 };
	while (word1 != NULL)
	{
		bool t = false;
		for (int i = 0; i < strlen(word1); i++) {
			if (isdigit(word1[i])) {
				digitindex++;
			}
			else { t = true;  break; }
		}
		if (t == false)
		{
			strcat(numbers, word1);
		}
		else { strncat(numbers, word1, digitindex); }
		digitindex = 0;
		strcat(numbers, " ");
		cleaner(sstrr, word1, a);
		strcpy(strr, sstrr.c_str());
		word1 = strtok(strr, str3);
	}
}

void numbers_tobegining_letters_toend(char* str1, char*str3, string &result)
{
	char* word1 = strtok(str1, str3);
	while (word1 != NULL)
	{
		string digits = ""; 
		string letters = ""; 
		for (int i = 0; i < strlen(word1); i++)
		{
			if (isdigit(word1[i]))
			{
				digits += word1[i]; 
			}
			else
			{
				letters += word1[i]; 
			}
		}
		result += digits + letters + " ";
		word1 = strtok(NULL, str3);
	}
}
void choice_sort(char ** lexems, int i)
{
	for (int k = 0; k < i - 1; ++k) {
		int minIndex = k;
		for (int j = k + 1; j < i; ++j) {
			if (strcmp(lexems[j], lexems[minIndex]) < 0) {
				minIndex = j;
			}
		}
		char* temp = lexems[k];
		lexems[k] = lexems[minIndex];
		lexems[minIndex] = temp;
	}
}

void sorting_str1_according_to_aphabet(char* str1, char *str3, ofstream &outfile)
{
	char** lexems = new char*[strlen(str1)+1];
	char* word = strtok(str1, str3);
	int i{ 0 };
	while (word != NULL and i < strlen(str1) + 1)
	{
		lexems [i] = new char[strlen(word) + 1];
		strcpy(lexems[i], word);
		word = strtok(NULL, str3);
		i++;
	}
	choice_sort(lexems, i);
	outfile << "отсортирванная строка str1 по алфовиту: " << endl;
	for (int j = 0; j < i; ++j) 
	{
		outfile << lexems[j] <<" ";
		delete[] lexems[j]; 
	}
	delete[] lexems;
}

void duplicate_all_words_delete_second_number_in_str1(string &str1s, string result, ofstream &outfile)
{
	string token;
	int numberCount{ 0 };
	size_t pos{ 0 };
	while ((pos = str1s.find(' ')) !=string :: npos) 
	{
		token = str1s.substr(0, pos);
		if (!token.empty() && std::isdigit(token[0])) 
		{
			numberCount++;
			if (numberCount == 2) 
			{
				str1s.erase(0, pos + 1);
				continue;
			}
		}
		result += token + " " + token + " ";
		str1s.erase(0, pos + 1);
	}
	if (!str1s.empty()) 
	{
		token = str1s;
		if (!token.empty() && std::isdigit(token[0]))
		{
			numberCount++;  if (numberCount==2){}else{result+= token + " " + token + " ";}
		}
		else {result += token + " " + token + " ";}
	}
	outfile<< result << endl;
}
int main()
{
	setlocale(LC_ALL, "RUS");
	string str1s, str2s, str3s;
	int a{ 0 };
	cout << "введите строку 1" << endl;
	getline(cin, str1s);
	cout << "введите строку 2" << endl;
	getline(cin, str2s);
	cout << "введите строку 3" << endl;
	getline(cin, str3s);
	cout << endl;
	char* str1 = new char[str1s.length() + 1];
	char* str2 = new char[str2s.length() + 1];
	char* str3 = new char[str3s.length() + 1];
	char* str1save = new char[str1s.length() + 1]; 
	strcpy(str1, str1s.c_str());
	strcpy(str1save, str1);
	strcpy(str2, str2s.c_str());
	strcpy(str3, str3s.c_str());
	if (strlen(str3) == 0)
	{
		cout << "введены некоректные данные" << endl;
		return 0;
	}
	ofstream outfile("answer.txt");
	if (!outfile) { cout << "файл не найден" << endl; return 1; }
	char* strr = new char[str1s.length() + 1];
	strcpy(strr, "");
	outfile << "уникальные слова строки str1:" << endl;
	uniquewordssearch(str1, str2, str3, strr, a, str2s, str1s); // вызов функции

	outfile << strr << endl << endl;
	char* savestrr = new char[strlen(strr) + 1];
	strcpy(savestrr, strr);
	char* numbers = new char[strlen(strr) + 1];
	strcpy (numbers, "");

	string result;
	strcpy(str1, str1save);
	numbers_tobegining_letters_toend(savestrr, str3, result); // функция перемещения цифр в лесеме в начало
	strcpy(strr, "");                                         // для облегчения работы функции по поиску чисел среди уникалных лексем
	strcpy(strr, result.c_str());
	outfile <<"результат перемещения цифр внутри лексем (нужно для нахождения всех цифр даже спрятанных по принципу анограммы): "
		<<endl<< result << endl<<endl;

	findnumbers10ss(str3, strr, a, numbers); // функция посика чисел в 10 с/с среди уникальных слов 1 строки

	outfile <<"все цифры из строки str1: "<<endl<< numbers << endl<<endl;

	sorting_str1_according_to_aphabet(str1, str3, outfile); // сортировка строки str1
	outfile << endl<<endl;

	result = "";
	outfile << "строка str1 с продублированными словами и удаленным вторым числом:" << endl;
	duplicate_all_words_delete_second_number_in_str1(str1s, result, outfile);
	outfile.close();

	delete[] savestrr;
	delete[] str1save;
	delete[] numbers;
	delete[] strr;
	delete[] str1;
	delete[] str2;
	delete[] str3;
	return 0;                                                           
}