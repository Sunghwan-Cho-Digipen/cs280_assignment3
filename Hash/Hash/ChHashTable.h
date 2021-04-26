/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: ChHashTable.h
Project: CS280 Assignment 3
Author: 
Created: 
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
	
	// You will need to create your own link list nodes (I would suggest singly because it's easier and less overhead, less chance for bugs)
	// Your private data can be however you like 
	struct SinglyLinkedList
	{
		struct Node
		{
			Node(T data, Node* pPrev = nullptr, Node* pNext = nullptr) : data(data), pPrev(pPrev), pNext(pNext) {}
			T data;
			Node* pNext;
		};

		SinglyLinkedList(Node* pHead = nullptr) : pHead(pHead), pTail(pHead) {};

		void Add(const T& data);
		void Delete(const T& data);
		void Clear();
		
		Node* pHead;
		Node* pTail;
	};

	SinglyLinkedList* container;
	
	unsigned Hash(unsigned key) const;

	HashStats stats;
	unsigned initialTableSize;
	double maxLoadFactor;
	double growthFactor;
};



#include "ChHashTable.hpp"
