#include <stdio.h>
#include <iostream>
#include "CCourse.h"
#include "CProf.h"
#include "CPres.h"
#include "CAdmin.h"
#include "CStudent.h"
#include "CSystem.h"

using namespace std;

int main() {
	
	CPres Ono;
	CAdmin Sarah;

	CProf Mike;
	CProf PJ;
	
	CStudent Jacky;
	CStudent Jasper;
	CStudent Jayden;
	CStudent Toast;
	CStudent Michael;

	CCourse MATH100;
	CCourse CPEN333;

	Jacky.AddCourse(&CPEN333);
	Jasper.AddCourse(&CPEN333);
	Toast.AddCourse(&CPEN333);
	Michael.AddCourse(&CPEN333);
	
	Jayden.AddCourse(&MATH100);
	Jayden.cheat(&MATH100);
	Mike.sus(&Jayden);
}
