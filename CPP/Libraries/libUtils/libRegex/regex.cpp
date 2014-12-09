#include "regex.h"
#include <regex>

using namespace std;
RegPattern::RegPattern (const string& needle)
    : pattern(NULL)
{
    try {
        pattern = reinterpret_cast<void *>(new regex(needle));
    } catch ( regex_error& e ) {
        // Re throw as something not stored in boost...
        throw RegError(e.what());
    }
}

RegPattern::~RegPattern() {
    delete reinterpret_cast<regex *>(pattern);
}

bool RegPattern::Search(const std::string& haystack) const {
    regex& patternRegex = *( reinterpret_cast<regex *>(pattern));
    return regex_search(haystack,patternRegex);
}

std::string RegPattern::Group(const std::string& haystack, size_t n) const {
    regex& patternRegex = *( reinterpret_cast<regex *>(pattern));
    smatch what;

    std::string match = "";
    if ( regex_search(haystack, what, patternRegex )) {
        if ( n < what.size() ) {
            return what[n];
        }
    }

    return match;
}

std::string RegPattern::Replace(const std::string& haystack, 
                         const std::string& newNeedle) const
{
    regex& patternRegex = *( reinterpret_cast<regex *>(pattern));
    return regex_replace(haystack,patternRegex,
                                         newNeedle,
                                         regex_constants::match_default | regex_constants::format_first_only);
}
