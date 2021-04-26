/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: Main.cpp
Project: CS280 Assignment 3
Author: Kevin Wright
Created: 2/8/21
******************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <set>

#include "ChHashTable.h"

struct Person
{
	unsigned int ID;
	char lastName[26];
	char firstName[26];
	float salary;
	unsigned years;
};

Person peopleData[] = 
{
  {101001, "Bruce", "Banner", 80000, 10},
  {102001, "Thor", "", 90000, 12},
  {103001, "Clint", "barton", 50000, 4},
  {104001, "Jane", "Foster", 44000, 1},
  {105001, "Wanda", "Maximoff", 50000, 4},
  {106001, "Pietro", "Maximoff", 40000, 1},
  {107001, "Vision", "", 80000, 10},
  {108001, "T'Challa", "", 17000, 9},
  {109001, "Shuri", "", 100000, 4},
  {110001, "Okoye", "", 10000, 3},
  {111001, "James", "Rhodes", 80000, 12},
  {112001, "Sam", "Wilson", 90000, 8},
  {113001, "Peter", "Parker", 92000, 8},
  {114001, "Myles", "Morales", 37000, 2},
  {115001, "Peter", "Quill", 62000, 3},
  {116001, "Gamora", "", 79000, 3},
  {117001, "Rocket", "Racoon", 60000, 5},
  {118001, "Groot", "", 25000, 1},
  {119001, "Drax", "The Destroyer", 48000, 4},
  {120001, "Mantis", "", 32000, 3},
  {121001, "Carol", "Danvers", 58000, 2},
  {122001, "Nebula", "", 64000, 3},
  {123001, "Scott", "Lang", 85000, 3},
  {124001, "Bucky", "Barnes", 19000, 5},
  {125001, "Steve", "Rogers", 95000, 8},
  {126001, "Tony", "Stark", 145000, 2},
  {127001, "Natasha", "Romanoff", 79000, 7},
  {128001, "Nick", "Fury", 92000, 6},
  {129001, "Phil", "Colson", 25000, 6},
  {130001, "Peggy", "Carter", 105000, 26},
  {131001, "Hank", "Pym", 95000, 16},
};

std::ostream& operator<<(std::ostream& os, const Person& person)
{
	os << std::setw(20) << std::left << (person.lastName + std::string(", ") + person.firstName);
	os << "Salary: " << person.salary << "\tYears: " << person.years;
	return os;
}

void GetRandomValues(unsigned* array, int size)
{
	std::mt19937 rand(1);	// Hard coded to this because the numbers looked good
	std::uniform_int_distribution<unsigned> dist(0, 0xFFFFFFFF);
	std::set<unsigned> set;
	for (int i = 0; i < size;) {
		int val = dist(rand);
		if (set.find(val) == set.end()) {
			set.insert(val);
			array[i++] = val;
		}
	}
}

