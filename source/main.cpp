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
	
	int ok_to_run = 0;
	int required_flags = 0;
	char config_file[255];
	boost::property_tree::ptree pt;

	if (argc > 2)
	{
		for (int i = 1; i < argc; i++) 
		{ 
			if (i + 1 != argc)
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
		
		// check for folders entry
		optional<const boost::property_tree::ptree&> pt_check_existence;
		pt_check_existence = pt.get_child_optional("folders");
		if(pt_check_existence)
		{
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("folders"))
			{
				int recurse = 0;
				std::cout << std::endl << "Searching Root Folder: " << v.first.data() << std::endl;
				if(strcmp(v.second.data().c_str(), "R") == 0)
				{
					recurse = 1;
				}
				metasorter sorter((char*)v.first.data(), pt);
				//sorter.parse_directory(recurse);
				sorter.traverse_directory(recurse);
			}
		}
		else
		{
			cout << "No folders defined in config file - aborting" << endl;
			exit(0);
		}
	}
	else
	{
		std::cout << "Usage:  metasort -c <config file>" << std::endl;
	}
	return err;
}
