#ifndef DEVTOOLS_UTILS_MISC_SEARCH_H__
#define DEVTOOLS_UTILS_MISC_SEARCH_H__
#include <string>
#include <vector>
#include <set>

class Searcher {
public:
    using IT = std::vector<std::string>::const_iterator;
    Searcher(const std::vector<std::string>& source );

    bool Search(const std::string& pattern);

    bool IsMatch(const IT& toTest);

    void Reset();

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
};

#endif
