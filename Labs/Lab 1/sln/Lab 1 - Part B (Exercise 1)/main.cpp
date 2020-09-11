#include <iostream>
#include "List.h"

using namespace std;

int main() {
	cout << "Creating a new list" << endl;
	List *l = new List();


	cout << "Putting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Insert((i+1) * 10);
		cout << l->Get(i) << endl;
	}
	cout << "Deleting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Delete();
	}
	cout << "Deleting list (no further items should be deleted)" << endl;
	delete l;

	cout << endl;

	cout << "Reinitializing list" << endl;
	l = new List();
	cout << "Putting 10 items in list" << endl;
	for (int i = 0; i < 10; i++) {
		l->Insert((i+1) * 10);
		cout << l->Get(i) << endl;
	}
	cout << "Deleting 5 items in list" << endl;
	for (int i = 0; i < 5; i++) {
		l->Delete();
	}
	cout << "Deleting list (5 remaining items should be deleted)" << endl;
	delete l;
	return 0;
}