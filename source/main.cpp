/*  main.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metasorter.h"
#include "usage.h"

std::string metasort_version("1.5.1");


int main(int argc, char* argv[])
{
	std::cout << "Metasort " << metasort_version << std::endl;
	std::cout << "Compiled " << __DATE__ << " " << __TIME__;
	
#if defined(_M_X64) || defined(__amd64__) || defined(_LP64) || defined(_WIN64)
	std::cout << " for x86-64" << std::endl << std::endl;
#else
	std::cout << " for x86" << std::endl << std::endl;
#endif

	int err = 0;
	int ok_to_run = 0;
	int required_flags = 0;
	

	
	int verbose = 0;

	// performance counters
	int files_examined = 0;
	int rule_matches = 0;
	int actions_performed = 0;
	int running_time = 0;
	
	//configuration *config = new configuration;
	metasorter sorter;

	boost::timer elapsed_time; // start timing

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

			if (i + 1 != argc)
			{
                if (strcmp(argv[i], "-c") == 0)
				{
					if (sorter.load_config_file(std::string(argv[i + 1])) == 1)
						ok_to_run = -1;
					else
						ok_to_run = 1;
					required_flags++;
                }
				if (strcmp(argv[i], "-i") == 0)
				{
					sorter.set_input_file(std::string(argv[i + 1]));
                }
			}
		}

	}

	if (ok_to_run == 1 && required_flags > 0)
	{		
		sorter.run();

		std::cout << std::endl << sorter.files_examined << " files examined." << std::endl;
		std::cout << sorter.rule_matches << " rule matches." << std::endl;
		std::cout << sorter.actions_performed << " actions performed." << std::endl;
		std::cout << std::endl << "Finished in " << elapsed_time.elapsed() << " seconds." << std::endl;
	}

	else if (ok_to_run == -1)
	{
		std::cout << std::endl << "Finished in " << elapsed_time.elapsed() << " seconds." << std::endl;
	}

	else
	{
		usage();
	}

	return err;
}

