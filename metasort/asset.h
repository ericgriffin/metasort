// asset.h
#ifndef ASSET_H
#define ASSET_H 1

class asset
    {
		
    public:
		asset();
		~asset();
		char path[255];
		char filename[255];
		char full_filename[255];
		char extension[255];
		int audio_streams[255]; // number of channels per stream
		int num_audio_streams; // number of audio streams



	private:
		
	};
	
#endif