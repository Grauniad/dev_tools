#include "search.h"
#include "logger.h"
using namespace std;

Searcher::Searcher(const vector<string>& source) 
    : strings(source), pattern(nullptr)
{
}

Searcher::~Searcher() {
    delete pattern;
}

bool Searcher::Search(const std::string& stringPattern) {
    bool match = false;
    Reset();
    delete pattern;

    try {
        pattern = new RegPattern(stringPattern);
        for ( IT it = strings.begin(); it < strings.end(); ++it) {
            const string& s = *it;

            if ( pattern->Search(s) ) {
                matches.insert(it);
                match = true;
            }
        }
    } catch (RegError& e ) {
        match = false;
        pattern = nullptr;
    }
    return match;
}

Searcher::IT Searcher::First() {
    auto it = matches.begin();
    if ( it != matches.end() ) {
        return *it;
    } else {
        return NoPos();
    }
}

Searcher::IT Searcher::Last() {
    if ( matches.size() > 0 ) {
        return *(--(matches.end()));
    } else {
        return NoPos();
    }
}

Searcher::IT Searcher::Next(const IT& start) {
    auto startIt = matches.upper_bound(start);
    if ( startIt == matches.end() ) {
        return NoPos();
    } else {
        return *startIt;
    }
}

Searcher::IT Searcher::Prev(const IT& start) {
    auto startIt = matches.lower_bound(start);
    /*
     * lower_bound returns the first element
     * that does *not* go before start.
     */
    if ( startIt == matches.end() ) {
        // startIt must be past the end...
        if ( matches.size() > 0 ) {
            return *(--startIt);
        } else {
            return NoPos();
        }
    } else {
        if ( startIt == matches.begin() ) {
            // This was the first match, no more to find
            return NoPos();
        } else {
            return *(--startIt);
        }
    }
    /*
     * Can't get here
     */
    LOG_FROM(LOG_ERROR,"Searcher::Prevc", "Searcher::Prev has reached a corrupted state")
    return NoPos();
}

Searcher::IT Searcher::NoPos() const { 
    return strings.cend(); 
}

bool Searcher::IsMatch(const IT& totest) {
    bool match = false;
    if ( matches.find(totest) != matches.end() ) {
        match = true;
    }
    return match;
}

void Searcher::Reset() {
    matches.clear();
    delete pattern;
    pattern = nullptr;
}

bool Searcher::UpdateCache() {
    bool match = false;
    matches.clear();
    if ( pattern ) {
        for ( IT it = strings.begin(); it < strings.end(); ++it) {
            const string& s = *it;
            if ( pattern->Search(s) ) {
                matches.insert(it);
                match = true;
            }
        }
    }
    return match;
}
