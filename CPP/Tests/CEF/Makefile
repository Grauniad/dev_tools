LINKED_LIBS= libCEF \
             libIOInterface\
             libUtils  \
			 libTest

BUILD_TIME_TESTS=jsObject
CPP_TAGS_FILE=dev_tools_cpp_tests_cef-c++.tags

USE_CEF=YES
USE_GTK=YES

TEST_LINK_CEF_RESOURCES=YES

.TEST_SETUP: test_sym_link

#
# CEF does not seem to support valgrind (even the sample apps explode with too
# many errors).
# 
# Until we find a better solution, supress valgrind on this folder, so we can
# still memory test the rest of our code
NO_MEMTEST=YES

NO_TRAVIS=YES

include ../makefile_tests.include
