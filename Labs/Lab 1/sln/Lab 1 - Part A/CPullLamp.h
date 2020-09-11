#ifndef CPULLLAMP_H
#define CPULLLAMP_H

#include "CLamp3Bulb.h"
class CPullLamp: public CLamp3Bulb
{
public:
	CPullLamp(int w1, int w2, int w3);
	void toggle(); 
private:
	void LampOn();
	void LampOff();
};

#endif
