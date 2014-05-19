#ifndef DEVTOOLS_UTILS_MISC_REGEX_H__
#define DEVTOOLS_UTILS_MISC_REGEX_H__
#include <string>

/*
 * A temporary utility (if you're reading this, delete it)
 * to handle stupid OS's which have decreped boost instations which can't
 * handle c++11
 *
 * We are doing nasty hacks so that this can be compiled under c++98 and
 * boost::regex doesn't leak out to the rest of the code base.
 *
 * I assure you I hate this as much as you do.  comewhat g++4.9 this will go
 * away and we can use the std regex library
 */

class RegError {
public:
    RegError(const std::string& _text) 
        : text(_text) { }
    std::string what() { return text;}
private:
    std::string text;
};

class RegPattern {
public:
    // This can throw RegError
    RegPattern(const std::string& needle);
    ~RegPattern();
    
    bool Search(const std::string& haystack) const;
private:
    void* pattern;
    
};

#endif
