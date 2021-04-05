#!/bin/sh

sudo apt-get install libgtest-dev cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
ls
sudo cp *.a /usr/lib