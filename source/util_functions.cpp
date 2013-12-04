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


// returns 0 if filesize does not change over x seconds, 1 if it changes
int filesize_changing(char filename[255])
{
	int changing = 0;
	ofstream file;
	file.open(filename, ios::app);
	file.seekp(0, ifstream::end);
    int filesize1 = (int)file.tellp();
	file.close();
	Wait(20000);
	file.open(filename, ios::app);
	file.seekp(0, std::ifstream::end);
	int filesize2 = (int)file.tellp();
	file.close();

	if(filesize1 != filesize2)
		changing = 1;
	return changing;
	
}