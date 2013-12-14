#include "metasorter.h"
#include "md5.h"
#include "util_functions.h"

using namespace boost::filesystem;


int metasorter::action_list(asset* _asset, std::string first, std::string second)
{
	list_mtx_.lock();
	ofstream listfile;
	listfile.open(second.c_str(), ios::app);
	listfile << _asset->full_filename << endl;
	listfile.close();
	list_mtx_.unlock();
	return true;
}


int metasorter::action_move(asset* _asset, std::string first, std::string second)
{
	if(filesize_changing(_asset->full_filename, file_inspection_time) == 1)
	{
		log_mtx_.lock();
		std::cout << _asset->full_filename << " is changing in filesize - skipping " << std::endl;
		logstring.assign(_asset->full_filename);
		logstring.append(" is changing in filesize - skipping ");
		logfile.write(logstring);
		log_mtx_.unlock();

		return 1;
	}
	std::string newfile(second);
	newfile.append(_asset->filename);

	log_mtx_.lock();
	logstring.assign("Moving ");
	logstring.append(_asset->full_filename);
	logstring.append(" to ");
	logstring.append(newfile);
	logfile.write(logstring);
	std::cout << "Moving " << _asset->full_filename << " to " << newfile << std::endl;
	log_mtx_.unlock();

	std::ifstream ifs(_asset->full_filename, std::ios::binary);
	std::ofstream ofs(newfile.c_str(), std::ios::binary);
	ofs << ifs.rdbuf();
	ifs.close();
	ofs.close();
	if( remove(_asset->full_filename) != 0 )
	{
		log_mtx_.lock();
		std::cout << "Error deleting file" << _asset->full_filename << std::endl;
		logstring.append("Error deleting file");
		logstring.append(_asset->full_filename);
		logfile.write(logstring);
		log_mtx_.unlock();
	}
	return true;
}


int metasorter::action_fastmove(asset* _asset, std::string first, std::string second)
{
	if(filesize_changing(_asset->full_filename, file_inspection_time) == 1)
	{
		log_mtx_.lock();
		std::cout << _asset->full_filename << " is changing in filesize - skipping " << std::endl;
		logstring.assign(_asset->full_filename);
		logstring.append(" is changing in filesize - skipping ");
		logfile.write(logstring);
		log_mtx_.unlock();

		return 1;
	}
	std::string newfile(second);
	newfile.append(_asset->filename);

	log_mtx_.lock();
	logstring.assign("Moving ");
	logstring.append(_asset->full_filename);
	logstring.append(" to ");
	logstring.append(newfile);
	logfile.write(logstring);
	std::cout << "Moving " << _asset->full_filename << " to " << newfile << std::endl;
	log_mtx_.unlock();

	boost::filesystem::rename(_asset->full_filename, newfile.c_str());	
	return true;
}


int metasorter::action_copy(asset* _asset, std::string first, std::string second)
{
	if(filesize_changing(_asset->full_filename, file_inspection_time) == 1)
	{
		log_mtx_.lock();
		std::cout << _asset->full_filename << " is changing in filesize - skipping " << std::endl;
		logstring.assign(_asset->full_filename);
		logstring.append(" is changing in filesize - skipping ");
		logfile.write(logstring);
		log_mtx_.unlock();

		return 1;
	}
	std::string newfile(second);
	newfile.append(_asset->filename);

	log_mtx_.lock();
	logstring.assign("Copying ");
	logstring.append(_asset->full_filename);
	logstring.append(" to ");
	logstring.append(newfile);
	logfile.write(logstring);
	std::cout << "Copying " << _asset->full_filename << " to " << newfile << std::endl;
	log_mtx_.unlock();

	std::ifstream ifs(_asset->full_filename, std::ios::binary);
	std::ofstream ofs(newfile.c_str(), std::ios::binary);
	ofs << ifs.rdbuf();
	ifs.close();
	ofs.close();
	return true;
}


