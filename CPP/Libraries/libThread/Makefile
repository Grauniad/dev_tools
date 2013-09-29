EXPORT_INCLUDES=$(shell echo *.h *.hpp)
SOURCES=$(shell echo *.cpp)

TARGET_LIB=libThread

# For ENVs
ADDITIONAL_INCLUDE_FLAGS+=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libOS/

# For timestamps
ADDITIONAL_INCLUDE_FLAGS+=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libTime/ 

# For Logger
ADDITIONAL_INCLUDE_FLAGS+=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libLog $(DEV_TOOLS_DIR)/CPP/Libraries/libIOInterface 

MODE=CPP
TAGS_FILE=utilities_thread_c++.tags


include ../../../../makefile.include
