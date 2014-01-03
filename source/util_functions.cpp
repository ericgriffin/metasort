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


int test_create_file(std::string& _filepath)
{
	int retval = 0;
	boost::filesystem::path* filepath = new boost::filesystem::path(_filepath);

	if (file_exists(_filepath))
	{
		retval = 1;
	}
	else
	{
		std::ofstream file(_filepath);
		if (file)
		{
			retval = 1;
			file.close();
		}
		if (boost::filesystem::exists(*filepath))
		{
			boost::filesystem::remove(*filepath);
		}
	}

	return retval;
}


int file_exists(std::string& _filepath)
{
	int retval = 0;
	boost::filesystem::path* filepath = new boost::filesystem::path(_filepath);
	if (boost::filesystem::exists(*filepath))
		retval = 1;
	delete filepath;
	return retval;
}


int path_exists(std::string& path)
{
	int retval = 0;
	boost::filesystem::path* dirpath = new boost::filesystem::path(path);
	if (boost::filesystem::is_directory(*dirpath))
		retval = 1;
	delete dirpath;
	return retval;
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
	time_t file_modified_time1;
	time_t file_modified_time2;

	file_modified_time1 = file_modified_time(file1);
	file_modified_time2 = file_modified_time(file2);
	
	if(difftime(file_modified_time1, file_modified_time2) == 0)
		result = 0;
	if(difftime(file_modified_time1, file_modified_time2) > 0)
		result = 1;
	if(difftime(file_modified_time1, file_modified_time2) < 0)
		result = -1;

	return result;
}


time_t file_modified_time(std::string file)
{
	std::time_t file_modified_time1;
	struct tm* clock1 = new tm;
	struct stat attrib1;
	stat(file.c_str(), &attrib1);
	clock1 = localtime(&(attrib1.st_mtime));

	file_modified_time1 = mktime(clock1);

	return file_modified_time1;
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
    while (it != s.end() && std::isdigit((unsigned char)*it)) ++it;
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