#include "filelister.h"


FileLister::FileLister(char* folder)
{       
	context = NULL;
	recurse = 1;
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

int FileLister::getNextFile()
{
	chk=FindNextFileA(hfind,&found);
    if(chk)
	{
		strcpy(filename, found.cFileName);
	}
	return chk;
}


void FileLister::ReturnNextFile(char* _filename)
{
		if(getNextFile() != 1)
		{
			end_of_file_listing = 1;
		}
		else
		{
			is_dir = 0;
		
			strcpy(full_filename, path);
			strcat(full_filename, filename);
		
			strcpy(extension_filename, filename);
			current_token = strtok_s(extension_filename,"\.", &context);
		
			while (current_token != NULL)
			{
				previous_token = current_token;
				current_token = strtok_s(NULL, "\.", &context);
			}

			if(found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(recurse == 1)
				{
					strcpy(recurse_path, path);
					strcat(recurse_path, "\\");
					strcat(recurse_path, filename);
					strcat(recurse_path, "\\");

					FileLister* fg = new FileLister(recurse_path);

					while(!fg->end_of_file_listing)
					{
						fg->ReturnNextFile(_filename);
					}
				}
				is_dir = 1;
			}
		
			if(is_dir != 1)
			{
				strcpy(_filename, full_filename);
				//cout << path << " : " << filename << endl;

			}
	
		}

	
	}