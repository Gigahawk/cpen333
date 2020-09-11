#include "CSwitch.h"
#include <iostream>

using namespace std;

CSwitch::CSwitch()
{
	cout << "CSwitch constructor being called" << endl;
	state = 0;
}

CSwitch::~CSwitch()
{
	cout << "CSwitch destructor being called.." << endl;
}

void CSwitch::print()
{
	cout << (state ? "on" : "off") << endl;
}
