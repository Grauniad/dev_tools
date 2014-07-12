EXPORT_INCLUDES=$(shell echo *.h *.hpp)
SOURCES=$(shell echo *.cpp)

TARGET_LIB=libIOInterface
TAGS_FILE=libIOInterface-c++.tags

MODE=CPP

LINKED_LIBS=libUtils libString

# We need the utils headers
ADDITIONAL_INCLUDE_FLAGS=$(DEV_TOOLS_DIR)/CPP/Libraries/libUtils/libLog/ $(DEV_TOOLS_DIR)/CPP/Libraries/libString/

include ../makefiles_libaries.include
