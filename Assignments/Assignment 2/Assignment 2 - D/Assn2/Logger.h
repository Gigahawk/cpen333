#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>

using namespace std;
class Logger
{
protected:
	string log_name = "";
	void log(const char* format, ...) {
		printf("[%10s] ", log_name.c_str());
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
		cout << endl;
	}
};

