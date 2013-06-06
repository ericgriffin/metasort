// metasorter.cpp

#include "metasorter.h"
#include "filelister.h"
#include "asset.h"


metasorter::metasorter(char* _path, boost::property_tree::ptree _pt)
{
	strcpy(path, _path);
	pt = _pt;
	
	optional<const boost::property_tree::ptree&> pt_check_existence;
	
	// check logging
	pt_check_existence = pt.get_child_optional("logging");
	if(pt_check_existence)
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("logging"))
		{
			if(strcmp(v.first.c_str(), "logfile") == 0)
			{
				logging = 1;
				logfile.open(v.second.data().c_str());
			}
			else
			{
				cout << "No logging defined in config file - aborting" << endl;
				exit(0);
			}
		}
	}
	else
	{
		cout << "No logging defined in config file - aborting" << endl;
		logfile.close();
		exit(0);
	}

	// check extensions
	pt_check_existence = pt.get_child_optional("extensions");
	if(!pt_check_existence)
	{
		cout << "No extensions defined in config file - aborting" << endl;
		logfile.close();
		exit(0);
	}

	// check rules
	pt_check_existence = pt.get_child_optional("rules");
	if(!pt_check_existence)
	{
		cout << "No rules defined in config file - aborting" << endl;
		logfile.close();
		exit(0);
	}

	logstring.assign("Entering folder ");
	logstring.append(path);
	logfile.write(logstring);

}


metasorter::~metasorter()
{
	logstring.assign("Leaving folder ");
	logstring.append(path);
	logfile.write(logstring);
	logfile.close();
}


int metasorter::parse_directory(int _recurse)
{
	int err = 0;
	FileLister* fl = new FileLister(path);
	asset* _asset = new asset;
		
	while(fl->getNextFile(fl->filename))
	{
		fl->is_dir = 0;
		
		strcpy(fl->full_filename, path);
		strcat(fl->full_filename, fl->filename);
		strcpy(fl->extension_filename, fl->filename);
		fl->current_token = strtok_s(fl->extension_filename,"\.", &fl->context);
		
		while (fl->current_token != NULL)
		{
			fl->previous_token = fl->current_token;
			fl->current_token = strtok_s(NULL, "\.", &fl->context);
		}

		strcpy(fl->extension, fl->previous_token);

		if(fl->found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // IS A DIRECTORY
		{
			if(_recurse == 1)
			{
				strcpy(fl->recurse_path, path);
				strcat(fl->recurse_path, "\\");
				strcat(fl->recurse_path, fl->filename);
				strcat(fl->recurse_path, "\\");
				metasorter recursed_sorter(fl->recurse_path, pt);
				recursed_sorter.parse_directory(_recurse);
			}
			fl->is_dir = 1;
		}
		
		if(fl->is_dir != 1)  // IS A FILE
		{
			strcpy(_asset->full_filename, fl->full_filename);
			strcpy(_asset->filename, fl->filename);
			strcpy(_asset->path, fl->path);
			strcpy(_asset->extension, fl->extension);

			// filter extensions
			int valid_extension = 0;

			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("extensions"))
			{
				if(strcmp(_asset->extension, v.first.data()) == 0)
				{
					valid_extension = 1;
				}
			}

			if(valid_extension == 1)
			{
				process_asset(_asset);					
			}
		}
	}

	delete _asset;
	delete fl;
	return err;
}



