#ifndef CSWITCH_H
#define CSWITCH_H
class CSwitch
{
public:
	CSwitch();
	~CSwitch();
	void turnon() { this->state = 1; }
	void turnoff() { this->state = 0; }
	int getState() { return state; }
	void print();
private:
	int state;
};
#endif
