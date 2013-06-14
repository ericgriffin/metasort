rm -rf ./stage/lib/
./bjam link=static threading=multi runtime-link=static release stage
./bjam link=static threading=multi runtime-link=static debug stage