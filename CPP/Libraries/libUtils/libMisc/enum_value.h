#ifndef DEV_TOOLS_CPP_LIBRARIES_UTILS_MISC_ENUM_MAPH__
#define DEV_TOOLS_CPP_LIBRARIES_UTILS_MISC_ENUM_MAPH__
#include <unordered_map>
#include <initializer_list>
#include <map>

template<typename ENUM>
struct EnumStringMaps {
    typedef const std::map<std::string,ENUM> StringMap;
    typedef const std::map<ENUM,std::string> EnumMap;

    typedef std::initializer_list<typename StringMap::value_type> Initializer;

    EnumStringMaps(Initializer list) 
       : toEnum(list)
    {
        std::map<ENUM,std::string>& mutableToString = 
            const_cast<std::map<ENUM,std::string>&>(toString);

        for (auto& p: list) {
            mutableToString[p.second] = p.first;
        }
    }

    StringMap toEnum;
    EnumMap   toString;
};

/**
 * Light weight extension to an enum providing value initialization and string
 * conversiion. 
 *
 * No additional storage is consumed, the only additional overhead is the
 * default value initialization. Copy etc works as expected for a scalar value.
 */
template <typename ENUM, ENUM error_value>
struct EnumValue {
    /***********************************************
     *                 Types
     **********************************************/
    typedef ENUM Value;
    typedef EnumStringMaps<Value> Map;

    /***********************************************
     *              Reference Data
     **********************************************/
    static const std::string NO_SUCH_VALUE;
    static EnumStringMaps<Value> string_map;

    /***********************************************
     *                State Data
     **********************************************/
    Value value;

    /***********************************************
     *               Construction
     **********************************************/
    inline EnumValue() : value(error_value) {}
    inline EnumValue(const ENUM& val) : value(val) {}

    EnumValue(const std::string& val) {
        SetValue(val);
    }

    /***********************************************
     *                Raw Access
     **********************************************/
     inline operator const Value&() const { return value; }
     inline operator Value&() { return value; }

    /***********************************************
     *              Status
     **********************************************/
     bool IsOk () const { 
         bool ok = (value != error_value);

         if ( ok ) {
             auto it = string_map.toString.find(value);
             ok = (it != string_map.toString.end());
         }

         return ok;
     }

    /***********************************************
     *              String Utilities
     **********************************************/

    const std::string& StringValue() const {
        auto it = string_map.toString.find(value);

        if ( it != string_map.toString.end() ) {
            return it->second;
        } else {
            return NO_SUCH_VALUE;
        }
    }

    const Value& SetValue(const std::string& newValue) {
        auto it = string_map.toEnum.find(newValue);

        if (it != string_map.toEnum.end() ) {
            value = it->second;
        } else {
            value = error_value;
        }

        return value;

    }

};
template <typename ENUM, ENUM default_value>
const std::string EnumValue<ENUM,default_value>::NO_SUCH_VALUE = "";

#endif
