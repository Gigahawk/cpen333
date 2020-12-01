#ifndef   CPres_H
#define   CPres_H

#include <stdio.h>
#include <string>
#include "CUser.h"
#include "CStudent.h"

using namespace std;

class CPres {
	CUser presUser;

public:
	CPres();
	~CPres();

	CStudent* theStudent;
	void investigate(CStudent* aStudent);
	void updateGrade(CStudent* aStudent);
	void updateRecord(CStudent* aStudent);
	void checkRecord(CStudent* aStudent);
	void suspendStudent(CStudent* aStudent);
	void updateProbationStatus(CStudent* aStudent);
};

#endif