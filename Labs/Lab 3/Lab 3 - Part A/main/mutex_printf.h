#ifndef __MUTEXPRINTF_H__
#define __MUTEXPRINTF_H__

#include <mutex>
#include <stdarg.h>

std::mutex m;

void mutex_printf(const char* format, ...) {
	m.lock();
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
	m.unlock();
}

#endif
