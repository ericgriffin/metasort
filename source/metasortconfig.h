/*  configuration.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _METASORTCONFIG_H_
#define _METASORTCONFIG_H_ 1

#include <iostream>
#include <ThirdParty/tinyxml2/tinyxml2.h>
#include "metasorter.h"


class metasortconfig
{
public:
	metasortconfig(class metasorter *);
	~metasortconfig();
	
	metasorter* _parent;
	std::string* config_file;
	std::string* input_file;
	tinyxml2::XMLDocument* config;
	tinyxml2::XMLElement* xmlroot;
	
	// initialization functions
	int read_configuration(std::string);
	int set_input_file(std::string);

};

#endif // _CONFIGURATION_H_