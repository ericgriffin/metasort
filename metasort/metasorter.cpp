#include "metasorter.h"


	metasorter::metasorter(char* _path)
	{
		strcpy(path, _path);
	}


	metasorter::~metasorter()
	{

	}

	void metasorter::timestamp(char* stamp)
	{
		time_t loctime;
		struct tm *loc_tm;
		loctime=time(NULL);
		loc_tm=localtime(&loctime);
		strftime(stamp,sizeof stamp,"%x %X",loc_tm);
	}



	int metasorter::parse_directory()
	{
		int err = 0;
		FileLister* fl = new FileLister(path);
		
		while(!fl->end_of_file_listing)
		{
			//fg->getNextFile();
			fl->ReturnNextFile(filename);
			
			if(!fl->end_of_file_listing)
			{
				cout << filename << endl;
				Read_Header_mediainfo(filename);
			}
		}


		delete fl;
		return err;
	}



	int metasorter::Read_Header_mediainfo(char* filename)
	{
		int err = 0;
		String To_Display;
		
		MediaInfo MI;

		FILE* F = NULL;
		err = fopen_s(&F, filename, "rb");
		if (F == 0)
			return 1;

		unsigned char* From_Buffer = new unsigned char[7 * 188];	//prepare a memory buffer for reading
		size_t From_Buffer_Size;

		fseek(F, 0, SEEK_END);	//determine file size
		long F_Size = ftell(F);
		fseek(F, 0, SEEK_SET);

		MI.Open_Buffer_Init(F_Size, 0);

		do 
		{
			From_Buffer_Size = fread(From_Buffer, 1, 7*188, F);	//read data into buffer
			size_t Status=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);	//Send the buffer to MediaInfo
			if (Status&0x08) //Bit3=Finished
				break;

			if (MI.Open_Buffer_Continue_GoTo_Get()!=(MediaInfo_int64u)-1)
			{
				fseek(F, (long)MI.Open_Buffer_Continue_GoTo_Get(), SEEK_SET);   //Position the file
				MI.Open_Buffer_Init(F_Size, ftell(F));                          //Informing MediaInfo we have seek
			}
		} while (From_Buffer_Size>0);

		fclose(F);

		MI.Open_Buffer_Finalize();
		
		//MI.Open(__T("testH.mpg"));

		To_Display = MI.Get(Stream_General, 0, __T("Format"));
		
		//To_Display += MI.Inform();
		
		cout << To_Display;
		
		return err;
	}

