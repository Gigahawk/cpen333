#include <stdio.h>
#include <iostream>
#include "CCourse.h"
#include "CProf.h"
#include "CPres.h"
#include "CAdmin.h"
#include "CStudent.h"
#include "CSystem.h"

using namespace std;

CSystem::CSystem() {
	cout << "CSystem Default Constructor being called." << endl;
}

CSystem::~CSystem() {
	cout << "CSystem Default Destructor being called." << endl;
}
