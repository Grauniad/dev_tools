#ifndef __ENV_HEADER__
#define __ENV_HEADER__

#include <string>
#include <sstream>

using namespace std;
/*
 * Class to handle environmental variables
 *
 *
 */

class ENV {
public:
    // Return the value of the env. variable, or default
    static string GetEnvString(const string& name, const string default_value="");

    static bool   IsSet(const string& name);

    static bool   IsInList(const string& name, const string& item);

    template <class T>
    static T GetEnvValue( const std::string& name,
                                    T defaultValue) {
        std::stringstream sval(GetEnvString(name,""));
        if ( sval.str() != "" ) {
            T val;
            sval >> val;
            return val;
        } else {
            return defaultValue;
        }
    }
private:
    /* data */
};

#endif
