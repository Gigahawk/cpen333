#include "CLamp3Bulb.h"
#include <iostream>

using namespace std;

CLamp3Bulb::CLamp3Bulb(int w1, int w2, int w3)
{
	cout << "CLamp3Bulb constructor being called.." << endl;
	bulbs[0] = new CBulb(w1);
	bulbs[1] = new CBulb(w2);
	bulbs[2] = new CBulb(w3);
	swch = new CSwitch();
}

CLamp3Bulb::CLamp3Bulb(const CLamp3Bulb& l)
{
	cout << "CLamp3Bulb copy constructor being called.." << endl;
	bulbs[0] = new CBulb(*l.bulbs[0]);
	bulbs[1] = new CBulb(*l.bulbs[1]);
	bulbs[2] = new CBulb(*l.bulbs[2]);
	swch = new CSwitch(*l.swch);
}

CLamp3Bulb::~CLamp3Bulb()
{
	cout << "CLamp3Bulb destructor being called.." << endl;
	delete bulbs[0];
	delete bulbs[1];
	delete bulbs[2];
	delete swch;
}

void CLamp3Bulb::LampOn()
{
	swch->turnon();
	for (auto b : bulbs)
		b->on();
}

void CLamp3Bulb::LampOff()
{
	swch->turnoff();
	for (auto b : bulbs)
		b->off();
}

void CLamp3Bulb::print()
{
	cout << (getState() ? "on" : "off") << endl;
}

int CLamp3Bulb::getPower()
{
	int power = 0;
	for (auto b : bulbs)
		power += b->getPower();
	return power;
}

CBulb* CLamp3Bulb::exchange(CBulb* newBulb, int idx)
{
	CBulb* oldBulb = bulbs[idx];
	bulbs[idx] = newBulb;
	return oldBulb;
}

