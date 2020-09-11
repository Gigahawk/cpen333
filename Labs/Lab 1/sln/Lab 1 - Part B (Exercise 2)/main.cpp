#include <iostream>
#include "CBulb.h"
#include "CSwitch.h"
#include "CList.h"

using namespace std;

template <typename T>
class Test {
public:
	Test();
	T val;
};

template <typename T>
Test<T>::Test() {
	cout << "ASD" << endl;
}

int main() {
	cout << "Creating a new CList() of double" << endl;
	CList<double>* l = new CList<double>;

	cout << "Putting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Insert(3.1415 * i);
		cout << l->Get(i) << endl;
	}
	cout << "Deleting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Delete();
	}
	cout << "Deleting list (no further items should be deleted)" << endl;
	delete l;

	cout << endl;

	cout << "Reinitializing Clist() with double" << endl;
	l = new CList<double>();
	cout << "Putting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Insert(3.1415 * i);
		cout << l->Get(i) << endl;
	}
	cout << "Deleting 5 items in list" << endl;
	for (int i = 0; i < 5; i++) {
		l->Delete();
	}
	cout << "Deleting list (5 remaining items should be deleted)" << endl;
	delete l;

	cout << endl;

	cout << "Creating a new CList() of CBulb*" << endl;
	CList<CBulb*> *bs = new CList<CBulb*>();
	// Wrong type
	//bs->Insert(10);
	cout << "Inserting 10 bulbs" << endl;
	for (int i = 0; i < 10; i++) {
		bs->Insert(new CBulb(i * 10));
		if (i % 2)
			bs->Get(i)->on();
		cout << *(bs->Get(i)) << endl;
	}
	cout << "Deleting list (10 bulbs should be deleted)" << endl;
	delete bs;

	cout << "Creating a new CList() of CSwitch*" << endl;
	CList<CSwitch*> *ss = new CList<CSwitch*>();
	cout << "Inserting 10 switches" << endl;
	for (int i = 0; i < 10; i++) {
		ss->Insert(new CSwitch());
		if (i % 2)
			ss->Get(i)->turnon();
		cout << *(ss->Get(i)) << endl;
	}
	cout << "Deleting list (10 bulbs should be deleted)" << endl;
	delete ss;

	cout << endl;

	cout << "Creating a new CList() of CBulb" << endl;
	CList<CBulb> *b = new CList<CBulb>();
	cout << "Inserting 10 bulbs" << endl;
	for (int i = 0; i < 10; i++) {
		b->Insert(CBulb(i * 10));
		// This won't work, Get() will produce a copy of the item in the list
		// Calling on() on it will just affect our local copy and then immediately
		// destruct it.
		//if (i % 2)
		//	b->Get(i).on();
	}
	cout << "Deleting list (10 bulbs should be deleted)" << endl;
	delete b;

	cout << "Creating a new CList() of CSwitch" << endl;
	CList<CSwitch> *s = new CList<CSwitch>();
	cout << "Inserting 10 switches" << endl;
	for (int i = 0; i < 10; i++) {
		s->Insert(CSwitch());
	}
	cout << "Deleting list (10 switches should be deleted)" << endl;
	delete s;


	return 0;
}
