#!/usr/bin/env bash

rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
conan install .. --settings os=Macos -g virtualenv
source activate.sh && ./bin/eigen-benchmarks && source deactivate.sh