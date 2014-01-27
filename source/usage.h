/*  usage.h
 *  Copyright (c) Eric Griffin
 *
 *  For conditions of distribution and use, see the
 *  LICENSE file in the root of the source tree.
 */

#ifndef _USAGE_H_
#define _USAGE_H_ 1

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <boost/filesystem.hpp>

void usage();
void generate_skeleton_config();
void list_actions();
void list_streams();
void list_general_parameters();
void list_audio_parameters();
void list_video_parameters();
void list_text_parameters();
void list_other_parameters();
void list_image_parameters();
void list_menu_parameters();

#endif // _USAGE_H_