#ifndef CLAMP3BULB_H
#define CLAMP3BULB_H

#include "CBulb.h"
#include "CSwitch.h"
class CLamp3Bulb
{
public:
	CLamp3Bulb(int w1, int w2, int w3);
	CLamp3Bulb(const CLamp3Bulb &l);
	~CLamp3Bulb();
	void LampOn();
	void LampOff();
	void print();
	int getState() { return swch->getState(); }
	int getPower();
	CBulb* exchange(CBulb* newBulb, int idx);
private:
	CBulb *bulbs[3];
	CSwitch *swch;
};

#endif