int metasorter::action_copyonce(asset* _asset, std::string first, std::string second)
{
	if(filesize_changing(_asset->full_filename, file_inspection_time) == 1)
	{
		log_mtx_.lock();
		std::cout << _asset->full_filename << " is changing in filesize - skipping " << std::endl;
		logstring.assign(_asset->full_filename);
		logstring.append(" is changing in filesize - skipping ");
		logfile.write(logstring);
		log_mtx_.unlock();

		return 1;
	}
	int file_in_history = 0;
	int copy_file = 1;
	std::string line;

	// set history file name
	boost::filesystem::path dest_file = boost::filesystem::path(second);
	string histfile_name = std::string(dest_file.parent_path().string().c_str());
	histfile_name.append("/.metasort.history");
			
	//determine file's modified time
	char buf[9];
	std::string file_modified_time_str;
	std::time_t file_modified_time;
	struct tm *struct_time;
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_mtime));
	file_modified_time = mktime(clock);
	struct_time = localtime(&file_modified_time);
	strftime(buf, sizeof(buf), "%Y%m%d", struct_time);
	file_modified_time_str = std::string(buf);
			
	//determine file's size
	std::ifstream file_info_file(_asset->full_filename, std::ios::binary | std::ios::in );
	file_info_file.seekg( 0, std::ios::end );
	std::string file_size_str;
	m_itoa((int)(file_info_file.tellg() / 1024), file_size_str, 10);

	hist_mtx_.lock();

	std::ifstream histfile(histfile_name.c_str());
	if(histfile.is_open())
	{
		while(histfile.good())
		{
			getline(histfile, line);
			if(strcmp(line.c_str(), _asset->full_filename) == 0)  //compare filename with history file
			{
				getline(histfile, line);
				if(strcmp(line.c_str(), file_modified_time_str.c_str()) == 0)  //compare file modified time with history file
				{
					getline(histfile, line);
					if(strcmp(line.c_str(), file_size_str.c_str()) == 0)  //compare file size with history file
					{
						file_in_history = 1;
						copy_file = 0;

						log_mtx_.lock();
						logstring.assign(_asset->full_filename);
						logstring.append(" exists in history file ");
						logstring.append(histfile_name);
						logstring.append(" - skipping");
						logfile.write(logstring);
						cout << _asset->full_filename << " exists in history file " << histfile_name << " - skipping" << endl;
						log_mtx_.unlock();

						continue;
					}
				}
			}
		}
				
		histfile.close();
				
		if(file_in_history == 0)
		{
			std::ofstream histfile_o(histfile_name.c_str(), ios::out | ios::app);
			if(histfile_o.is_open())
			{
				histfile_o.write(_asset->full_filename, strlen(_asset->full_filename));
				histfile_o.write("\n", 1);
				histfile_o.write(file_modified_time_str.c_str(), strlen(file_modified_time_str.c_str()));
				histfile_o.write("\n", 1);
				histfile_o.write(file_size_str.c_str(), strlen(file_size_str.c_str()));
				histfile_o.write("\n", 1);
			}
			else
			{
				log_mtx_.lock();
				logstring.assign("Cannot open history file ");
				logstring.append(histfile_name);
				logfile.write(logstring);
				log_mtx_.unlock();
			}
					
			histfile_o.close();
		}
	}
	else
	{
		log_mtx_.lock();
		logstring.assign("History file in ");
		logstring.append(dest_file.parent_path().string().c_str());
		logstring.append(" doesn't exist - creating it.");
		logfile.write(logstring);
		log_mtx_.unlock();

		std::ofstream histfile_o(histfile_name.c_str());
		if(histfile_o.is_open())
		{
			histfile_o.write(_asset->full_filename, strlen(_asset->full_filename));
			histfile_o.write("\n", 1);
			histfile_o.write(file_modified_time_str.c_str(), strlen(file_modified_time_str.c_str()));
			histfile_o.write("\n", 1);
			histfile_o.write(file_size_str.c_str(), strlen(file_size_str.c_str()));
			histfile_o.write("\n", 1);
			histfile_o.close();
		}
		else
		{
			log_mtx_.lock();
			logstring.assign("Cannot create history file ");
			logstring.append(dest_file.parent_path().string().c_str());
			logfile.write(logstring);
			log_mtx_.unlock();
		}	
	}

	hist_mtx_.unlock();

	if(copy_file == 1)
	{
		// copy the file
		std::string newfile(second);
		newfile.append(_asset->filename);

		log_mtx_.lock();
		logstring.assign("Copying ");
		logstring.append(_asset->full_filename);
		logstring.append(" to ");
		logstring.append(newfile);
		logfile.write(logstring);
		std::cout << "Copying " << _asset->full_filename << " to " << newfile << std::endl;
		log_mtx_.unlock();

		std::ifstream ifs(_asset->full_filename, std::ios::binary);
		std::ofstream ofs(newfile.c_str(), std::ios::binary);
		ofs << ifs.rdbuf();
		ifs.close();
		ofs.close();
	}
	return true;
}


int metasorter::action_exec(asset* _asset, std::string first, std::string second)
{
	std::string execstring(second);
	execstring.append(" ");
	string_replace(execstring, "%s", _asset->full_filename);
	while(string_replace(execstring, "/", "\\"));
			
	log_mtx_.lock();
	logstring.assign("Executing: ");
	logstring.append(execstring);
	logfile.write(logstring);
	std::cout << "Executing: " << execstring << std::endl;
	log_mtx_.unlock();

	std::system(execstring.c_str());
	return true;
}


