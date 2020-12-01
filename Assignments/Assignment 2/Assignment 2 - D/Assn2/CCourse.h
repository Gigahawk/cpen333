#ifndef   CCourse_H
#define   CCourse_H

#include <stdio.h>
#include <string>
#include "CUser.h"

using namespace std;

class CStudent;

class CCourse {
	int courseDept;
	int courseNum;

public:
	CCourse();
	CCourse(int _courseDept, int _courseNum);
	~CCourse();

	//functions
	CStudent* theStudent;
	void AddStudent(CStudent* aStudent);
	void DelStudent();

	int getCourseDept();
	int getCourseNum();
};

#endif