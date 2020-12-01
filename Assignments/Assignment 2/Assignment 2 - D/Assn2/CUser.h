#ifndef   CUser_H
#define   CUser_H

#include <stdio.h>
#include <iostream>

using namespace std;

class CUser {
private:
	string Name;
	long ID;
	string password;

public:
	CUser();
	~CUser();

	void setID(long _ID);
	void setName(string _name);

	long printID();
	string printName();
};

#endif