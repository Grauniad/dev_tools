LINKED_LIBS= libCEF \
             libJSON \
             libIOInterface\
             libUtils  \
			 libTest

BUILD_TIME_TESTS=jsTests
CPP_TAGS_FILE=dev_tools_cpp_tests_cef-c++.tags

USE_CEF=YES
USE_GTK=YES
USE_JSON=YES

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

MODE=CPP

include ../makefile_tests.include
