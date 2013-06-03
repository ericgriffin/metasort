// metadata-sorter.cpp : Defines the entry point for the console application.
//

#include <iostream>


#include "metasorter.h"

using namespace std;

int main(int argc, char* argv[])
{

	int err = 0;
	int runtype = 0;

	if (argc == 1)
	{
		cout << "usage: metasort <filename>" << endl << "       metasort -d <path>" << endl << "       metasort -dr <path>" << endl;
		exit(0);
	}
	else
	{
		if (strcmp(argv[1], "-d") == 0)
			runtype = 1;
		else if (strcmp(argv[1], "-dr") == 0)
			runtype = 2;
		else
		{
			runtype = 3;
			//single file
		}
	}

	metasorter sorter(argv[1]);
	sorter.parse_directory();

	return err;
}


