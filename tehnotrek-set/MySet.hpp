
//TODO: check main TODO
#define _SCL_SECURE_NO_WARNINGS
#pragma once
#include <functional>
#include <initializer_list>

#define MYSET_DEFAULT_TABLE_SIZE 256

class MySet
{
public:

	explicit MySet(unsigned int t_size = MYSET_DEFAULT_TABLE_SIZE);
	MySet(std::initializer_list<int> list, unsigned int t_size = MYSET_DEFAULT_TABLE_SIZE);
	MySet(const MySet& other);
	MySet(MySet&& other);

	~MySet();

	MySet& operator=(MySet other);
	MySet& operator=(int number);

	MySet& operator+=(const MySet& other);
	MySet& operator-=(const MySet& other);
	MySet& operator*=(const MySet& other);

	MySet& operator+=(const int number);
	MySet& operator-=(const int number);
	MySet& operator*=(const int other);

	friend MySet operator+(MySet left, const MySet& right);
	friend MySet operator-(MySet left, const MySet& right);
	friend MySet operator*(MySet left, const MySet& right);

	friend MySet operator+(MySet left, int right);
	friend MySet operator-(MySet left, int right);
	friend MySet operator*(MySet left, int right);

	friend MySet operator+(int left, MySet right);
	friend MySet operator-(int left, MySet right);
	friend MySet operator*(int left, MySet right);

	void foreach(std::function<void(const int)> func);
	void clear();
	bool contains(const int number) const;

private:
	class Bucket
	{
	public:

		Bucket() = default;
		Bucket(const Bucket& other);
		Bucket(Bucket&& other);

		~Bucket();

		Bucket& operator=(Bucket other);

		void add(const int number);
		void remove(const int number);
		void clear();

		bool contains(const int number) const;
		bool empty() const;
		
		void foreach(std::function<void(const int)> func);

	private:
		struct ListNode
		{
			ListNode *prev = nullptr, *next = nullptr;
			int value;
		};

		ListNode* root = nullptr;

		void swap(Bucket& first, Bucket& second) const;
	};

	unsigned int table_size;
	Bucket *table;

	void swap(MySet& first, MySet& second) const;

	unsigned int hash(const int number) const;
	void add(const int number);
	void remove(const int number);
};
