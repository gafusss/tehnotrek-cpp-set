#include <iostream>
#include <sstream>
#include "MySet.hpp"

using namespace std;

/*
��� ������������ ������ ��������� ����� ����������� ���� ��������� ����� ����� ������������ �����
� ������ ����� ������� ������ � �������. ������ ������� ������ ������������ ���������.

��������, ���� ���������� ����������� ����� ����, �� ��������� ������ �������� �� ���� �����
����������� ���� ��������� "5d4h7m+1w5m3s", ������������� ��� � ��� ������� ������ day_time � ���������� ��� ���� �������.

���� ��� ���, �� ����� ������� ������� �� github, ����� �������� �� ����� ��������� �����������.
��� ����� �� ���������� ���� ������ �� ����������� � ������������ � ���� ���� ���� ��������� ��� ����������.
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
		char op;
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
	system("PAUSE");
	return 0;
}