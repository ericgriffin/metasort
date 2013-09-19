#include "util_functions.h"

void m_itoa(int value, std::string& buf, int base)
{
	int i = 30;
	buf = "";
	for(; value && i ; --i, value /= base) buf = "0123456789abcdef"[value % base] + buf;
}