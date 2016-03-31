#pragma once
#include <utility>
#include <functional>
#include <string>
#include <regex>

class MySet
{
public:

	// ReSharper disable once CppMemberFunctionMayBeConst
	void clear()
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			table[i].clear();
		}
	}

	//Конструктор по умолчанию
	explicit MySet(unsigned int t_size = 256) :table_size(t_size > 256 ? t_size : 256), table(new Bucket[table_size]) {};

	explicit MySet(std::string str, unsigned int t_size = 256):MySet(t_size)
	{
		std::regex reg("[\\s*(\\d+,\\s?)(\\d+)\\s*]");
	}

	//Конструктор копирования
	MySet(const MySet& other) :table_size(other.table_size), table(new Bucket[table_size])
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			table[i] = other.table[i];
		}
	};

	//Конструктор переноса
	MySet(MySet&& other) :table_size(other.table_size), table(other.table)
	{
		other.table = nullptr;
		other.table_size = 0;
	}

	//Деструктор
	~MySet()
	{
		delete[] table;
	}

	//Оператор присваивания некопирующий
	MySet& operator=(MySet other)
	{
		swap(*this, other);
		return *this;
	}

	MySet& operator=(int number)
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			table[i].clear();
		}
		table[hash(number)].add(number);
		return *this;
	}

	MySet& operator+=(const MySet& other)
	{
		for (unsigned int i = 0; i < other.table_size; i++)
		{
			other.table[i].foreach([this](const int number) -> void {this->add(number);});
		}
		return *this;
	}

	MySet& operator-=(const MySet& other)
	{
		for (unsigned int i = 0; i < other.table_size; i++)
		{
			other.table[i].foreach([this](const int number) -> void {this->remove(number);});
		}
		return *this;
	}

	MySet& operator*=(const MySet& other)
	{
		for (unsigned int i = 0; i < other.table_size; i++)
		{
			table[i].foreach([this, &other](const int number)->void {
				if (!other.contains(number))
				{
					this->remove(number);
				}
			});
		}
		return *this;
	}

	MySet& operator+=(const int number)
	{
		add(number);
		return *this;
	}

	MySet& operator-=(const int number)
	{
		remove(number);
		return *this;
	}

	MySet& operator*=(const int other)
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			table[i].foreach([this, other](const int number)->void {
				if (number != other)
				{
					this->remove(number);
				}
			});
		}
		return *this;
	}

	//Оператор сложить(set)
	friend MySet operator+(MySet left, const MySet& right)
	{
		left += right;
		return left;
	}

	//Оператор вычесть(set)
	friend MySet operator-(MySet left, const MySet& right)
	{
		left -= right;
		return left;
	}

	//Оператор умножить(set)
	friend MySet operator*(MySet left, const MySet& right)
	{
		left *= right;
		return left;
	}

	//Оператор сложить(int)
	friend MySet operator+(MySet left, int right)
	{
		left += right;
		return left;
	}

	//Оператор вычесть(int)
	friend MySet operator-(MySet left, int right)
	{
		left -= right;
		return left;
	}

	//Оператор умножить(int)
	friend MySet operator*(MySet left, int right)
	{
		left *= right;
		return left;
	}

	//Оператор сложить(int)
	friend MySet operator+(int left, MySet right)
	{
		right += left;
		return right;
	}

	//Оператор вычесть(int)
	friend MySet operator-(int left, MySet right)
	{
		right -= left;
		return right;
	}

	//Оператор умножить(int)
	friend MySet operator*(int left, MySet right)
	{
		right *= left;
		return right;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void foreach(std::function<void(const int)> func)
	{
		for (unsigned int i = 0; i < table_size; i++)
		{
			table[i].foreach(func);
		}
	}

private:
	class Bucket
	{
	public:
		bool contains(const int number) const
		{
			const ListNode * cur = root;
			while (cur != nullptr)
			{
				if (cur->value == number)
				{
					return true;
				}
				cur = cur->next;
			}
			return false;
		}

		void add(const int number)
		{
			if (root == nullptr)
			{
				root = new ListNode();
				root->value = number;
				return;
			}
			ListNode* cur = root;
			while (cur->next != nullptr)
			{
				if (cur->value == number)
				{
					return;
				}
				cur = cur->next;
			}
			if (cur->value != number)
			{
				cur->next = new ListNode();
				cur->next->prev = cur;
				cur->next->value = number;
			}
		}

		void remove(const int number)
		{
			ListNode* cur = root;
			while (cur != nullptr)
			{
				if (cur->value == number)
				{
					if (cur->prev == nullptr)
					{
						root = cur->next;
						if (root != nullptr)
						{
							root->prev = nullptr;
						}
						delete cur;
						return;
					}
					cur->prev->next = cur->next;
					if (cur->next != nullptr)
					{
						cur->next->prev = cur->prev;
					}
					delete cur;
					return;
				}
				cur = cur->next;
			}
		}
		//Конструктор по умолчанию
		Bucket() {};


		//Деструктор
		~Bucket()
		{
			clear();
		}

		void clear()
		{
			while (root != nullptr)
			{
				ListNode* next = root->next;
				delete root;
				root = next;
			}
		}

		//Оператор присваивания
		Bucket& operator=(Bucket other)
		{
			swap(*this, other);
			return *this;
		}

		//Конструктор копирования
		Bucket(const Bucket& other)
		{
			ListNode* other_cur = other.root;
			ListNode** cur = &root;
			ListNode* prev = nullptr;
			while (other_cur != nullptr)
			{
				*cur = new ListNode();
				(*cur)->prev = prev;
				(*cur)->value = other_cur->value;
				prev = *cur;
				cur = &((*cur)->next);
				other_cur = other_cur->next;
			}
		}

		bool empty() const
		{
			return root != nullptr;
		}
		struct ListNode
		{
			ListNode *prev = nullptr, *next = nullptr;
			int value;
		};

		// ReSharper disable once CppMemberFunctionMayBeConst
		void foreach(std::function<void(const int)> func)
		{
			ListNode* cur = root;
			while (cur != nullptr)
			{
				int value = cur->value; //allows foreach(delete number)
				cur = cur->next;		//
				func(value);
			}
		}
	private:


		ListNode* root = nullptr;

		//Конструктор переноса
		Bucket(Bucket&& other) :root(other.root)
		{
			other.root = nullptr;
		}

		//Оператор присваивания совсем не копирующий
		Bucket& operator=(Bucket&& other)
		{
			if (this == &other)
			{
				throw "You probably shouldn't have moved something into itself";
			}

			while (root != nullptr)
			{
				ListNode* next = root->next;
				delete root;
				root = next;
			}

			root = other.root;
			other.root = nullptr;

			return *this;
		}

		void swap(Bucket& first, Bucket& second) const
		{
			std::swap(first.root, second.root);
		}
	};

	unsigned int table_size;
	Bucket *table;

	void swap(MySet& first, MySet& second) const
	{
		std::swap(first.table_size, second.table_size);
		std::swap(first.table, second.table);
	}

	bool contains(const int number) const
	{
		return table[hash(number)].contains(number);
	}

	unsigned int hash(const int number) const
	{
		return (number >= 0) ? (number % table_size) : (table_size + ((number + 1) % table_size) - 1);
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void add(const int number)
	{
		table[hash(number)].add(number);
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void remove(const int number)
	{
		table[hash(number)].remove(number);
	}
};
