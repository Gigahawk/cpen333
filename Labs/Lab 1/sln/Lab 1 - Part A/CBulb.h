#ifndef CBULB_H
#define CBULB_H

class CBulb
{
public:
	CBulb(int watts = 10);
	~CBulb();
	void print();
	void on() { state = 1; }
	void off() { state = 0; }
	void setwatts(int watts) { this->watts = watts; }
	int getstate() { return state; }
	int getPower();
	//void setstate(int s) { this->state = s; }
private:
	int state;
	int watts;
};

#endif // !CBULB_H
