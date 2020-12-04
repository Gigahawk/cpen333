#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>

using namespace std;

void show_log(bool enable);

class LogMgr {
public:
	static LogMgr* get_instance() {
		if (inst == nullptr) {
			inst = new LogMgr();
		}
		return inst;
	}
	bool enable = true;
private:
	static LogMgr* inst;
	LogMgr() {}
};

class Logger
{
protected:
	string log_name = "";
	void log(const char* format, ...) {
		LogMgr* lm = LogMgr::get_instance();
		if (!lm->enable)
			return;
		printf("[%10s] ", log_name.c_str());
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
		cout << endl;
	}
};

