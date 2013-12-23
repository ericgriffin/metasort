// main.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metasorter.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

void generate_skeleton_config();

using namespace std;
using namespace boost::filesystem;


int main(int argc, char* argv[])
{
	std::cout << "Metasort 1.4.5" << std::endl << std::endl;
	//std::cout << "Copyright 2013 Eric Griffin" << std::endl << std::endl;

	int err = 0;	
	int ok_to_run = 0;
	int required_flags = 0;
	int config_num = 0;
	int input_file_process = 0;
	int input_file_num = 0;
	char* input_file = (char*)malloc(sizeof(char[255][255]));
	char* config_file = (char*)malloc(sizeof(char[255][255]));
	boost::property_tree::ptree* pt = new boost::property_tree::ptree[255];

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++) 
		{ 
			if (strcmp(argv[i], "-g") == 0 )
			{
				generate_skeleton_config();
				exit(0);
            }

			if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "/h") == 0)
			{
				std::cout << "Usage: metasort -c <config file> [-i <filename>] [-g]" << std::endl;
				std::cout << "-c <config file>  --  specify configuration file to run" << std::endl;
				std::cout << "-i <filename>  --  specify a single file to process (requires -c)" << std::endl;
				std::cout << "-g  --  create a skeleton configuration file in the current directory" << std::endl << std::endl;
				exit(0);
            }

			if (i + 1 != argc)
			{
                if (strcmp(argv[i], "-c") == 0)
				{
                    strcpy(&config_file[config_num], argv[i + 1]);
					config_num++;
					required_flags++;
                }
				if (strcmp(argv[i], "-i") == 0)
				{
					input_file_process = 1;
                    strcpy(&input_file[input_file_num], argv[i + 1]);
					input_file_num++;
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
			//boost::property_tree::info_parser::read_info(&config_file[q], pt[q]);
			boost::property_tree::xml_parser::read_xml(&config_file[q], pt[q]);
		
			// check for folders entry
			optional<const boost::property_tree::ptree&> pt_check_existence;
			pt_check_existence = pt[q].get_child_optional("folders");
			
			
			if(input_file_process == 0)  // if processing folders from config
			{
				if(pt_check_existence)
				{
					BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt[q].get_child("folders"))
					{
						int recurse = 0;
						std::cout << "Searching Root Folder: " << v.first.data() << std::endl;
						if(strcmp(v.second.data().c_str(), "R") == 0)
						{
							recurse = 1;
						}
						metasorter sorter((char*)v.first.data(), pt[q]);
						sorter.traverse_directory(recurse);
						sorter.tp.wait();
					}
					std::cout << endl << "Finished." << std::endl;
				}
				else
				{
					cout << endl << "No folders defined in config file - aborting" << std::endl;
					exit(0);
				}
			}
			else  // processing files from argv[]
			{
				for(int input_file_counter = 0; input_file_counter < input_file_num; input_file_counter++)
				{
					metasorter sorter(&input_file[input_file_counter], pt[q]);
					sorter.process_file();
				}
				std::cout << endl << "Finished." << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Usage: metasort -c <config file> [-i <filename>] [-g]" << std::endl << std::endl;
	}

	delete[] pt;
	delete[] input_file;
	delete[] config_file;
	return err;
}


void generate_skeleton_config()
{
	boost::filesystem::path working_path_raw(boost::filesystem::current_path());
	std::string *config = new std::string();
	std::string *working_path = new std::string(working_path_raw.string().c_str());
	
	//change back-slashes to forward-slashes
	std::replace(working_path->begin(), working_path->end(), '\\', '/');

	config->append("folders\n{\n\t;\"");
	config->append(working_path->c_str());
	config->append("/\" R\n}\n\nextensions\n{\n");
	config->append("\t[REGEX].*");
	config->append("\n}\n\nconfig\n{\n");
	config->append("\tlogfile \"");
	config->append(working_path->c_str());
	config->append("/metasort.log\"");
	config->append("\n}\n\nrules\n{\n");
	config->append("\tRule1_list \"");
	config->append(working_path->c_str());
	config->append("/metasort_list.txt\"\n\t{\n");
	config->append("\t\tGeneral_0\n\t\t{\n");
	config->append("\t\t\tfile_size > 50\n\t\t}\n");
	config->append("\n\t}\n}\n");

	std::ofstream f;
	std::string *skeleton_config_file = new std::string(working_path_raw.string().c_str());
	skeleton_config_file->append("/metasort_config.ini");
	std::cout << "Generating skeleton config file: " << skeleton_config_file->c_str() << std::endl << std::endl;
    f.open(skeleton_config_file->c_str(), std::ios::out);
	f << config->c_str();
	f.close();
}
