#include "usage.h"


void usage()
{
	std::cout << "Usage: metasort -c <config file> [-i <filename>] [-g]" << std::endl;
	std::cout << "-c <config file>\t--  configuration file to use" << std::endl;
	std::cout << "-i <filename>\t\t--  process a single file (requires -c)" << std::endl;
	std::cout << "-g\t\t\t--  create a skeleton config file in current directory" << std::endl;
	std::cout << std::endl << "Configuration details:" << std::endl;
	std::cout << "-lactions\t\t--  show actions" << std::endl;
	std::cout << "-lstreams\t\t--  show streams" << std::endl;
	std::cout << "-lgeneral\t\t--  showt general parameters" << std::endl;
	std::cout << "-laudio\t\t\t--  show audio parameters" << std::endl;
	std::cout << "-lvideo\t\t\t--  show video parameters" << std::endl;
	std::cout << "-ltext\t\t\t--  show text parameters" << std::endl;
	std::cout << "-lother\t\t\t--  show other parameters" << std::endl;
	std::cout << "-limage\t\t\t--  show image parameters" << std::endl;
	std::cout << "-lmenu\t\t\t--  show menu parameters" << std::endl;
	std::cout << std::endl;
}


void generate_skeleton_config()
{
	boost::filesystem::path working_path_raw(boost::filesystem::current_path());
	std::string *config = new std::string();
	std::string *working_path = new std::string(working_path_raw.string().c_str());

	//change back-slashes to forward-slashes
	std::replace(working_path->begin(), working_path->end(), '\\', '/');

	config->append("<metasort version=\"1.x\">\n");
	config->append("\t<folder path=\"");
	config->append(working_path->c_str());
	config->append("\" recursive=\"yes\" />\n\n");
	config->append("\t<extension value=\"[REGEX].*\" />\n\n");
	config->append("\t<logging path=\"");
	config->append(working_path->c_str());
	config->append("/metasort.log\" />\n\n");
	config->append("\t<rule name=\"Rule1\">\n");
	config->append("\t\t<action type=\"list\" parameter=\"");
	config->append(working_path->c_str());
	config->append("/metasort_list.txt\" />\n");
	config->append("\t\t<stream type=\"general\" number=\"0\">\n");
	config->append("\t\t\t<parameter name=\"file_size\" value=\">50\" />\n");
	config->append("\t\t</stream>\n");
	config->append("\t</rule>\n");
	config->append("</metasort>\n");

	std::ofstream f;
	std::string *skeleton_config_file = new std::string(working_path_raw.string().c_str());
	skeleton_config_file->append("/metasort_config.xml");
	std::cout << "Generating skeleton config file: " << skeleton_config_file->c_str() << std::endl << std::endl;
	f.open(skeleton_config_file->c_str(), std::ios::out);
	f << config->c_str();
	f.close();
}


void list_actions()
{
	std::cout << "Actions: " << std::endl << std::endl;
	std::cout << "type = list\t\tparameter = <destination>\n";
	std::cout << "type = move\t\tparameter = <destination>\n";
	std::cout << "type = fastmove\t\tparameter = <destination>\n";
	std::cout << "type = copy\t\tparameter = <destination>\n";
	std::cout << "type = copyonce\t\tparameter = <destination>\n";
	std::cout << "type = exec\t\tparameter = <command> (%s=filename)\n";
	std::cout << "type = delete\t\tparameter = \"IAMSURE\"\n";
	std::cout << "type = md5file\t\tparameter = <filename> (%s=filename)\n";
	std::cout << "type = copyonceCUSTOM1\tparameter = <destination>\n";
}


void list_streams()
{
	std::cout << "Streams: " << std::endl << std::endl;
	std::cout << "type = general\tnumber = 0\n";
	std::cout << "type = audio\tnumber = <0-n>\n";
	std::cout << "type = video\tnumber = <0-n>\n";
	std::cout << "type = text\tnumber = <0-n>\n";
	std::cout << "type = other\tnumber = <0-n>\n";
	std::cout << "type = image\tnumber = <0-n>\n";
	std::cout << "type = menu\tnumber = <0-n>\n";
}


void list_general_parameters()
{

}


void list_audio_parameters()
{

}


void list_video_parameters()
{

}


void list_text_parameters()
{

}


void list_other_parameters()
{

}


void list_image_parameters()
{

}


void list_menu_parameters()
{

}


