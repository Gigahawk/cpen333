#include <iostream>
#include "List.h"

using namespace std;

List::List()
	:ptr(nullptr)
{
	cout << "List constructor called" << endl;
}

List::~List()
{
	cout << "List destructor called" << endl;
	Node* next;
	while (ptr) {
		next = ptr->next;
		cout << "Deleting node with value " << ptr->val << endl;
		delete ptr;
		ptr = next;
	}
}

void List::Insert(int val)
{
	auto ptr = this->ptr;
	auto n = new Node(val);
	n->val = val;
	if (!ptr) {
		this->ptr = n;
		return;
	}

	while (ptr->next)
		ptr = ptr->next;
	ptr->next = n;
}

void List::Delete()
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

int List::Get(int idx)
{
	auto ptr = this->ptr;
	for (int i = 0; i < idx; i++) {
		ptr = ptr->next;
	}
	return ptr->val;
}

