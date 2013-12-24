// metasorter.cpp

#include "metasorter.h"
#include "asset.h"
#include "util_functions.h"

using namespace boost::filesystem;

metasorter::metasorter(char* _path, boost::property_tree::ptree _pt)
{
	strcpy(path, _path);
	pt = _pt;
	tp.size_controller().resize(4);
	file_inspection_time = 20000;
	logging = 0;
	optional<const boost::property_tree::ptree&> pt_check_existence;

	pt_check_existence = pt.get_child_optional("config");
	if(pt_check_existence)
	{
		BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("config"))
		{
			if(strcmp(v.first.c_str(), "logfile") == 0)
			{
				logging = 1;
				log_mtx_.lock();
				logfile.open(v.second.data().c_str());
				log_mtx_.unlock();
			}
			
			if(strcmp(v.first.c_str(), "file_inspection_time") == 0)
			{
				file_inspection_time = atoi(v.second.data().c_str());
			}

			if(strcmp(v.first.c_str(), "threadpool_size") == 0)
			{
				tp.size_controller().resize(atoi(v.second.data().c_str()));
			}
		}
	}

	if(logging == 0)
	{
		log_mtx_.lock();
		cout << "No logging defined in config file - aborting" << endl;
		log_mtx_.unlock();
		exit(0);
	}

	// check extensions
	pt_check_existence = pt.get_child_optional("extensions");
	if(!pt_check_existence)
	{
		log_mtx_.lock();
		cout << "No extensions defined in config file - aborting" << endl;
		logfile.close();
		log_mtx_.unlock();
		exit(0);
	}

	// check rules
	pt_check_existence = pt.get_child_optional("rules");
	if(!pt_check_existence)
	{
		log_mtx_.lock();
		cout << "No rules defined in config file - aborting" << endl;
		logfile.close();
		log_mtx_.unlock();
		exit(0);
	}

	log_mtx_.lock();
	logstring.assign("Entering folder ");
	logstring.append(path);
	logfile.write(logstring);
	log_mtx_.unlock();

}


metasorter::~metasorter()
{
	log_mtx_.lock();
	logstring.assign("Leaving folder ");
	logstring.append(path);
	logfile.write(logstring);
	logfile.close();
	log_mtx_.unlock();
}


int metasorter::traverse_directory(int _recurse)
{
	
	int err = 0;

	
	boost::filesystem::path dir_path = boost::filesystem::path(path);
	boost::filesystem::directory_iterator itr(dir_path);
	boost::filesystem::recursive_directory_iterator itr_r(dir_path);

	if(_recurse == 0)
	{
		// non-recursive search
		while (itr != boost::filesystem::directory_iterator())
		{
			// if it is a file
			if(itr->status().type() != directory_file)
			{
				asset* _asset = new asset;
				strcpy(_asset->full_filename, itr->path().string().c_str());
				strcpy(_asset->filename, itr->path().filename().string().c_str());
				strcpy(_asset->path, path);
				strcpy(_asset->extension, itr->path().filename().extension().string().c_str());

				// filter extensions
							
				if(process_extensions(_asset) == 1)
				{
					tp.schedule(boost::bind(&metasorter::process_asset, this, _asset));  // multi-threaded
					//process_asset(_asset);  // single-threaded
				}
			}
			++itr;
		}
	}

	if(_recurse == 1)
	{
		// recursive search
		while (itr_r != boost::filesystem::recursive_directory_iterator())
		{
			// if it is a file
            if(itr_r->status().type() != directory_file)
			{
				asset* _asset = new asset;
				strcpy(_asset->full_filename, itr_r->path().string().c_str());
				strcpy(_asset->filename, itr_r->path().filename().string().c_str());
				strcpy(_asset->path, path);
				strcpy(_asset->extension, itr_r->path().filename().extension().string().c_str());

				// filter extensions
				

				if(process_extensions(_asset) == 1)
				{
					tp.schedule(boost::bind(&metasorter::process_asset, this, _asset));  // multi-threaded
					//process_asset(_asset);  // single-threded
				}
			}
			++itr_r;
		}
	}

	return err;
}



int metasorter::process_file()
{
	int err = 0;

	asset* _asset = new asset;
	boost::filesystem::path file_path = boost::filesystem::path(path);

	strcpy(_asset->full_filename, file_path.string().c_str());
	strcpy(_asset->filename, file_path.filename().string().c_str());
	strcpy(_asset->path, file_path.parent_path().string().c_str());
	strcpy(_asset->extension, file_path.filename().extension().string().c_str());

	// filter extensions

	if(process_extensions(_asset) == 1)
	{
		process_asset(_asset);
	}

	return err;
}


