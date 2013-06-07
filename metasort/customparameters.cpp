#include "metasorter.h"
#include "filelister.h"
#include "asset.h"
#include "customparameters.h"

int metasorter::custom_parameters(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI, asset* _asset, MediaInfoLib::stream_t stream_type, int stream_number, MediaInfoLib::String parameter)
{
	int found = 0;
	if(stream_type == Stream_General)
	{
		if(wcscmp(parameter.c_str(), L"audio_layout") == 0)
		{
			found = audio_layout(_asset_param_val, _MI);
		}
		if(wcscmp(parameter.c_str(), L"file_size") == 0)
		{
			found = file_size(_asset_param_val, _MI);
		}
		if(wcscmp(parameter.c_str(), L"file_age") == 0)
		{
			found = file_age(_asset_param_val, _MI);
		}
	}
	return found;
}


int audio_layout(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI)
{
	MediaInfoLib::String audiostreamsstr;
	MediaInfoLib::String streamchannelsstr;
	char* audiostreamschar = new char[255];
	char* streamchannels = new char[255];
	int audiostreams = 0;
	
	audiostreamsstr.assign(_MI.Get(Stream_General, 0, L"AudioCount").c_str(), sizeof(audiostreamsstr));
	
	wcstombs(audiostreamschar, audiostreamsstr.c_str(), sizeof(audiostreamschar));
	audiostreams = atoi((const char*)audiostreamschar);

	_asset_param_val.assign(L"");

	for(int i = 0; i < audiostreams; i++)
	{
		streamchannelsstr.assign(_MI.Get(Stream_Audio, i, L"Channel(s)").c_str(), sizeof(audiostreamsstr));
		_asset_param_val.append(streamchannelsstr.c_str());
	}

	return 1;
}


int file_size(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI)
{
	/*std::ifstream file_info_file(_asset->full_filename, std::ios::binary | std::ios::in );
	file_info_file.seekg( 0, std::ios::end );
					
	std::string tempstring;
	String tempstring2;
	tempstring = std::to_string(file_info_file.tellg() / 1024);
	wchar_t *tempstring3 = new wchar_t[255];
	mbstowcs(tempstring3, tempstring.c_str(), sizeof(tempstring3));
	tempstring2.assign(tempstring3);
	asset_param_val.assign(tempstring2);
	*/
	return 1;
}

int file_age(MediaInfoLib::String &_asset_param_val, MediaInfo &_MI)
{
	int age = 0;

	/*
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    hFind = FindFirstFile((LPCWSTR)_asset->full_filename, &FindFileData);
	const FILETIME  ftFile = FindFileData.ftCreationTime;
	CTime ctFile = ftFile;
    CTime ctNow = ctNow.GetCurrentTime();
	CTimeSpan tsAge;
    tsAge = ctNow - ctFile;
	FILETIME ftAge;
    ULARGE_INTEGER ulAgeInSeconds;
	ulAgeInSeconds.QuadPart = tsAge.GetTotalSeconds() * 1E7;
	ftAge.dwLowDateTime=ulAgeInSeconds.LowPart;
    ftAge.dwHighDateTime = ulAgeInSeconds.HighPart;
	SYSTEMTIME stAge;
    FileTimeToSystemTime(&ftAge,&stAge);
	strYears.Format("%d", stAge.wYear-1601);
    strMonths.Format("%d",stAge.wMonth-1);
    strDays.Format("%d",stAge.wDay-1);
	*/

	return 1;
}