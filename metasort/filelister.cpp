// filelister.cpp

#include "filelister.h"

FileLister::FileLister(char* folder)
{       
	context = NULL;
	is_dir = 0;
	end_of_file_listing = 0;
	strcpy(path, folder);
	sprintf(folderstar,"%s\\*.*",folder);
	hfind = FindFirstFileA(folderstar, &found);
    FindNextFileA(hfind, &found);
}


FileLister::~FileLister()
{
	FindClose(hfind);
}

int FileLister::getNextFile(char* fname)
{
	chk=FindNextFileA(hfind,&found);
    if(chk)
	{
		strcpy(fname, found.cFileName);
	}
	return chk;
}

