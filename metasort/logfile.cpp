#include "logfile.h"

LogFile::LogFile(char* path)
{       
	
}


LogFile::~LogFile()
{

}


char* LogFile::timestamp()
{
	char stamp[100];
	time_t loctime;
	struct tm *loc_tm;
	loctime=time(NULL);
	loc_tm=localtime(&loctime);
	strftime(stamp,sizeof stamp,"%x %X",loc_tm);
	return stamp;
}