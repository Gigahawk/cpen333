#include "ScreenWriter.h"

void ScreenWriter::WriteToScreen(uint8_t c1, uint8_t c2, const char* format, ...)
{
	m->Wait();
	MOVE_CURSOR(c1, c2);
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	fflush(stdout);
	va_end(argptr);
	m->Signal();
}
