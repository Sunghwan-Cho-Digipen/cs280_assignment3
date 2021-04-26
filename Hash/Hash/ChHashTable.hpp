/******************************************************************
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

File Name: ChHashTable.hpp
Project: CS280 Assignment 3
Author: Kevin Wright, Sunghwan Cho
Created: 04/26/2021
******************************************************************/
#pragma once

#include "ChHashTable.h"
#include <cmath>

template <typename T>
ChHashTable<T>::ChHashTable(unsigned initialTableSize, double maxLoadFactor, double growthFactor)
	: container(new SinglyLinkedList[initialTableSize]), stats(HashStats(initialTableSize)),
	initialTableSize(initialTableSize), maxLoadFactor(maxLoadFactor),
	growthFactor(growthFactor)
{
	
}

template <typename T>
ChHashTable<T>::~ChHashTable()
{
	Clear();
	delete[] container;
}

template <typename T>
void ChHashTable<T>::Insert(unsigned key, const T& data)
{
	const double LOAD_FACTOR = static_cast<double>(++stats.count) / static_cast<double>(stats.tableSize);
	if (LOAD_FACTOR > maxLoadFactor)
	{
		SinglyLinkedList* legacyContainer = container;
		const unsigned TOTAL_SIZE = stats.tableSize;
		const unsigned NEW_TABLE_SIZE = static_cast<unsigned>(std::ceil(TOTAL_SIZE * growthFactor));
		stats.tableSize = NEW_TABLE_SIZE;
		container = new SinglyLinkedList[NEW_TABLE_SIZE];
		for (unsigned i = 0; i < TOTAL_SIZE; ++i)
		{
			SinglyLinkedList& SLL = legacyContainer[i];
			while (SLL.pHead != nullptr)
			{
				Node* currentNode = SLL.pHead;
				SLL.pHead = currentNode->pNext;
				currentNode->pNext = nullptr;
				--SLL.size;
				container[Hash(currentNode->key)].AddNodeToList(currentNode);
			}
		}
		++stats.expansions;
		delete[] legacyContainer; 
	}
	SinglyLinkedList& SLL = container[Hash(key)];
	stats.probes += SLL.GetSize();
	SLL.CreateAndAddNode(key, data);
	++stats.allocations;
}

template <typename T>
void ChHashTable<T>::Remove(unsigned key)
{
	SinglyLinkedList& SLL = container[Hash(key)];
	unsigned count = 0;
	const int ORIGINAL_LIST_SIZE = SLL.GetSize();
	SLL.Delete(key, count);
	stats.probes += count;
	const int CHANGED_LIST_SIZE = SLL.GetSize();
	if (ORIGINAL_LIST_SIZE == CHANGED_LIST_SIZE)
	{
		std::string string = "Hash Key: " + std::to_string(key) + " not found";
		throw HashTableException(string);
	}
	--stats.count;
}

template <typename T>
void ChHashTable<T>::Clear()
{
	const int TOTAL_SIZE = stats.tableSize;
	for(int i = 0; i < TOTAL_SIZE; ++i)
	{
		container[i].Clear();
	}
	stats.count = 0;
}

template <typename T>
const T& ChHashTable<T>::operator[](unsigned key)
{
	unsigned count = 0;
	Node* targetNode = container[Hash(key)].Find(key, count);
	stats.probes += count;

	if(targetNode == nullptr)
	{
		std::string string = "Hash Key: " + std::to_string(key) + " not found";
		throw HashTableException(string);
	}
	
	return targetNode->data;
} 

template <typename T>
unsigned ChHashTable<T>::Hash(unsigned key) const
{ 
	return key % stats.tableSize;
}

