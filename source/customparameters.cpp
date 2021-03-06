/*  customparameters.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#include "metasorter.h"

int metasorter::custom_parameters(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI, asset* _asset, MediaInfoLib::stream_t stream_type, int stream_number, MediaInfoLib::String parameter, int &MI_fetched)
{
	int found = 0;
	if(stream_type == Stream_General)
	{
		if(wcscmp(parameter.c_str(), L"audio_layout") == 0)
		{
			found = proc_audio_layout(_asset_param_val, _MI, _asset, MI_fetched);
		}
		if(wcscmp(parameter.c_str(), L"file_size") == 0)
		{
			found = proc_file_size(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_modified_age") == 0)
		{
			found = proc_file_modified_age(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_created_age") == 0)
		{
			found = proc_file_created_age(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_created_date") == 0)
		{
			found = proc_file_created_date(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_modified_date") == 0)
		{
			found = proc_file_modified_date(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_name") == 0)
		{
			found = proc_file_name(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_extension") == 0)
		{
			found = proc_file_extension(_asset_param_val, _asset);
		}
		if(wcscmp(parameter.c_str(), L"file_path") == 0)
		{
			found = proc_file_path(_asset_param_val, _asset);
		}
	}
	return found;
}


int metasorter::proc_audio_layout(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI, asset* _asset, int &MI_fetched)
{
	MediaInfoLib::String audiostreamsstr;
	MediaInfoLib::String streamchannelsstr;
	char* audiostreamschar = new char[255];
	char* streamchannels = new char[255];
	int audiostreams = 0;

	if(MI_fetched == 0)
	{
		call_MediaInfo(_MI, _asset);
		MI_fetched = 1;
	}
	audiostreamsstr.assign(_MI.Get(Stream_General, 0, L"AudioCount").c_str(), sizeof(audiostreamsstr));

	wcstombs(audiostreamschar, audiostreamsstr.c_str(), audiostreamsstr.length() + 1);
	audiostreams = atoi((const char*)audiostreamschar);

	_asset_param_val.assign(L"");

	for(int i = 0; i < audiostreams; i++)
	{
		streamchannelsstr.assign(_MI.Get(Stream_Audio, i, L"Channel(s)").c_str(), sizeof(audiostreamsstr));
		_asset_param_val.append(streamchannelsstr.c_str());
	}

	delete[] audiostreamschar;
	delete[] streamchannels;
	return 1;
}

int metasorter::proc_file_name(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];
	mbstowcs(tempstring3, _asset->filename, strlen(_asset->filename) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;
	return 1;
}

int metasorter::proc_file_extension(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];
	mbstowcs(tempstring3, _asset->extension, strlen(_asset->extension) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;
	return 1;
}

int metasorter::proc_file_path(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];
	mbstowcs(tempstring3, _asset->path, strlen(_asset->path) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;
	return 1;
}

int metasorter::proc_file_size(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	std::ifstream file_info_file(_asset->full_filename, std::ios::binary | std::ios::in );
	file_info_file.seekg( 0, std::ios::end );
	std::string tempstring;
	MediaInfoLib::String tempstring2;
	metasortutil::m_itoa((int)(file_info_file.tellg() / 1024), tempstring, 10);
	wchar_t *tempstring3 = new wchar_t[255];
	mbstowcs(tempstring3, tempstring.c_str(), strlen(tempstring.c_str()) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;
	return 1;
}



int metasorter::proc_file_modified_age(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	std::string tempstring;
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];

	std::time_t file_modified_time;
	std::time_t now = time(NULL);
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_mtime));
	file_modified_time = mktime(clock);

    metasortutil::m_itoa((int)(difftime(now, file_modified_time) / 60), tempstring, 10);
	mbstowcs(tempstring3, tempstring.c_str(), strlen(tempstring.c_str()) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;

	return 1;
}


int metasorter::proc_file_created_age(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	std::string tempstring;
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];

	std::time_t file_create_time;
	std::time_t now = time(NULL);
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_ctime));
	file_create_time = mktime(clock);

    metasortutil::m_itoa((int)(difftime(now, file_create_time) / 60), tempstring, 10);
	mbstowcs(tempstring3, tempstring.c_str(), strlen(tempstring.c_str()) + 1);
	
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);

	delete[] tempstring3;
	return 1;
}


int metasorter::proc_file_modified_date(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	char buf[9];
	std::string tempstring;
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];

	std::time_t file_modified_time;
	struct tm *struct_time;
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_mtime));
	file_modified_time = mktime(clock);
	struct_time = localtime(&file_modified_time);
	strftime(buf, sizeof(buf), "%Y%m%d", struct_time);
	tempstring = std::string(buf);

	mbstowcs(tempstring3, tempstring.c_str(), strlen(tempstring.c_str()) + 1);

	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);

	delete[] tempstring3;
	return 1;
}


int metasorter::proc_file_created_date(MediaInfoLib::String &_asset_param_val, asset* _asset)
{
	char buf[9];
	std::string tempstring;
	MediaInfoLib::String tempstring2;
	wchar_t *tempstring3 = new wchar_t[255];

	std::time_t file_create_time;
	struct tm *struct_time;
	struct tm* clock = new tm;
	struct stat attrib;
	stat(_asset->full_filename, &attrib);
	clock = localtime(&(attrib.st_ctime));
	file_create_time = mktime(clock);
	struct_time = localtime(&file_create_time);
	strftime(buf, sizeof(buf), "%Y%m%d", struct_time);
	tempstring = std::string(buf);

	mbstowcs(tempstring3, tempstring.c_str(), strlen(tempstring.c_str()) + 1);
	tempstring2.assign(tempstring3);
	_asset_param_val.assign(tempstring2);
	delete[] tempstring3;

	return 1;
}
