rm -rf ./stage/lib/
./bjam link=static threading=multi address-model=64 runtime-link=static release stage
./bjam link=static threading=multi address-model=64 runtime-link=static debug stage