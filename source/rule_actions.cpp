#include "metasorter.h"


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