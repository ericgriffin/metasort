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