git submodule update --init --recursive
mkdir build
cd build
cmake -G "Eclipse CDT4 - Unix Makefiles" ..
make
cd ..
