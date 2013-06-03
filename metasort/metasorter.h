// SpotPoller.h
#ifndef SPOTPOLLER_H
#define SPOTPOLLER_H 1

#include <stdexcept>
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <time.h>

#include "filelister.h"
#include "MediaInfoDLL/MediaInfoDLL.h" 
#define MediaInfoNameSpace MediaInfoDLL;

using namespace MediaInfoNameSpace;
using namespace std;
    class metasorter
    {
		
    public:
		metasorter(char*);
		~metasorter();
		int Read_Header_mediainfo(char*);
		void timestamp(char*);
		int parse_directory();
		char path[255];
 
	private:
		static const int MAX_CHAR = 1024;
		RETCODE retcode;
		int find_matches_ids[1024];
		int find_matches_count;
		char filename[255];
	};
	

#endif