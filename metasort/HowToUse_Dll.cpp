
#ifdef MEDIAINFO_LIBRARY
    #include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
    #define MediaInfoNameSpace MediaInfoLib;
#else //MEDIAINFO_LIBRARY
    #include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
    #define MediaInfoNameSpace MediaInfoDLL;
#endif //MEDIAINFO_LIBRARY
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace MediaInfoNameSpace;

#ifdef __MINGW32__
    #ifdef _UNICODE
        #define _itot _itow
    #else //_UNICODE
        #define _itot itoa
    #endif //_UNICODE
#endif //__MINGW32



int main (int argc, Char *argv[])
{

    //From: preparing an example file for reading
    FILE* F=fopen("Example.ogg", "rb"); //You can use something else than a file
    if (F==0)
        return 1;

    //From: preparing a memory buffer for reading
    unsigned char* From_Buffer=new unsigned char[7*188]; //Note: you can do your own buffer
    size_t From_Buffer_Size; //The size of the read file buffer

    //From: retrieving file size
    fseek(F, 0, SEEK_END);
    long F_Size=ftell(F);
    fseek(F, 0, SEEK_SET);

    //Initializing MediaInfo
    MediaInfo MI;

    //Preparing to fill MediaInfo with a buffer
    MI.Open_Buffer_Init(F_Size, 0);

    //The parsing loop
    do
    {
        //Reading data somewhere, do what you want for this.
        From_Buffer_Size=fread(From_Buffer, 1, 7*188, F);

        //Sending the buffer to MediaInfo
        size_t Status=MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
        if (Status&0x08) //Bit3=Finished
            break;

        //Testing if there is a MediaInfo request to go elsewhere
        if (MI.Open_Buffer_Continue_GoTo_Get()!=(MediaInfo_int64u)-1)
        {
            fseek(F, (long)MI.Open_Buffer_Continue_GoTo_Get(), SEEK_SET);   //Position the file
            MI.Open_Buffer_Init(F_Size, ftell(F));                          //Informing MediaInfo we have seek
        }
    }
    while (From_Buffer_Size>0);

    //Finalizing
    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

    //Get() example
    String To_Display=MI.Get(Stream_General, 0, __T("Format"));

    #ifdef _UNICODE
        std::wcout << To_Display;
    #else
        std::cout  << To_Display;
    #endif
}

