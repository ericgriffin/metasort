
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <ctime>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/thread.hpp>

bool Wait(const unsigned long &);
void m_itoa(int, std::string&, int);
bool string_replace(std::string&, const std::string&, const std::string&);
wchar_t* charToWChar(const char*);
bool is_number(const std::string&);
int filesize_changing(char[255], int);