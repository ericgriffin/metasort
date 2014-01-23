// main.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metasorter.h"
#include "usage.h"


using namespace std;
using namespace boost::filesystem;

std::string metasort_version("1.5.1");
std::string config_version("1.x");


int main(int argc, char* argv[])
{
	std::cout << "Metasort " << metasort_version << std::endl;
	std::cout << "Compiled " << __DATE__ << " " << __TIME__;
	
#if defined(_M_X64) || defined(__amd64__) || defined(_LP64) || defined(_WIN64)
	std::cout << " for x86-64";
#else
	std::cout << " for x86";
#endif

	std::cout << std::endl << std::endl;

	int err = 0;	
	int ok_to_run = 0;
	int required_flags = 0;
	int config_num = 0;
	int input_file_process = 0;
	int input_file_num = 0;
	int files_examined = 0;
	int rule_matches = 0;
	int actions_performed = 0;
	int running_time = 0;
	int verbose = 0;
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

			if (strcmp(argv[i], "-lactions") == 0)
			{
				list_actions();
				exit(0);
			}
			if (strcmp(argv[i], "-lstreams") == 0)
			{
				list_streams();
				exit(0);
			}
			if (strcmp(argv[i], "-lgeneral") == 0)
			{
				list_general_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-laudio") == 0)
			{
				list_audio_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-lvideo") == 0)
			{
				list_video_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-ltext") == 0)
			{
				list_text_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-lother") == 0)
			{
				list_other_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-limage") == 0)
			{
				list_image_parameters();
				exit(0);
			}
			if (strcmp(argv[i], "-lmenu") == 0)
			{
				list_menu_parameters();
				exit(0);
			}

			if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "/h") == 0)
			{
				usage();
				exit(0);
            }

			if (strcmp(argv[i], "-v") == 0)
			{
				verbose = 1;
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
		boost::timer elapsed_time; // start timing
		
		for(int q = 0; q < config_num; q++)
		{			
			// see if config file exists
			if (!file_exists(std::string(&config_file[q])))
			{
				std::cout << endl << "ERROR - Config file " << &config_file[q] << " does not exist - skipping" << std::endl;
				continue;
			}

			// parse the xml config file
			if(config[q].LoadFile(&config_file[q]) != 0)
			{
				std::cout << endl << "ERROR - XML not valid in config file " << &config_file[q] << " - skipping" << std::endl;
				continue;
			}

			tinyxml2::XMLElement* xmlroot = config[q].FirstChildElement("metasort");
			
			// check config file version
			if(!xmlroot->Attribute("version") || config_version.compare(xmlroot->Attribute("version")) != 0)
			{
				std::cout << "ERROR - Wrong config version for " << &config_file[q] << ". ";
				std::cout << "Expecting <metasort version=\"" << config_version << "\">" << std::endl;
				config[q].Clear();
				continue;
			}

			// check for folders entry in config
			if(!xmlroot->FirstChildElement("folder") || !xmlroot->FirstChildElement("folder")->Attribute("path"))
			{
				std::cout << "ERROR - No search folders defined in config file " << &config_file[q] << std::endl;
				std::cout << "Expecting <folder path=\"[PATH]\" />" << std::endl;
				config[q].Clear();
				continue;
			}
			
			if(input_file_process == 0)  // if processing folders from config
			{
				// iterate through search paths
				for(tinyxml2::XMLElement *e = xmlroot->FirstChildElement("folder"); e != NULL; e = e->NextSiblingElement("folder"))
				{
					//check to see if path is valid
					if (!path_exists(std::string(e->Attribute("path"))))
					{
						std::cout << "ERROR - search path does not exist: " << e->Attribute("path") << " - skipping" << std::endl;
						continue;
					}

					//check if folder should be searched recursively
					int recurse = 0;
					if(e->Attribute("recursive"))
					{
						if(std::string("yes").compare(e->Attribute("recursive")) == 0 || std::string("1").compare(e->Attribute("recursive")) == 0 || std::string("true").compare(e->Attribute("recursive")) == 0)
							recurse = 1;
					}

					metasorter sorter((char*)e->Attribute("path"), &config[q]);
					if (verbose == 1)
						sorter.verbose = 1;

					if (recurse == 1)
						std::cout << std::endl << "Processing folder recursively: " << e->Attribute("path") << std::endl;
					else
						std::cout << std::endl << "Processing folder: " << e->Attribute("path") << std::endl;

					sorter.traverse_directory(recurse);
					sorter.tp.wait();

					files_examined += sorter.files_examined;
					rule_matches += sorter.rule_matches;
					actions_performed += sorter.actions_performed;
				}
			}
			else  // if processing files from argv[]
			{
				for(int input_file_counter = 0; input_file_counter < input_file_num; input_file_counter++)
				{
					metasorter sorter(&input_file[input_file_counter], &config[q]);
					if (verbose == 1)
						sorter.verbose = 1;
					sorter.process_file();

					files_examined += sorter.files_examined;
					rule_matches += sorter.rule_matches;
					actions_performed += sorter.actions_performed;
				}
			}
		}

		std::cout << std::endl << files_examined << " files examined." << std::endl;
		std::cout << rule_matches << " rule matches." << std::endl;
		std::cout << actions_performed << " actions performed." << std::endl;
		std::cout << std::endl << "Finished in " << elapsed_time.elapsed() << " seconds." << std::endl;
	}

	else
	{
		usage();
	}

	delete[] config;
	free(input_file);
	free(config_file);
	return err;
}

