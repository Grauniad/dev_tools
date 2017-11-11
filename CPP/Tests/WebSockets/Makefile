LINKED_LIBS= libWebSockets \
             libJSON\
             libThreadComms\
             libString\
             libIOInterface\
             libUtils\
             libTest

BUILD_TIME_TESTS=requestReply
CPP_TAGS_FILE=dev_tools_cpp_tests_websockets--c++.tags
MODE=CPP

USE_BOOST=YES
USE_THREADS=YES
USE_GTEST=YES
USE_WEB_SOCKET_CPP=YES
USE_JSON=YES
USE_FPIC=NO
BOOST_MODULES+=regex
EXTERNAL_LIBS=-lssl -lcrypto

include ../makefile_tests.include
