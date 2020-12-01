#include <stdio.h>
#include <iostream>
#include "CUser.h"

using namespace std;

CUser::CUser() {
	cout << "CUser Default Constructor being called." << endl;
}

CUser::~CUser() {
	cout << "CUser Default Destructor being called." << endl;
}

void CUser::setID(long _ID) {
	ID = _ID;
}

void CUser::setName(string _name) {
	Name = _name;
}

long CUser::printID() {
	return ID;
}

string CUser::printName() {
	return Name;
}