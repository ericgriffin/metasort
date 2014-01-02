// main.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mediainfo/../ThirdParty/tinyxml2/tinyxml2.h>

#include "metasorter.h"


void generate_skeleton_config();

using namespace std;
using namespace boost::filesystem;

std::string metasort_version("1.4.5");
std::string config_version("1.x");


int main(int argc, char* argv[])
{
	std::cout << "Metasort " << metasort_version << std::endl << std::endl;
	//std::cout << "Copyright 2013 Eric Griffin" << std::endl << std::endl;

	int err = 0;	
	int ok_to_run = 0;
	int required_flags = 0;
	int config_num = 0;
	int input_file_process = 0;
	int input_file_num = 0;
	char* input_file = (char*)malloc(sizeof(char[255][255]));
	char* config_file = (char*)malloc(sizeof(char[255][255]));
	tinyxml2::XMLDocument* config = new tinyxml2::XMLDocument[255];

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
			// parse the xml config file
			if(config[q].LoadFile(&config_file[q]) != 0)
			{
				cout << endl << "ERROR - Could not load config file " << &config_file[q] << " - skipping" << std::endl;
				config[q].Clear();
				continue;
			}

			tinyxml2::XMLElement* xmlroot = config[q].FirstChildElement("metasort");
			
			// check config file version
			if(config_version.compare(xmlroot->Attribute("version")) != 0)
			{
				std::cout << "ERROR - Wrong config version for " << &config_file[q] << ". ";
				std::cout << "Expecting <metasort version=\"" << config_version << "\">" << std::endl;
				config[q].Clear();
				continue;
			}

			// check for folders entry in config
			if(!xmlroot->FirstChildElement("folder"))
			{
				cout << "ERROR - No search folders defined in config file " << &config_file[q] << endl;
				config[q].Clear();
				continue;
			}		
			
			if(input_file_process == 0)  // if processing folders from config
			{
				for(tinyxml2::XMLElement *e = xmlroot->FirstChildElement("folder"); e != NULL; e = e->NextSiblingElement("folder"))
				{
					std::cout << "Processing root folder " << e->Attribute("path");
					
					//check if folder should be searched recursively
					int recurse = 0;
					if(e->Attribute("recursive"))
					{
						if(std::string("yes").compare(e->Attribute("recursive")) == 0 || std::string("1").compare(e->Attribute("recursive")) == 0 || std::string("true").compare(e->Attribute("recursive")) == 0)
						{
							recurse = 1;
							std::cout << " recursively";
						}
					}

					std::cout << endl;

					metasorter sorter((char*)e->Attribute("path"), &config[q]);
					sorter.traverse_directory(recurse);
					sorter.tp.wait();
				}

				std::cout << endl << "Finished." << std::endl;
			}
			else  // if processing files from argv[]
			{
				for(int input_file_counter = 0; input_file_counter < input_file_num; input_file_counter++)
				{
					metasorter sorter(&input_file[input_file_counter], &config[q]);
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

	delete[] config;
	free(input_file);
	free(config_file);
	return err;
}


void generate_skeleton_config()
{
	boost::filesystem::path working_path_raw(boost::filesystem::current_path());
	std::string *config = new std::string();
	std::string *working_path = new std::string(working_path_raw.string().c_str());
	
	//change back-slashes to forward-slashes
	std::replace(working_path->begin(), working_path->end(), '\\', '/');

	config->append("<metasort version=\"1.x\">\n");
	config->append("\t<folder path=\"");
	config->append(working_path->c_str());
	config->append("\" recursive=\"yes\" />\n\n");
	config->append("\t<extension value=\"[REGEX].*\" />\n\n");
	config->append("\t<logging path=\"");
	config->append(working_path->c_str());
	config->append("/metasort.log\" />\n\n");
	config->append("\t<rule name=\"Rule1\">\n");
	config->append("\t\t<action type=\"list\" parameter=\"");
	config->append(working_path->c_str());
	config->append("/metasort_list.txt\" />\n");
	config->append("\t\t<stream type=\"general\" number=\"0\">\n");
	config->append("\t\t\t<parameter name=\"file_size\" value=\">50\" />\n");
	config->append("\t\t</stream>\n");
	config->append("\t</rule>\n");
	config->append("</metasort>\n");

	std::ofstream f;
	std::string *skeleton_config_file = new std::string(working_path_raw.string().c_str());
	skeleton_config_file->append("/metasort_config.ini");
	std::cout << "Generating skeleton config file: " << skeleton_config_file->c_str() << std::endl << std::endl;
    f.open(skeleton_config_file->c_str(), std::ios::out);
	f << config->c_str();
	f.close();
}
