#!/bin/bash
cd ./zlib/
./configure
make
make install
cd ./../
cd ./ZenLib/Project/GNU/Library/
./autogen
./configure
make
make install
cd ./../../../../
cd ./MediaInfoLib/Project/GNU/Library/
./autogen
./configure
make
make install
cd ./../../../../
cd ./boost/
(exec ./bootstrap.sh)
(exec ./build_boost64.sh)