void TestInsertBasic()
{
	std::cout << "==================== Insert Test Basic ====================" << std::endl;

	ChHashTable<void*> hash(11, 3.0, 2.0);
	const unsigned int size = 11;
	for (unsigned i = 101; i < size + 101; i++) {
		hash.Insert(i, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	std::cout << std::endl;
}

void TestInsertProbes()
{
	std::cout << "==================== Insert Test Probes ====================" << std::endl;

	ChHashTable<void*> hash(11, 3.0, 2.0);
	const unsigned size = 11;
	unsigned values[size];
	GetRandomValues(values, size);
	for (unsigned i = 0; i < size; i++) {
		hash.Insert(values[i], &values[size]);

		hash.DumpTable();
		hash.DumpStats();
	}
	std::cout << std::endl;
}

void TestInsertRand22()
{
	std::cout << "==================== Insert Test Rand 22 ====================" << std::endl;

	ChHashTable<unsigned int*> hash(11, 3.0, 2.0);
	const unsigned size = 22;
	unsigned values[size];
	GetRandomValues(values, size);
	for (unsigned i = 0; i < size; i++) {
		hash.Insert(values[i], &values[size]);
	}
	hash.DumpTable();
	hash.DumpStats();
	std::cout << std::endl;
}

void TestInsertPerson()
{
	std::cout << "==================== Insert Test Person ====================" << std::endl;

	ChHashTable<Person> hash(11, 3.0, 2.0);
	const unsigned size = sizeof(peopleData) / sizeof(*peopleData);
	for (unsigned i = 0; i < size; i++) {
		hash.Insert(peopleData[i].ID, peopleData[i]);
	}

	hash.DumpTable();
	hash.DumpStats();

	std::cout << std::endl;
}

void TestInsertDuplicate()
{
	std::cout << "==================== Insert Test Duplicate ====================" << std::endl;

	ChHashTable<int*> hash(11, 3.0, 2.0);
	const unsigned int size = 11;
	for (unsigned i = 101; i < size + 101; i++)
	{
		hash.Insert(i, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(-104, nullptr);

	try 
	{
		std::cout << "Insert a duplicate item" << std::endl;
		hash.Insert(104, nullptr);
	}
	catch (HashTableException& e)
	{
		std::cout << "Caught HashTableException - " << e.what() << std::endl;
	}
	std::cout << std::endl;
}

void TestFind()
{
	std::cout << "==================== Find Test ====================" << std::endl;

	ChHashTable<unsigned int> hash(11, 3.0, 2.0);
	const unsigned size = 11;
	unsigned values[size];
	GetRandomValues(values, size);
	for (unsigned i = 0; i < size; i++)
	{
		hash.Insert(values[i], values[i]);
	}
	hash.DumpTable();
	hash.DumpStats();

	for (unsigned i = 0; i < size; i++)
	{
		std::cout << "hash[" << i << "] = Key:" << values[i] << "\tData:" << hash[values[i]] << std::endl;
		
		hash.DumpStats();
	}
	std::cout << std::endl;
}


void TestFindPerson()
{
	std::cout << "==================== Find Test Person ====================" << std::endl;

	ChHashTable<Person> hash(11, 3.0, 2.0);
	const unsigned size = sizeof(peopleData) / sizeof(*peopleData);
	for (unsigned i = 0; i < size; i++) 
	{
		hash.Insert(peopleData[i].ID, peopleData[i]);
	}

	hash.DumpStats();
	std::cout << std::endl;

	std::cout << std::setprecision(6);
	for (unsigned i = 0; i < size; i++) 
	{
		std::cout << "hash[" << i << "] = Key:" << peopleData[i].ID << "\tPerson:" << hash[peopleData[i].ID] << std::endl;
	}
	hash.DumpStats();
	std::cout << std::endl;
}

void TestFindNonFound()
{
	std::cout << "==================== Find Test Not Found ====================" << std::endl;

	ChHashTable<unsigned int> hash(11, 3.0, 2.0);
	try 
	{
		unsigned int result = hash[0];
		std::cout << "hash[" << 0 << "] = Key:" << 0 << "\tData:" << result << std::endl;
	}
	catch (HashTableException& e) 
	{
		std::cout << "Caught HashTableException - " << e.what() << std::endl;
	}

	const unsigned size = 30;
	unsigned values[size];
	GetRandomValues(values, size);
	for (unsigned i = 0; i < size-3; i++) 
	{
		hash.Insert(values[i], values[i]);
	}
	hash.DumpTable();
	hash.DumpStats();

	for (unsigned i = 0; i < size; i++) 
	{
		try 
		{
			unsigned int result = hash[values[i]];
			std::cout << "hash[" << i << "] = Key:" << values[i] << "\tData:" << result << std::endl;
		}
		catch (HashTableException& e) 
		{
			std::cout << "Caught HashTableException - " << e.what() << std::endl;
		}
	}
	hash.DumpStats();
	std::cout << std::endl;
}

void TestRemovePerson()
{
	std::cout << "==================== Remove Test Person ====================" << std::endl;

	ChHashTable<Person> hash(11, 3.0, 2.0);
	const unsigned size = sizeof(peopleData) / sizeof(*peopleData);
	for (unsigned i = 0; i < size; i++) 
	{
		hash.Insert(peopleData[i].ID, peopleData[i]);
	}

	hash.DumpStats();
	std::cout << std::endl << "Remove People" << std::endl;
	hash.Remove(106001);
	hash.Remove(107001);
	hash.Remove(116001);
	hash.Remove(129001);
	hash.Remove(130001);

	try 
	{
		hash.Remove(999999);
	}
	catch (HashTableException& e) 
	{
		std::cout << "Caught HashTableException - " << e.what() << std::endl;
	}

	std::cout << std::setprecision(6);
	for (unsigned i = 0; i < size; i++) 
	{
 		try 
		{
			Person result = hash[peopleData[i].ID];
			std::cout << "hash[" << i << "] = Key:" << peopleData[i].ID << "\tPerson:" << result << std::endl;
		}
		catch (HashTableException& e) 
		{
			std::cout << "Caught HashTableException - " << e.what() << std::endl;
		}
	}
	hash.DumpStats();
	std::cout << std::endl;
}

void TestResize()
{
	std::cout << "==================== Resize Test ====================" << std::endl;

	ChHashTable<void*> hash(6, 2.0, 2.0);
	int index = 0;
	for (; index < 12; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
	for (++index; index < 24; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
	for (++index; index < 48; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
}

void TestResize2()
{
	std::cout << "==================== Resize Test 2 ====================" << std::endl;

	ChHashTable<void*> hash(6, 2.8, 1.5);
	int index = 0;
	for (; index < 16; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
	for (++index; index < 25; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
	for (++index; index < 39; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Insert(index, nullptr);
	hash.DumpTable();
	hash.DumpStats();
}

void TestNoResize()
{
	std::cout << "==================== Resize Test No Change ====================" << std::endl;

	ChHashTable<char*> hash(11, 5.0, 2.0);
	const unsigned size = 45;
	unsigned values[size];
	GetRandomValues(values, size);
	for (unsigned i = 0; i < size; i++) 
	{
		hash.Insert(values[i], nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	std::cout << std::endl;
}

void TestClear()
{
	std::cout << "==================== Clear Test ====================" << std::endl;

	ChHashTable<void*> hash(6, 3.0, 1.5);
	int index = 0;
	for (; index < 30; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
	hash.Clear();
	hash.DumpTable();
	hash.DumpStats();
	for (; index < 60; index++) 
	{
		hash.Insert(index, nullptr);
	}
	hash.DumpTable();
	hash.DumpStats();
}

int main()
{
	int test_num = -1;
#ifndef _MSC_VER
	std::cin >> test_num;
#endif

	void(*tests[])() = 
	{
		TestInsertBasic, TestInsertProbes, TestInsertRand22, TestInsertPerson, TestInsertDuplicate,
		TestFind, TestFindPerson, TestFindNonFound,
		TestRemovePerson, 
		TestResize, TestResize2, TestNoResize,
		TestClear,
	};
	int numTests = static_cast<int>(sizeof(tests) / sizeof(*tests));

	try 
	{
		if (test_num > 0) 
		{
			tests[test_num - 1]();
		}
		else 
		{
			for (int i = 0; i < numTests; i++) 
			{
				tests[i]();
			}
		}
	}
	catch (HashTableException& e) 
	{
		std::cout << "Caught HashTableException: " << e.what() << std::endl;
	}
	catch (std::exception& e) 
	{
		std::cout << "Caught unknown exception: " << e.what() << std::endl;
	}
}
