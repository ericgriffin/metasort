/*  usage.cpp
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */


#include "usage.h"


void usage()
{
	std::cout << std::endl << "Usage: metasort -c <config file> [-i <filename>] [-g]" << std::endl;
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
	std::cout << "type = moveCUSTOM1\tparameter = <destination>\n";
	std::cout << "type = fastmoveCUSTOM1\tparameter = <destination>\n";
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


const std::string action_types[20] =
{
	{ "list" },
	{ "move" },
	{ "fastmove" },
	{ "copy" },
	{ "copyonce" },
	{ "exec" },
	{ "delete" },
	{ "md5file" },
	{ "copyonceCUSTOM1" },
	{ "moveCUSTOM1" },
	{ "fastmoveCUSTOM1" },
	{ "END" }
};


const std::string stream_types[10] =
{
	{ "general" },
	{ "audio" },
	{ "video" },
	{ "text" },
	{ "other" },
	{ "image" },
	{ "menu" },
	{ "END" }
};


const std::string general_parameters[500][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;General", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "GeneralCount;1", "N NI", "Number of general streams" },
	{ "VideoCount", "N NI", "Number of video streams" },
	{ "AudioCount", "N NI", "Number of audio streams" },
	{ "TextCount", "N NI", "Number of text streams" },
	{ "OtherCount", "N NI", "Number of other streams" },
	{ "ImageCount", "N NI", "Number of image streams" },
	{ "MenuCount", "N NI", "Number of menu streams" },
	{ "Video_Format_List", "N NT", "Video Codecs in this file, separated by /" },
	{ "Video_Format_WithHint_List", "N NT", "Video Codecs in this file with popular name (hint), separated by /" },
	{ "Video_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Video_Language_List", "N NT", "Video languagesin this file, full names, separated by /" },
	{ "Audio_Format_List", "N NT", "Audio Codecs in this file,separated by /" },
	{ "Audio_Format_WithHint_List", "N NT", "Audio Codecs in this file with popular name (hint), separated by /" },
	{ "Audio_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Audio_Language_List", "N NT", "Audio languages in this file separated by /" },
	{ "Text_Format_List", "N NT", "Text Codecs in this file, separated by /" },
	{ "Text_Format_WithHint_List", "N NT", "Text Codecs in this file with popular name (hint),separated by /" },
	{ "Text_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Text_Language_List", "N NT", "Text languages in this file, separated by /" },
	{ "Other_Format_List", "N NT", "Other formats in this file, separated by /" },
	{ "Other_Format_WithHint_List", "N NT", "Other formats in this file with popular name (hint), separated by /" },
	{ "Other_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Other_Language_List", "N NT", "Chapters languages in this file, separated by /" },
	{ "Image_Format_List", "N NT", "Image Codecs in this file, separated by /" },
	{ "Image_Format_WithHint_List", "N NT", "Image Codecs in this file with popular name (hint), separated by /" },
	{ "Image_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Image_Language_List", "N NT", "Image languages in this file, separated by /" },
	{ "Menu_Format_List", "N NT", "Menu Codecsin this file, separated by /" },
	{ "Menu_Format_WithHint_List", "N NT", "Menu Codecs in this file with popular name (hint),separated by /" },
	{ "Menu_Codec_List", "N NT", "Deprecated, do not use in new projects" },
	{ "Menu_Language_List", "N NT", "Menu languages in this file, separated by /" },
	{ "CompleteName", "Y YT", "Complete name (Folder+Name+Extension)" },
	{ "FolderName", "N NT", "Folder name only" },
	{ "FileName", "N NT", "File name only" },
	{ "FileExtension", "N NT", "File extension only" },
	{ "CompleteName_Last", "Y YT", "Complete name (Folder+Name+Extension) of the last file (in the case of a sequence of files)" },
	{ "FolderName_Last", "N NT", "Folder name only of the last file (in the case of a sequence of files)" },
	{ "FileName_Last", "N NT", "File name only of the last file (in the case of a sequence of files)" },
	{ "FileExtension_Last", "N NT", "File extension only of the last file (in the case of a sequence of files)" },
	{ "Format", "Y YT", "Format used" },
	{ "Format/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Format/Info", "Y NT", "Info about this Format" },
	{ "Format/Url", "N NT", "Link to a description of this format" },
	{ "Format/Extensions", "N NT", "Known extensions of this format" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y YT", "Version of this format" },
	{ "Format_Profile", "Y YT", "Profile of the Format" },
	{ "Format_Compression", "Y YT", "Compression method us" },
	{ "Format_Settings", "Y YT", "Settings needed for decoder used" },
	{ "InternetMediaType", "N YT", "Internet Media Type (aka MIME Type, Content-Type)" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about this codec" },
	{ "CodecID/Hint", "Y NT", "A hint/popular name for this codec" },
	{ "CodecID/Url", "N NT", "A link to more details about this codec ID" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Interleaved", "N YT", "If Audio and video are muxed" },
	{ "Codec", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Info", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Url", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Extensions", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_Automatic", "N NT", "Deprecated, do not use in new projects" },
	{ "FileSize", " byte;N YT", "File size in bytes" },
	{ "FileSize/String", "Y NT", "File size (with measure)" },
	{ "FileSize/String1", "N NT", "File size (with measure, 1 digit mini)" },
	{ "FileSize/String2", "N NT", "File size (with measure, 2 digit mini)" },
	{ "FileSize/String3", "N NT", "File size (with measure, 3 digit mini)" },
	{ "FileSize/String4", "N NT", "File size (with measure, 4 digit mini)" },
	{ "Duration", " ms;N YI", "Play time of the stream in ms" },
	{ "Duration/String", "Y NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if zero" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.MMM" },
	{ "Duration_Start", "Y YT", "" },
	{ "Duration_End", "Y YT", "" },
	{ "OverallBitRate_Mode", "N YT", "Bit rate mode of all streams (VBR, CBR)" },
	{ "OverallBitRate_Mode/String", "Y NT", "Bit rate mode of all streams (Variable, Constant)" },
	{ "OverallBitRate", " bps;N YF", "Bit rate of all streams in bps" },
	{ "OverallBitRate/String", "Y NT", "Bit rate of all streams (with measure)" },
	{ "OverallBitRate_Minimum", " bps;N YF", "Minimum Bit rate in bps" },
	{ "OverallBitRate_Minimum/String", "Y NT", "Minimum Bit rate (with measurement)" },
	{ "OverallBitRate_Nominal", " bps;N YF", "Nominal Bit rate in bps" },
	{ "OverallBitRate_Nominal/String", "Y NT", "Nominal Bit rate (with measurement)" },
	{ "OverallBitRate_Maximum", " bps;N YF", "Maximum Bit rate in bps" },
	{ "OverallBitRate_Maximum/String", "Y NT", "Maximum Bit rate (with measurement)" },
	{ "Delay", " ms;N YI", "Delay fixed in the stream (relative) IN MS" },
	{ "Delay/String", "N NT", "Delay with measurement" },
	{ "Delay/String1", "N NT", "Delay with measurement" },
	{ "Delay/String2", "N NT", "Delay with measurement" },
	{ "Delay/String3", "N NT", "format : HH:MM:SS.MMM" },
	{ "StreamSize", " byte;N YI", "Stream size in bytes" },
	{ "StreamSize/String", "N NT", "" },
	{ "StreamSize/String1", "N NT", "" },
	{ "StreamSize/String2", "N NT", "" },
	{ "StreamSize/String3", "N NT", "" },
	{ "StreamSize/String4", "N NT", "" },
	{ "StreamSize/String5", "N NT", "With proportion" },
	{ "StreamSize_Proportion", "N NT", "Stream size divided by file size" },
	{ "HeaderSize", "N YI", "" },
	{ "DataSize", "N YI", "" },
	{ "FooterSize", "N YI", "" },
	{ "IsStreamable", "N YT", "" },
	{ "Album_ReplayGain_Gain", " dB;N YT", "The gain to apply to reach 89dB SPL on playback" },
	{ "Album_ReplayGain_Gain/String", "Y YT", "" },
	{ "Album_ReplayGain_Peak", "Y YT", "The maximum absolute peak value of the it" },
	{ "Encryption", "Y YT", "" },
	{ "Title", "N NT", "(Generic)Title of fi" },
	{ "Title/More", "N NT", "(Generic)More info about the title of fi" },
	{ "Title/Url", "N NT", "(Generic)U" },
	{ "Domain", "Y YT", "Univers movies belong to, e.g. Starwars, Stargate, Buffy, Dragonbal" },
	{ "Collection", "Y YT", "Name of the series, e.g. Starwars movies, Stargate SG-1, Stargate Atlantis, Buffy, Ang" },
	{ "Season", "Y YT", "Name of the season, e.g. Strawars first Trilogy, Season" },
	{ "Season_Position", "Y YI", "Number of the Seas" },
	{ "Season_Position_Total", "Y YI", "Place of the season e.g. 2 of" },
	{ "Movie", "Y YT", "Name of the movie. Eg : Starwars, a new ho" },
	{ "Movie/More", "Y YT", "More infos about the mov" },
	{ "Movie/Country", "Y YT", "Country, where the movie was procuc" },
	{ "Movie/Url", "Y YT", "Homepage for the mov" },
	{ "Album", "Y YT", "Name of an audio-album. Eg : The joshua tr" },
	{ "Album/More", "Y YT", "More infos about the alb" },
	{ "Album/Sort", "Y YT", "" },
	{ "Album/Performer", "Y YT", "Album performer/artist of this fi" },
	{ "Album/Performer/Sort", "Y YT", "" },
	{ "Album/Performer/Url", "Y YT", "Homepage of the album performer/arti" },
	{ "Comic", "Y YT", "Name of the comi" },
	{ "Comic/More", "Y YT", "" },
	{ "Comic/Position_Total", "Y YI", "" },
	{ "Part", "Y YT", "Name of the part. e.g. CD1, C" },
	{ "Part/Position", "Y YI", "Number of the pa" },
	{ "Part/Position_Total", "Y YI", "Place of the part e.g. 2 of" },
	{ "Track", "Y YT", "Name of the track. e.g. track1, track" },
	{ "Track/More", "Y YT", "More infos about the tra" },
	{ "Track/Url", "Y YT", "Link to a site about this tra" },
	{ "Track/Sort", "Y YT", "" },
	{ "Track/Position", "Y YI", "Number of this tra" },
	{ "Track/Position_Total", "Y YI", "Place of this track, e.g. 3 of " },
	{ "Grouping", "Y YT", "iTunes groupi" },
	{ "Chapter", "Y YT", "Name of the chapte" },
	{ "SubTrack", "Y YT", "Name of the subtrac" },
	{ "Original/Album", "Y YT", "Original name of album, serie." },
	{ "Original/Movie", "Y YT", "Original name of the mov" },
	{ "Original/Part", "Y YT", "Original name of the part in the original suppo" },
	{ "Original/Track", "Y YT", "Original name of the track in the original suppo" },
	{ "Compilation", "Y YT", "iTunes compilati" },
	{ "Compilation/String", "Yes;Y YT", "iTunes compilati" },
	{ "Performer", "Y YT", "Main performer/artist of this fi" },
	{ "Performer/Sort", "Y YT", "" },
	{ "Performer/Url", "Y YT", "Homepage of the performer/arti" },
	{ "Original/Performer", "Y YT", "Original artist(s)/performer(s" },
	{ "Accompaniment", "Y YT", "Band/orchestra/accompaniment/musicia" },
	{ "Composer", "Y YT", "Name of the original compose" },
	{ "Composer/Nationality", "Y YT", "Nationality of the main composer of the item, mostly for classical musi" },
	{ "Arranger", "Y YT", "The person who arranged the piece. e.g. Rave" },
	{ "Lyricist", "Y YT", "The person who wrote the lyrics for a musical ite" },
	{ "Original/Lyricist", "Y YT", "Original lyricist(s)/text writer(s" },
	{ "Conductor", "Y YT", "The artist(s) who performed the work. In classical music this would be the conductor, orchestra, soloist" },
	{ "Director", "Y YT", "Name of the directo" },
	{ "AssistantDirector", "Y YT", "Name of the assistant directo" },
	{ "DirectorOfPhotography", "Y YT", "The name of the director of photography, also known as cinematographe" },
	{ "SoundEngineer", "Y YT", "The name of the sound engineer or sound recordis" },
	{ "ArtDirector", "Y YT", "The person who oversees the artists and craftspeople who build the set" },
	{ "ProductionDesigner", "Y YT", "The person responsible for designing the Overall visual appearance of a movi" },
	{ "Choregrapher", "Y YT", "The name of the choregraphe" },
	{ "CostumeDesigner", "Y YT", "The name of the costume designe" },
	{ "Actor", "Y YT", "Real name of an actor or actress playing a role in the movi" },
	{ "Actor_Character", "Y YT", "Name of the character an actor or actress plays in this movi" },
	{ "WrittenBy", "Y YT", "The author of the story or scrip" },
	{ "ScreenplayBy", "Y YT", "The author of the screenplay or scenario (used for movies and TV shows" },
	{ "EditedBy", "Y YT", "Editors na" },
	{ "CommissionedBy", "Y YT", "name of the person or organization that commissioned the subject of the fi" },
	{ "Producer", "Y YT", "Name of the producer of the movi" },
	{ "CoProducer", "Y YT", "The name of a co-produce" },
	{ "ExecutiveProducer", "Y YT", "The name of an executive produce" },
	{ "MusicBy", "Y YT", "Main music-artist for a mov" },
	{ "DistributedBy", "Y YT", "Company the item is mainly distributed b" },
	{ "OriginalSourceForm/DistributedBy", "Y YT", "Name of the person or organization who supplied the original subje" },
	{ "MasteredBy", "Y YT", "The engineer who mastered the content for a physical medium or for digital distributio" },
	{ "EncodedBy", "Y YT", "Name of the person or organisation that encoded/ripped the audio fil" },
	{ "RemixedBy", "Y YT", "Name of the artist(s), that interpreted, remixed, or otherwise modified the ite" },
	{ "ProductionStudio", "Y YT", "Main production studi" },
	{ "ThanksTo", "Y YT", "A very general tag for everyone else that wants to be liste" },
	{ "Publisher", "Y YT", "Name of the organization publishing the album (i.e. the 'record label') or movi" },
	{ "Publisher/URL", "Y YT", "Publishers official webpag" },
	{ "Label", "Y YT", "Brand or trademark associated with the marketing of music recordings and music video" },
	{ "Genre", "Y YT", "The main genre of the audio or video. e.g. classical, ambient-house, synthpop, sci-fi, drama, et" },
	{ "Mood", "Y YT", "Intended to reflect the mood of the item with a few keywords, e.g. Romantic, Sad, Uplifting, et" },
	{ "ContentType", "Y YT", "The type of the item. e.g. Documentary, Feature Film, Cartoon, Music Video, Music, Sound FX, et" },
	{ "Subject", "Y YT", "Describes the topic of the file, such as Aerial view of Seattle" },
	{ "Description", "Y YT", "A short description of the contents, such as Two birds flyin" },
	{ "Keywords", "Y YT", "Keywords to the item separated by a comma, used for searchin" },
	{ "Summary", "Y YT", "A plot outline or a summary of the stor" },
	{ "Synopsis", "Y YT", "A description of the story line of the ite" },
	{ "Period", "Y YT", "Describes the period that the piece is from or about. e.g. Renaissanc" },
	{ "LawRating", "Y YT", "Depending on the country it's the format of the rating of a movie (P, R, X in the USA, an age in other countries or a URI defining a logo" },
	{ "LawRating_Reason", "Y YT", "Reason for the law rati" },
	{ "ICRA", "Y YT", "The ICRA rating. (Previously RSAC" },
	{ "Released_Date", "Y YT", "The date/year that the item was release" },
	{ "Original/Released_Date", "Y YT", "The date/year that the item was originaly release" },
	{ "Recorded_Date", "Y YT", "The time/date/year that the recording bega" },
	{ "Encoded_Date", "Y YT", "The time/date/year that the encoding of this item was completed bega" },
	{ "Tagged_Date", "Y YT", "The time/date/year that the tags were done for this ite" },
	{ "Written_Date", "Y YT", "The time/date/year that the composition of the music/script bega" },
	{ "Mastered_Date", "Y YT", "The time/date/year that the item was tranfered to a digitalmediu" },
	{ "File_Created_Date", "N NT", "The time that the file was created on the file syst" },
	{ "File_Created_Date_Local", "N NT", "The time that the file was created on the file system (Warning: this field depends of local configuration, do not use it in an international databas" },
	{ "File_Modified_Date", "N NT", "The time that the file was modified on the file syst" },
	{ "File_Modified_Date_Local", "N NT", "The time that the file was modified on the file system (Warning: this field depends of local configuration, do not use it in an international databas" },
	{ "Recorded_Location", "Y YT", "Location where track was recorded. (See COMPOSITION_LOCATION for forma" },
	{ "Written_Location", "Y YT", "Location that the item was originaly designed/written. Information should be stored in the following format: country code, state/province, city where the coutry code is the same 2 octets as in Internet domains, or possibly ISO-3166. e.g. US, Texas, Austin or US, , Austi" },
	{ "Archival_Location", "Y YT", "Location, where an item is archived, e.eg. Louvre,Paris,Fran" },
	{ "Encoded_Application", "Y YT", "Name of the software package used to create the file, such as Microsoft WaveEdi" },
	{ "Encoded_Application/Url", "N YT", "Name of the software package used to create the file, such as Microsoft WaveEdi" },
	{ "Encoded_Library", "N YT", "Software used to create the fi" },
	{ "Encoded_Library/String", "Y NT", "Software used to create the fi" },
	{ "Encoded_Library/Name", "N NT", "Name of the the encoding-softwa" },
	{ "Encoded_Library/Version", "N NT", "Version of encoding-softwa" },
	{ "Encoded_Library/Date", "N NT", "Release date of softwa" },
	{ "Encoded_Library_Settings", "Y YT", "Parameters used by the softwa" },
	{ "Cropped", "Y YT", "Describes whether an image has been cropped and, if so, how it was croppe" },
	{ "Dimensions", "Y YT", "Specifies the size of the original subject of the file. eg 8.5 in h, 11 in" },
	{ "DotsPerInch", "Y YT", "Stores dots per inch setting of the digitizer used to produce the fi" },
	{ "Lightness", "Y YT", "Describes the changes in lightness settings on the digitizer required to produce the fi" },
	{ "OriginalSourceMedium", "Y YT", "Original medium of the material, e.g. vinyl, Audio-CD, Super8 or BetaM" },
	{ "OriginalSourceForm", "Y YT", "Original form of the material, e.g. slide, paper, m" },
	{ "OriginalSourceForm/NumColors", "Y YT", "Number of colors requested when digitizing, e.g. 256 for images or 32 bit RGB for vid" },
	{ "OriginalSourceForm/Name", "Y YT", "Name of the product the file was originally intended f" },
	{ "OriginalSourceForm/Cropped", "Y YT", "Describes whether an image has been cropped and, if so, how it was cropped. e.g. 16:9 to 4:3, top and bott" },
	{ "OriginalSourceForm/Sharpness", "Y YT", "Identifies the changes in sharpness for the digitizer requiered to produce the fi" },
	{ "Tagged_Application", "Y YT", "Software used to tag this fi" },
	{ "BPM", "Y YT", "Average number of beats per minu" },
	{ "ISRC", "Y YT", "International Standard Recording Code, excluding the ISRC prefix and including hyphen" },
	{ "ISBN", "Y YT", "International Standard Book Numbe" },
	{ "BarCode", "Y YT", "EAN-13 (13-digit European Article Numbering) or UPC-A (12-digit Universal Product Code) bar code identifie" },
	{ "LCCN", "Y YT", "Library of Congress Control Numbe" },
	{ "CatalogNumber", "Y YT", "A label-specific catalogue number used to identify the release. e.g. TIC 0" },
	{ "LabelCode", "Y YT", "A 4-digit or 5-digit number to identify the record label, typically printed as (LC) xxxx or (LC) 0xxxx on CDs medias or covers, with only the number being store" },
	{ "Owner", "Y YT", "Owner of the fi" },
	{ "Copyright", "Y YT", "Copyright attributio" },
	{ "Copyright/Url", "Y YT", "Link to a site with copyright/legal informatio" },
	{ "Producer_Copyright", "Y YT", "The copyright information as per the productioncopyright holde" },
	{ "TermsOfUse", "Y YT", "License information, e.g., All Rights Reserved,Any Use Permitte" },
	{ "ServiceName", "Y YT", "" },
	{ "ServiceChannel", "Y YT", "" },
	{ "Service/Url", "Y YT", "" },
	{ "ServiceProvider", "Y YT", "" },
	{ "ServiceProviderr/Url", "Y YT", "" },
	{ "ServiceType", "Y YT", "" },
	{ "NetworkName", "Y YT", "" },
	{ "OriginalNetworkName", "Y YT", "" },
	{ "Country", "Y YT", "" },
	{ "TimeZone", "Y YT", "" },
	{ "Cover", "Y YT", "Is there a cov" },
	{ "Cover_Description", "Y YT", "short descriptio, e.g. Earth in spa" },
	{ "Cover_Type", "Y YT", "" },
	{ "Cover_Mime", "Y YT", "" },
	{ "Cover_Data", "N YT", "Cover, in binary format encoded BASE" },
	{ "Lyrics", "Y YT", "Text of a so" },
	{ "Comment", "Y YT", "Any comment related to the conten" },
	{ "Rating", "Y YT", "A numeric value defining how much a person likes the song/movie. The number is between 0 and 5 with decimal values possible (e.g. 2.7), 5(.0) being the highest possible ratin" },
	{ "Added_Date", "Y YT", "Date/year the item was added to the owners collecti" },
	{ "Played_First_Date", "Y YT", "The date, the owner first played an it" },
	{ "Played_Last_Date", "Y YT", "The date, the owner last played an it" },
	{ "Played_Count", "Y YI", "Number of times an item was play" },
	{ "EPG_Positions_Begin", "N YI", "" },
	{ "EPG_Positions_End", "N YI", "" },
	{ "audio_layout", "N YI", "###### where # is the number of channels in audio stream n = number of characters from left" },
	{ "file_size", "N YI", "size of the file in Kilobytes" },
	{ "file_modified_age", "N YI", "difference between the current time and the file's modified date in minutes" },
	{ "file_created_age", "N YI", "difference between the current time and the file's created date in minutes" },
	{ "file_modified_date", "N YI", "the file's modified date in ISO format [YYYYMMDD]" },
	{ "file_created_date", "N YI", "the file's created date in ISO format [YYYYMMDD]" },
	{ "file_name", "N YI", "the file name excluding the path" },
	{ "file_extension", "N YI", "the file's extension, including the '.'" },
	{ "file_path", "N YI", "the file's path, excluding the filename" },
	{ "END", "END", "END" }
};


const std::string audio_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Audio", "N NT", "Stream type na" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Format", "Y YT", "Format us" },
	{ "Format/Info", "Y NT", "Info about the form" },
	{ "Format/Url", "N NT", "Homepage of this form" },
	{ "Format_Commercial", "N YT", "Commercial name used by vendor for theses setings or Format field if there is no differen" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is o" },
	{ "Format_Version", "Y YT", "Version of this form" },
	{ "Format_Profile", "Y YT", "Profile of this Form" },
	{ "Format_Compression", "Y YT", "Compression method us" },
	{ "Format_Settings", "N YT", "Settings needed for decoder used, summa" },
	{ "Format_Settings_SBR", "Yes;N YT", "" },
	{ "Format_Settings_SBR/String", "N NT", "" },
	{ "Format_Settings_PS", "Yes;N YT", "" },
	{ "Format_Settings_PS/String", "N NT", "" },
	{ "Format_Settings_Mode", "Y YT", "" },
	{ "Format_Settings_ModeExtension", "Y YT", "" },
	{ "Format_Settings_Emphasis", "Y YT", "" },
	{ "Format_Settings_Floor", "Y YT", "" },
	{ "Format_Settings_Firm", "Y YT", "" },
	{ "Format_Settings_Endianness", "Y YT", "" },
	{ "Format_Settings_Sign", "Y YT", "" },
	{ "Format_Settings_Law", "Y YT", "" },
	{ "Format_Settings_ITU", "Y YT", "" },
	{ "Format_Settings_Wrapping", "Y YT", "Wrapping mode (Frame wrapped or Clip wrapped)" },
	{ "InternetMediaType", "N YT", "Internet Media Type (aka MIME Type, Content-Typ" },
	{ "MuxingMode", "Y YT", "How this stream is muxed in the contain" },
	{ "MuxingMode_MoreInfo", "Y NT", "More info (text) about the muxing mo" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about codec " },
	{ "CodecID/Hint", "Y NT", "Hint/popular name for this codec " },
	{ "CodecID/Url", "N NT", "Homepage for more details about this codec " },
	{ "CodecID_Description", "Y YT", "Manual description given by the contain" },
	{ "Codec", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec/String", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec/Family", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec/Info", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec/Url", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec/CC", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Description", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Profile", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Automatic", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Floor", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Firm", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Endianness", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Sign", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_Law", "N NT", "Deprecated, do not use in new projec" },
	{ "Codec_Settings_ITU", "N NT", "Deprecated, do not use in new projec" },
	{ "Duration", " ms;N YI", "Play time of the stre" },
	{ "Duration/String", "Y NT", "Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if ze" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.M" },
	{ "Duration_FirstFrame", " ms;N YI", "Duration of the first frame if it is longer than others, in ms;" },
	{ "Duration_FirstFrame/String", "Y NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String1", "N NT", "Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_FirstFrame/String2", "N NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String3", "N NT", "Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Duration_LastFrame", " ms;N YI", "Duration of the last frame if it is longer than others, in ms;" },
	{ "Duration_LastFrame/String", "Y NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String1", "N NT", "Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_LastFrame/String2", "N NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String3", "N NT", "Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration", " ms;N YI", "Source Play time of the stre" },
	{ "Source_Duration/String", "Y NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String1", "N NT", "Source Play time in format : HHh MMmn SSs MMMms, XX omited if ze" },
	{ "Source_Duration/String2", "N NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String3", "N NT", "Source Play time in format : HH:MM:SS.M" },
	{ "Source_Duration_FirstFrame", " ms;N YI", "Source Duration of the first frame if it is longer than others, in ms;" },
	{ "Source_Duration_FirstFrame/String", "Y NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String1", "N NT", "Source Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_FirstFrame/String2", "N NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String3", "N NT", "Source Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration_LastFrame", " ms;N YI", "Source Duration of the last frame if it is longer than others, in ms;" },
	{ "Source_Duration_LastFrame/String", "Y NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String1", "N NT", "Source Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_LastFrame/String2", "N NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String3", "N NT", "Source Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "BitRate_Mode", "N YT", "Bit rate mode (VBR, CB" },
	{ "BitRate_Mode/String", "Y NT", "Bit rate mode (Constant, Variabl" },
	{ "BitRate", " bps;N YF", "Bit rate in b" },
	{ "BitRate/String", "Y NT", "Bit rate (with measuremen" },
	{ "BitRate_Minimum", " bps;N YF", "Minimum Bit rate in b" },
	{ "BitRate_Minimum/String", "Y NT", "Minimum Bit rate (with measuremen" },
	{ "BitRate_Nominal", " bps;N YF", "Nominal Bit rate in b" },
	{ "BitRate_Nominal/String", "Y NT", "Nominal Bit rate (with measuremen" },
	{ "BitRate_Maximum", " bps;N YF", "Maximum Bit rate in b" },
	{ "BitRate_Maximum/String", "Y NT", "Maximum Bit rate (with measuremen" },
	{ "BitRate_Encoded", " bps;N YF", "Encoded (with forced padding) bit rate in bps, if some container padding is present" },
	{ "BitRate_Encoded/String", "Y NT", "Encoded (with forced padding) bit rate (with measurement), if some container padding is present" },
	{ "Channel(s)", " channel;N YI", "Number of channe" },
	{ "Channel(s)/String", "Y NT", "Number of channels (with measuremen" },
	{ "Channel(s)_Original", " channel;N YI", "Number of channe" },
	{ "Channel(s)_Original/String", "Y NT", "Number of channels (with measuremen" },
	{ "ChannelPositions", "Y YT", "Position of channe" },
	{ "ChannelPositions/String2", "N NT", "Position of channels (x/y.z forma" },
	{ "ChannelLayout", "N YT", "Layout of channels (in the strea" },
	{ "SamplingRate", " Hz;N YF", "Sampling ra" },
	{ "SamplingRate/String", "Y NT", "in K" },
	{ "SamplingCount", "N NI", "Sample count (based on sampling rat" },
	{ "Source_SamplingCount", "N NI", "Source Sample count (based on sampling rat" },
	{ "FrameRate", " fps;N YF", "Frames per second" },
	{ "FrameRate/String", "N NT", "Frames per second (with measurement)" },
	{ "FrameCount", "N NI", "Frame count (a frame contains a count of samples depends of the forma" },
	{ "Source_FrameCount", "N NI", "Source Frame count (a frame contains a count of samples depends of the forma" },
	{ "Resolution", " bit;N NI", "Deprecated, do not use in new projec" },
	{ "Resolution/String", "N NT", "Deprecated, do not use in new projec" },
	{ "BitDepth", " bit;N YI", "Resolution in bits (8, 16, 20, 2" },
	{ "BitDepth/String", "Y NT", "Resolution in bits (8, 16, 20, 2" },
	{ "Compression_Mode", "N YT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Mode/String", "Y NT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Ratio", "Y YF", "Current stream size divided by uncompressed stream si" },
	{ "Delay", " ms;N NI", "Delay fixed in the stream (relative) IN MS;" },
	{ "Delay/String", "N NT", "Delay with measurement;" },
	{ "Delay/String1", "N NT", "Delay with measurement;" },
	{ "Delay/String2", "N NT", "Delay with measurement;" },
	{ "Delay/String3", "N NT", "Delay in format : HH:MM:SS.MMM;" },
	{ "Delay/String4", "N NT", "Delay in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Delay_Settings", "N NT", "Delay settings (in case of timecode for example);" },
	{ "Delay_DropFrame", "N NT", "Delay drop frame;" },
	{ "Delay_Source", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Source/String", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Original", " ms;N NI", "Delay fixed in the raw stream (relative) IN MS;" },
	{ "Delay_Original/String", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String1", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String2", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String3", "N NT", "Delay in format: HH:MM:SS.M" },
	{ "Delay_Original/String4", "N NT", "Delay in format: HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available);" },
	{ "Delay_Original_Settings", "N NT", "Delay settings (in case of timecode for exampl" },
	{ "Delay_Original_DropFrame", "N NT", "Delay drop frame info;" },
	{ "Delay_Original_Source", "N NT", "Delay source (Stream or empty);" },
	{ "Video_Delay", " ms;N NI", "Delay fixed in the stream (absolute / video)" },
	{ "Video_Delay/String", "Y NT", "" },
	{ "Video_Delay/String1", "N NT", "" },
	{ "Video_Delay/String2", "N NT", "" },
	{ "Video_Delay/String3", "N NT", "" },
	{ "Video_Delay/String4", "N NT", "" },
	{ "Video0_Delay", " ms;N NI", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String1", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String2", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String3", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String4", "N NT", "Deprecated, do not use in new projects" },
	{ "ReplayGain_Gain", " dB;N YT", "The gain to apply to reach 89dB SPL on playba" },
	{ "ReplayGain_Gain/String", "Y YT", "" },
	{ "ReplayGain_Peak", "Y YT", "The maximum absolute peak value of the it" },
	{ "StreamSize", " byte;N YI", "Streamsize in byt" },
	{ "StreamSize/String", "Y NT", "Streamsize in with percentage val" },
	{ "StreamSize/String1", "N NT", "" },
	{ "StreamSize/String2", "N NT", "" },
	{ "StreamSize/String3", "N NT", "" },
	{ "StreamSize/String4", "N NT", "" },
	{ "StreamSize/String5", "N NT", "Streamsize in with percentage val" },
	{ "StreamSize_Proportion", "N NT", "Stream size divided by file si" },
	{ "Source_StreamSize", " byte;N YI", "Source Streamsize in byt" },
	{ "Source_StreamSize/String", "Y NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize/String1", "N NT", "" },
	{ "Source_StreamSize/String2", "N NT", "" },
	{ "Source_StreamSize/String3", "N NT", "" },
	{ "Source_StreamSize/String4", "N NT", "" },
	{ "Source_StreamSize/String5", "N NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize_Proportion", "N NT", "Source Stream size divided by file si" },
	{ "StreamSize_Encoded", " byte;N YI", "Encoded Streamsize in byt" },
	{ "StreamSize_Encoded/String", "Y NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded/String1", "N NT", "" },
	{ "StreamSize_Encoded/String2", "N NT", "" },
	{ "StreamSize_Encoded/String3", "N NT", "" },
	{ "StreamSize_Encoded/String4", "N NT", "" },
	{ "StreamSize_Encoded/String5", "N NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded_Proportion", "N NT", "Encoded Stream size divided by file si" },
	{ "Source_StreamSize_Encoded", " byte;N YI", "Source Encoded Streamsize in byt" },
	{ "Source_StreamSize_Encoded/String", "Y NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded/String1", "N NT", "" },
	{ "Source_StreamSize_Encoded/String2", "N NT", "" },
	{ "Source_StreamSize_Encoded/String3", "N NT", "" },
	{ "Source_StreamSize_Encoded/String4", "N NT", "" },
	{ "Source_StreamSize_Encoded/String5", "N NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded_Proportion", "N NT", "Source Encoded Stream size divided by file si" },
	{ "Alignment", "N YT", "How this stream file is aligned in the contain" },
	{ "Alignment/String", "Y NT", "Where this stream file is aligned in the contain" },
	{ "Interleave_VideoFrames", "N YF", "Between how many video frames the stream is insert" },
	{ "Interleave_Duration", "N YI", "Between how much time (ms) the stream is insert" },
	{ "Interleave_Duration/String", "Y NT", "Between how much time and video frames the stream is inserted (with measuremen" },
	{ "Interleave_Preload", "N YI", "How much time is buffered before the first video fra" },
	{ "Interleave_Preload/String", "Y NT", "How much time is buffered before the first video frame (with measuremen" },
	{ "Title", "Y YI", "Name of the tra" },
	{ "Encoded_Library", "N YT", "Software used to create the fi" },
	{ "Encoded_Library/String", "Y NT", "Software used to create the fi" },
	{ "Encoded_Library/Name", "N NT", "Info from the softwa" },
	{ "Encoded_Library/Version", "N NT", "Version of softwa" },
	{ "Encoded_Library/Date", "N NT", "Release date of softwa" },
	{ "Encoded_Library_Settings", "Y YT", "Parameters used by the softwa" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-c" },
	{ "Language/String", "Y NT", "Language (ful" },
	{ "Language/String1", "N NT", "Language (ful" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empt" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "Default", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Default/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Encoded_Date", "Y YT", "UTC time that the encoding of this item was completed bega" },
	{ "Tagged_Date", "Y YT", "UTC time that the tags were done for this ite" },
	{ "Encryption", "Y YT", "" },
	{ "END", "END", "END" }
};


const std::string video_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Video", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Format", "Y YT", "Format used" },
	{ "Format/Info", "Y NT", "Info about Format" },
	{ "Format/Url", "N NT", "Link" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y YT", "Version of this format" },
	{ "Format_Profile", "Y YT", "Profile of the Format" },
	{ "Format_Compression", "N YT", "Compression mode of the Format" },
	{ "MultiView_BaseProfile", "Y YT", "Multiview, profile of the base stream" },
	{ "MultiView_Count", "Y YT", "Multiview, count of views" },
	{ "MultiView_Layout", "Y YT", "Multiview, how views are muxed in the container in case of it is not muxing in the stream" },
	{ "Format_Settings", "N YT", "Settings needed for decoder used, summary" },
	{ "Format_Settings_BVOP", "Yes;N YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_BVOP/String", "Y NT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_QPel", "Yes;N YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_QPel/String", "Y NT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_GMC", " warppoint;N YI", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_GMC/String", "Y NT", "" },
	{ "Format_Settings_Matrix", "N YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_Matrix/String", "Y NT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_Matrix_Data", "N NT", "Matrix, in binary format encoded BASE64. Order = intra, non-intra, gray intra, gray non-intra" },
	{ "Format_Settings_CABAC", "Yes;N YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_CABAC/String", "Y NT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_RefFrames", " frame;N YI", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_RefFrames/String", "Y NT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_Pulldown", "Y YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_FrameMode", "Y YT", "Settings needed for decoder used, detailled" },
	{ "Format_Settings_GOP", "Y YT", "Settings needed for decoder used, detailled (M=x N=y)" },
	{ "Format_Settings_FrameStructures", "Y YT", "Settings needed for decoder used, detailled (Type of frame, and field/frame info)" },
	{ "Format_Settings_Wrapping", "Y YT", "Wrapping mode (Frame wrapped or Clip wrapped)" },
	{ "InternetMediaType", "N YT", "Internet Media Type (aka MIME Type, Content-Type)" },
	{ "MuxingMode", "Y YT", "How this file is muxed in the container" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info on the codec" },
	{ "CodecID/Hint", "Y NT", "Hint/popular name for this codec" },
	{ "CodecID/Url", "N NT", "Homepage for more details about this codec" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Codec", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Family", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Info", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Url", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/CC", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Profile", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Description", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_PacketBitStream", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_BVOP", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_QPel", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_GMC", " warppoint;N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_GMC/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_Matrix", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_Matrix_Data", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_CABAC", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec_Settings_RefFrames", "N NT", "Deprecated, do not use in new projects" },
	{ "Duration", " ms;N YI", "Play time of the stream in ms;" },
	{ "Duration/String", "Y NT", "Play time in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.MMM;" },
	{ "Duration/String4", "N NT", "Play time in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Duration_FirstFrame", " ms;N YI", "Duration of the first frame if it is longer than others, in ms;" },
	{ "Duration_FirstFrame/String", "Y NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String1", "N NT", "Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_FirstFrame/String2", "N NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String3", "N NT", "Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Duration_LastFrame", " ms;N YI", "Duration of the last frame if it is longer than others, in ms;" },
	{ "Duration_LastFrame/String", "Y NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String1", "N NT", "Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_LastFrame/String2", "N NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String3", "N NT", "Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration", " ms;N YI", "Source Play time of the stre" },
	{ "Source_Duration/String", "Y NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String1", "N NT", "Source Play time in format : HHh MMmn SSs MMMms, XX omited if ze" },
	{ "Source_Duration/String2", "N NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String3", "N NT", "Source Play time in format : HH:MM:SS.M" },
	{ "Source_Duration_FirstFrame", " ms;N YI", "Source Duration of the first frame if it is longer than others, in ms;" },
	{ "Source_Duration_FirstFrame/String", "Y NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String1", "N NT", "Source Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_FirstFrame/String2", "N NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String3", "N NT", "Source Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration_LastFrame", " ms;N YI", "Source Duration of the last frame if it is longer than others, in ms;" },
	{ "Source_Duration_LastFrame/String", "Y NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String1", "N NT", "Source Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_LastFrame/String2", "N NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String3", "N NT", "Source Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "BitRate_Mode", "N YT", "Bit rate mode (VBR, CBR);" },
	{ "BitRate_Mode/String", "Y NT", "Bit rate mode (Variable, Cconstant);" },
	{ "BitRate", " bps;N YF", "Bit rate in bps;" },
	{ "BitRate/String", "Y NT", "Bit rate (with measurement);" },
	{ "BitRate_Minimum", " bps;N YF", "Minimum Bit rate in bps;" },
	{ "BitRate_Minimum/String", "Y NT", "Minimum Bit rate (with measurement)" },
	{ "BitRate_Nominal", " bps;N YF", "Nominal Bit rate in bps" },
	{ "BitRate_Nominal/String", "Y NT", "Nominal Bit rate (with measurement)" },
	{ "BitRate_Maximum", " bps;N YF", "Maximum Bit rate in bps" },
	{ "BitRate_Maximum/String", "Y NT", "Maximum Bit rate (with measurement)" },
	{ "BitRate_Encoded", " bps;N YF", "Encoded (with forced padding) bit rate in bps, if some container padding is present" },
	{ "BitRate_Encoded/String", "Y NT", "Encoded (with forced padding) bit rate (with measurement), if some container padding is present" },
	{ "Width", " pixel;N YI", "Width (aperture size if present) in pixel" },
	{ "Width/String", "Y NT", "Width (aperture size if present) with measurement (pixel)" },
	{ "Width_Offset", " pixel;N YI", "Offset between original width and displayed width (aperture size) in pixel" },
	{ "Width_Offset/String", "N NT", "Offset between original width and displayed width (aperture size)  in pixel" },
	{ "Width_Original", " pixel;N YI", "Original (in the raw stream) width in pixel" },
	{ "Width_Original/String", "Y NT", "Original (in the raw stream) width with measurement (pixel)" },
	{ "Height", " pixel;N YI", "Height (aperture size if present) in pixel" },
	{ "Height/String", "Y NT", "Height (aperture size if present) with measurement (pixel)" },
	{ "Height_Offset", " pixel;N YI", "Offset between original height and displayed height (aperture size) in pixel" },
	{ "Height_Offset/String", "N NT", "Offset between original height and displayed height (aperture size)  in pixel" },
	{ "Height_Original", " pixel;N YI", "Original (in the raw stream) height in pixel" },
	{ "Height_Original/String", "Y NT", "Original (in the raw stream) height with measurement (pixel)" },
	{ "PixelAspectRatio", "N YF", "Pixel Aspect ratio" },
	{ "PixelAspectRatio/String", "N NT", "Pixel Aspect ratio" },
	{ "PixelAspectRatio_Original", "N YF", "Original (in the raw stream) Pixel Aspect ratio" },
	{ "PixelAspectRatio_Original/String", "N NT", "Original (in the raw stream) Pixel Aspect ratio" },
	{ "DisplayAspectRatio", "N YF", "Display Aspect ratio" },
	{ "DisplayAspectRatio/String", "Y NT", "Display Aspect ratio" },
	{ "DisplayAspectRatio_Original", "N YF", "Original (in the raw stream) Display Aspect ratio" },
	{ "DisplayAspectRatio_Original/String", "Y NT", "Original (in the raw stream) Display Aspect ratio" },
	{ "ActiveFormatDescription", "N YN", "Active Format Description (AFD value)" },
	{ "ActiveFormatDescription/String", "Y NT", "Active Format Description (text)" },
	{ "ActiveFormatDescription_MuxingMode", "N YT", "Active Format Description (AFD value) muxing mode (Ancillary or Raw stream)" },
	{ "Rotation", "N YT", "Rotation" },
	{ "Rotation/String", "Y NT", "Rotation (if not horizontal)" },
	{ "FrameRate_Mode", "N YT", "Frame rate mode (CFR, VFR)" },
	{ "FrameRate_Mode/String", "Y NT", "Frame rate mode (Constant, Variable)" },
	{ "FrameRate_Mode_Original", "N YT", "Original frame rate mode (CFR, VFR)" },
	{ "FrameRate_Mode_Original/String", "Y NT", "Original frame rate mode (Constant, Variable)" },
	{ "FrameRate", " fps;N YF", "Frames per second" },
	{ "FrameRate/String", "Y NT", "Frames per second (with measurement)" },
	{ "FrameRate_Original", " fps;N YF", "Original (in the raw stream) frames per second" },
	{ "FrameRate_Original/String", "Y NT", "Original (in the raw stream) frames per second (with measurement)" },
	{ "FrameRate_Minimum", " fps;N YF", "Minimum Frames per second" },
	{ "FrameRate_Minimum/String", "Y NT", "Minimum Frames per second (with measurement)" },
	{ "FrameRate_Nominal", " fps;N YF", "Nominal Frames per second" },
	{ "FrameRate_Nominal/String", "Y NT", "Nominal Frames per second (with measurement)" },
	{ "FrameRate_Maximum", " fps;N YF", "Maximum Frames per second" },
	{ "FrameRate_Maximum/String", "Y NT", "Maximum Frames per second (with measurement)" },
	{ "FrameCount", "N NI", "Number of frames" },
	{ "Source_FrameCount", "N NI", "Source Number of frames" },
	{ "Standard", "Y NT", "NTSC or PAL" },
	{ "Resolution", " bit;N NI", "Deprecated, do not use in new projects" },
	{ "Resolution/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Colorimetry", "N NT", "Deprecated, do not use in new projects" },
	{ "ColorSpace", "Y YT", "" },
	{ "ChromaSubsampling", "Y YT", "" },
	{ "BitDepth", " bit;N YI", "16/24/32" },
	{ "BitDepth/String", "Y NT", "16/24/32 bits" },
	{ "ScanType", "N YT", "" },
	{ "ScanType/String", "Y NT", "" },
	{ "ScanType_Original", "N YT", "" },
	{ "ScanType_Original/String", "Y NT", "" },
	{ "ScanOrder", "N YT", "" },
	{ "ScanOrder/String", "Y NT", "" },
	{ "ScanOrder_StoredDisplayedInverted", "Y YT", "" },
	{ "ScanOrder_Original", "N YT", "" },
	{ "ScanOrder_Original/String", "Y NT", "" },
	{ "Interlacement", "N NT", "Deprecated, do not use in new projects;" },
	{ "Interlacement/String", "N NT", "Deprecated, do not use in new projects;" },
	{ "Compression_Mode", "N YT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Mode/String", "Y NT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Ratio", "Y YF", "Current stream size divided by uncompressed stream si" },
	{ "Bits-(Pixel*Frame)", "Y NF", "bits/(Pixel*Frame) (like Gordian Knot);" },
	{ "Delay", " ms;N NI", "Delay fixed in the stream (relative) IN MS;" },
	{ "Delay/String", "N NT", "Delay with measurement;" },
	{ "Delay/String1", "N NT", "Delay with measurement;" },
	{ "Delay/String2", "N NT", "Delay with measurement;" },
	{ "Delay/String3", "N NT", "Delay in format : HH:MM:SS.MMM;" },
	{ "Delay/String4", "N NT", "Delay in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Delay_Settings", "N NT", "Delay settings (in case of timecode for example);" },
	{ "Delay_DropFrame", "N NT", "Delay drop frame;" },
	{ "Delay_Source", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Source/String", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Original", " ms;N NI", "Delay fixed in the raw stream (relative) IN MS;" },
	{ "Delay_Original/String", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String1", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String2", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String3", "N NT", "Delay in format: HH:MM:SS.M" },
	{ "Delay_Original/String4", "N NT", "Delay in format: HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available);" },
	{ "Delay_Original_Settings", "N NT", "Delay settings (in case of timecode for exampl" },
	{ "Delay_Original_DropFrame", "N NT", "Delay drop frame info;" },
	{ "Delay_Original_Source", "N NT", "Delay source (Stream or empty);" },
	{ "TimeStamp_FirstFrame", " ms;N YI", "TimeStamp fixed in the stream (relative) IN MS;" },
	{ "TimeStamp_FirstFrame/String", "Y NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String1", "N NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String2", "N NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String3", "N NT", "TimeStamp in format : HH:MM:SS.MMM;" },
	{ "TimeCode_FirstFrame", "Y YC", "Time code in HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available) format" },
	{ "TimeCode_Settings", "Y YT", "Time code settings" },
	{ "TimeCode_Source", "Y YT", "Time code source (Container, Stream, SystemScheme1, SDTI, ANC...)" },
	{ "StreamSize", " byte;N YI", "Streamsize in byt" },
	{ "StreamSize/String", "Y NT", "Streamsize in with percentage val" },
	{ "StreamSize/String1", "N NT", "" },
	{ "StreamSize/String2", "N NT", "" },
	{ "StreamSize/String3", "N NT", "" },
	{ "StreamSize/String4", "N NT", "" },
	{ "StreamSize/String5", "N NT", "Streamsize in with percentage val" },
	{ "StreamSize_Proportion", "N NT", "Stream size divided by file si" },
	{ "Source_StreamSize", " byte;N YI", "Source Streamsize in byt" },
	{ "Source_StreamSize/String", "Y NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize/String1", "N NT", "" },
	{ "Source_StreamSize/String2", "N NT", "" },
	{ "Source_StreamSize/String3", "N NT", "" },
	{ "Source_StreamSize/String4", "N NT", "" },
	{ "Source_StreamSize/String5", "N NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize_Proportion", "N NT", "Source Stream size divided by file si" },
	{ "StreamSize_Encoded", " byte;N YI", "Encoded Streamsize in byt" },
	{ "StreamSize_Encoded/String", "Y NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded/String1", "N NT", "" },
	{ "StreamSize_Encoded/String2", "N NT", "" },
	{ "StreamSize_Encoded/String3", "N NT", "" },
	{ "StreamSize_Encoded/String4", "N NT", "" },
	{ "StreamSize_Encoded/String5", "N NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded_Proportion", "N NT", "Encoded Stream size divided by file si" },
	{ "Source_StreamSize_Encoded", " byte;N YI", "Source Encoded Streamsize in byt" },
	{ "Source_StreamSize_Encoded/String", "Y NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded/String1", "N NT", "" },
	{ "Source_StreamSize_Encoded/String2", "N NT", "" },
	{ "Source_StreamSize_Encoded/String3", "N NT", "" },
	{ "Source_StreamSize_Encoded/String4", "N NT", "" },
	{ "Source_StreamSize_Encoded/String5", "N NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded_Proportion", "N NT", "Source Encoded Stream size divided by file si" },
	{ "Alignment", "Y NT", "How this stream file is aligned in the contain" },
	{ "Alignment/String", "N YT", "" },
	{ "Title", "Y YI", "Name of the tra" },
	{ "Encoded_Application", "Y YT", "Software. Identifies the name of the software package used to create the file, such as Microsoft WaveEdi" },
	{ "Encoded_Application/Url", "N YT", "Software. Identifies the name of the software package used to create the file, such as Microsoft WaveEdi" },
	{ "Encoded_Library", "N YT", "Software used to create the fi" },
	{ "Encoded_Library/String", "Y NT", "Software used to create the fi" },
	{ "Encoded_Library/Name", "N NT", "Info from the softwa" },
	{ "Encoded_Library/Version", "N NT", "Version of softwa" },
	{ "Encoded_Library/Date", "N NT", "Release date of softwa" },
	{ "Encoded_Library_Settings", "Y YT", "Parameters used by the softwa" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-c" },
	{ "Language/String", "Y NT", "Language (ful" },
	{ "Language/String1", "N NT", "Language (ful" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empt" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "Default", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Default/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Encoded_Date", "Y YT", "UTC time that the encoding of this item was completed bega" },
	{ "Tagged_Date", "Y YT", "UTC time that the tags were done for this ite" },
	{ "Encryption", "Y YT", "" },
	{ "BufferSize", "N YT", "Defines the size of the buffer needed to decode the sequence." },
	{ "colour_description_present", "N YT", "Presence of colour description" },
	{ "colour_primaries", "Y YT", "Chromaticity coordinates of the source primaries" },
	{ "transfer_characteristics", "Y YT", "Opto-electronic transfer characteristic of the source picture" },
	{ "matrix_coefficients", "Y YT", "Matrix coefficients used in deriving luma and chroma signals from the green, blue, and red primaries" },
	{ "colour_description_present_Original", "N YT", "Presence of colour description" },
	{ "colour_primaries_Original", "Y YT", "Chromaticity coordinates of the source primaries" },
	{ "transfer_characteristics_Original", "Y YT", "Opto-electronic transfer characteristic of the source picture" },
	{ "matrix_coefficients_Original", "Y YT", "Matrix coefficients used in deriving luma and chroma signals from the green, blue, and red primaries" },
	{ "END", "END", "END" }
};


const std::string text_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Text", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Format", "Y YT", "Format used" },
	{ "Format/Info", "Y NT", "Info about Format" },
	{ "Format/Url", "N NT", "Link" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y NT", "Version of this format" },
	{ "Format_Profile", "Y NT", "Profile of the Format" },
	{ "Format_Compression", "Y NT", "Compression method us" },
	{ "Format_Settings", "Y NT", "Settings needed for decoder used" },
	{ "Format_Settings_Wrapping", "Y YT", "Wrapping mode (Frame wrapped or Clip wrapped)" },
	{ "InternetMediaType", "N YT", "Internet Media Type (aka MIME Type, Content-Type)" },
	{ "MuxingMode", "Y YT", "How this stream is muxed in the container" },
	{ "MuxingMode_MoreInfo", "Y NT", "More info (text) about the muxing mode" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about codec ID" },
	{ "CodecID/Hint", "Y NT", "A hint for this codec ID" },
	{ "CodecID/Url", "N NT", "A link for more details about this codec ID" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Codec", "N YT", "Deprecated" },
	{ "Codec/String", "N NT", "Deprecated" },
	{ "Codec/Info", "N NT", "Deprecated" },
	{ "Codec/Url", "N NT", "Deprecated" },
	{ "Codec/CC", "N NT", "Deprecated;" },
	{ "Duration", " ms;N YI", "Play time of the stream;" },
	{ "Duration/String", "Y NT", "Play time (formated);" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.MMM;" },
	{ "Duration/String4", "N NT", "Play time in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Duration_FirstFrame", " ms;N YI", "Duration of the first frame if it is longer than others, in ms;" },
	{ "Duration_FirstFrame/String", "Y NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String1", "N NT", "Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_FirstFrame/String2", "N NT", "Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_FirstFrame/String3", "N NT", "Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Duration_LastFrame", " ms;N YI", "Duration of the last frame if it is longer than others, in ms;" },
	{ "Duration_LastFrame/String", "Y NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String1", "N NT", "Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Duration_LastFrame/String2", "N NT", "Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Duration_LastFrame/String3", "N NT", "Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration", " ms;N YI", "Source Play time of the stre" },
	{ "Source_Duration/String", "Y NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String1", "N NT", "Source Play time in format : HHh MMmn SSs MMMms, XX omited if ze" },
	{ "Source_Duration/String2", "N NT", "Source Play time in format : XXx YYy only, YYy omited if ze" },
	{ "Source_Duration/String3", "N NT", "Source Play time in format : HH:MM:SS.M" },
	{ "Source_Duration_FirstFrame", " ms;N YI", "Source Duration of the first frame if it is longer than others, in ms;" },
	{ "Source_Duration_FirstFrame/String", "Y NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String1", "N NT", "Source Duration of the first frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_FirstFrame/String2", "N NT", "Source Duration of the first frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_FirstFrame/String3", "N NT", "Source Duration of the first frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "Source_Duration_LastFrame", " ms;N YI", "Source Duration of the last frame if it is longer than others, in ms;" },
	{ "Source_Duration_LastFrame/String", "Y NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String1", "N NT", "Source Duration of the last frame if it is longer than others, in format : HHh MMmn SSs MMMms, XX omited if zero;" },
	{ "Source_Duration_LastFrame/String2", "N NT", "Source Duration of the last frame if it is longer than others, in format : XXx YYy only, YYy omited if zero;" },
	{ "Source_Duration_LastFrame/String3", "N NT", "Source Duration of the last frame if it is longer than others, in format : HH:MM:SS.MMM;" },
	{ "BitRate_Mode", "N YT", "Bit rate mode (VBR, CBR);" },
	{ "BitRate_Mode/String", "Y NT", "Bit rate mode (Constant, Variable);" },
	{ "BitRate", " bps;N YF", "Bit rate in bps;" },
	{ "BitRate/String", "Y NT", "Bit rate (with measurement);" },
	{ "BitRate_Minimum", " bps;N YF", "Minimum Bit rate in bps;" },
	{ "BitRate_Minimum/String", "Y NT", "Minimum Bit rate (with measurement);" },
	{ "BitRate_Nominal", " bps;N YF", "Nominal Bit rate in bps;" },
	{ "BitRate_Nominal/String", "Y NT", "Nominal Bit rate (with measurement);" },
	{ "BitRate_Maximum", " bps;N YF", "Maximum Bit rate in bps;" },
	{ "BitRate_Maximum/String", "Y NT", "Maximum Bit rate (with measurement)" },
	{ "BitRate_Encoded", " bps;N YF", "Encoded (with forced padding) bit rate in bps, if some container padding is present" },
	{ "BitRate_Encoded/String", "Y NT", "Encoded (with forced padding) bit rate (with measurement), if some container padding is present" },
	{ "Width", " pixel;N YI", "Width" },
	{ "Width/String", "Y NT", "" },
	{ "Height", " pixel;N YI", "Height" },
	{ "Height/String", "Y NT", "" },
	{ "FrameRate_Mode", "N YT", "Frame rate mode (CFR, VFR)" },
	{ "FrameRate_Mode/String", "Y NT", "Frame rate mode (Constant, Variable)" },
	{ "FrameRate", " fps;N YF", "Frames per second" },
	{ "FrameRate/String", "Y NT", "Frames per second (with measurement)" },
	{ "FrameRate_Minimum", " fps;N YF", "Minimum Frames per second" },
	{ "FrameRate_Minimum/String", "Y NT", "Minimum Frames per second (with measurement)" },
	{ "FrameRate_Nominal", " fps;N YF", "Nominal Frames per second" },
	{ "FrameRate_Nominal/String", "Y NT", "Nominal Frames per second (with measurement)" },
	{ "FrameRate_Maximum", " fps;N YF", "Maximum Frames per second" },
	{ "FrameRate_Maximum/String", "Y NT", "Maximum Frames per second (with measurement)" },
	{ "FrameRate_Original", " fps;N YF", "Original (in the raw stream) Frames per second" },
	{ "FrameRate_Original/String", "Y NT", "Original (in the raw stream) Frames per second (with measurement)" },
	{ "FrameCount", "N NI", "Number of frames" },
	{ "Source_FrameCount", "N NI", "Source Number of frames" },
	{ "ColorSpace", "Y YT", "" },
	{ "ChromaSubsampling", "Y YT", "" },
	{ "Resolution", " bit;N NI", "Deprecated, do not use in new projects" },
	{ "Resolution/String", "N NT", "Deprecated, do not use in new projects" },
	{ "BitDepth", " bit;N YI", "" },
	{ "BitDepth/String", "Y NT", "" },
	{ "Compression_Mode", "N YT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Mode/String", "Y NT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Ratio", "Y YF", "Current stream size divided by uncompressed stream si" },
	{ "Delay", " ms;N NI", "Delay fixed in the stream (relative) IN MS;" },
	{ "Delay/String", "N NT", "Delay with measurement;" },
	{ "Delay/String1", "N NT", "Delay with measurement;" },
	{ "Delay/String2", "N NT", "Delay with measurement;" },
	{ "Delay/String3", "N NT", "Delay in format : HH:MM:SS.MMM;" },
	{ "Delay/String4", "N NT", "Delay in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Delay_Settings", "N NT", "Delay settings (in case of timecode for example);" },
	{ "Delay_DropFrame", "N NT", "Delay drop frame;" },
	{ "Delay_Source", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Source/String", "N NT", "Delay source (Container or Stream or empty);" },
	{ "Delay_Original", " ms;N NI", "Delay fixed in the raw stream (relative) IN MS;" },
	{ "Delay_Original/String", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String1", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String2", "N NT", "Delay with measurement;" },
	{ "Delay_Original/String3", "N NT", "Delay in format: HH:MM:SS.M" },
	{ "Delay_Original/String4", "N NT", "Delay in format: HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available);" },
	{ "Delay_Original_Settings", "N NT", "Delay settings (in case of timecode for exampl" },
	{ "Delay_Original_DropFrame", "N NT", "Delay drop frame info;" },
	{ "Delay_Original_Source", "N NT", "Delay source (Stream or empty);" },
	{ "Video_Delay", " ms;N NI", "Delay fixed in the stream (absolute / video)" },
	{ "Video_Delay/String", "Y NT", "" },
	{ "Video_Delay/String1", "N NT", "" },
	{ "Video_Delay/String2", "N NT", "" },
	{ "Video_Delay/String3", "N NT", "" },
	{ "Video_Delay/String4", "N NT", "" },
	{ "Video0_Delay", " ms;N NI", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String1", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String2", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String3", "N NT", "Deprecated, do not use in new projects" },
	{ "Video0_Delay/String4", "N NT", "Deprecated, do not use in new projects" },
	{ "StreamSize", " byte;N YI", "Streamsize in byt" },
	{ "StreamSize/String", "Y NT", "Streamsize in with percentage val" },
	{ "StreamSize/String1", "N NT", "" },
	{ "StreamSize/String2", "N NT", "" },
	{ "StreamSize/String3", "N NT", "" },
	{ "StreamSize/String4", "N NT", "" },
	{ "StreamSize/String5", "N NT", "Streamsize in with percentage val" },
	{ "StreamSize_Proportion", "N NT", "Stream size divided by file si" },
	{ "Source_StreamSize", " byte;N YI", "Source Streamsize in byt" },
	{ "Source_StreamSize/String", "Y NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize/String1", "N NT", "" },
	{ "Source_StreamSize/String2", "N NT", "" },
	{ "Source_StreamSize/String3", "N NT", "" },
	{ "Source_StreamSize/String4", "N NT", "" },
	{ "Source_StreamSize/String5", "N NT", "Source Streamsize in with percentage val" },
	{ "Source_StreamSize_Proportion", "N NT", "Source Stream size divided by file si" },
	{ "StreamSize_Encoded", " byte;N YI", "Encoded Streamsize in byt" },
	{ "StreamSize_Encoded/String", "Y NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded/String1", "N NT", "" },
	{ "StreamSize_Encoded/String2", "N NT", "" },
	{ "StreamSize_Encoded/String3", "N NT", "" },
	{ "StreamSize_Encoded/String4", "N NT", "" },
	{ "StreamSize_Encoded/String5", "N NT", "Encoded Streamsize in with percentage val" },
	{ "StreamSize_Encoded_Proportion", "N NT", "Encoded Stream size divided by file si" },
	{ "Source_StreamSize_Encoded", " byte;N YI", "Source Encoded Streamsize in byt" },
	{ "Source_StreamSize_Encoded/String", "Y NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded/String1", "N NT", "" },
	{ "Source_StreamSize_Encoded/String2", "N NT", "" },
	{ "Source_StreamSize_Encoded/String3", "N NT", "" },
	{ "Source_StreamSize_Encoded/String4", "N NT", "" },
	{ "Source_StreamSize_Encoded/String5", "N NT", "Source Encoded Streamsize in with percentage val" },
	{ "Source_StreamSize_Encoded_Proportion", "N NT", "Source Encoded Stream size divided by file si" },
	{ "Title", "Y YI", "Name of the track" },
	{ "Encoded_Library", "N YT", "Software used to create the file" },
	{ "Encoded_Library/String", "Y NT", "Software used to create the file" },
	{ "Encoded_Library/Name", "N NT", "Info from the software" },
	{ "Encoded_Library/Version", "N NT", "Version of software" },
	{ "Encoded_Library/Date", "N NT", "Release date of software" },
	{ "Encoded_Library_Settings", "Y YT", "Parameters used by the software" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn)" },
	{ "Language/String", "Y NT", "Language (full)" },
	{ "Language/String1", "N NT", "Language (full)" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empty)" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "Default", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Default/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Summary", "N NT", "" },
	{ "Encoded_Date", "Y YT", "The time that the encoding of this item was completed bega" },
	{ "Tagged_Date", "Y YT", "The time that the tags were done for this ite" },
	{ "Encryption", "Y YT", "" },
	{ "END", "END", "END" }
};


const std::string other_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Other", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Type", "Y YT", "Type" },
	{ "Format", "Y YT", "Format used" },
	{ "Format/Info", "N NT", "Info about Format" },
	{ "Format/Url", "N NT", "Link" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y NT", "Version of this format" },
	{ "Format_Profile", "Y NT", "Profile of the Format" },
	{ "Format_Compression", "Y NT", "Compression method us" },
	{ "Format_Settings", "Y NT", "Settings needed for decoder used" },
	{ "MuxingMode", "Y YT", "How this file is muxed in the container" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about this codec" },
	{ "CodecID/Hint", "Y NT", "A hint/popular name for this codec" },
	{ "CodecID/Url", "N NT", "A link to more details about this codec ID" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Duration", " ms;N YI", "Play time of the stream in ms" },
	{ "Duration/String", "Y NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if zero" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.MMM" },
	{ "Duration_Start", "Y YT", "" },
	{ "Duration_End", "Y YT", "" },
	{ "FrameRate", " fps;N YF", "Frames per second" },
	{ "FrameRate/String", "Y NT", "Frames per second (with measurement)" },
	{ "FrameCount", "N NI", "Number of frames" },
	{ "TimeStamp_FirstFrame", " ms;N YI", "TimeStamp fixed in the stream (relative) IN MS;" },
	{ "TimeStamp_FirstFrame/String", "Y NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String1", "N NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String2", "N NT", "TimeStamp with measurement;" },
	{ "TimeStamp_FirstFrame/String3", "N NT", "TimeStamp in format : HH:MM:SS.MMM;" },
	{ "TimeCode_FirstFrame", "Y YC", "Time code in HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available) format" },
	{ "TimeCode_Settings", "Y YT", "Time code settings" },
	{ "Title", "Y YI", "Name of this menu" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn)" },
	{ "Language/String", "Y NT", "Language (full)" },
	{ "Language/String1", "N NT", "Language (ful" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empt" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "END", "END", "END" }
};


const std::string image_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Image", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Title", "Y YI", "Name of the track" },
	{ "Format", "Y YT", "Format used" },
	{ "Format/Info", "Y NT", "Info about Format" },
	{ "Format/Url", "N NT", "Link" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y NT", "Version of this format" },
	{ "Format_Profile", "Y NT", "Profile of the Format" },
	{ "Format_Compression", "N YT", "Compression mode of the Format" },
	{ "Format_Settings", "Y NT", "Settings needed for decoder used" },
	{ "Format_Settings_Wrapping", "Y YT", "Wrapping mode (Frame wrapped or Clip wrapped)" },
	{ "InternetMediaType", "N YT", "Internet Media Type (aka MIME Type, Content-Type)" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about codec ID" },
	{ "CodecID/Hint", "Y NT", "A hint for this codec ID" },
	{ "CodecID/Url", "N NT", "A link for more details about this codec ID" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Codec", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/String", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Family", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Info", "N NT", "Deprecated, do not use in new projects" },
	{ "Codec/Url", "N NT", "Deprecated, do not use in new projects" },
	{ "Width", " pixel;N YI", "Width (aperture size if present) in pixel" },
	{ "Width/String", "Y NT", "Width (aperture size if present) with measurement (pixel)" },
	{ "Width_Offset", " pixel;N YI", "Offset between original width and displayed width (aperture size) in pixel" },
	{ "Width_Offset/String", "N NT", "Offset between original width and displayed width (aperture size)  in pixel" },
	{ "Width_Original", " pixel;N YI", "Original (in the raw stream) width in pixel" },
	{ "Width_Original/String", "Y NT", "Original (in the raw stream) width with measurement (pixel)" },
	{ "Height", " pixel;N YI", "Height (aperture size if present) in pixel" },
	{ "Height/String", "Y NT", "Height (aperture size if present) with measurement (pixel)" },
	{ "Height_Offset", " pixel;N YI", "Offset between original height and displayed height (aperture size) in pixel" },
	{ "Height_Offset/String", "N NT", "Offset between original height and displayed height (aperture size)  in pixel" },
	{ "Height_Original", " pixel;N YI", "Original (in the raw stream) height in pixel" },
	{ "Height_Original/String", "Y NT", "Original (in the raw stream) height with measurement (pixel)" },
	{ "PixelAspectRatio", "N YF", "Pixel Aspect ratio" },
	{ "PixelAspectRatio/String", "N NT", "Pixel Aspect ratio" },
	{ "PixelAspectRatio_Original", "N YF", "Original (in the raw stream) Pixel Aspect ratio" },
	{ "PixelAspectRatio_Original/String", "N NT", "Original (in the raw stream) Pixel Aspect ratio" },
	{ "DisplayAspectRatio", "N YF", "Display Aspect ratio" },
	{ "DisplayAspectRatio/String", "Y NT", "Display Aspect ratio" },
	{ "DisplayAspectRatio_Original", "N YF", "Original (in the raw stream) Display Aspect ratio" },
	{ "DisplayAspectRatio_Original/String", "Y NT", "Original (in the raw stream) Display Aspect ratio" },
	{ "ColorSpace", "Y YT", "" },
	{ "ChromaSubsampling", "Y YT", "" },
	{ "Resolution", " bit;N NI", "Deprecated, do not use in new projects" },
	{ "Resolution/String", "N NT", "Deprecated, do not use in new projects" },
	{ "BitDepth", " bit;N YI", "" },
	{ "BitDepth/String", "Y NT", "" },
	{ "Compression_Mode", "N YT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Mode/String", "Y NT", "Compression mode (Lossy or Lossless)" },
	{ "Compression_Ratio", "Y YF", "Current stream size divided by uncompressed stream si" },
	{ "StreamSize", " byte;N YI", "Stream size in bytes" },
	{ "StreamSize/String", "Y NT", "" },
	{ "StreamSize/String1", "N NT", "" },
	{ "StreamSize/String2", "N NT", "" },
	{ "StreamSize/String3", "N NT", "" },
	{ "StreamSize/String4", "N NT", "" },
	{ "StreamSize/String5", "N NT", "With proporti" },
	{ "StreamSize_Proportion", "N NT", "Stream size divided by file si" },
	{ "Encoded_Library", "N YT", "Software used to create the fi" },
	{ "Encoded_Library/String", "Y NT", "Software used to create the fi" },
	{ "Encoded_Library/Name", "N NT", "Info from the softwa" },
	{ "Encoded_Library/Version", "N NT", "Version of softwa" },
	{ "Encoded_Library/Date", "N NT", "Release date of softwa" },
	{ "Encoded_Library_Settings", "Y YT", "Parameters used by the softwa" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-c" },
	{ "Language/String", "Y NT", "Language (ful" },
	{ "Language/String1", "N NT", "Language (ful" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empt" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "Default", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Default/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced", "Yes;N YT", "Set if that track should be used if no language found matches the user preference." },
	{ "Forced/String", "Y NT", "Set if that track should be used if no language found matches the user preference." },
	{ "Summary", "N NT", "" },
	{ "Encoded_Date", "Y YT", "The time that the encoding of this item was completed bega" },
	{ "Tagged_Date", "Y YT", "The time that the tags were done for this ite" },
	{ "Encryption", "Y YT", "" },
	{ "colour_description_present", "N YT", "Presence of colour description" },
	{ "colour_primaries", "Y YT", "Chromaticity coordinates of the source primaries" },
	{ "transfer_characteristics", "Y YT", "Opto-electronic transfer characteristic of the source picture" },
	{ "matrix_coefficients", "Y YT", "Matrix coefficients used in deriving luma and chroma signals from the green, blue, and red primaries" },
	{ "colour_description_present_Original", "N YT", "Presence of colour description" },
	{ "colour_primaries_Original", "Y YT", "Chromaticity coordinates of the source primaries" },
	{ "transfer_characteristics_Original", "Y YT", "Opto-electronic transfer characteristic of the source picture" },
	{ "matrix_coefficients_Original", "Y YT", "Matrix coefficients used in deriving luma and chroma signals from the green, blue, and red primaries" },
	{ "END", "END", "END" }
};


const std::string menu_parameters[600][3] =
{
	{ "Count", "N NI", "Count of objects available in this stream" },
	{ "Status", "N NI", "bit field (0=IsAccepted, 1=IsFilled, 2=IsUpdated, 3=IsFinished)" },
	{ "StreamCount", "N NI", "Count of streams of that kind available" },
	{ "StreamKind;Menu", "N NT", "Stream type name" },
	{ "StreamKind/String", "N NT", "Stream type name" },
	{ "StreamKindID", "N NI", "Number of the stream (base=0)" },
	{ "StreamKindPos", "N NI", "When multiple streams, number of the stream (base=1)" },
	{ "StreamOrder", "N YI", "Stream order in the file, whatever is the kind of stream (base=0)" },
	{ "FirstPacketOrder", "N YI", "Order of the first fully decodable packet met in the file, whatever is the kind of stream (base=0)" },
	{ "Inform", "N NT", "Last **Inform** call" },
	{ "ID", "N YI", "The ID for this stream in this file" },
	{ "ID/String", "Y NT", "The ID for this stream in this file" },
	{ "UniqueID", "N YI", "The unique ID for this stream, should be copied with stream copy" },
	{ "UniqueID/String", "Y NT", "The unique ID for this stream, should be copied with stream copy" },
	{ "MenuID", "N YI", "The menu ID for this stream in this file" },
	{ "MenuID/String", "Y NT", "The menu ID for this stream in this file" },
	{ "Format", "N YT", "Format used" },
	{ "Format/Info", "N NT", "Info about Format" },
	{ "Format/Url", "N NT", "Link" },
	{ "Format_Commercial", "N NT", "Commercial name used by vendor for theses setings or Format field if there is no difference" },
	{ "Format_Commercial_IfAny", "Y YT", "Commercial name used by vendor for theses setings if there is one" },
	{ "Format_Version", "Y NT", "Version of this format" },
	{ "Format_Profile", "Y NT", "Profile of the Format" },
	{ "Format_Compression", "Y NT", "Compression method us" },
	{ "Format_Settings", "Y NT", "Settings needed for decoder used" },
	{ "CodecID", "Y YT", "Codec ID (found in some container" },
	{ "CodecID/String", "Y NT", "Codec ID (found in some container" },
	{ "CodecID/Info", "Y NT", "Info about this codec" },
	{ "CodecID/Hint", "Y NT", "A hint/popular name for this codec" },
	{ "CodecID/Url", "N NT", "A link to more details about this codec ID" },
	{ "CodecID_Description", "Y YT", "Manual description given by the container" },
	{ "Codec", "N YT", "Deprecated" },
	{ "Codec/String", "N NT", "Deprecated" },
	{ "Codec/Info", "N NT", "Deprecated" },
	{ "Codec/Url", "N NT", "Deprecated" },
	{ "Duration", " ms;N YI", "Play time of the stream in ms" },
	{ "Duration/String", "Y NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String1", "N NT", "Play time in format : HHh MMmn SSs MMMms, XX omited if zero" },
	{ "Duration/String2", "N NT", "Play time in format : XXx YYy only, YYy omited if zero" },
	{ "Duration/String3", "N NT", "Play time in format : HH:MM:SS.MMM" },
	{ "Duration_Start", "Y YT", "" },
	{ "Duration_End", "Y YT", "" },
	{ "Delay", " ms;N NI", "Delay fixed in the stream (relative) IN MS;" },
	{ "Delay/String", "N NT", "Delay with measurement;" },
	{ "Delay/String1", "N NT", "Delay with measurement;" },
	{ "Delay/String2", "N NT", "Delay with measurement;" },
	{ "Delay/String3", "N NT", "Delay in format : HH:MM:SS.MMM;" },
	{ "Delay/String4", "N NT", "Delay in format : HH:MM:SS:FF (HH:MM:SS;FF for drop frame, if available)" },
	{ "Delay_Settings", "N NT", "Delay settings (in case of timecode for example);" },
	{ "Delay_DropFrame", "N NT", "Delay drop frame;" },
	{ "Delay_Source", "N NT", "Delay source (Container or Stream or empty);" },
	{ "List_StreamKind", "N YT", "List of programs available" },
	{ "List_StreamPos", "N YT", "List of programs available" },
	{ "List", "N YT", "List of programs available" },
	{ "List/String", "Y NT", "List of programs available" },
	{ "Title", "Y YI", "Name of this menu" },
	{ "Language", "N YT", "Language (2-letter ISO 639-1 if exists, else 3-letter ISO 639-2, and with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn)" },
	{ "Language/String", "Y NT", "Language (full)" },
	{ "Language/String1", "N NT", "Language (ful" },
	{ "Language/String2", "N NT", "Language (2-letter ISO 639-1 if exists, else empt" },
	{ "Language/String3", "N NT", "Language (3-letter ISO 639-2 if exists, else empt" },
	{ "Language/String4", "N NT", "Language (2-letter ISO 639-1 if exists with optional ISO 3166-1 country separated by a dash if available, e.g. en, en-us, zh-cn, else empt" },
	{ "Language_More", "Y YT", "More info about Language (e.g. Director's Commen" },
	{ "ServiceName", "Y YT", "" },
	{ "ServiceChannel", "Y YT", "" },
	{ "Service/Url", "Y YT", "" },
	{ "ServiceProvider", "Y YT", "" },
	{ "ServiceProviderr/Url", "Y YT", "" },
	{ "ServiceType", "Y YT", "" },
	{ "NetworkName", "Y YT", "" },
	{ "Original/NetworkName", "Y YT", "" },
	{ "Countries", "Y YT", "" },
	{ "TimeZones", "Y YT", "" },
	{ "LawRating", "Y YT", "Depending on the country it's the format of the rating of a movie (P, R, X in the USA, an age in other countries or a URI defining a logo" },
	{ "LawRating_Reason", "Y YT", "Reason for the law rati" },
	{ "Chapters_Pos_Begin", "N NI", "Used by third-party developers to know about the beginning of the chapters list, to be used by Get(Stream_Menu, x, Pos), where Pos is an Integer between Chapters_Pos_Begin and Chapters_Pos_E" },
	{ "Chapters_Pos_End", "N NI", "Used by third-party developers to know about the end of the chapters list (this position excluded)" },
	{ "END", "END", "END" }
};



void list_general_parameters()
{
	std::cout << "General Parameters: " << std::endl << std::endl;
	int i = 0;
	while (general_parameters[i][0].compare("END"))
	{
		std::cout << general_parameters[i][0] << "  " << general_parameters[i][1] << "  " << general_parameters[i][2] << std::endl;
		i++;
	}
}


void list_audio_parameters()
{
	std::cout << "Audio Parameters: " << std::endl << std::endl;
	int i = 0;
	while (audio_parameters[i][0].compare("END"))
	{
		std::cout << audio_parameters[i][0] << "  " << audio_parameters[i][1] << "  " << audio_parameters[i][2] << std::endl;
		i++;
	}
}


void list_video_parameters()
{
	std::cout << "Video Parameters: " << std::endl << std::endl;
	int i = 0;
	while (video_parameters[i][0].compare("END"))
	{
		std::cout << video_parameters[i][0] << " | " << video_parameters[i][1] << " | " << video_parameters[i][2] << std::endl;
		i++;
	}
}


void list_text_parameters()
{
	std::cout << "Text Parameters: " << std::endl << std::endl;
	int i = 0;
	while (text_parameters[i][0].compare("END"))
	{
		std::cout << text_parameters[i][0] << "  " << text_parameters[i][1] << "  " << text_parameters[i][2] << std::endl;
		i++;
	}
}


void list_other_parameters()
{
	std::cout << "Other Parameters: " << std::endl << std::endl;
	int i = 0;
	while (other_parameters[i][0].compare("END"))
	{
		std::cout << other_parameters[i][0] << "  " << other_parameters[i][1] << "  " << other_parameters[i][2] << std::endl;
		i++;
	}
}


void list_image_parameters()
{
	std::cout << "Image Parameters: " << std::endl << std::endl;
	int i = 0;
	while (image_parameters[i][0].compare("END"))
	{
		std::cout << image_parameters[i][0] << "  " << image_parameters[i][1] << "  " << image_parameters[i][2] << std::endl;
		i++;
	}
}


void list_menu_parameters()
{
	std::cout << "Menu Parameters: " << std::endl << std::endl;
	int i = 0;
	while (menu_parameters[i][0].compare("END"))
	{
		std::cout << menu_parameters[i][0] << "  " << menu_parameters[i][1] << "  " << menu_parameters[i][2] << std::endl;
		i++;
	}
}


int action_lookup(std::string action)
{
	int found = 0;
	int i = 0;

	while (action_types[i].compare("END"))
	{
		if (action_types[i].compare(action) == 0)
		{
			found = 1;
			break;
		}
		i++;
	}
	return found;
}


int stream_lookup(std::string stream)
{
	int found = 0;
	int i = 0;

	while (stream_types[i].compare("END"))
	{
		if (stream_types[i].compare(stream) == 0)
		{
			found = 1;
			break;
		}
		i++;
	}
	return found;
}


int parameter_lookup(std::string type, std::string parameter)
{
	int parameter_found = 0;
	int stream_found = 0;
	int i = 0;

	while (stream_types[i].compare("END"))
	{
		if (stream_types[i].compare(type) == 0)
		{
			stream_found = 1;
			break;
		}
		i++;
	}

	if (stream_found)
	{
		if (type.compare("video") == 0)
		{
			while (video_parameters[i][0].compare("END"))
			{
				if (video_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("audio") == 0)
		{
			while (audio_parameters[i][0].compare("END"))
			{
				if (audio_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("general") == 0)
		{
			while (general_parameters[i][0].compare("END"))
			{
				if (general_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("text") == 0)
		{
			while (text_parameters[i][0].compare("END"))
			{
				if (text_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("other") == 0)
		{
			while (other_parameters[i][0].compare("END"))
			{
				if (other_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("image") == 0)
		{
			while (image_parameters[i][0].compare("END"))
			{
				if (image_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}

		if (type.compare("menu") == 0)
		{
			while (menu_parameters[i][0].compare("END"))
			{
				if (menu_parameters[i][0].compare(parameter) == 0)
				{
					parameter_found = 1;
					break;
				}
				i++;
			}
		}
	}

	return parameter_found;
}