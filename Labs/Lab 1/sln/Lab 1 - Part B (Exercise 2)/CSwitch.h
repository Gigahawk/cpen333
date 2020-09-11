#ifndef CSWITCH_H
#define CSWITCH_H
#include <iostream>

class CSwitch
{
public:
	CSwitch();
	~CSwitch();
	void turnon() { this->state = 1; }
	void turnoff() { this->state = 0; }
	int getState() { return state; }
	void print();
	friend std::ostream& operator << (std::ostream & os,  const CSwitch& m) {
		return os << "Cswitch with state " <<  m.state;
	}
private:
	int state;
};
#endif
