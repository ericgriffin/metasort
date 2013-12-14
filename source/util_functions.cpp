#include "util_functions.h"



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


string get_file_extension(const string& s)
{
   size_t i = s.rfind('.', s.length( ));
   if (i != string::npos) {
      return(s.substr(i+1, s.length( ) - i));
   }
   return(std::string(""));
}


int compare_file_modified_time(std::string file1, std::string file2)
{
	int result = 0;

	std::time_t file_modified_time1;
	std::time_t file_modified_time2;
	struct tm* clock1 = new tm;
	struct tm* clock2 = new tm;
	struct stat attrib1;
	struct stat attrib2;
	stat(file1.c_str(), &attrib1);
	stat(file2.c_str(), &attrib2);
	clock1 = localtime(&(attrib1.st_mtime));
	clock2 = localtime(&(attrib2.st_mtime));
	file_modified_time1 = mktime(clock1);
	file_modified_time2 = mktime(clock2);
	if(difftime(file_modified_time1, file_modified_time2) == 0)
		result = 0;
	if(difftime(file_modified_time1, file_modified_time2) > 0)
		result = 1;
	if(difftime(file_modified_time1, file_modified_time2) < 0)
		result = -1;

	return result;
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

	if(!file.is_open())  //check to see if the file is locked
	{
		changing = 1;
	}
	else  //check to see if the filesize is changing
	{
		file.seekp(0, ifstream::end);
		int filesize1 = (int)file.tellp();
		file.close();
		Wait(wait_time);
		file.open(filename, ios::app);
		file.seekp(0, std::ifstream::end);
		int filesize2 = (int)file.tellp();
		if(filesize1 != filesize2)
			changing = 1;
	}

	file.close();
	return changing;
}