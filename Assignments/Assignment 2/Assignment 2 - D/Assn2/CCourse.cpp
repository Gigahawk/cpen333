#include <stdio.h>
#include <iostream>
#include "CCourse.h"
#include "CStudent.h"

#define UNDECLARED	0;
#define ENPH		1;
#define CPEN		2;
#define MECH		3;
#define IGEN		4;
#define ELEC		5;
#define CIVL		6;
#define CHBE		7;
#define BMEG		8;
#define GEOE		9;
#define MTRL		10;
#define MINE		11;
#define MANU		12;
#define ENVE		13;

using namespace std;

CCourse::CCourse() {
	cout << "CAdmin Default Constructor being called." << endl;
	courseDept = UNDECLARED;
	courseNum = 000;
}

CCourse::CCourse(int _courseDept, int _courseNum) {
	cout << "CAdmin Default Constructor being called." << endl;
	courseDept = _courseDept;
	courseNum = _courseNum;
}

CCourse::~CCourse() {
	cout << "CAdmin Default Destructor being called." << endl;
}

void CCourse::AddStudent(CStudent* aStudent) {
	theStudent = aStudent;
	theStudent->theCourse = this;
}

void CCourse::DelStudent() {
	if (theStudent != NULL) {
		theStudent->theCourse = NULL;
		theStudent = NULL;
	}
}

int CCourse::getCourseDept() {
	return courseDept;
}

int CCourse::getCourseNum() {
	return courseNum;
}