#ifndef __SCREENWRITER_H__
#define __SCREENWRITER_H__
#include "../rt.h"

class ScreenWriter
{
public:
	ScreenWriter(const string n) :
		m(new CMutex(string("__MUTEX__") + n)) {}
	~ScreenWriter() { delete m; }
	void WriteToScreen(uint8_t c1, uint8_t c2, const char* format, ...);
private:
	CMutex* m;
};
#endif
