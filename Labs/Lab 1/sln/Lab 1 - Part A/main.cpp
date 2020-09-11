#include <iostream>
#include "CBulb.h"
#include "CSwitch.h"
#include "CLamp3Bulb.h"
#include "CPullLamp.h"

using namespace std;


void printBulbState(CBulb* b);
void printSwitchState(CSwitch* s);
void printLampState(CLamp3Bulb* l);

int main() {
	cout << "Testing CBulb()" << endl;
	// CBulb defaults to 10W
	CBulb *b = new CBulb();
	printBulbState(b);
	cout << "Turning bulb on" << endl;
	b->on();
	printBulbState(b);
	cout << "Turning bulb off" << endl;
	b->off();
	printBulbState(b);
	
	cout << endl;

	cout << "Testing CSwitch()" << endl;
	CSwitch *s = new CSwitch();
	printSwitchState(s);
	cout << "Turning switch on" << endl;
	s->turnon();
	printSwitchState(s);
	cout << "Turning switch off" << endl;
	s->turnoff();
	printSwitchState(s);

	cout << endl;

	cout << "Testing CLamp3Bulb()" << endl;
	CLamp3Bulb* l = new CLamp3Bulb(100, 100, 100);
	printLampState(l);
	cout << "Turning lamp on" << endl;
	l->LampOn();
	printLampState(l);
	cout << "Turning lamp off" << endl;
	l->LampOff();
	cout << "Swapping 0th bulb" << endl;
	// b now contains a 100W bulb
	b = l->exchange(b, 0);
	cout << "Turning lamp on" << endl;
	l->LampOn();
	printLampState(l);

	cout << endl;

	cout << "Testing CPullLamp()" << endl;
	CPullLamp* p = new CPullLamp(100, 200, 100);
	printLampState((CLamp3Bulb*)p);
	cout << "Toggling lamp" << endl;
	p->toggle();
	printLampState((CLamp3Bulb*)p);
	cout << "Toggling lamp" << endl;
	p->toggle();
	cout << "Swapping 1st bulb" << endl;
	// b now contains the 200W bulb
	b = p->exchange(b, 1);
	cout << "Toggling lamp" << endl;
	p->toggle();
	printLampState((CLamp3Bulb*)p);

	delete b;
	delete s;
	delete l;
	delete p;


	
	return 0;
}

void printBulbState(CBulb* b) {
	cout << "Bulb is ";
	b->print();
	cout << "Bulb is drawing " << b->getPower() << "W" << endl;
}

void printSwitchState(CSwitch* s) {
	cout << "Switch is ";
	s->print();
}

void printLampState(CLamp3Bulb* l) {
	cout << "Lamp is ";
	l->print();
	cout << "Lamp is drawing " << l->getPower() << "W" << endl;
}
