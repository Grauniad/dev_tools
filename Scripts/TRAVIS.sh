#!/bin/bash

# Build with the TRAVIS tool chain
TOOL_CHAIN=TRAVIS PROJECT_ROOT_DIR=$PWD make $*
