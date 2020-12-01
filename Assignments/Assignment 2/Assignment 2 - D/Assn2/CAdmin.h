#ifndef   CAdmin_H
#define   CAdmin_H

#include <stdio.h>
#include <string>
#include "CUser.h"

class CStudent;

using namespace std;

class CAdmin {
	CUser adminUser;

public:
	CAdmin();
	~CAdmin();

	//functions
	CStudent* theStudent;
	void updateGrade(CStudent* aStudent);
	void updateProbationStatus(CStudent* aStudent);

};

#endif