int metasorter::process_asset(asset* _asset)
{
	int err = 0;
	String To_Display;
		
	MediaInfo MI;

	FILE* F = NULL;
	err = fopen_s(&F, _asset->full_filename, "rb");
	if (F == 0)
	{
		logstring.assign("Can't open file ");
		logstring.append(_asset->full_filename);
		logstring.append(" for processing");
		logfile.write(logstring);
		std::cout << "Can't open file: " << _asset->full_filename << " for processing" << std::endl;
		return 1;
	}
	unsigned char* From_Buffer = new unsigned char[7 * 188];	//prepare a memory buffer for reading
	size_t From_Buffer_Size;

	fseek(F, 0, SEEK_END);	//determine file size
	long F_Size = ftell(F);
	fseek(F, 0, SEEK_SET);

	MI.Open_Buffer_Init(F_Size, 0);

	do 
	{
		From_Buffer_Size = fread(From_Buffer, 1, 7 * 188, F);	//read data into buffer
		size_t Status=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);	//Send the buffer to MediaInfo
		if (Status&0x08) //Bit3=Finished
			break;

		if(MI.Open_Buffer_Continue_GoTo_Get() != (MediaInfo_int64u) - 1)
		{
			fseek(F, (long)MI.Open_Buffer_Continue_GoTo_Get(), SEEK_SET);   //Position the file
			MI.Open_Buffer_Init(F_Size, ftell(F));                          //Informing MediaInfo we have seek
		}
	} while (From_Buffer_Size > 0);

	fclose(F);
	MI.Open_Buffer_Finalize();
	

	// Process rules from config file
	boost::property_tree::ptree pt1 = pt.get_child("rules");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt1)
	{
		int match = 1;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &u, pt1.get_child(v.first.data()))
		{
			int file_info = 0;

			//separate stream from stream number
			std::string stream;
			int stream_number = 0;
			std::string s = u.first.data();
			std::string delimiter = "_";
			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				token = s.substr(0, pos);
				stream.assign(token);
				s.erase(0, pos + delimiter.length());
			}
			stream_number = atoi(s.c_str());
			
			#ifdef MEDIAINFO_LIBRARY
				MediaInfoLib::stream_t stream_type = Stream_General;
			#else
				MediaInfoDLL::stream_t stream_type;
			#endif

			if(stream.compare( "audio") == 0) stream_type = Stream_Audio;
			if(stream.compare("video") == 0) stream_type = Stream_Video;
			if(stream.compare("general") == 0) stream_type = Stream_General;
			if(stream.compare("text") == 0) stream_type = Stream_Text;
			if(stream.compare("other") == 0) stream_type = Stream_Other;
			if(stream.compare("image") == 0) stream_type = Stream_Image;
			if(stream.compare("menu") == 0) stream_type = Stream_Menu;
			if(stream.compare("file") == 0) file_info = 1;

			BOOST_FOREACH(boost::property_tree::ptree::value_type &y, pt1.get_child(v.first.data()).get_child(u.first.data()))
			{				
				int exclude = 0;
				int greater_than = 0;
				int less_than = 0;
				String asset_param_val;

				// parameter name from config file
				wchar_t *parameter1 = new wchar_t[255];
				mbstowcs(parameter1, y.first.c_str(), strlen(y.first.c_str()) + 1);
				String parameter = String(parameter1);

				// parameter value from config file
				wchar_t *parameter_val1 = new wchar_t[255];
				mbstowcs(parameter_val1, y.second.data().c_str(), strlen(y.second.data().c_str()) + 1);
				String parameter_val = String(parameter_val1);
				
				// assign asset parameter value
				if(file_info == 0)
				{
					if(custom_parameters(asset_param_val, MI, _asset, stream_type, stream_number, parameter) == 1) { }
					else
					{
						asset_param_val.assign(MI.Get(stream_type, stream_number, parameter).c_str(), sizeof(asset_param_val));
						//std::wcout << "Stream Type:" << stream.c_str() << " Stream #:" << stream_number << " Parameter:" << parameter.c_str() << " Value:" << asset_param_val.c_str() << std::endl; 
					}
				}
				else if(file_info == 1)
				{
					if(wcscmp(parameter.c_str(), L"filesize") == 0)
					{
						std::ifstream file_info_file(_asset->full_filename, std::ios::binary | std::ios::in );
						file_info_file.seekg( 0, std::ios::end );
					
						std::string tempstring;
						String tempstring2;
						tempstring = std::to_string(file_info_file.tellg() / 1024);
						wchar_t *tempstring3 = new wchar_t[255];
						mbstowcs(tempstring3, tempstring.c_str(), sizeof(tempstring3));
						tempstring2.assign(tempstring3);
						asset_param_val.assign(tempstring2);
					}
					if(wcscmp(parameter.c_str(), L"fileage") == 0)
					{
						file_age(_asset);
					}


				}

				// check for exclusive parameter
				String param_prefix;
				param_prefix.assign(parameter_val,0,1);
				if(param_prefix.compare(L"!") == 0)
				{
					exclude = 1;
					parameter_val.assign(parameter_val, 1, parameter_val.length());
				}

				// check for greater-than or less-than
				param_prefix.assign(parameter_val,0,1);
				if(param_prefix.compare(L">") == 0)
				{
					greater_than = 1;
					parameter_val.assign(parameter_val, 1, parameter_val.length());
				}
				if(param_prefix.compare(L"<") == 0)
				{
					less_than = 1;
					parameter_val.assign(parameter_val, 1, parameter_val.length());
				}

				if(less_than == 1 || greater_than == 1)
				{
					char* asset_param_intval = new char[255];
					char* configparam_intval = new char[255];
					wcstombs(asset_param_intval, asset_param_val.c_str(), sizeof(asset_param_intval));
					wcstombs(configparam_intval, parameter_val.c_str(), sizeof(configparam_intval));

					// handle less-than comparison
					if(less_than == 1)
					{
						if(atoi((const char*)asset_param_intval) < atoi((const char*)configparam_intval))
						{
							if(exclude == 0) { }
							else match = 0; 
						}
						else
						{
							if(exclude == 1) { }
							else match = 0;
						}
					}

					// handle greater-than comparison
					if(greater_than == 1)
					{
						if(atoi((const char*)asset_param_intval) > atoi((const char*)configparam_intval))
						{
							if(exclude == 0) { }
							else match = 0;
						}
						else
						{
							if(exclude == 1) { }
							else match = 0;
						}
					}
				}

				// handle default comparison
				if(greater_than == 0 && less_than == 0)
				{
					if(wcscmp(asset_param_val.c_str(), parameter_val.c_str()) == 0)				
					{
						if(exclude == 0) { }
						else match = 0;
					}
					else
					{
						if(exclude == 1) { }
						else match = 0;
					}	
				}
			}
		}
		if(match == 1)
		{
			// rule matches - process rule
			process_rule(_asset, v.first.data(), v.second.data());
			
			// don't continue processing remaining rules if file has been moved/deleted
			char delimiters[] = "_";
			char *tok  = new char[255];
			tok = strtok((char*)v.first.data(), delimiters);
    
			while (tok != 0)
			{
				if(strcmp(tok, "move") == 0 || strcmp(tok, "delete") == 0)
				{
					return 1;
				}
				tok = strtok(NULL, delimiters);
			}			
		}
	}
	return err;
}


