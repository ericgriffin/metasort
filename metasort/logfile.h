#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <time.h>

using namespace std;


class LogFile
{	

public:
	char path[255];
	char filename[255];
	
	LogFile(char*);
	~LogFile();

	char* timestamp();
	
};