#!/bin/bash
if [ -z build ]; then
  rm -rf build
fi  
mkdir build
cd build
cmake ../
make -j
