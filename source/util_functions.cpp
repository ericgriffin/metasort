#include "util_functions.h"
#include <time.h>

using namespace std;

bool Wait(const unsigned long &Time)
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(Time));
    return 1;
}


void m_itoa(int value, std::string& buf, int base)
{
	int i = 30;
	buf = "";
	for(; value && i ; --i, value /= base) buf = "0123456789abcdef"[value % base] + buf;
}


bool string_replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}


bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


// returns 0 if filesize does not change over x seconds, 1 if it changes
int filesize_changing(char filename[255], int wait_time)
{
	int changing = 0;
	ofstream file;
	file.open(filename, ios::app);
	file.seekp(0, ifstream::end);
    int filesize1 = (int)file.tellp();
	file.close();
	Wait(wait_time);
	file.open(filename, ios::app);
	file.seekp(0, std::ifstream::end);
	int filesize2 = (int)file.tellp();
	file.close();

	if(filesize1 != filesize2)
		changing = 1;
	return changing;
	
}