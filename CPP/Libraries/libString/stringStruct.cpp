#include "stringStruct.h"
#include "tokenizor.h"
#include <cstdlib>
#include <limits>

using namespace std;

const string StringStructFactory::NO_SUCH_ELMENT = "";
const size_t StringStructFactory::NO_SUCH_INDEX = 
                 numeric_limits<size_t>::max();

StringStructFactory::StringStructFactory(const string& definitions) 
    : elements(Parts(definitions).Vector()), 
      size(elements.size())
{
    for (size_t i = 0; i<size; ++i ) {
        elMap[elements[i]] = i;
    }
}

vector<size_t> StringStructFactory::GetIdxs(const std::string& els) {
    vector<size_t> list;
    Parts elList(els);
    for (const string& el:elList.Vector()) {
        list.push_back(GetIdx(el));
    }
    return list;
}

size_t StringStructFactory::GetIdx(const string& el) {
    auto it = elMap.find(el);
    if ( it != elMap.end() ) {
        return it->second;
    } else {
        return NO_SUCH_INDEX;
    }
}
const std::string& StringStructFactory::GetName(const size_t& idx) {
    if ( idx < elements.size() ) {
        return elements[idx];
    } else {
        return NO_SUCH_ELMENT;
    }
}

StringStruct StringStructFactory::New(const char* from) const {
    StringStruct s(size);
    s.Reset(from);
    return s;
}

void StringStruct::Reset(const char* from) {
    for (size_t i = 0; i< size; ++i ) {
        if ( *from == '\0') {
            for ( size_t j = i; j< size; ++j) {
                array[j] = 0;
            }
            break;
        }
        // strtol returns 0 for invalid ints
        // Because c programers can't do const correctly, *sigh*
        array[i] = strtol(from,const_cast<char **>(&from),10);
    }
}

StringStruct& StringStruct::operator+=(const StringStruct& rhs) {
    for (size_t i = 0; i< size; ++i ) {
        array[i] += rhs.array[i];
    }
    return *this;
}

StringStruct& StringStruct::operator-=(const StringStruct& rhs) {
    for (size_t i = 0; i< size; ++i ) {
        array[i] -= rhs.array[i];
    }
    return *this;
}