int metasorter::call_MediaInfo(MediaInfo &MI, asset* _asset)
{
	FILE* F = NULL;
	F = fopen(_asset->full_filename, "rb");

	if (F == 0)
	{
		log_mtx_.lock();
		logstring.assign("Can't open file ");
		logstring.append(_asset->full_filename);
		logstring.append(" for processing");
		logfile.write(logstring);
		std::cout << "Can't open file: " << _asset->full_filename << " for processing" << std::endl;
		log_mtx_.unlock();
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
	delete[] From_Buffer;
	return true;
}


int metasorter::process_asset(asset* _asset)
{
	int err = 0;
	String To_Display;

	MediaInfo MI;
	int MI_fetched = 0;
	
	// Process rules from config file
	boost::property_tree::ptree pt1 = pt.get_child("rules");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt1)
	{
		int match = 1;
		BOOST_FOREACH(boost::property_tree::ptree::value_type &u, pt1.get_child(v.first.data()))
		{
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

			if(stream.compare("audio") == 0) stream_type = Stream_Audio;
			if(stream.compare("video") == 0) stream_type = Stream_Video;
			if(stream.compare("general") == 0) stream_type = Stream_General;
			if(stream.compare("text") == 0) stream_type = Stream_Text;
			if(stream.compare("other") == 0) stream_type = Stream_Other;
			if(stream.compare("image") == 0) stream_type = Stream_Image;
			if(stream.compare("menu") == 0) stream_type = Stream_Menu;

			BOOST_FOREACH(boost::property_tree::ptree::value_type &y, pt1.get_child(v.first.data()).get_child(u.first.data()))
			{
				int exclude = 0;
				int greater_than = 0;
				int less_than = 0;
				int is_regex = 0;
				String asset_param_val;

				// parameter name from config file
				wchar_t* parameter_char = charToWChar(y.first.c_str());
				String parameter = String(parameter_char);
				delete parameter_char;

				// parameter value from config file
				wchar_t* parameter_val_char = charToWChar(y.second.data().c_str());
				String parameter_val = String(parameter_val_char);
				delete parameter_val_char;

				// assign asset parameter value
				if(custom_parameters(asset_param_val, MI, _asset, stream_type, stream_number, parameter, MI_fetched) == 1) { }
				else
				{
					if(MI_fetched == 0)
					{
						call_MediaInfo(MI, _asset);
						MI_fetched = 1;
					}
					asset_param_val.assign(MI.Get(stream_type, stream_number, parameter).c_str(), sizeof(asset_param_val));
				}

				// check for and strip exclusive character
				String param_prefix;
				param_prefix.assign(parameter_val,0,1);
				if(param_prefix.compare(L"!") == 0)
				{
					exclude = 1;
					parameter_val.assign(parameter_val, 1, parameter_val.length());
				}

				// check for and strip greater-than or less-than
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

				// check for and strip regular expression
				param_prefix.assign(parameter_val,0,7);
				if(param_prefix.compare(L"[REGEX]") == 0)
				{
					is_regex = 1;
					parameter_val.assign(parameter_val, 7, parameter_val.length());
				}

				// handle greater-than less-than
				if(less_than == 1 || greater_than == 1)
				{
					// convert values to integers
					char* asset_param_intval = new char[255];
					char* configparam_intval = new char[255];
					wcstombs(asset_param_intval, asset_param_val.c_str(), wcslen(asset_param_val.c_str()) + 1);
					wcstombs(configparam_intval, parameter_val.c_str(), wcslen(parameter_val.c_str()) + 1);

					// handle less-than comparison
					if(less_than == 1)
					{
						//cout << "Comparing Less than: " << atoi((const char*)asset_param_intval) << " TO " << atoi((const char*)configparam_intval) << endl;
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
						//cout << "Comparing Greater than: " << atoi((const char*)asset_param_intval) << " TO " << atoi((const char*)configparam_intval) << endl;
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

					delete asset_param_intval;
					delete configparam_intval;
				}

				// handle regex comparison
				if(greater_than == 0 && less_than == 0 && is_regex == 1)
				{
					// convert parameter_val to char*
					char* pattern = new char[255];
					wcstombs(pattern, parameter_val.c_str(), wcslen(parameter_val.c_str()) + 1);
					boost::regex EXPR(pattern, boost::regex::basic);

					// convert MediaInfo::String::asset_param_val to std::string
					std::string asset_param_val_str;
					char* asset_param_val_char = new char[255];
					wcstombs(asset_param_val_char, asset_param_val.c_str(), asset_param_val.length() + 1);
					asset_param_val_str.assign(asset_param_val_char);

					if(boost::regex_match(asset_param_val_str, EXPR) == 1)
					{
						if(exclude == 0) { }
						else match = 0;
					}
					else
					{
						if(exclude == 1) { }
						else match = 0;
					}

					delete[] pattern;
					delete[] asset_param_val_char;
				}

				// handle default comparison
				if(greater_than == 0 && less_than == 0 && is_regex == 0)
				{
					//cout << "Comparing: " << asset_param_val << " TO " << parameter_val << endl;
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
			const char delimiters[] = "_";
			char *tok;
			std::string rulename = std::string((char*)v.first.data());
			tok = strtok((char*)rulename.c_str(), delimiters);

			while (tok != 0)
			{
				if(strcmp(tok, "move") == 0 || strcmp(tok, "delete") == 0)
				{
					return 1;
				}
				tok = strtok(NULL, delimiters);
			}
			delete tok;
		}
	}
	delete _asset;
	return err;
}


int metasorter::process_rule(asset* _asset, std::string first, std::string second)
{
	int err = 0;

	log_mtx_.lock();
	logstring.assign(_asset->full_filename);
	logstring.append(" matches rule: ");
	logstring.append(first);
	logstring.append(":");
	logstring.append(second);
	logfile.write(logstring);
	std::cout << std::endl << _asset->full_filename << " MATCHES RULE " << first << std::endl;
	log_mtx_.unlock();

	char_separator<char> sep("_");
	tokenizer< char_separator<char> > tokens(first, sep);
	int second_token = 0;  //keeps track of which token (before or after the '_') is being evaluated
	BOOST_FOREACH (const std::string& t, tokens)
	{
		if (second_token == 0)
			second_token = 1;

		else if(second_token == 1)  //only evaluate the token after the '_'
		{
			second_token = 0;

			if(t.compare("list") == 0)
				action_list(_asset, first, second);

			else if(t.compare("move") == 0)
				action_move(_asset, first, second);

			else if(t.compare("fastmove") == 0)
				action_fastmove(_asset, first, second);

			else if(t.compare("copy") == 0)
				action_copy(_asset, first, second);

			else if(t.compare("copyonce") == 0)
				action_copyonce(_asset, first, second);

			else if(t.compare("exec") == 0)
				action_exec(_asset, first, second);

			else if(t.compare("delete") == 0)
				action_delete(_asset, first, second);

			else if(t.compare("copyonceCUSTOM1") == 0)
				action_copyonceCUSTOM1(_asset, first, second);

			else if(t.compare("md5file") == 0)
				action_md5file(_asset, first, second);

			else 
			{
				log_mtx_.lock();
				logstring.assign("**ERROR** rule action: ");
				logstring.append(first);
				logstring.append(" is undefined");
				logfile.write(logstring);
				std::cout << std::endl << "**ERROR** rule action: " << first << " is undefined" << std::endl;
				log_mtx_.unlock();
			}
		}
	}

	return err;
}


int metasorter::process_extensions(asset* _asset)
{
	int valid_extension = 0;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("extensions"))
	{
		int ext_is_regex = 0;
		String extension_prefix;
		String extension_regex_suffix;
		wchar_t* temp_str = charToWChar(v.first.data());
		String full_extension = String(temp_str);
		delete temp_str;
		extension_prefix.assign(full_extension,0,7);
		
		if(extension_prefix.compare(L"[REGEX]") == 0)
		{
			ext_is_regex = 1;
			extension_regex_suffix.assign(full_extension, 7, full_extension.length());
			
			// convert extension_regex_suffix to char*
			char* pattern = new char[255];
			wcstombs(pattern, extension_regex_suffix.c_str(), wcslen(extension_regex_suffix.c_str()) + 1);
			boost::regex EXPR(pattern, boost::regex::basic);

			// convert MediaInfo::String::asset_param_val to std::string
			std::string asset_extension_str;			
			asset_extension_str.assign(_asset->extension);

			if(boost::regex_match(asset_extension_str, EXPR) == 1)
			{
				valid_extension = 1;
			}

			delete[] pattern;
		}

		else
		{
			if(strcmp(_asset->extension, v.first.data()) == 0)
			{
				valid_extension = 1;
			}
		}
	}
	return valid_extension;
}





