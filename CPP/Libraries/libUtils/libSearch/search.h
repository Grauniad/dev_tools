#ifndef DEVTOOLS_UTILS_MISC_SEARCH_H__
#define DEVTOOLS_UTILS_MISC_SEARCH_H__
#include <string>
#include <vector>
#include <set>
#include "regex_utils.h"

class Searcher {
public:
    using IT = std::vector<std::string>::const_iterator;
    Searcher(const std::vector<std::string>& source );

    virtual ~Searcher();

    bool Search(const std::string& pattern);

    bool IsMatch(const IT& toTest);

    void Reset();

    /*
     * re-compute the cache between these bounds
     */
    bool UpdateCache();

    /*
     * Get results
     */
    IT First(); 
    IT Next(const IT& start); 
    IT Prev(const IT& start); 
    IT Last(); 

    /*
     * No such result
     */
    IT NoPos() const;

private:
    const std::vector<std::string>&  strings;
    std::set<IT> matches;
    RegPattern*  pattern;
};

#endif
