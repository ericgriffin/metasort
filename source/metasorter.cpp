// metasorter.cpp

#include "metasorter.h"
#include "asset.h"
#include "util_functions.h"

using namespace boost::filesystem;

metasorter::metasorter(char* _path, tinyxml2::XMLDocument* _config)
{
	config = _config;
	strcpy(path, _path);
	tp.size_controller().resize(4);
	file_inspection_time = 20000;
	files_examined = 0;
	rule_matches = 0;
	actions_performed = 0;
	verbose = 0;

	if (int config_error = check_config(config) == 1)
	{
		log_mtx_.lock();
		std::cout << "Configuration Error - exiting" << std::endl;
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


int metasorter::check_config(tinyxml2::XMLDocument* config)
{
	logging = 0;
	extensions = 0;
	rules = 0;

	tinyxml2::XMLElement* xmlroot = config->FirstChildElement("metasort");

	/* check logging entry in config file */
	if (xmlroot->FirstChildElement("logging"))
	{
		if (xmlroot->FirstChildElement("logging")->Attribute("path"))
		{
			logging = 1;
			log_mtx_.lock();
			logfile.open(xmlroot->FirstChildElement("logging")->Attribute("path"));
			log_mtx_.unlock();
		}
		if (xmlroot->FirstChildElement("logging")->Attribute("console"))
		{
			if (std::string("yes").compare(xmlroot->FirstChildElement("logging")->Attribute("console")) == 0 || std::string("1").compare(xmlroot->FirstChildElement("logging")->Attribute("console")) == 0)
				verbose = 1;
		}
	}

	if (logging == 0)
	{
		log_mtx_.lock();
		std::cout << std::endl << "ERROR - No logging defined. Expecting <logging path=[path]/> - aborting." << std::endl;
		std::cout << "Finished." << std::endl;
		log_mtx_.unlock();
		exit(0);
	}

	/* check optional parameters in config file */
	if (xmlroot->FirstChildElement("file_inspection"))
	{
		if (xmlroot->FirstChildElement("file_inspection")->Attribute("time"))
			file_inspection_time = atoi(xmlroot->FirstChildElement("file_inspection")->Attribute("time"));
		else
		{
			log_mtx_.lock();
			std::cout << "WARNING - file_inspection time override exists but no value is set." << endl;
			log_mtx_.unlock();
		}
	}

	if (xmlroot->FirstChildElement("threadpool"))
	{
		if (xmlroot->FirstChildElement("threadpool")->Attribute("size"))
			tp.size_controller().resize(atoi(xmlroot->FirstChildElement("threadpool")->Attribute("size")));
		else
		{
			log_mtx_.lock();
			std::cout << "WARNING - threadpool size override exists but no value is set." << endl;
			log_mtx_.unlock();
		}
	}

	// check extensions entries in config file
	if (xmlroot->FirstChildElement("extension"))
	{
		if (xmlroot->FirstChildElement("extension")->Attribute("value"))
			extensions = 1;
	}
	if (extensions == 0)
	{
		log_mtx_.lock();
		std::cout << "ERROR - No extensions defined. Expecting <extension value=[value]/> - aborting" << std::endl;
		std::cout << "Finished." << std::endl;
		logfile.close();
		log_mtx_.unlock();
		exit(0);
	}

	// check rule entries in config file
	if (xmlroot->FirstChildElement("rule"))
	{
		if (xmlroot->FirstChildElement("rule")->FirstChildElement("action"))
		{
			rules = 1;
		}
	}
	if (rules == 0)
	{
		log_mtx_.lock();
		std::cout << "ERROR - No rules defined - aborting" << std::endl;
		std::cout << "Finished." << std::endl;
		logfile.close();
		log_mtx_.unlock();
		exit(0);
	}

	return 0;
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
					/*process_asset(_asset);*/  // single-threaded
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
					/*process_asset(_asset);*/  // single-threded
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


int metasorter::process_stream_blocks(asset* _asset, tinyxml2::XMLElement *v, int logical_op)
{
	MediaInfo MI;
	int MI_fetched = 0;
	int match = 1;
	int or_match = 0;

	// recurse "or" blocks
	for (tinyxml2::XMLElement *u = v->FirstChildElement("or"); u != NULL; u = u->NextSiblingElement("or"))
	{
		int rec_or_match = process_stream_blocks(_asset, u, 1);
		
		if (logical_op == 1)
		{
			or_match = or_match | rec_or_match;
			match = or_match;
		}
		else
			match = match & rec_or_match;
	}

	// recurse "and" blocks
	for (tinyxml2::XMLElement *u = v->FirstChildElement("and"); u != NULL; u = u->NextSiblingElement("and"))
	{
		int rec_and_match = process_stream_blocks(_asset, u, 0);
		
		if (logical_op == 1)
		{
			or_match = or_match | rec_and_match;
			match = or_match;
		}
		else
			match = match & rec_and_match;
	}

	for (tinyxml2::XMLElement *u = v->FirstChildElement("stream"); u != NULL; u = u->NextSiblingElement("stream"))
	{
		//determine stream and stream number
		std::string stream(u->Attribute("type"));
		int stream_number = atoi(u->Attribute("number"));

#ifdef MEDIAINFO_LIBRARY
		MediaInfoLib::stream_t stream_type = Stream_General;
#else
		MediaInfoDLL::stream_t stream_type;
#endif

		if (stream.compare("audio") == 0) stream_type = Stream_Audio;
		if (stream.compare("video") == 0) stream_type = Stream_Video;
		if (stream.compare("general") == 0) stream_type = Stream_General;
		if (stream.compare("text") == 0) stream_type = Stream_Text;
		if (stream.compare("other") == 0) stream_type = Stream_Other;
		if (stream.compare("image") == 0) stream_type = Stream_Image;
		if (stream.compare("menu") == 0) stream_type = Stream_Menu;

		for (tinyxml2::XMLElement *y = u->FirstChildElement("parameter"); y != NULL; y = y->NextSiblingElement("parameter"))
		{
			int parameter_match = 1;
			int exclude = 0;
			int greater_than = 0;
			int less_than = 0;
			int is_regex = 0;
			int is_range = 0;
			String asset_param_val;
			String parameter;
			String parameter_val;

			// parameter name from config file
			wchar_t* parameter_char = charToWChar(y->Attribute("name"));
			parameter = String(parameter_char);
			delete parameter_char;


			// parameter value from config file
			if (y->Attribute("value"))
			{
				wchar_t* parameter_val_char = charToWChar(y->Attribute("value"));
				parameter_val = String(parameter_val_char);
				delete parameter_val_char;
			}

			// parameter range from config file
			if (y->Attribute("range"))
			{
				wchar_t* parameter_val_char = charToWChar(y->Attribute("range"));
				parameter_val = String(parameter_val_char);
				delete parameter_val_char;
				is_range = 1;
			}

			// assign asset parameter value
			if (custom_parameters(asset_param_val, MI, _asset, stream_type, stream_number, parameter, MI_fetched) == 1) {}
			else
			{
				if (MI_fetched == 0)
				{
					call_MediaInfo(MI, _asset);
					MI_fetched = 1;
				}
				asset_param_val.assign(MI.Get(stream_type, stream_number, parameter).c_str(), sizeof(asset_param_val));
			}

			// check for and strip exclusive character
			String param_prefix;
			param_prefix.assign(parameter_val, 0, 1);
			if (param_prefix.compare(L"!") == 0)
			{
				exclude = 1;
				parameter_val.assign(parameter_val, 1, parameter_val.length());
			}

			// check for and strip greater-than or less-than
			param_prefix.assign(parameter_val, 0, 1);
			if (param_prefix.compare(L">") == 0)
			{
				greater_than = 1;
				parameter_val.assign(parameter_val, 1, parameter_val.length());
			}
			if (param_prefix.compare(L"<") == 0)
			{
				less_than = 1;
				parameter_val.assign(parameter_val, 1, parameter_val.length());
			}

			// check for and strip regular expression
			param_prefix.assign(parameter_val, 0, 7);
			if (param_prefix.compare(L"[REGEX]") == 0)
			{
				is_regex = 1;
				parameter_val.assign(parameter_val, 7, parameter_val.length());
			}

			// handle greater-than less-than
			if (less_than == 1 || greater_than == 1)
			{
				// convert values to integers
				char* asset_param_intval = new char[255];
				char* configparam_intval = new char[255];
				wcstombs(asset_param_intval, asset_param_val.c_str(), wcslen(asset_param_val.c_str()) + 1);
				wcstombs(configparam_intval, parameter_val.c_str(), wcslen(parameter_val.c_str()) + 1);

				// handle less-than comparison
				if (less_than == 1)
				{
					//cout << "Comparing Less than: " << atoi((const char*)asset_param_intval) << " TO " << atoi((const char*)configparam_intval) << endl;
					if (atoi((const char*)asset_param_intval) < atoi((const char*)configparam_intval))
					{
						if (exclude == 0) {}
						else
						{
							match = 0;
							parameter_match = 0;
						}
					}
					else
					{
						if (exclude == 1) {}
						else 
						{
							match = 0;
							parameter_match = 0;
						}
					}
				}

				// handle greater-than comparison
				if (greater_than == 1)
				{
					//cout << "Comparing Greater than: " << atoi((const char*)asset_param_intval) << " TO " << atoi((const char*)configparam_intval) << endl;
					if (atoi((const char*)asset_param_intval) > atoi((const char*)configparam_intval))
					{
						if (exclude == 0) {}
						else
						{
							match = 0;
							parameter_match = 0;
						}
					}
					else
					{
						if (exclude == 1) {}
						else
						{
							match = 0;
							parameter_match = 0;
						}
					}
				}

				delete[] asset_param_intval;
				delete[] configparam_intval;
			}


			// handle range comparison
			if (is_range == 1)
			{
				vector<string> param_low_high;
				boost::split(param_low_high, parameter_val, is_any_of(":"));

				if (param_low_high.size() < 2)
				{
					cout << "ERROR: parameter range used without \":\"";
					return 0;
				}
				
				char* asset_param_intval = new char[255];
				char* configparam_low_intval = new char[255];
				char* configparam_high_intval = new char[255];
				
				strcpy(configparam_low_intval, param_low_high.at(0).c_str());
				strcpy(configparam_high_intval, param_low_high.at(1).c_str());

				wcstombs(asset_param_intval, asset_param_val.c_str(), wcslen(asset_param_val.c_str()) + 1);
				
				if (atoi((const char*)asset_param_intval) >= atoi((const char*)configparam_low_intval) && atoi((const char*)asset_param_intval) <= atoi((const char*)configparam_high_intval))
				{
					if (exclude == 0) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}
				else
				{
					if (exclude == 1) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}

			}


			// handle regex comparison
			if (greater_than == 0 && less_than == 0 && is_regex == 1)
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

				if (boost::regex_match(asset_param_val_str, EXPR) == 1)
				{
					if (exclude == 0) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}
				else
				{
					if (exclude == 1) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}

				delete[] pattern;
				delete[] asset_param_val_char;
			}

			// handle default comparison
			if (greater_than == 0 && less_than == 0 && is_regex == 0 && is_range == 0)
			{
				//cout << "Comparing: " << asset_param_val << " TO " << parameter_val << endl;
				if (wcscmp(asset_param_val.c_str(), parameter_val.c_str()) == 0)
				{
					if (exclude == 0) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}
				else
				{
					if (exclude == 1) {}
					else
					{
						match = 0;
						parameter_match = 0;
					}
				}
			}

			// catch first failed match on "and" blocks and break loop
			if (logical_op == 0 && match == 0)
			{
				break;
			}

			// catch first match on "or" blocks and break loop
			if (logical_op == 1 && parameter_match == 1)
			{
				or_match = 1;
				break;
			}
		}

		// don't evaluate more "and" parameters if match is already false
		if (logical_op == 0 && match == 0)
		{
			break;
		}

		// don't evaluate more "or" parameters if or_match is already true
		if (logical_op == 1 && or_match == 1)
		{
			match = or_match;
			break;
		}
	}

	return match;
}



int metasorter::process_asset(asset* _asset)
{
	int err = 0;
	int stop_processing_rules = 0;
	String To_Display;
	
	tinyxml2::XMLElement* xmlroot = config->FirstChildElement("metasort");
	
	// Process rules from config file
	for (tinyxml2::XMLElement *v = xmlroot->FirstChildElement("rule"); v != NULL; v = v->NextSiblingElement("rule"))
	{
		int match = 1;

		// process streams outside of conditional blocks
		match = process_stream_blocks(_asset, v, 0);
		
		

		if(match == 1)
		{
			// rule matches - process rule actions
			std::string rule_name(v->Attribute("name"));
			log_mtx_.lock();
			logstring.assign("MATCH - ");
			logstring.append(rule_name);
			logstring.append(" : ");
			logstring.append(_asset->full_filename);
			logfile.write(logstring);
			if (verbose)
				std::cout << std::endl << "MATCH - " << rule_name << " : " << _asset->full_filename << std::endl;
			log_mtx_.unlock();

			// loop through rule actions
			for (tinyxml2::XMLElement *q = v->FirstChildElement("action"); q != NULL; q = q->NextSiblingElement("action"))
			{
				std::string rule_type(q->Attribute("type"));
				std::string rule_parameter(q->Attribute("parameter"));

				process_rule(_asset, rule_name, rule_type, rule_parameter);
				actions_performed++;

				// don't continue processing remaining rules if file has been moved/deleted
				if (strcmp(q->Attribute("type"), "move") == 0 || strcmp(q->Attribute("type"), "delete") == 0 || strcmp(q->Attribute("type"), "fastmove") == 0 || strcmp(q->Attribute("type"), "moveCUSTOM1") == 0 || strcmp(q->Attribute("type"), "fastmoveCUSTOM1") == 0)
				{
					stop_processing_rules = 1;
					break;
				}
			}

			rule_matches++;

			// don't continue processing remaining rules if file has been moved/deleted
			if (stop_processing_rules == 1)
			{
				break;
			}
		}

	}

	files_examined++;

	delete _asset;
	return err;
}


int metasorter::process_rule(asset* _asset, std::string rule_name, ::string first, std::string second)
{
	int err = 0;

	if (first.compare("list") == 0)
		action_list(_asset, rule_name, second);

	else if (first.compare("move") == 0)
		action_move(_asset, rule_name, second);

	else if (first.compare("fastmove") == 0)
		action_fastmove(_asset, rule_name, second);

	else if (first.compare("copy") == 0)
		action_copy(_asset, rule_name, second);

	else if (first.compare("copyonce") == 0)
		action_copyonce(_asset, rule_name, second);

	else if (first.compare("exec") == 0)
		action_exec(_asset, rule_name, second);

	else if (first.compare("delete") == 0)
		action_delete(_asset, rule_name, second);

	else if (first.compare("copyonceCUSTOM1") == 0)
		action_copyonceCUSTOM1(_asset, rule_name, second);

	else if (first.compare("moveCUSTOM1") == 0)
		action_moveCUSTOM1(_asset, rule_name, second);

	else if (first.compare("fastmoveCUSTOM1") == 0)
		action_fastmoveCUSTOM1(_asset, rule_name, second);

	else if (first.compare("md5file") == 0)
		action_md5file(_asset, rule_name, second);

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

	return err;
}


int metasorter::process_extensions(asset* _asset)
{
	int valid_extension = 0;

	tinyxml2::XMLElement* xmlroot = config->FirstChildElement("metasort");

	for (tinyxml2::XMLElement *v = xmlroot->FirstChildElement("extension"); v != NULL; v = v->NextSiblingElement("extension"))
	{
		int ext_is_regex = 0;
		String extension_prefix;
		String extension_regex_suffix;
		wchar_t* temp_str = charToWChar(v->Attribute("value"));
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
			if(strcmp(_asset->extension, v->Attribute("value")) == 0)
			{
				valid_extension = 1;
			}
		}
	}
	return valid_extension;
}





