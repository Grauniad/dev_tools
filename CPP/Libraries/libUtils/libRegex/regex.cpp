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

std::string RegPattern::Group(const std::string& haystack, size_t n) const {
    boost::regex& patternRegex = *( reinterpret_cast<boost::regex *>(pattern));
    boost::smatch what;

    std::string match = "";
    if ( boost::regex_search(haystack, what, patternRegex )) {
        if ( n < what.size() ) {
            return what[n];
        }
    }

    return match;
}

std::string RegPattern::Replace(const std::string& haystack, 
                         const std::string& newNeedle) const
{
    boost::regex& patternRegex = *( reinterpret_cast<boost::regex *>(pattern));
    return boost::regex_replace(haystack,patternRegex,
                                         newNeedle,
                                         boost::match_default | boost::format_first_only);
}
