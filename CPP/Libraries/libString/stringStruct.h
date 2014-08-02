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
    const std::string& GetName(const size_t& idx);

    StringStruct New() const;

    StringStruct New(const char* input) const;

    static const std::string NO_SUCH_ELMENT;

private:
    std::vector<std::string>  elements;
    std::map<std::string,size_t> elMap;
    size_t size;
};

class StringStruct {
public:
    StringStruct(size_t _size)
       : array(new long[_size]), size(_size) { } 

    StringStruct(StringStruct&& moveFrom)
       : array(moveFrom.array) , size(moveFrom.size)
    { 
        moveFrom.array = nullptr;
    }

    ~StringStruct() { delete[] array; } 

    long& operator[](const size_t& idx) {
        return array[idx];
    }

    const long& operator[](const size_t& idx) const {
        return array[idx];
    }

    /*
     * The caller has responsibility for proving the structs are of the same
     * size...
     */
    StringStruct& operator+=(const StringStruct& rhs);
    StringStruct& operator-=(const StringStruct& rhs);

    // Prevent copy constructing: there has to be a clear owner of the
    // heap memory
    StringStruct(const StringStruct& copy) = delete;
    StringStruct& operator=(const StringStruct& copy) = delete;

    size_t Size() { return size; } 
private:
    long* array;
    size_t size;
};

inline StringStruct StringStructFactory::New() const {
    StringStruct s(NumElements());
    for (size_t i = 0; i< size; ++i ) {
        s[i] = 0;
    }
    return s;
}
#endif
