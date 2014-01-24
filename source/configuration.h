// asset.h
#ifndef CONFIGURATION_H
#define CONFIGURATION_H 1

#include <ThirdParty/tinyxml2/tinyxml2.h>

class configuration
{
public:
	configuration();
	~configuration();
	
	char* config_file;
	char* input_file;
	tinyxml2::XMLDocument* config;

private:

};

#endif