EXPORT_INCLUDES=$(shell echo *.h)
SOURCES=$(shell echo *.cpp)

# For ENVs
ADDITIONAL_INCLUDE_FLAGS+=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libOS/

# For timestamps
ADDITIONAL_INCLUDE_FLAGS+=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libTime/ $(DEV_TOOLS_DIR)/CPP/Libraries/libThread/

MODE=CPP
TAGS_FILE=utilities_log_c++.tags


include ../makefile.libUtils.include
