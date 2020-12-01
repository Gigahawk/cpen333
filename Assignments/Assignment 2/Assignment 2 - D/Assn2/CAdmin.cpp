#include <stdio.h>
#include <iostream>
#include "CAdmin.h"
#include "CStudent.h"

// academic standings
#define GOOD		0
#define PROBATION	1
#define FAILED		2

// settings
#define FALSE		0
#define TRUE		1

using namespace std;

CAdmin::CAdmin() {
	cout << "CAdmin Default Constructor being called." << endl;
}

CAdmin::~CAdmin() {
	cout << "CAdmin Default Destructor being called." << endl;
}

void CAdmin::updateGrade(CStudent* aStudent) {
	// it's just a bunch of ifs
}

void CAdmin::updateProbationStatus(CStudent* aStudent) {
	theStudent = aStudent;
	int yrLevel;
	yrLevel = theStudent->yearLevel;

	float sessAvg;
	sessAvg = theStudent->sessionalAvg;

	int standing;
	standing = theStudent->standing;

	if (yrLevel == 1) {
		if (sessAvg >= 60.0) {
			//assign "good" standing
		}
		if ((sessAvg <= 59.9) && (sessAvg >= 50.0) && (standing != PROBATION)) {
			//assign "probation" standing
			//banned
		}
		if ((sessAvg <= 59.9) && (sessAvg >= 50.0) && (standing == PROBATION)) {
			//assign "failed year" standing
			//student.banned == TRUE;
		}
		else { // you def failed
			//assign "failed year" standing
			//student.banned == TRUE;
		}
	}
	else if (yrLevel >= 2) {
		if (sessAvg >= 55.0) {
			//assign "good" standing
		}
		if ((sessAvg <= 54.9) && (sessAvg >= 50.0) && (standing != PROBATION)) {
			//assign "probation" standing
			//not banned
		}
		if ((sessAvg <= 54.9) && (sessAvg >= 50.0) && (standing == PROBATION)) {
			//assign "failed" standing
			//student.banned == TRUE;
		}
		else { // you def failed
			aStudent->standing == FAILED;
			//student.banned == TRUE;
		}
	}
}