// metasorter.h
#ifndef METASORTER_H
#define METASORTER_H 1

#define MEDIAINFO_LIBRARY 1

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <stdio.h>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/case_conv.hpp> 

#include "asset.h"
#include "logfile.h"

#ifdef MEDIAINFO_LIBRARY
    #include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
    #define MediaInfoNameSpace MediaInfoLib;
#else
    #include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
    #define MediaInfoNameSpace MediaInfoDLL;
#endif

using namespace MediaInfoNameSpace;
using namespace boost;
using namespace std;

#ifdef __MINGW32__
    #ifdef _UNICODE
        #define _itot _itow
    #else
        #define _itot itoa
    #endif
#endif


class metasorter
{
		
public:
	metasorter(char*, boost::property_tree::ptree pt);
	~metasorter();
	
	char path[255];
	boost::property_tree::ptree pt;
	std::string logstring;
	LogFile logfile;

	int process_asset(asset*);
	int traverse_directory(int);
	int process_file();
	int process_extensions(asset*);
	int process_rule(asset*, std::string, std::string);
	bool string_replace(std::string&, const std::string&, const std::string&);
	wchar_t* charToWChar(const char*);
	int custom_parameters(MediaInfoLib::String &, MediaInfo &, asset *, MediaInfoLib::stream_t, int, MediaInfoLib::String);

private:
	static const int MAX_CHAR = 1024;
	int find_matches_ids[1024];
	int find_matches_count;
	int logging;
};
	

#endif