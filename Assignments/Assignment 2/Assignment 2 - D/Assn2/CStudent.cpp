#include <stdio.h>
#include <iostream>
#include "CStudent.h"
#include "CCourse.h"
#include "CUser.h"

// major codes
// ENPH, CPEN, MECH, IGEN, ELEC, CIVL, CHBE, BMEG, GEOE, MTRL, MINE, MANU, ENVE
#define UNDECLARED	0
#define ENPH		1
#define CPEN		2
#define MECH		3
#define IGEN		4
#define ELEC		5
#define CIVL		6
#define CHBE		7
#define BMEG		8
#define GEOE		9
#define MTRL		10
#define MINE		11
#define MANU		12
#define ENVE		13

using namespace std;

CStudent::CStudent() {
	cout << "CStudent Default Constructor being called." << endl;
	studentUser.setName("blank");
	studentUser.setID(00000000);
	yearLevel = 1;
	major = UNDECLARED;
}

CStudent::CStudent(string _Name, long _ID, int _year, int _major) {
	studentUser.setName(_Name);
	studentUser.setID(_ID);
	yearLevel = _year;
	major = _major;
}

CStudent::~CStudent() {
	cout << "CStudent Default Destructor being called." << endl;
}

void CStudent::AddCourse(CCourse* aCourse) {
	// test pre-reqs for registering course


	theCourse = aCourse;
	theCourse->theStudent = this;
}

void CStudent::DelCourse() {
	if (theCourse != NULL) {
		theCourse->theStudent = NULL;
		theCourse = NULL;
	}
}

void CStudent::PrintStudentDetails() {	// prints details of the student and his/her course
	cout << "My Name is: " << studentUser.printName() << endl;
	cout << "My ID is: " << studentUser.printID() << endl;
	if (theCourse != NULL) {	// provided association is valid
		cout << "I am Taking Course: " << theCourse->getCourseDept() << endl;
	}
}

void CStudent::payFees() {
	tuitionOwing = 0;
}

void CStudent::submitRanking(int _choice1, int _choice2, int _choice3) {
	// student lists as many majors as they want in order of preference
	// assume no caps on major size
	// check student's sessional average - if they meet threshold, let em in
	int choice1 = _choice1;
	int choice2 = _choice2;
	int choice3 = _choice3;


	if (sessionalAvg >= 85) {
		major = choice1;
	}
	else if ((sessionalAvg <= 84.9) && (sessionalAvg >= 75)) {
		// can register for everything except ENPH
		if (choice1 != ENPH) {
			major = choice1;
		}
		else {
			major = choice2;
		}
	}
	else if ((sessionalAvg <= 74.9) && (sessionalAvg >= 65)) {
		if (choice1 >= 4) { // can register for everything except ENPH, CPEN, MECH (1, 2, 3)
			major = choice1;
		}
		else if (choice2 >= 4) {
			major = choice2;
		}
		else if (choice3 >= 4) {
			major = choice3;
		}
		else {
			major = UNDECLARED;
		}
	}
	else if ((sessionalAvg <= 64.9) && (sessionalAvg >= 60)) {
		// can register for everything except ENPH, CPEN, MECH, IGEN, ELEC, CIVL, CHBE, BMEG
		if (choice1 >= 9) { // can register for everything except ENPH, CPEN, MECH (1, 2, 3)
			major = choice1;
		}
		else if (choice2 >= 9) {
			major = choice2;
		}
		else if (choice3 >= 9) {
			major = choice3;
		}
		else {
			major = UNDECLARED;
		}
	}
	else {
		// if you're below 60, you don't qualify for 2nd year promo anyways
		major = UNDECLARED;
	}
}

int CStudent::getYrLevel() {
	return yearLevel;
}

float CStudent::getSessAvg() {
	return sessionalAvg;
}

float CStudent::cheat(CCourse* aCourse) {
	return sessionalAvg;
}