int metasorter::action_delete(asset* _asset, std::string first, std::string second)
{
	if(second.compare("IAMSURE") == 0)
	{
		if( remove(_asset->full_filename) != 0 )
		{
			log_mtx_.lock();
			std::cout << "Error deleting file" << _asset->full_filename << std::endl;
			logstring.append("Error deleting file");
			logstring.append(_asset->full_filename);
			logfile.write(logstring);
			log_mtx_.unlock();
		}
	}
	return true;
}


// copies files one time (saves history), sets all characters to uppercase and removes ' ', '-', '_' from filename
int metasorter::action_copyonceCUSTOM1(asset* _asset, std::string first, std::string second)
{
	if(filesize_changing(_asset->full_filename, file_inspection_time) == 1)
	{
		log_mtx_.lock();
		std::cout << _asset->full_filename << " is changing in filesize - skipping " << std::endl;
		logstring.assign(_asset->full_filename);
		logstring.append(" is changing in filesize - skipping ");
		logfile.write(logstring);
		log_mtx_.unlock();

		return 1;
	}
	int file_in_history = 0;
	int copy_file = 1;
	std::string line;

	// set history file name
	boost::filesystem::path dest_file = boost::filesystem::path(second);
	string histfile_name = std::string(dest_file.parent_path().string().c_str());
	histfile_name.append("/.metasort.history");
			
	//determine file's modified time
	char buf[9];
	std::string file_modified_time_str;
	std::time_t file_modified_time;
	struct tm *struct_time;
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_mtime));
	file_modified_time = mktime(clock);
	struct_time = localtime(&file_modified_time);
	strftime(buf, sizeof(buf), "%Y%m%d", struct_time);
	file_modified_time_str = std::string(buf);
			
	//determine file's size
	std::ifstream file_info_file(_asset->full_filename, std::ios::binary | std::ios::in );
	file_info_file.seekg( 0, std::ios::end );
	std::string file_size_str;
	m_itoa((int)(file_info_file.tellg() / 1024), file_size_str, 10);

	hist_mtx_.lock();

	std::ifstream histfile(histfile_name.c_str());
	if(histfile.is_open())
	{
		while(histfile.good())
		{
			getline(histfile, line);
			if(strcmp(line.c_str(), _asset->full_filename) == 0)  //compare filename with history file
			{
				getline(histfile, line);
				if(strcmp(line.c_str(), file_modified_time_str.c_str()) == 0)  //compare file modified time with history file
				{
					getline(histfile, line);
					if(strcmp(line.c_str(), file_size_str.c_str()) == 0)  //compare file size with history file
					{
						file_in_history = 1;
						copy_file = 0;

						log_mtx_.lock();
						logstring.assign(_asset->full_filename);
						logstring.append(" exists in history file ");
						logstring.append(histfile_name);
						logstring.append(" - skipping");
						logfile.write(logstring);
						cout << _asset->full_filename << " exists in history file " << histfile_name << " - skipping" << endl;
						log_mtx_.unlock();

						continue;
					}
				}
			}
		}
				
		histfile.close();
				
		if(file_in_history == 0)
		{
			std::ofstream histfile_o(histfile_name.c_str(), ios::out | ios::app);
			if(histfile_o.is_open())
			{
				histfile_o.write(_asset->full_filename, strlen(_asset->full_filename));
				histfile_o.write("\n", 1);
				histfile_o.write(file_modified_time_str.c_str(), strlen(file_modified_time_str.c_str()));
				histfile_o.write("\n", 1);
				histfile_o.write(file_size_str.c_str(), strlen(file_size_str.c_str()));
				histfile_o.write("\n", 1);
			}
			else
			{
				log_mtx_.lock();
				logstring.assign("Cannot open history file ");
				logstring.append(histfile_name);
				logfile.write(logstring);
				log_mtx_.unlock();
			}
					
			histfile_o.close();
		}
	}
	else
	{
		log_mtx_.lock();
		logstring.assign("History file in ");
		logstring.append(dest_file.parent_path().string().c_str());
		logstring.append(" doesn't exist - creating it.");
		logfile.write(logstring);
		log_mtx_.unlock();

		std::ofstream histfile_o(histfile_name.c_str());
		if(histfile_o.is_open())
		{
			histfile_o.write(_asset->full_filename, strlen(_asset->full_filename));
			histfile_o.write("\n", 1);
			histfile_o.write(file_modified_time_str.c_str(), strlen(file_modified_time_str.c_str()));
			histfile_o.write("\n", 1);
			histfile_o.write(file_size_str.c_str(), strlen(file_size_str.c_str()));
			histfile_o.write("\n", 1);
			histfile_o.close();
		}
		else
		{
			log_mtx_.lock();
			logstring.assign("Cannot create history file ");
			logstring.append(dest_file.parent_path().string().c_str());
			logfile.write(logstring);
			log_mtx_.unlock();
		}	
	}

	hist_mtx_.unlock();

	if(copy_file == 1)
	{
		// copy the file
		std::string newfile(second);

		//convert filename to uppercase
		std::string tempfilename_uppercase(_asset->filename);
		std::string filename_CAPS = boost::algorithm::to_upper_copy(tempfilename_uppercase);

		//remove space, underscore and dash from filenames
		std::string::iterator end_pos;
		end_pos = std::remove(filename_CAPS.begin(), filename_CAPS.end(), ' ');
		filename_CAPS.erase(end_pos, filename_CAPS.end());
		end_pos = std::remove(filename_CAPS.begin(), filename_CAPS.end(), '-');
		filename_CAPS.erase(end_pos, filename_CAPS.end());
		end_pos = std::remove(filename_CAPS.begin(), filename_CAPS.end(), '_');
		filename_CAPS.erase(end_pos, filename_CAPS.end());
		
		newfile.append(filename_CAPS);
				
		log_mtx_.lock();
		logstring.assign("Copying ");
		logstring.append(_asset->full_filename);
		logstring.append(" to ");
		logstring.append(newfile);
		logfile.write(logstring);
		std::cout << "Copying " << _asset->full_filename << " to " << newfile << std::endl;
		log_mtx_.unlock();

		std::ifstream ifs(_asset->full_filename, std::ios::binary);
		std::ofstream ofs(newfile.c_str(), std::ios::binary);
		ofs << ifs.rdbuf();
		ifs.close();
		ofs.close();
	}
	return true;
}


