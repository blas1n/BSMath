#!/bin/sh

sudo apt-get install libgtest-dev cmake
cd /usr/src/gtest
ls
sudo cmake CMakeLists.txt
ls
sudo make
sudo cp ./lib/*.a /usr/lib