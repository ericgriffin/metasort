AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
-DUNICODE -D_CRT_SECURE_NO_WARNINGS -DNDEBUG -D_CONSOLE -O3 \
-I../../lib/boost \
-I../../lib/MediaInfoLib/Source \
-I../../lib/ZenLib/Source \
-I../../lib/zlib


bin_PROGRAMS = metasort

metasort_SOURCES = \
			../../source/usage.cpp \
			../../source/asset.cpp \
			../../source/metasortconfig.cpp \
			../../source/customparameters.cpp \
			../../source/logfile.cpp \
			../../source/metasortutil.cpp \
			../../source/actions.cpp \
			../../source/md5.cpp \
			../../source/metasorter.cpp \
			../../source/main.cpp

metasort_LDFLAGS = \
			-L../../lib/boost/stage/lib \
			-L../../lib/MediaInfoLib/Project/GNU/Library/.libs \
			-L../../lib/ZenLib/Project/GNU/Library/.libs \
			-L../../lib/zlib

			
			
