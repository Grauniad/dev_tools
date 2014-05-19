#include "regex.h"
#include <boost/regex.hpp>

using namespace std;
RegPattern::RegPattern (const string& needle)
    : pattern(NULL)
{
    try {
        pattern = reinterpret_cast<void *>(new boost::regex(needle));
    } catch ( boost::regex_error& e ) {
        // Re throw as something not stored in boost...
        throw RegError(e.what());
    }
}

RegPattern::~RegPattern() {
    delete reinterpret_cast<boost::regex *>(pattern);
}

bool RegPattern::Search(const std::string& haystack) const {
    boost::regex& patternRegex = *( reinterpret_cast<boost::regex *>(pattern));
    return boost::regex_search(haystack,patternRegex);
}
