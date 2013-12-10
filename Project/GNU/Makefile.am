AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
-DUNICODE -D_CRT_SECURE_NO_WARNINGS -DNDEBUG -D_CONSOLE -O3 \
-I../../Libraries/boost \
-I../../Libraries/MediaInfoLib/Source \
-I../../Libraries/ZenLib/Source \
-I../../Libraries/zlib


bin_PROGRAMS = metasort

metasort_SOURCES = \
			../../source/asset.cpp \
			../../source/customparameters.cpp \
			../../source/logfile.cpp \
			../../source/util_functions.cpp \
			../../source/rule_actions.cpp \
			../../source/metasorter.cpp \
			../../source/main.cpp

metasort_LDFLAGS = \
			-L../../Libraries/boost/stage/lib \
			-L../../Libraries/MediaInfoLib/Project/GNU/Library/.libs \
			-L../../Libraries/ZenLib/Project/GNU/Library/.libs \
			-L../../Libraries/zlib

			
			