int metasorter::action_md5file(asset* _asset, std::string first, std::string second)
{
	
	int base_exists = 0;
	std::string asset_full_filename(_asset->full_filename);
	std::replace(asset_full_filename.begin(), asset_full_filename.end(), '\\', '/');

	//strip quotation marks around string if they exist
	if(second[0] == '"')
	{
		second.erase(0,1);
	}
	if(second[second.length()-1] == '"')
	{
		second.erase(second.size()-1);
	}

	//determine file extension of 'second' parameter
	std::string action_param_extension(".");
	action_param_extension.append(get_file_extension(second));

	//if file extension is not same as the md5 file's extension
	if(action_param_extension.compare(_asset->extension) != 0)
	{
		int md5_exists = 0;
		std::string md5_filename(second);
		std::string md5_search_filename;

		string_replace(md5_filename, "%s", _asset->filename);

		boost::filesystem::path md5_file_fullpath(md5_filename);
		boost::filesystem::path md5_dir_path = md5_file_fullpath.parent_path();
		boost::filesystem::directory_iterator itr(md5_dir_path);
		
		//search for an existing md5 file for this file
		while (itr != boost::filesystem::directory_iterator())
		{
			// if it is a file
			if(itr->status().type() != directory_file)
			{
				md5_search_filename = itr->path().string();
				std::replace(md5_search_filename.begin(), md5_search_filename.end(), '\\', '/');

				//if an md5 file already exists make sure it is up to date
				if(md5_filename.compare(md5_search_filename) == 0)
				{
					md5_exists = 1;

					//std::cout << "MODTIMECOMPARE: " << compare_file_modified_time(asset_full_filename, md5_search_filename) << std::endl;
					std::cout << asset_full_filename.c_str() << " : " << md5_search_filename << std::endl;

					if(compare_file_modified_time(asset_full_filename, md5_search_filename) > 0)
					{
						std::cout << "compare matched" << std::endl;

						md5_exists = 0;

						log_mtx_.lock();
						logstring.assign("MD5 file ");
						logstring.append(md5_search_filename);
						logstring.append(" is older than ");
						logstring.append(_asset->full_filename);
						logstring.append(" - recreating MD5 file.");
						logfile.write(logstring);
						log_mtx_.unlock();
						std::cout << "MD5 file" << md5_search_filename.c_str() << " is older than " << _asset->full_filename;
						std::cout << " - recreating MD5 file." << std::endl;

						if(remove(md5_search_filename.c_str()) != 0)
						{
							log_mtx_.lock();
							logstring.assign("Error deleting ");
							logstring.append(md5_search_filename);
							std::cout << "Error deleting " << md5_search_filename.c_str() << std::endl;
							md5_exists = 1;
						}
					}
				}
			}
			++itr;
		}

		if(md5_exists == 0)  //create an MD5 file
		{	
			log_mtx_.lock();
			std::cout << "Creating MD5 file " << md5_filename.c_str() << std::endl;
			log_mtx_.unlock();
			MD5 md5;

		}
	}

	else  // file is an MD5 file - has same extension as specified in config file
	{

	}

	return true;
}