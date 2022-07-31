git submodule update --init --recursive
rm -rf build
mkdir build
cd build
cmake -G "Eclipse CDT4 - Unix Makefiles" ..
make
cd ..
