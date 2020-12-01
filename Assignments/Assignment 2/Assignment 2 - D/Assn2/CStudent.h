#ifndef   CStudent_H
#define   CStudent_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "CUser.h"

// academic standings
#define GOOD		0
#define PROBATION	1
#define FAILED		2

// settings
#define FALSE		0
#define TRUE		1

using namespace std;

class CCourse;

class CStudent {
	CUser studentUser;
	
	float sessionalAvg = 60;
	int standing = GOOD; // default GOOD

	int coursesRegistered = 0; //default
	float tuitionOwing = 0;

	int instancesMisconduct = 0; // default
	bool banned = FALSE; //not banned from registration
	float susLevel = 0; // random float bt 0 & 1, if susLevel is above 0.5, and prof checks, then get sussed.
	
	int yearLevel; // for determining academic probation
	int major;

public:
	CStudent();
	CStudent(string _Name, long _ID, int _year, int _major);
	~CStudent();	

	CCourse* theCourse;
	void AddCourse(CCourse* aCourse);
	void DelCourse();
	
	void PrintStudentDetails();

	void payFees();
	void submitRanking(int _choice1, int _choice2, int _choice3);

	int getYrLevel();
	float getSessAvg();

	float cheat(CCourse* aCourse);
};

#endif