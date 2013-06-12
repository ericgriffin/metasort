// logfile.cpp

#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
# include <cstdlib>
# include <iomanip>
# include <ctime>

using namespace std;

class LogFile
{	
public:
	std::string path;
	std::string filename;

	LogFile();
	~LogFile();

	ofstream logfile;
	double cpu_time();
	int *time_numbers();
	void timestamp();
	char *timestring();
	int open(std::string);
	int close();
	void write(std::string);
};