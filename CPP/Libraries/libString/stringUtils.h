#ifndef DEV_TOOLS_LIBRARIES_LIBSTRING_UTILS_H__
#define DEV_TOOLS_LIBRARIES_LIBSTRING_UTILS_H__
#include <string>

/*
 * Use the input string as tokens to populate the skeleton.
 *
 * Supported args: 
 *   ${#}:  The #th token from input
 *   $(#*}: The #th tokens, + all other (space separated) tokens
 *   $(#@}: The #th tokens, + all other (quoted) tokens
 *
 * Eg: Substitute("${2} ${1} ${3*}", "one two three \"four five \" six")
 *      => "two one three four five six"
 *
 * Eg: Substitute("${2} ${1} ${3@}", "one two three \"four five \" six")
 *      => "two one \"three\" \"four five \" \"six\""
 */
namespace StringUtils {
    std::string Substitute( const std::string& skeleton, 
                            const std::string& input);

    void Trim(std::string& toTrim);
}
#endif
