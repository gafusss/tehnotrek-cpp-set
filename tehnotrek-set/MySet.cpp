#include "MySet.hpp"

// ReSharper disable once CppMemberFunctionMayBeConst
void MySet::clear()
{
	for (unsigned int i = 0; i < table_size; i++)
	{
		table[i].clear();
	}
}

MySet::MySet(unsigned int t_size):
	table_size(t_size > MYSET_DEFAULT_TABLE_SIZE ? t_size : MYSET_DEFAULT_TABLE_SIZE),
	table(new Bucket[table_size])
{}

MySet::MySet(std::initializer_list<int> list, unsigned int t_size):
	MySet(t_size)
{
	for (int number : list)
	{
		add(number);
	}
}

MySet::MySet(const MySet& other):table_size(other.table_size), table(new Bucket[table_size])
{
	std::copy(other.table, other.table + table_size, table);
}

MySet::MySet(MySet&& other):table_size(other.table_size), table(other.table)
{
	other.table = nullptr;
	other.table_size = 0;
}

MySet::~MySet()
{
	delete[] table;
}

MySet& MySet::operator=(MySet other)
{
	swap(*this, other);
	return *this;
}

MySet& MySet::operator=(int number)
{
	MySet tmp({number});
	swap(*this, tmp);
	return *this;
}

MySet& MySet::operator+=(const MySet& other)
{
	for (unsigned int i = 0; i < other.table_size; i++)
	{
		other.table[i].foreach([this](const int number) -> void {this->add(number);});
	}
	return *this;
}

MySet& MySet::operator-=(const MySet& other)
{
	for (unsigned int i = 0; i < other.table_size; i++)
	{
		other.table[i].foreach([this](const int number) -> void {this->remove(number);});
	}
	return *this;
}

MySet& MySet::operator*=(const MySet& other)
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

MySet& MySet::operator+=(const int number)
{
	add(number);
	return *this;
}

MySet& MySet::operator-=(const int number)
{
	remove(number);
	return *this;
}

MySet& MySet::operator*=(const int other)
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

// ReSharper disable once CppMemberFunctionMayBeConst
void MySet::foreach(std::function<void(const int)> func)
{
	for (unsigned int i = 0; i < table_size; i++)
	{
		table[i].foreach(func);
	}
}

bool MySet::Bucket::contains(const int number) const
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

void MySet::Bucket::add(const int number)
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

void MySet::Bucket::remove(const int number)
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

MySet::Bucket::~Bucket()
{
	clear();
}

void MySet::Bucket::clear()
{
	while (root != nullptr)
	{
		ListNode* next = root->next;
		delete root;
		root = next;
	}
}

MySet::Bucket& MySet::Bucket::operator=(Bucket other)
{
	swap(*this, other);
	return *this;
}

MySet::Bucket::Bucket(const Bucket& other)
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

bool MySet::Bucket::empty() const
{
	return root != nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MySet::Bucket::foreach(std::function<void(const int)> func)
{
	ListNode* cur = root;
	while (cur != nullptr)
	{
		int value = cur->value; //allows foreach(delete number)
		cur = cur->next;		//
		func(value);
	}
}

MySet::Bucket::Bucket(Bucket&& other):root(other.root)
{
	other.root = nullptr;
}

void MySet::Bucket::swap(Bucket& first, Bucket& second) const
{
	std::swap(first.root, second.root);
}

void MySet::swap(MySet& first, MySet& second) const
{
	std::swap(first.table_size, second.table_size);
	std::swap(first.table, second.table);
}

bool MySet::contains(const int number) const
{
	return table[hash(number)].contains(number);
}

unsigned MySet::hash(const int number) const
{
	return (number >= 0) ? (number % table_size) : (table_size + ((number + 1) % table_size) - 1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MySet::add(const int number)
{
	table[hash(number)].add(number);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void MySet::remove(const int number)
{
	table[hash(number)].remove(number);
}

MySet operator+(MySet left, const MySet& right)
{
	left += right;
	return left;
}

MySet operator-(MySet left, const MySet& right)
{
	left -= right;
	return left;
}

MySet operator*(MySet left, const MySet& right)
{
	left *= right;
	return left;
}

MySet operator+(MySet left, int right)
{
	left += right;
	return left;
}

MySet operator-(MySet left, int right)
{
	left -= right;
	return left;
}

MySet operator*(MySet left, int right)
{
	left *= right;
	return left;
}

MySet operator+(int left, MySet right)
{
	right += left;
	return right;
}

MySet operator-(int left, MySet right)
{
	right -= left;
	return right;
}

MySet operator*(int left, MySet right)
{
	right *= left;
	return right;
}