int metasorter::process_rule(asset* _asset, std::string first, std::string second)
{
	int err = 0;
	logstring.assign(_asset->full_filename);
	logstring.append(" matches rule: ");
	logstring.append(first);
	logstring.append(":");
	logstring.append(second);
	logfile.write(logstring);
	std::cout << std::endl << _asset->full_filename << " matches rule " << first << ":" << second << std::endl;

	char_separator<char> sep("_");
	tokenizer< char_separator<char> > tokens(first, sep);
	BOOST_FOREACH (const std::string& t, tokens)
	{
		
		if(t.compare("list") == 0)
		{
			ofstream listfile;
			listfile.open(second, ios::app);
			listfile << _asset->full_filename << endl;
			listfile.close();
		}

		if(t.compare("move") == 0)
		{
			std::string newfile(second);
			newfile.append(_asset->filename);
			logstring.assign("Moving ");
			logstring.append(_asset->full_filename);
			logstring.append(" to ");
			logstring.append(newfile);
			logfile.write(logstring);
			std::cout << "Moving " << _asset->full_filename << " to " << newfile << std::endl;
			std::ifstream ifs(_asset->full_filename, std::ios::binary);
			std::ofstream ofs(newfile, std::ios::binary);
			ofs << ifs.rdbuf();
			ifs.close();
			ofs.close();
				if( remove(_asset->full_filename) != 0 )
				{
					std::cout << "Error deleting file" << _asset->full_filename << std::endl;
					logstring.append("Error deleting file");
					logstring.append(_asset->full_filename);
					logfile.write(logstring);
				}
		}

		if(t.compare("copy") == 0)
		{
			std::string newfile(second);
			newfile.append(_asset->filename);
			logstring.assign("Copying ");
			logstring.append(_asset->full_filename);
			logstring.append(" to ");
			logstring.append(newfile);
			logfile.write(logstring);
			std::cout << "Copying " << _asset->full_filename << " to " << newfile << std::endl;
			std::ifstream ifs(_asset->full_filename, std::ios::binary);
			std::ofstream ofs(newfile, std::ios::binary);
			ofs << ifs.rdbuf();
			ifs.close();
			ofs.close();
		}

		if(t.compare("exec") == 0)
		{
			std::string execstring(second);
			execstring.append(" ");
			string_replace(execstring, "%s", _asset->full_filename);
			while(string_replace(execstring, "/", "\\"));
			logstring.assign("Executing: ");
			logstring.append(execstring);
			logfile.write(logstring);
			std::cout << "Executing: " << execstring << std::endl;
			system(execstring.c_str());
		}

		if(t.compare("delete") == 0)
		{
			if(second.compare("IAMSURE") == 0)
			{
				if( remove(_asset->full_filename) != 0 )
				{
					std::cout << "Error deleting file" << _asset->full_filename << std::endl;
					logstring.append("Error deleting file");
					logstring.append(_asset->full_filename);
					logfile.write(logstring);
				}
			}
		}
	}

	return err;
}


bool metasorter::string_replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


int metasorter::file_age(asset* _asset)
{
	int age = 0;

	/*
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    hFind = FindFirstFile((LPCWSTR)_asset->full_filename, &FindFileData);
	const FILETIME  ftFile = FindFileData.ftCreationTime;
	CTime ctFile = ftFile;
    CTime ctNow = ctNow.GetCurrentTime();
	CTimeSpan tsAge;
    tsAge = ctNow - ctFile;
	FILETIME ftAge;
    ULARGE_INTEGER ulAgeInSeconds;
	ulAgeInSeconds.QuadPart = tsAge.GetTotalSeconds() * 1E7;
	ftAge.dwLowDateTime=ulAgeInSeconds.LowPart;
    ftAge.dwHighDateTime = ulAgeInSeconds.HighPart;
	SYSTEMTIME stAge;
    FileTimeToSystemTime(&ftAge,&stAge);
	strYears.Format("%d", stAge.wYear-1601);
    strMonths.Format("%d",stAge.wMonth-1);
    strDays.Format("%d",stAge.wDay-1);
	*/
	return age;
}