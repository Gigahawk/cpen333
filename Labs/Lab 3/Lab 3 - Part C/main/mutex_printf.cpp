#include "mutex_printf.h"

void mutex_printf(const char* format, ...)
{
	m.lock();
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
	m.unlock();
}
