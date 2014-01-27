/*  logfile.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#include "logfile.h"

LogFile::LogFile()
{
}


LogFile::~LogFile()
{
}


int LogFile::open(std::string _filename)
{
	mtx_.lock();
	int err = 0;
	logfile.open(_filename.c_str(), std::fstream::app);
	mtx_.unlock();
	return err;
}

int LogFile::close()
{
	mtx_.lock();
	int err = 0;
	logfile.close();
	mtx_.unlock();
	return err;
}

void LogFile::write(std::string _logdata)
{
	mtx_.lock();
	logfile << timestring() << " " << _logdata << std::endl;
	mtx_.unlock();
}



double LogFile::cpu_time()
{
	double value;
	value = ( double ) std::clock ( ) / ( double ) CLOCKS_PER_SEC;
	return value;
}


int * LogFile::time_numbers()
{
	const struct std::tm *tm_ptr;
	std::time_t now;
	int *value;

	now = std::time ( 0 );
	tm_ptr = std::localtime ( &now );

	value = new int[6];

	value[0] = 1900 + tm_ptr->tm_year;
	value[1] = 1 + tm_ptr->tm_mon;
	value[2] = tm_ptr->tm_mday;
	value[3] = tm_ptr->tm_hour;
	value[4] = tm_ptr->tm_min;
	value[5] = tm_ptr->tm_sec;

	return value;
}


void LogFile::timestamp()
{
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct std::tm *tm_ptr;
	size_t len;
	std::time_t now;

	now = std::time ( NULL );
	tm_ptr = std::localtime ( &now );

	len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

	std::cout << time_buffer << "\n";
	return;
# undef TIME_SIZE
}


char * LogFile::timestring()
{
# define TIME_SIZE 40

	const struct std::tm *tm_ptr;
	size_t len;
	std::time_t now;
	char *s;

	now = std::time ( NULL );
	tm_ptr = std::localtime ( &now );

	s = new char[TIME_SIZE];

	len = std::strftime ( s, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );
	return s;
# undef TIME_SIZE
}
