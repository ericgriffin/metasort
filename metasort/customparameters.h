#include <sys/types.h>
#include <sys/stat.h>
#include <boost/filesystem/operations.hpp>

#include "metasorter.h"
#include "asset.h"


int audio_layout(MediaInfoLib::String&, MediaInfo&);
int file_size(MediaInfoLib::String&, asset*);
int file_modified_age(MediaInfoLib::String&, asset*);
int file_created_age(MediaInfoLib::String&, asset*);