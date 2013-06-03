// main.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metasorter.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace std;


int main(int argc, char* argv[])
{
	int err = 0;
	int config_status = 0;
	int runtype = 0;
	int recurse = 0;
	int ok_to_run = 0;
	int required_flags = 0;
	char directory[255];
	char config_file[255];
	boost::property_tree::ptree pt;

	if (argc > 2)
	{
		for (int i = 1; i < argc; i++) 
		{ 
			if (strcmp(argv[i], "-R") == 0)
			{
				recurse = 1;
            }
			else if (i + 1 != argc)
			{
                if (strcmp(argv[i], "-c") == 0)
				{
                    strcpy(config_file, argv[i + 1]);
					required_flags++;
                } 
			}
		}
		if(required_flags == 1)
		{
			ok_to_run = 1;
		}
	}

	if(ok_to_run)
	{		
		boost::property_tree::info_parser::read_info(config_file, pt);
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("watch_folders"))
		{
			cout << "Looking in Root Folder: " << v.first.data() << endl;
			metasorter sorter((char*)v.first.data(), pt);
			sorter.parse_directory(recurse);
		}
	}
	else
	{
		std::cout << "Usage:  metasort [flags] -c <config file>" << std::endl;
		std::cout << "\nOptional Flags: " << std::endl;
		std::cout << "\t-R : recurse subdirectories" << std::endl;
	}
	return err;
}
