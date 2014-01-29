/*  configuration.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */


#include "metasortconfig.h"


std::string config_version("1.x");


metasortconfig::metasortconfig(class metasorter* _parent)
{
	this->_parent = _parent;
	
	config = new tinyxml2::XMLDocument;
	config_file = new std::string;
	input_file = new std::string;
}


metasortconfig::~metasortconfig()
{
	delete config;
	delete config_file;
	delete input_file;
}


int metasortconfig::set_input_file(std::string file)
{
	int err = 0;

	input_file->assign(file);

	if (!metasortutil::file_exists(*input_file))
	{
		std::cout << std::endl << "ERROR - Config file " << *input_file << " does not exist" << std::endl;
		return 1;
	}
	
	return err;
}


int metasortconfig::read_configuration(std::string file)
{
	int err = 0;
	int logging = 0;
	int extensions = 0;
	int rules = 0;

	config_file->assign(file);

	// see if config file exists
	if (!metasortutil::file_exists(*config_file))
	{
		std::cout << std::endl << "ERROR - Config file " << *config_file << " does not exist" << std::endl;
		return 1;
	}

	// parse the xml config file
	if (config->LoadFile(config_file->c_str()) != 0)
	{
		std::cout << std::endl << "ERROR - XML not valid in config file " << *config_file << std::endl;
		err = 1;
		return 1;
	}

	// if <metasort> tag exists, assign pointer to the root element
	if (!err)
	{
		if (config->FirstChildElement("metasort"))
		{
			xmlroot = config->FirstChildElement("metasort");
		}
		else
		{
			std::cout << "ERROR - no <metasort> tag in config file " << *config_file << ". ";
			std::cout << "Expecting xml root to be <metasort version=\"" << config_version << "\">" << std::endl;
			config->Clear();
			err = 1;
		}
	}
	
	if (!err)
	{
		// check config file version
		if (!xmlroot->Attribute("version") || config_version.compare(xmlroot->Attribute("version")) != 0)
		{
			std::cout << "ERROR - Wrong config version for " << *config_file << ". ";
			std::cout << "Expecting <metasort version=\"" << config_version << "\">" << std::endl;
			err = 1;
		}

		// check for folders entry in config
		if (!xmlroot->FirstChildElement("folder") || !xmlroot->FirstChildElement("folder")->Attribute("path"))
		{
			std::cout << "ERROR - No search folders defined in config file " << *config_file << std::endl;
			std::cout << "Expecting <folder path=\"[PATH]\" />" << std::endl;
			err = 1;
		}
		else
		{
			for (tinyxml2::XMLElement *e = xmlroot->FirstChildElement("folder"); e != NULL; e = e->NextSiblingElement("folder"))
			{
				//check to see if path is valid
				if (!metasortutil::path_exists(std::string(e->Attribute("path"))))
				{
					std::cout << "ERROR - search path does not exist: " << e->Attribute("path") << std::endl;
					err = 1;
					continue;
				}
			}
		}

		/* check logging entry in config file */
		if (xmlroot->FirstChildElement("logging"))
		{
			if (xmlroot->FirstChildElement("logging")->Attribute("path"))
			{
				logging = 1;
				_parent->log_mtx_.lock();
				_parent->logfile.open(xmlroot->FirstChildElement("logging")->Attribute("path"));
				_parent->log_mtx_.unlock();
			}
			if (xmlroot->FirstChildElement("logging")->Attribute("console"))
			{
				if (std::string("yes").compare(xmlroot->FirstChildElement("logging")->Attribute("console")) == 0 || std::string("1").compare(xmlroot->FirstChildElement("logging")->Attribute("console")) == 0)
					_parent->verbose = 1;
			}
		}

		if (logging == 0)
		{
			_parent->log_mtx_.lock();
			std::cout << std::endl << "ERROR - No logging defined. Expecting <logging path=[path]/>" << std::endl;
			_parent->log_mtx_.unlock();
			err = 1;
		}

		/* check optional parameters in config file */
		if (xmlroot->FirstChildElement("file_inspection"))
		{
			if (xmlroot->FirstChildElement("file_inspection")->Attribute("time"))
				_parent->file_inspection_time = atoi(xmlroot->FirstChildElement("file_inspection")->Attribute("time"));
			else
			{
				_parent->log_mtx_.lock();
				std::cout << "WARNING - file_inspection time override exists but no value is set." << std::endl;
				_parent->log_mtx_.unlock();
			}
		}

		if (xmlroot->FirstChildElement("threadpool"))
		{
			if (xmlroot->FirstChildElement("threadpool")->Attribute("size"))
				_parent->tp.size_controller().resize(atoi(xmlroot->FirstChildElement("threadpool")->Attribute("size")));
			else
			{
				_parent->log_mtx_.lock();
				std::cout << "WARNING - threadpool size override exists but no value is set." << std::endl;
				_parent->log_mtx_.unlock();
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
			_parent->log_mtx_.lock();
			std::cout << "ERROR - No extensions defined. Expecting <extension value=[value]/>" << std::endl;
			_parent->logfile.close();
			_parent->log_mtx_.unlock();
			err = 1;
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
			_parent->log_mtx_.lock();
			std::cout << "ERROR - No rules defined" << std::endl;
			_parent->logfile.close();
			_parent->log_mtx_.unlock();
			err = 1;
		}

		if (err == 1)
			config->Clear();
	}

	return err;
}