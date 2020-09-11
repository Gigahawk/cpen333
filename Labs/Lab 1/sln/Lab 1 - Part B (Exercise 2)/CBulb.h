#ifndef CBULB_H
#define CBULB_H
#include <iostream>

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
	friend std::ostream& operator << (std::ostream & os,  const CBulb& m) {
		return os << "CBulb with wattage " << m.watts << "W, state: " << m.state;
	}
private:
	int state;
	int watts;
};

#endif // !CBULB_H
