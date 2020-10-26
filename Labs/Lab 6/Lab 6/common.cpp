#include "common.h"

string format(const char* fmt, ...) {
	va_list a;
	va_start(a, fmt);
	size_t l = vsnprintf(nullptr, 0, fmt, a) + 1;
	va_end(a);
	vector<char> v(l);
	va_start(a, fmt);
	vsnprintf(&v[0], l, fmt, a);
	va_end(a);
	return &v[0];
}
