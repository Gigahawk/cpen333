#include <stdio.h>
#include <iostream>
#include "CProf.h"
#include "CStudent.h"

using namespace std;

CProf::CProf() {
	cout << "CProf Default Constructor being called." << endl;
}

CProf::~CProf() {
	cout << "CProf Default Destructor being called." << endl;
}

void CProf::sus(CStudent* aStudent) {
	// read student's susLevel
	// if student's susLevel is > 80%,
	// send statement of case to president
}

void CProf::submitGrade(CStudent* aStudent) {
	// try to update student account to update transcript
	// student account cross-checks student ID with class list
}