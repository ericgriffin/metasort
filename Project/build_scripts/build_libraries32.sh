#!/bin/bash
#build zlib
cd ../../Libraries/
cd ./zlib/
./configure
make
make install
#build Zenlib
cd ./../
cd ./ZenLib/Project/GNU/Library/
./autogen
./configure
make
make install
#build MediaInfo
cd ./../../../../
cd ./MediaInfoLib/Project/GNU/Library/
./autogen
./configure
make
make install
#build boost
cd ./../../../../
cd ./boost/
(exec ./bootstrap.sh)
rm -rf ./stage/lib/
./bjam link=static threading=multi runtime-link=static release stage
./bjam link=static threading=multi runtime-link=static debug stage
