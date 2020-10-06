#ifndef __MUTEXPRINTF_H__
#define __MUTEXPRINTF_H__

#include <mutex>
#include <stdarg.h>

static std::mutex m;
void mutex_printf(const char* format, ...); 



#endif
