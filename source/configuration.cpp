// configuration.cpp

#include "configuration.h"

configuration::configuration()
{
	config = new tinyxml2::XMLDocument[255];
	config_file = (char*)malloc(sizeof(char[255][255]));
	input_file = (char*)malloc(sizeof(char[255][255]));
}


configuration::~configuration()
{
	delete[] config;
	free(config_file);
	free(input_file);
}
