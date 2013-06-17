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
	int ok_to_run = 0;
	int required_flags = 0;
	int config_num = 0;
	char config_file[255][255];
	boost::property_tree::ptree pt[255];

	if (argc > 2)
	{
		for (int i = 1; i < argc; i++) 
		{ 
			if (i + 1 != argc)
			{
                if (strcmp(argv[i], "-c") == 0)
				{
                    strcpy(config_file[config_num], argv[i + 1]);
					config_num++;
					required_flags++;
                } 
			}
		}
		if(required_flags > 0)
		{
			ok_to_run = 1;
		}
	}

	if(ok_to_run)
	{		
		for(int q = 0; q < config_num; q++)
		{
			boost::property_tree::info_parser::read_info(config_file[q], pt[q]);
		
			// check for folders entry
			optional<const boost::property_tree::ptree&> pt_check_existence;
			pt_check_existence = pt[q].get_child_optional("folders");
			if(pt_check_existence)
			{
				BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt[q].get_child("folders"))
				{
					int recurse = 0;
					std::cout << std::endl << "Searching Root Folder: " << v.first.data() << std::endl;
					if(strcmp(v.second.data().c_str(), "R") == 0)
					{
						recurse = 1;
					}
					metasorter sorter((char*)v.first.data(), pt[q]);
					sorter.traverse_directory(recurse);
				}
			}
			else
			{
				cout << "No folders defined in config file - aborting" << endl;
				exit(0);
			}
		}
	}
	else
	{
		std::cout << "Usage:  metasort -c <config file>" << std::endl;
	}
	return err;
}
