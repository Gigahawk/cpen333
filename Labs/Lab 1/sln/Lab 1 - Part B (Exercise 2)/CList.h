#ifndef CLIST_H
#define CLIST_H

#include <iostream>
#include "CList.h"

using namespace std;

template<typename T>
struct Node {
	Node(T val)
		: next(nullptr), val(val)
	{}
	Node<T>* next;
	T val;
};

template<typename T>
class CList
{
public:
	CList<T>();
	~CList<T>();
	void Insert(T val);
	void Delete();
	T Get(int idx);

private:
	Node<T>* ptr;
};

// Template classes must be precompiled, and hence implementation must be in the header
// Alternatively, the implementation file (.cpp) must be included in main.cpp, etc.

template<typename T>
CList<T>::CList()
	:ptr(nullptr)
{
	cout << "CList constructor called" << endl;
}

template<typename T>
CList<T>::~CList()
{
	cout << "CList destructor called" << endl;
	Node<T>* next;
	string val;
	while (ptr) {
		next = ptr->next;
		// This line only compiles if every type using it implements the << operator
		// Comment out if that isn't the case
		cout << "Deleting node with value " << ptr->val << endl;
		delete ptr;
		ptr = next;
	}
}

template<typename T>
void CList<T>::Insert(T val)
{
	auto ptr = this->ptr;
	auto n = new Node<T>(val);
	n->val = val;
	if (!ptr) {
		this->ptr = n;
		return;
	}

	while (ptr->next)
		ptr = ptr->next;
	ptr->next = n;
}

template<typename T>
void CList<T>::Delete()
{
	auto ptr = this->ptr;
	if (!ptr->next) {
		delete ptr;
		this->ptr = nullptr;
		return;
	}
	while (ptr->next->next)
		ptr = ptr->next;
	delete ptr->next;
	ptr->next = nullptr;
}

template<typename T>
T CList<T>::Get(int idx)
{
	auto ptr = this->ptr;
	for (int i = 0; i < idx; i++) {
		ptr = ptr->next;
	}
	return ptr->val;
}

#endif
