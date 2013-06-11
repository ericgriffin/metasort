#include <sys/types.h>
#include <sys/stat.h>
#include <boost/filesystem/operations.hpp>

#include "metasorter.h"
#include "asset.h"


int proc_audio_layout(MediaInfoLib::String &, MediaInfo &);
int proc_file_name(MediaInfoLib::String &, asset *);
int proc_file_extension(MediaInfoLib::String &, asset *);
int proc_file_path(MediaInfoLib::String &, asset *);
int proc_file_size(MediaInfoLib::String &, asset *);
int proc_file_modified_age(MediaInfoLib::String &, asset *);
int proc_file_created_age(MediaInfoLib::String&, asset *);
int proc_file_modified_date(MediaInfoLib::String &, asset *);
int proc_file_created_date(MediaInfoLib::String&, asset *);