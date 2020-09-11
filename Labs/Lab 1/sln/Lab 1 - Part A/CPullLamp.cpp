#include "CPullLamp.h"
#include <iostream>

using namespace std;

CPullLamp::CPullLamp(int w1, int w2, int w3)
	:CLamp3Bulb(w1, w2, w3)
{
	cout << "CPullLamp constructor called..." << endl;
}

void CPullLamp::toggle()
{
	getState() ? CLamp3Bulb::LampOff() : CLamp3Bulb::LampOn();
}
