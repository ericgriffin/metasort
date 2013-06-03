// logfile.cpp

#include <time.h>

class LogFile
{	

public:
	char path[255];
	char filename[255];
	
	LogFile(char*);
	~LogFile();

	char* timestamp();
	
};