/*  metasorter.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */


#ifndef _METASORTER_H_
#define _METASORTER_H_ 1

#define MEDIAINFO_LIBRARY 1


/* 64-bit int */
#if defined(__MINGW32__) || defined(__CYGWIN32__) || defined(__UNIX__) || defined(__MACOSX__)
#undef  MAXTYPE_INT
#define MAXTYPE_INT 64
typedef unsigned long long MediaInfo_int64u;
#elif defined(__WIN32__) || defined(_WIN32)
#undef  MAXTYPE_INT
#define MAXTYPE_INT 64
typedef unsigned __int64   MediaInfo_int64u;
#else
#pragma message This machine has no 64-bit integer type?
#endif


#ifdef __MINGW32__
#ifdef _UNICODE
#define _itot _itow
#else
#define _itot itoa
#endif
#endif


#ifdef MEDIAINFO_LIBRARY
#include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
#define MediaInfoNameSpace MediaInfoLib;
#else
#include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
#define MediaInfoNameSpace MediaInfoDLL;
#endif

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/case_conv.hpp> 
#include <boost/threadpool.hpp>
#include <boost/timer.hpp>

#include <ThirdParty/tinyxml2/tinyxml2.h>

#include "configuration.h"
#include "asset.h"
#include "logfile.h"
#include "metasortutil.h"
#include "md5.h"


using namespace MediaInfoNameSpace;


class metasorter
{
		
public:
	metasorter();
	~metasorter();

	char path[255];
	class configuration* config;
	std::string logstring;
	LogFile logfile;
	boost::threadpool::pool tp;
	int verbose;

	int file_inspection_time;
	int logging;
	int extensions;
	int rules;
	
	int files_examined;
	int rule_matches;
	int actions_performed;

	// 0 if scanning paths from config file
	// 1 if reading individual files from argv
	int run_type;
	
	int run();
	int load_config_file(std::string);
	int set_input_file(std::string);
	int check_config(tinyxml2::XMLDocument*);
	int process_asset(asset*);
	int process_stream_blocks(asset*, tinyxml2::XMLElement *, int);
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
	int action_moveCUSTOM1(asset*, std::string, std::string);
	int action_fastmoveCUSTOM1(asset*, std::string, std::string);
	
	boost::mutex list_mtx_;
	boost::mutex log_mtx_;
	boost::mutex hist_mtx_;
	boost::mutex file_examined_mtx_;
	boost::mutex rule_matches_mtx_;
	boost::mutex actions_performed_mtx_;
	

private:
	static const int DEFAULT_THREADPOOL_SIZE = 4;
	static const int DEFAULT_FILE_INSPECTION_TIME = 20000;
	static const int MAX_CHAR = 1024;

};
	

#endif // _METASORTER_H_