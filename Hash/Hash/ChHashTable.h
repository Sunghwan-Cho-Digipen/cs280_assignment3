/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: ChHashTable.h
Project: CS280 Assignment 3
Author: Kevin Wright, Sunghwan Cho
Created: 04/26/2021
******************************************************************/
#pragma once

#include <string>
#include <stdexcept>


class HashTableException : public std::runtime_error {
public:
	HashTableException(const std::string& message) : runtime_error(message) {}
};


template <typename T>
class ChHashTable {
public:
	ChHashTable(unsigned initialTableSize, double maxLoadFactor = 3.0, double growthFactor = 2.0);
	~ChHashTable();

	void Insert(unsigned key, const T& data);
	void Remove(unsigned key);
	void Clear();
	const T&operator [](unsigned key);

	void DumpTable();
	void DumpStats();
private:
	struct HashStats
	{
		HashStats(unsigned initialTableSize) : count(0), tableSize(initialTableSize), probes(0), expansions(0), allocations(0) {};
		unsigned count;      // Number of elements in the table
		unsigned tableSize;  // Size of the table (total slots)
		unsigned probes;     // Number of probes performed
		unsigned expansions; // Number of times the table grew
		unsigned allocations;// Number of nodes created
	};
	
	struct Node
	{
		Node(T data, unsigned key, Node* pNext = nullptr) : data(data), key(key), pNext(pNext) {}
		T data;
		unsigned key;
		Node* pNext;
	};
	
	struct SinglyLinkedList
	{
	public:

		SinglyLinkedList(Node* pHead = nullptr) : pHead(pHead) {};
		~SinglyLinkedList();
		
		Node* Find(unsigned Key, unsigned& count);
		void CreateAndAddNode(unsigned key, const T& data); // This is used until loadFactor > maxLoadFactor
		void AddNodeToList(Node* node); // This is used in re allocation
		void Delete(unsigned key, unsigned& count);
		void Clear();
		void PrintAll();
		int GetSize();
		bool DoesKeyMatches(unsigned key);

		
		Node* pHead;
		int size = 0;
	};

	SinglyLinkedList* container;
	
	unsigned Hash(unsigned key) const;

	HashStats stats;
	unsigned initialTableSize;
	double maxLoadFactor;
	double growthFactor;
};



#include "ChHashTable.hpp"
