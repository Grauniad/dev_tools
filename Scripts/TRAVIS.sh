#!/bin/bash

#
# Make sure DEV_TOOLS's submodules have been intialised
#
pushd DEV_TOOLS/

git submodule init
git submodule update

popd


#
# Install GNU c++11 compiler
#
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install -qq g++-4.8;
sudo apt-get install -qq gfortran-4.8

# Build with the TRAVIS tool chain
TOOL_CHAIN=TRAVIS PROJECT_ROOT_DIR=$PWD make $*
