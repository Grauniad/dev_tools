#ifndef DEV_TOOLS_LIBRARIES_LIBSTRING_STRING_STRUCT_H__
#define DEV_TOOLS_LIBRARIES_LIBSTRING_STRING_STRUCT_H__
#include <string>
#include <vector>
#include <map>

class StringStruct;
class StringStructFactory {
public:
    StringStructFactory(const std::string& definitions);

    size_t NumElements() const {
        return size;
    }

    size_t GetIdx(const std::string& el);

    StringStruct New() const;

    StringStruct New(const char* input) const;

private:
    std::vector<std::string>  elements;
    std::map<std::string,size_t> elMap;
    size_t size;
};

class StringStruct {
public:
    StringStruct(size_t size)
       : array(new int[size]) { } 

    StringStruct(StringStruct&& moveFrom)
       : array(moveFrom.array) 
    { 
        moveFrom.array = nullptr;
    }

    ~StringStruct() { delete[] array; } 

    int& operator[](const size_t& idx) {
        return array[idx];
    }

    // Prevent copy constructing: there has to be a clear owner of the
    // heap memory
    StringStruct(const StringStruct& copy) = delete;
    StringStruct& operator=(const StringStruct& copy) = delete;
private:
    int* array;
};

inline StringStruct StringStructFactory::New() const {
    StringStruct s(NumElements());
    for (size_t i = 0; i< size; ++i ) {
        s[i] = 0;
    }
    return s;
}
#endif
