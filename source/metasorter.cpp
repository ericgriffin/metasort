/*  metasorter.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#include "metasorter.h"


metasorter::metasorter()
{
	config = new metasortconfig(this);
	file_inspection_time = DEFAULT_FILE_INSPECTION_TIME;
	tp.size_controller().resize(DEFAULT_THREADPOOL_SIZE);
	files_examined = 0;
	rule_matches = 0;
	actions_performed = 0;
	verbose = 0;
	debug = 0;
	run_type = 0;
}


metasorter::~metasorter()
{
	delete config;
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
			if (itr->status().type() != boost::filesystem::directory_file)
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
			if (itr_r->status().type() != boost::filesystem::directory_file)
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
	int not_match = 1;

	// recurse "not" blocks
	for (tinyxml2::XMLElement *u = v->FirstChildElement("not"); u != NULL; u = u->NextSiblingElement("not"))
	{
		int rec_not_match = process_stream_blocks(_asset, u, 2);

		if (logical_op == 1)
		{
			or_match = (or_match | rec_not_match);
			match = or_match;
		}
		else if (logical_op == 2)
		{
			not_match = (not_match & ((rec_not_match) ^ 1));
			match = not_match;
		}
		else
			match = (match & rec_not_match);
	}

	// recurse "or" blocks
	for (tinyxml2::XMLElement *u = v->FirstChildElement("or"); u != NULL; u = u->NextSiblingElement("or"))
	{
		int rec_or_match = process_stream_blocks(_asset, u, 1);
		
		if (logical_op == 1)
		{
			or_match = (or_match | rec_or_match);
			match = or_match;
		}
		else if (logical_op == 2)
		{
			not_match = (not_match & ((rec_or_match) ^ 1));
			match = not_match;
		}
		else
			match = (match & rec_or_match);
	}

	// recurse "and" blocks
	for (tinyxml2::XMLElement *u = v->FirstChildElement("and"); u != NULL; u = u->NextSiblingElement("and"))
	{
		int rec_and_match = process_stream_blocks(_asset, u, 0);
		
		if (logical_op == 1)
		{
			or_match = (or_match | rec_and_match);
			match = or_match;
		}
		else if (logical_op == 2)
		{
			not_match = (not_match & ((rec_and_match) ^ 1));
			match = not_match;
		}
		else
			match = (match & rec_and_match);
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
			MediaInfoLib::String asset_param_val;
			MediaInfoLib::String parameter;
			MediaInfoLib::String parameter_val;

			// parameter name from config file
			wchar_t* parameter_char = metasortutil::charToWChar(y->Attribute("name"));
			parameter = MediaInfoLib::String(parameter_char);
			delete parameter_char;


			// parameter value from config file
			if (y->Attribute("value"))
			{
				wchar_t* parameter_val_char = metasortutil::charToWChar(y->Attribute("value"));
				parameter_val = MediaInfoLib::String(parameter_val_char);
				delete parameter_val_char;
			}

			// parameter range from config file
			if (y->Attribute("range"))
			{
				wchar_t* parameter_val_char = metasortutil::charToWChar(y->Attribute("range"));
				parameter_val = MediaInfoLib::String(parameter_val_char);
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
			MediaInfoLib::String param_prefix;
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
					// debug information
					if (debug)
					{
						char* parameter_mbs = new char[255];
						wcstombs(parameter_mbs, parameter.c_str(), wcslen(parameter.c_str()) + 1);
						
						log_mtx_.lock();
						logstring.assign("Comparing ");
						logstring.append(_asset->full_filename);
						logstring.append(" (");
						logstring.append(stream);
						logstring.append(" ");
						logstring.append(boost::lexical_cast<std::string>(stream_number));
						logstring.append(" ");
						logstring.append(parameter_mbs);
						logstring.append("): ");
						logstring.append(asset_param_intval);
						logstring.append(" ");
						if (exclude == 1)
							logstring.append("!");
						logstring.append("< ");
						logstring.append(configparam_intval);
						logfile.write(logstring);

						if (verbose)
						{
							std::cout << "Comparing " << _asset->full_filename << " (" << stream << " " << stream_number << " " << parameter_mbs << "): " << ((const char*)asset_param_intval) << " ";
							if (exclude == 1)
								std::cout << "!";
							std::cout << "< " << atof((const char*)configparam_intval) << std::endl; // debug mode
						}

						log_mtx_.unlock();
						delete[] parameter_mbs;
					}

					if (atof((const char*)asset_param_intval) < atof((const char*)configparam_intval))
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
					// debug information
					if (debug)
					{
						char* parameter_mbs = new char[255];
						wcstombs(parameter_mbs, parameter.c_str(), wcslen(parameter.c_str()) + 1);

						log_mtx_.lock();
						logstring.assign("Comparing ");
						logstring.append(_asset->full_filename);
						logstring.append(" (");
						logstring.append(stream);
						logstring.append(" ");
						logstring.append(boost::lexical_cast<std::string>(stream_number));
						logstring.append(" ");
						logstring.append(parameter_mbs);
						logstring.append("): ");
						logstring.append(asset_param_intval);
						logstring.append(" ");
						if (exclude == 1)
							logstring.append("!");
						logstring.append("> ");
						logstring.append(configparam_intval);
						logfile.write(logstring);

						if (verbose)
						{
							std::cout << "Comparing " << _asset->full_filename << " (" << stream << " " << stream_number << " " << parameter_mbs << "): " << (const char*)asset_param_intval << " ";
							if (exclude == 1)
								std::cout << "!";
							std::cout << "> " << (const char*)configparam_intval << std::endl; // debug mode
						}

						log_mtx_.unlock();
						delete[] parameter_mbs;
					}

					if (atof((const char*)asset_param_intval) > atof((const char*)configparam_intval))
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
				std::vector<std::string> param_low_high;
				boost::split(param_low_high, parameter_val, boost::is_any_of(":"));

				if (param_low_high.size() < 2)
				{
					std::cout << "ERROR: parameter range used without \":\"";
					return 0;
				}
				
				char* asset_param_intval = new char[255];
				char* configparam_low_intval = new char[255];
				char* configparam_high_intval = new char[255];
				
				strcpy(configparam_low_intval, param_low_high.at(0).c_str());
				strcpy(configparam_high_intval, param_low_high.at(1).c_str());

				wcstombs(asset_param_intval, asset_param_val.c_str(), wcslen(asset_param_val.c_str()) + 1);
				
				// debug information
				if (debug)
				{
					char* parameter_mbs = new char[255];
					wcstombs(parameter_mbs, parameter.c_str(), wcslen(parameter.c_str()) + 1);

					log_mtx_.lock();
					logstring.assign("Comparing ");
					logstring.append(_asset->full_filename);
					logstring.append(" (");
					logstring.append(stream);
					logstring.append(" ");
					logstring.append(boost::lexical_cast<std::string>(stream_number));
					logstring.append(" ");
					logstring.append(parameter_mbs);
					logstring.append("): ");
					logstring.append(configparam_low_intval);
					if (exclude == 1)
						logstring.append(" >= ");
					else
						logstring.append(" <= ");
					logstring.append(asset_param_intval);
					if (exclude == 1)
						logstring.append(" <= ");
					else
						logstring.append(" >= ");
					logstring.append(configparam_high_intval);
					logfile.write(logstring);

					if (verbose)
					{
						std::cout << "Comparing range " << _asset->full_filename << " (" << stream << " " << stream_number << " " << parameter_mbs << "): " << configparam_low_intval;
						if (exclude == 1)
							std::cout << " >= ";
						else
							std::cout << " <= ";
						std::cout << asset_param_intval;
						if (exclude == 1)
							std::cout << " <= ";
						else
							std::cout << " >= ";
						std::cout << configparam_high_intval << std::endl; // debug mode
					}

					log_mtx_.unlock();
					delete[] parameter_mbs;
				}

				if (atof((const char*)asset_param_intval) >= atof((const char*)configparam_low_intval) && atof((const char*)asset_param_intval) <= atof((const char*)configparam_high_intval))
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
				delete[] asset_param_intval;
				delete[] configparam_low_intval;
				delete[] configparam_high_intval;
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
				if (debug)
				{
					char* asset_param_mbsval = new char[255];
					char* configparam_mbsval = new char[255];
					char* parameter_mbs = new char[255];
					wcstombs(asset_param_mbsval, asset_param_val.c_str(), wcslen(asset_param_val.c_str()) + 1);
					wcstombs(configparam_mbsval, parameter_val.c_str(), wcslen(parameter_val.c_str()) + 1);
					wcstombs(parameter_mbs, parameter.c_str(), wcslen(parameter.c_str()) + 1);

					log_mtx_.lock();
					logstring.assign("Comparing ");
					logstring.append(_asset->full_filename);
					logstring.append(" (");
					logstring.append(stream);
					logstring.append(" ");
					logstring.append(boost::lexical_cast<std::string>(stream_number));
					logstring.append(" ");
					logstring.append(parameter_mbs);
					logstring.append("): ");
					logstring.append(asset_param_mbsval);
					logstring.append(" ");
					if (exclude == 1)
						logstring.append("!");
					logstring.append("= ");
					logstring.append(configparam_mbsval);
					logfile.write(logstring);
					
					if (verbose)
					{
						std::wcout << "Comparing " << _asset->full_filename << " (";
						std::cout << stream;
						std::wcout << " " << stream_number << " " << parameter_mbs << "): " << asset_param_val.c_str() << " ";
						if (exclude == 1)
							std::cout << "!";
						std::wcout << "= " << parameter_val.c_str() << std::endl; // debug mode
					}

					log_mtx_.unlock();

					delete[] asset_param_mbsval;
					delete[] configparam_mbsval;
					delete[] parameter_mbs;
				}

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

			// catch first match on "not" blocks and break loop
			if (logical_op == 2 && parameter_match == 1)
			{
				not_match = 0;
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

		// don't evaluate more "not" parameters if not_match is already false
		if (logical_op == 2)
		{
			match = not_match;
			if (not_match == 0)
			{
				break;
			}
		}
	}

	return match;
}



int metasorter::process_asset(asset* _asset)
{
	int err = 0;
	int stop_processing_rules = 1;
	
	tinyxml2::XMLElement* xmlroot = config->config->FirstChildElement("metasort");

	// Process rules from config file
	for (tinyxml2::XMLElement *v = xmlroot->FirstChildElement("rule"); v != NULL; v = v->NextSiblingElement("rule"))
	{
		int match = 1;

		// see if "continue" parameter is set - evaluates next rule even if current rule matches
		if (v->Attribute("continue"))
		{
			if (strcmp(v->Attribute("continue"), "1") == 0 || strcmp(v->Attribute("continue"), "true") == 0 || strcmp(v->Attribute("continue"), "yes") == 0)
			{
				stop_processing_rules = 0;
			}
			else
			{
				stop_processing_rules = 1;
			}
		}

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
				actions_performed_mtx_.lock();
				actions_performed++;
				actions_performed_mtx_.unlock();

				// don't continue processing remaining rules if file has been moved/deleted
				if (strcmp(q->Attribute("type"), "move") == 0 || strcmp(q->Attribute("type"), "delete") == 0 || strcmp(q->Attribute("type"), "fastmove") == 0 || strcmp(q->Attribute("type"), "moveCUSTOM1") == 0 || strcmp(q->Attribute("type"), "fastmoveCUSTOM1") == 0)
				{
					stop_processing_rules = 1;
					break;
				}
			}

			rule_matches_mtx_.lock();
			rule_matches++;
			rule_matches_mtx_.unlock();

			// don't continue processing remaining rules if file has been moved/deleted
			if (stop_processing_rules == 1)
			{
				break;
			}
		}
	}

	file_examined_mtx_.lock();
	files_examined++;
	file_examined_mtx_.unlock();

	delete _asset;
	return err;
}


int metasorter::process_rule(asset* _asset, std::string rule_name, std::string first, std::string second)
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

	//tinyxml2::XMLElement* xmlroot = metasort_config->config->FirstChildElement("metasort");

	for (tinyxml2::XMLElement *v = config->xmlroot->FirstChildElement("extension"); v != NULL; v = v->NextSiblingElement("extension"))
	{
		int ext_is_regex = 0;
		MediaInfoLib::String extension_prefix;
		MediaInfoLib::String extension_regex_suffix;
		wchar_t* temp_str = metasortutil::charToWChar(v->Attribute("value"));
		MediaInfoLib::String full_extension = MediaInfoLib::String(temp_str);
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


int metasorter::load_config_file(std::string file)
{
	int err = 0;
	log_mtx_.lock();
	logstring.assign("Using configuration file ");
	logstring.append(file);
	logfile.write(logstring);
	std::cout << std::endl << "Using configuration file: " << file << std::endl;
	log_mtx_.unlock();

	err = config->read_configuration(file);

	if (err == 1)
	{
		log_mtx_.lock();
		logstring.assign("ABORTING - Configuration error in: ");
		logstring.append(file);
		logfile.write(logstring);
		std::cout << std::endl << "ABORTING - Configuration error in " << file << std::endl;
		log_mtx_.unlock();
	}
	return err;
}


int metasorter::set_input_file(std::string file)
{
	int err = 0;
	run_type = 1;
	err = config->set_input_file(file);
	if (err == 1)
	{
		log_mtx_.lock();
		logstring.assign("ABORTING - Error opening ");
		logstring.append(file);
		logfile.write(logstring);
		std::cout << std::endl << "ABORTING - Error opening " << file << std::endl;
		log_mtx_.unlock();
	}
	return err;
}

int metasorter::run()
{
	int err = 0;

	if (run_type == 0)  // if processing folders from config
	{
		// iterate through search paths
		for (tinyxml2::XMLElement *e = config->xmlroot->FirstChildElement("folder"); e != NULL; e = e->NextSiblingElement("folder"))
		{
			//check to see if path is valid
			if (!metasortutil::path_exists(std::string(e->Attribute("path"))))
			{
				std::cout << "ERROR - search path does not exist: " << e->Attribute("path") << " - skipping" << std::endl;
				continue;
			}
			else
			{
				strcpy(path, e->Attribute("path"));
			}

			//check if folder should be searched recursively
			int recurse = 0;
			if (e->Attribute("recursive"))
			{
				if (std::string("yes").compare(e->Attribute("recursive")) == 0 || std::string("1").compare(e->Attribute("recursive")) == 0 || std::string("true").compare(e->Attribute("recursive")) == 0)
					recurse = 1;
			}

			if (recurse == 1)
			{
				log_mtx_.lock();
				logstring.assign("Scanning folder and subfolders ");
				logstring.append(path);
				logfile.write(logstring);
				std::cout << std::endl << "Scanning folder and subfolders: " << e->Attribute("path") << std::endl;
				log_mtx_.unlock();
			}
			else
			{
				log_mtx_.lock();
				logstring.assign("Scanning folder ");
				logstring.append(path);
				logfile.write(logstring);
				std::cout << std::endl << "Scanning folder: " << e->Attribute("path") << std::endl;
				log_mtx_.unlock();
			}
			traverse_directory(recurse);
			tp.wait();
		
			log_mtx_.lock();
			logstring.assign("Finished scanning folder ");
			logstring.append(path);
			logfile.write(logstring);
			log_mtx_.unlock();
		}
	}
	else  // if processing input files from argv[]
	{
		strcpy(path, config->input_file->c_str());

		log_mtx_.lock();
		logstring.assign("Scanning input file ");
		logstring.append(path);
		logfile.write(logstring);
		std::cout << std::endl << "Scanning input file " << path << std::endl;
		log_mtx_.unlock();

		process_file();
		
		log_mtx_.lock();
		logstring.assign("Finished scanning file ");
		logstring.append(path);
		logfile.write(logstring);
		log_mtx_.unlock();

	}

	log_mtx_.lock();
	logstring.assign("Finished with config file: ");
	logstring.append(*config->config_file);
	logfile.write(logstring);
	std::cout << std::endl << "Finished with config file: " << *config->config_file << std::endl;
	logfile.close();
	log_mtx_.unlock();

	return err;
}
