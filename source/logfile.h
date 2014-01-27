/*  logfile.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _LOGFILE_H_
#define _LOGFILE_H_ 1

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <boost/thread.hpp>

class LogFile
{	
public:
	std::string path;
	std::string filename;

	LogFile();
	~LogFile();

	boost::mutex mtx_;

	std::ofstream logfile;
	double cpu_time();
	int *time_numbers();
	void timestamp();
	char *timestring();
	int open(std::string);
	int close();
	void write(std::string);
};

#endif // _LOGFILE_H_