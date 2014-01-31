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
		std::cout << std::endl << "ERROR - Input file " << *input_file << " does not exist" << std::endl;
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
			if (xmlroot->FirstChildElement("logging")->Attribute("debug"))
			{
				if (std::string("yes").compare(xmlroot->FirstChildElement("logging")->Attribute("debug")) == 0 || std::string("1").compare(xmlroot->FirstChildElement("logging")->Attribute("debug")) == 0)
					_parent->debug = 1;
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

		err |= validatestreams(xmlroot, 0, "");
		err |= validateactions(xmlroot, 0);
		err |= validateparameters(xmlroot, 0, "");

		if (err == 1)
			config->Clear();
	}

	return err;
}


int metasortconfig::validateparameters(tinyxml2::XMLElement *v, int err, std::string rule_name)
{
	// skip non parameter tags
	for (tinyxml2::XMLElement *e = v->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (std::string("rule").compare(e->Name()) == 0)
			rule_name.assign(e->ToElement()->Attribute("name"));
		err = validateparameters(e, err, rule_name);
	}

	// check rule parameters
	for (tinyxml2::XMLElement *u = v->FirstChildElement("parameter"); u != NULL; u = u->NextSiblingElement("parameter"))
	{
		if (parameter_lookup(u->Parent()->ToElement()->Attribute("type"), u->Attribute("name")) == 0)
		{
			_parent->log_mtx_.lock();
			_parent->logstring.assign("ERROR - parameter '");
			_parent->logstring.append(u->Attribute("name"));
			_parent->logstring.append("' in '");
			_parent->logstring.append(u->Parent()->ToElement()->Attribute("type"));
			_parent->logstring.append("' stream in rule '");
			_parent->logstring.append(rule_name);
			_parent->logstring.append("' is undefined.");
			_parent->logfile.write(_parent->logstring);
			std::cout << std::endl << "ERROR - parameter '" << u->Attribute("name");
			std::cout << "' in '" << u->Parent()->ToElement()->Attribute("type") << "' stream in rule '";
			std::cout << rule_name << "' is undefined." << std::endl;
			_parent->log_mtx_.unlock();
			err = 1;
		}
	}

	return err;
}


int metasortconfig::validateactions(tinyxml2::XMLElement *v, int err)
{

	// skip non parameter tags
	for (tinyxml2::XMLElement *e = v->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		err = validateactions(e, err);
	}

	// check rule parameters
	for (tinyxml2::XMLElement *u = v->FirstChildElement("action"); u != NULL; u = u->NextSiblingElement("action"))
	{
		if (action_lookup(u->Attribute("type")) == 0)
		{
			_parent->log_mtx_.lock();
			_parent->logstring.assign("ERROR - action '");
			_parent->logstring.append(u->Attribute("type"));
			_parent->logstring.append("' in rule '");
			_parent->logstring.append(u->Parent()->ToElement()->Attribute("name"));
			_parent->logstring.append("' is undefined.");
			_parent->logfile.write(_parent->logstring);
			std::cout << std::endl << "ERROR - action '" << u->Attribute("type");
			std::cout << "' in rule '" << u->Parent()->ToElement()->Attribute("name") << "' is undefined." << std::endl;
			_parent->log_mtx_.unlock();
			err = 1;
		}
		else
		{
			// check whether destination path exists for move/copy actions
			if (strcmp("move", u->Attribute("type")) == 0 || strcmp("fastmove", u->Attribute("type")) == 0 ||
				strcmp("copy", u->Attribute("type")) == 0 || strcmp("copyonce", u->Attribute("type")) == 0 ||
				strcmp("copyonceCUSTOM1", u->Attribute("type")) == 0 || strcmp("moveCUSTOM1", u->Attribute("type")) == 0 ||
				strcmp("fastmoveCUSTOM1", u->Attribute("type")) == 0)
			{
				std::string path_to_check(u->Attribute("parameter"));
				char ch = path_to_check.back();
				// strip characters from end until / or \ remains
				while (ch != '/' && ch != '\\')
				{
					//strip last character from parameter
					path_to_check.assign(path_to_check.substr(0, path_to_check.size() - 1));
					ch = *path_to_check.rbegin();
				}

				if (metasortutil::path_exists(path_to_check) == 0)
				{
					_parent->log_mtx_.lock();
					_parent->logstring.assign("ERROR - destination for action '");
					_parent->logstring.append(u->Attribute("type"));
					_parent->logstring.append("' in rule '");
					_parent->logstring.append(u->Parent()->ToElement()->Attribute("name"));
					_parent->logstring.append("' is invalid.");
					_parent->logfile.write(_parent->logstring);
					std::cout << std::endl << "ERROR - destination for action '" << u->Attribute("type");
					std::cout << "' in rule '" << u->Parent()->ToElement()->Attribute("name") << "' is invalid." << std::endl;
					_parent->log_mtx_.unlock();
					err = 1;
				}
			}
		}
	}

	return err;
}


int metasortconfig::validatestreams(tinyxml2::XMLElement *v, int err, std::string rule_name)
{

	// skip non parameter tags
	for (tinyxml2::XMLElement *e = v->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		if (std::string("rule").compare(e->Name()) == 0)
			rule_name.assign(e->ToElement()->Attribute("name"));
		err = validatestreams(e, err, rule_name);
	}

	// check rule parameters
	for (tinyxml2::XMLElement *u = v->FirstChildElement("stream"); u != NULL; u = u->NextSiblingElement("stream"))
	{
		if (stream_lookup(u->Attribute("type")) == 0)
		{
			_parent->log_mtx_.lock();
			_parent->logstring.assign("ERROR - stream '");
			_parent->logstring.append(u->Attribute("type"));
			_parent->logstring.append("' in rule ");
			_parent->logstring.append(u->Parent()->ToElement()->Attribute("name"));
			_parent->logstring.append(" is undefined.");
			_parent->logfile.write(_parent->logstring);
			std::cout << std::endl << "ERROR - stream '" << u->Attribute("type");
			std::cout << "' in rule '" << rule_name << "' is undefined." << std::endl;
			_parent->log_mtx_.unlock();
			err = 1;
		}
	}

	return err;
}