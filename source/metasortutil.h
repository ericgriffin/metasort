/*  util_functions.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _UTIL_FUNCTIONS_H_
#define _UTIL_FUNCTIONS_H_ 1

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <ctime>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include "asset.h"

namespace metasortutil
{

	bool Wait(const unsigned long &);
	void m_itoa(int, std::string&, int);
	int test_create_file(std::string&);
	int file_exists(const std::string&);
	int path_exists(const std::string&);
	bool string_replace(std::string&, const std::string&, const std::string&);
	wchar_t* charToWChar(const char*);
	bool is_number(const std::string&);
	int filesize_changing(char[255], int);
	std::string get_file_extension(const std::string&);
	int compare_file_modified_time(std::string, std::string);
	time_t file_modified_time(std::string);
	char * timestring();
	int fill_file_placeholders(asset*, std::string&);
}

#endif // _UTIL_FUNCTIONS_H_