#ifndef   CProf_H
#define   CProf_H

#include <stdio.h>
#include <string>
#include "CUser.h"

using namespace std;

class CProf {
	CUser profUser;

public:
	CProf();
	~CProf();

	//functions
	CStudent* theStudent;
	void sus(CStudent* aStudent); // 
	void submitGrade(CStudent* aStudent); // submit grade to admin
};

#endif