template <typename T>
void ChHashTable<T>::DumpTable()
{
	std::cout << "Initial size: " << initialTableSize << std::endl;
	std::cout << "Max load factor: " << maxLoadFactor << std::endl;
	std::cout << "Growth factor: " << growthFactor << std::endl;

	for (unsigned int i = 0; i < stats.tableSize; i++) 
	{
		std::cout << "Slot: " << std::setw(3) << i;
		
		container[i].PrintAll();
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template <typename T>
void ChHashTable<T>::DumpStats()
{
	std::cout << "Number of probes: " << stats.probes << std::endl;
	std::cout << "Number of expansions: " << stats.expansions << std::endl;
	std::cout << "Items: " << stats.count << ", TableSize: " << stats.tableSize << std::endl;
	std::cout << "Load factor: " << std::setprecision(3) << (double)stats.count / (double)stats.tableSize << std::endl;
	std::cout << "Nodes created: " << stats.allocations << std::endl;
	std::cout << std::endl;
}

template <typename T>
ChHashTable<T>::SinglyLinkedList::~SinglyLinkedList()
{
	Clear();
}

template <typename T>
typename ChHashTable<T>::Node* ChHashTable<T>::SinglyLinkedList::Find(unsigned Key, unsigned& count)
{
	for (Node* currentNode = pHead; currentNode != nullptr; currentNode = currentNode->pNext)
	{
		++count; // I need to do this because of the increasment of probe
		if (currentNode->key == Key)
		{
			return currentNode;
		}
	}

	return nullptr;
}


template <typename T>
void ChHashTable<T>::SinglyLinkedList::CreateAndAddNode(unsigned key, const T& data)
{
	if (DoesKeyMatches(key) == true)
	{
		std::string string = "Hash Key: " + std::to_string(key) + " already exists";
		throw HashTableException(string);
	}
	
	if(pHead == nullptr)
	{
		pHead = new Node(data, key);
		++size;
		return;
	}
	
	Node* newNode = new Node(data, key);
	newNode -> pNext = pHead;
	pHead = newNode;
	++size;
}

template <typename T>
void ChHashTable<T>::SinglyLinkedList::AddNodeToList(Node* node)
{
	if(pHead == nullptr)
	{
		pHead = node;
		++size;
		return;
	}

	node->pNext = pHead;
	pHead = node;
	++size;
}

template <typename T>
void ChHashTable<T>::SinglyLinkedList::Delete(unsigned key, unsigned& count)
{
	if (pHead == nullptr)
	{
		return;
	}
	
	++count;// I need to do this because of the increasment of probe
	if (pHead->key == key)
	{
		Node* targetNode = pHead;
		pHead = targetNode->pNext;
		delete targetNode;
		--size;
		return;
	}

	for (Node* currentNode = pHead; currentNode->pNext != nullptr; currentNode = currentNode->pNext)
	{
		++count;// I need to do this because of the increasment of probe
		if (currentNode->pNext->key == key)
		{
			Node* targetNode = currentNode->pNext;
			currentNode->pNext = targetNode->pNext;
			delete targetNode;
			--size;
			return;
		}
	}
}

template <typename T>
void ChHashTable<T>::SinglyLinkedList::Clear()
{
	Node* currentNode = pHead;
	while(currentNode != nullptr)
	{
		Node* nextNode = currentNode->pNext;
		delete currentNode;
		currentNode = nextNode;
	}
	pHead = nullptr;
	size = 0;
}

template <typename T>
void ChHashTable<T>::SinglyLinkedList::PrintAll()
{
	for (Node* currentNode = pHead; currentNode != nullptr; currentNode = currentNode->pNext)
	{
		std::cout << " --> " << currentNode->key;
	}
}

template<typename T>
int ChHashTable<T>::SinglyLinkedList::GetSize()
{
	return size;
}


template <typename T>
bool ChHashTable<T>::SinglyLinkedList::DoesKeyMatches(unsigned key)
{
	if(pHead == nullptr)
	{
		return false;
	}

	for(Node* currentNode = pHead; currentNode != nullptr; currentNode = currentNode->pNext)
	{
		if(currentNode->key == key)
		{
			return true;
		}
	}

	return false;
}
