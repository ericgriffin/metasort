#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>

using namespace std;


class FileLister
{	
    HANDLE hfind;
    char folderstar[255];
    int chk;

public:
	char path[255];
	char filename[255];
	char extension_filename[255];
	char full_filename[255];
	char recurse_path[255];
	char* current_token;
	char* previous_token;
	char* context;
	int recurse;
	int is_dir;
	int end_of_file_listing;
	
	WIN32_FIND_DATAA found;

	FileLister(char*);
	~FileLister();

	int getNextFile();
	void ReturnNextFile(char*);
	
};