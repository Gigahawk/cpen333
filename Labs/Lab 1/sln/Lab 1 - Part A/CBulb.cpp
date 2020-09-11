#include "CBulb.h"
#include <iostream>

using namespace std;

CBulb::CBulb(int watts) :state(0), watts(watts)
{
	cout << "CBulb Constructor being called.." << endl;
}

CBulb::~CBulb()
{
	cout << "CBulb destructor being called.." << endl;
}

void CBulb::print()
{
	cout << (state ? "on" : "off") << endl;
}

int CBulb::getPower()
{
	return state ? watts : 0;
}


