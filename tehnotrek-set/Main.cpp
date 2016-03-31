
//TODO: std::copy iterator and remove define? + in hpp
#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>

#include "MySet.hpp"

using namespace std;

/*
Для тестирования работы программы нужно реализовать ввод выражений через поток стандартного входа
и разбор таких входных данных в объекты. Формат входных данных определяется студентом.

Например, если необходимо реализовать класс даты, то программа должна получать на вход через
стандартный ввод выражение "5d4h7m+1w5m3s", транслировать его в два объекта класса day_time и складывать два этих объекта.

Если еще нет, то нужно создать аккаунт на github, тогда решением ДЗ будет отдельный репозиторий.
Для сдачи ДЗ необходимо дать ссылку на репозиторий в комментариях к этой теме либо отправить мне сообщением.
*/

int main(int argc, char* argv[], char* env[])
{
	char c;
	MySet b;
	MySet a;
	string s;

	while (true)
	{
		a.clear();
		b.clear();
		getline(cin, s);
		std::stringstream ss(s);
		while (ss >> c)
		{
			if (c == '[')
			{
				break;
			}
		}
		string str;
		int num;
		while (ss >> c)
		{
			if (c == ' ' || c == ',')
			{
				continue;
			}
			if (c == ']')
			{
				break;
			}
			str += c;
			if (ss.peek() == ' ' || ss.peek() == ',' || ss.peek() == ']')
			{
				num = std::stoi(str);
				a += num;
				str = "";
			}
			if (ss.peek() == ']')
			{
				ss.ignore();
				break;
			}
		}
		char op = '\0';
		while (ss >> c)
		{
			if (c == ' ')
			{
				continue;
			}
			op = c;
			break;
		}

		while (ss >> c)
		{
			if (c == '[')
			{
				break;
			}
		}

		while (ss >> c)
		{
			if (c == ' ' || c == ',')
			{
				continue;
			}

			if (c == ']')
			{
				break;
			}

			str += c;
			if (ss.peek() == ' ' || ss.peek() == ',' || ss.peek() == ']')
			{
				num = std::stoi(str);
				b += num;
				str = "";
			}
			if (ss.peek() == ']')
			{
				ss.ignore();
				break;
			}
		}

		cout << "a = [";
		a.foreach([](const int number)->void {cout << number << ", ";});
		cout << "]" << endl;

		cout << "op = " << op << endl;

		cout << "b = [";
		b.foreach([](const int number)->void {cout << number << ", ";});
		cout << "]" << endl;



		switch (op)
		{
		case '+':
			a += b;
			break;
		case '-':
			a -= b;
			break;
		case '*':
			a *= b;
			break;
		default: throw "wtf";
		}

		cout << "result = [";
		a.foreach([](const int number)->void {cout << number << ", ";});
		cout << "]" << endl;
	}
}