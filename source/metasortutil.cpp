/*  util_functions.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#include "metasortutil.h"


bool metasortutil::Wait(const unsigned long &Time)
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(Time));
    return 1;
}


void metasortutil::m_itoa(int value, std::string& buf, int base)
{
	int i = 30;
	buf = "";
	for(; value && i ; --i, value /= base) buf = "0123456789abcdef"[value % base] + buf;
}


int metasortutil::test_create_file(std::string& _filepath)
{
	int retval = 0;
	boost::filesystem::path* filepath = new boost::filesystem::path(_filepath);

	if (metasortutil::file_exists(_filepath))
	{
		retval = 1;
	}
	else
	{
		std::ofstream file(_filepath.c_str());
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


int metasortutil::file_exists(const std::string& _filepath)
{
	int exists = 0;
	boost::filesystem::path* filepath = new boost::filesystem::path(_filepath);
	if (boost::filesystem::exists(*filepath))
		exists = 1;
	delete filepath;
	return exists;
}


int metasortutil::path_exists(const std::string& path)
{
	int exists = 0;
	boost::filesystem::path* dirpath = new boost::filesystem::path(path);
	if (boost::filesystem::exists(*dirpath))
	{
		if (boost::filesystem::is_directory(*dirpath))
			exists = 1;
	}
	delete dirpath;
	return exists;
}


bool metasortutil::string_replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


std::string metasortutil::get_file_extension(const std::string& s)
{
   size_t i = s.rfind('.', s.length( ));
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length( ) - i));
   }
   return(std::string(""));
}


int metasortutil::compare_file_modified_time(std::string file1, std::string file2)
{
	int result = 0;
	time_t file_modified_time1;
	time_t file_modified_time2;

	file_modified_time1 = metasortutil::file_modified_time(file1);
	file_modified_time2 = metasortutil::file_modified_time(file2);
	
	if(difftime(file_modified_time1, file_modified_time2) == 0)
		result = 0;
	if(difftime(file_modified_time1, file_modified_time2) > 0)
		result = 1;
	if(difftime(file_modified_time1, file_modified_time2) < 0)
		result = -1;

	return result;
}


time_t metasortutil::file_modified_time(std::string file)
{
	std::time_t file_modified_time1;
	struct tm* clock1 = new tm;
	struct stat attrib1;
	stat(file.c_str(), &attrib1);
	clock1 = localtime(&(attrib1.st_mtime));

	file_modified_time1 = mktime(clock1);

	return file_modified_time1;
}


wchar_t* metasortutil::charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}


bool metasortutil::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit((unsigned char)*it)) ++it;
    return !s.empty() && it == s.end();
}


// returns 0 if filesize does not change over x seconds, 1 if it changes
int metasortutil::filesize_changing(char filename[255], int wait_time)
{
	int changing = 0;
	std::ofstream file;
	file.open(filename, std::ios::app);

	if(!file.is_open())  //check to see if the file is locked
	{
		changing = 1;
	}
	else  //check to see if the filesize is changing
	{
		file.seekp(0, std::ifstream::end);
		int filesize1 = (int)file.tellp();
		file.close();
		Wait(wait_time);
		file.open(filename, std::ios::app);
		file.seekp(0, std::ifstream::end);
		int filesize2 = (int)file.tellp();
		if(filesize1 != filesize2)
			changing = 1;
	}

	file.close();
	return changing;
}


char * metasortutil::timestring()
{
# define TIME_SIZE 40

	const struct std::tm *tm_ptr;
	size_t len;
	std::time_t now;
	char *s;

	now = std::time(NULL);
	tm_ptr = std::localtime(&now);

	s = new char[TIME_SIZE];

	len = std::strftime(s, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr);
	return s;
# undef TIME_SIZE
}


int metasortutil::fill_file_placeholders(asset* _asset, std::string& execstring)
{
	int err = 0;
	metasortutil::string_replace(execstring, "%s", _asset->full_filename);
	metasortutil::string_replace(execstring, "%p", _asset->path);
	metasortutil::string_replace(execstring, "%f", _asset->filename);
	metasortutil::string_replace(execstring, "%e", _asset->extension);
	return err;
}