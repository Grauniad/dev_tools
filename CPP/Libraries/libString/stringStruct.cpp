#include "stringStruct.h"
#include "tokenizor.h"
#include <cstdlib>

using namespace std;

StringStructFactory::StringStructFactory(const string& definitions) 
    : elements(Parts(definitions).Vector()), 
      size(elements.size())
{
    for (size_t i = 0; i<size; ++i ) {
        elMap[elements[i]] = i;
    }
}

size_t StringStructFactory::GetIdx(const string& el) {
    auto it = elMap.find(el);
    if ( it != elMap.end() ) {
        return it->second;
    } else {
        return 0;
    }
}

StringStruct StringStructFactory::New(const char* from) const {
    StringStruct s(size);
    for (size_t i = 0; i< size; ++i ) {
        if ( *from == '\0') {
            for ( size_t j = i; j< size; ++j) {
                s[j] = 0;
            }
            break;
        }
        // strtol returns 0 for invalid ints
        // Because c programers can't do const correctly, *sigh*
        s[i] = strtol(from,const_cast<char **>(&from),10);
    }
    return s;
}
