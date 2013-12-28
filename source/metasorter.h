// metasorter.h
#ifndef METASORTER_H
#define METASORTER_H 1

#define MEDIAINFO_LIBRARY 1

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
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
#include <boost/threadpool.hpp>

#include <mediainfo/../ThirdParty/tinyxml2/tinyxml2.h>

#include "asset.h"
#include "logfile.h"
#include "util_functions.h"


#ifdef MEDIAINFO_LIBRARY
    #include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
    #define MediaInfoNameSpace MediaInfoLib;
#else
    #include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
    #define MediaInfoNameSpace MediaInfoDLL;
#endif

using namespace MediaInfoNameSpace;
using namespace boost;
using namespace boost::threadpool;
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
	metasorter(char*, tinyxml2::XMLDocument* config);
	~metasorter();
	
	char path[255];
	//boost::property_tree::ptree pt;  //remove this
	tinyxml2::XMLDocument* config;
	std::string logstring;
	LogFile logfile;
	pool tp;
	int file_inspection_time;
	
	int process_asset(asset*);
	int traverse_directory(int);
	int process_file();
	int process_extensions(asset*);
	int process_rule(asset*, std::string, std::string, std::string);
	int call_MediaInfo(MediaInfo &, asset *);
	int custom_parameters(MediaInfoLib::String &, MediaInfo &, asset *, MediaInfoLib::stream_t, int, MediaInfoLib::String, int &);

	//custom parameter functions
	int proc_audio_layout(MediaInfoLib::String &, MediaInfo &, asset*, int &);
	int proc_file_name(MediaInfoLib::String &, asset *);
	int proc_file_extension(MediaInfoLib::String &, asset *);
	int proc_file_path(MediaInfoLib::String &, asset *);
	int proc_file_size(MediaInfoLib::String &, asset *);
	int proc_file_modified_age(MediaInfoLib::String &, asset *);
	int proc_file_created_age(MediaInfoLib::String&, asset *);
	int proc_file_modified_date(MediaInfoLib::String &, asset *);
	int proc_file_created_date(MediaInfoLib::String&, asset *);

	//rule action functions
	int action_list(asset*, std::string, std::string);
	int action_move(asset*, std::string, std::string);
	int action_fastmove(asset*, std::string, std::string);
	int action_copy(asset*, std::string, std::string);
	int action_copyonce(asset*, std::string, std::string);
	int action_exec(asset*, std::string, std::string);
	int action_delete(asset*, std::string, std::string);
	int action_md5file(asset*, std::string, std::string);
	int action_copyonceCUSTOM1(asset*, std::string, std::string);

private:
	static const int MAX_CHAR = 1024;
	int find_matches_ids[1024];
	int find_matches_count;
	int logging;
	int extensions;
	int rules;
	boost::mutex list_mtx_;
	boost::mutex log_mtx_;
	boost::mutex hist_mtx_;

};
	

#endif