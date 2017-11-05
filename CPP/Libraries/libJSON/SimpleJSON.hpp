/*
 * SimpleJSON.hpp
 *
 *  Created on: 1 Apr 2015
 *      Author: lhumphreys
 */

#ifndef SIMPLEJSON_HPP_
#define SIMPLEJSON_HPP_

#include <limits>
#include <type_traits>
#include <rapidjson/error/en.h>
#include <util_time.h>

/*****************************************************************************
 *                          JSON Builder
 *****************************************************************************/

template <class WRTIER>
SimpleJSONBuilderBase<WRTIER>::SimpleJSONBuilderBase() 
{
    writer.StartObject();
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Clear() {
    writer.ResetAndClear();

    writer.StartObject();
}

template <class WRTIER>
std::string SimpleJSONBuilderBase<WRTIER>::GetAndClear() {
    writer.EndObject();

    std::string result = writer.GetString();

    Clear();

    return result;
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::AddName(const std::string& name) {
    writer.String(name.c_str());
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::AddNullField(const std::string& name) {
    AddName(name);
    writer.Null();
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const std::string& value) {
    writer.String(value.c_str());
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const int& value) {
    writer.Int(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const int64_t& value) {
    writer.Int64(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const unsigned & value) {
    writer.Uint(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const uint64_t& value) {
    writer.Uint64(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const double& value) {
    writer.Double(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::Add(const bool& value) {
    writer.Bool(value);
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::StartArray(const std::string& name) {
    writer.String(name.c_str());
    writer.StartArray();
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::EndArray() {
    writer.EndArray();
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::StartAnonymousObject() {
    writer.StartObject();
}

template <class WRTIER>
void SimpleJSONBuilderBase<WRTIER>::EndObject() {
    writer.EndObject();
}

/*****************************************************************************
 *                          Base Array Field
 *****************************************************************************/
template <typename TYPE>
FieldArrayBase<TYPE>::FieldArrayBase() {
    FieldArrayBase::Clear();
}

template <typename TYPE>
void FieldArrayBase<TYPE>::Clear() {
    FieldBase::Clear();
    value.clear();
    inArray = false;
}

template <typename TYPE>
bool FieldArrayBase<TYPE>::StartArray() {
    if(inArray) {
        throw spJSON::WrongTypeError{this->Name()};
    } else {
        inArray = true;
    }
    return true;
}

template <typename TYPE>
bool FieldArrayBase<TYPE>::EndArray(rapidjson::SizeType elementCount)
{
    if(inArray) {
        inArray = false;
    } else {
        throw spJSON::ParseError();
    }
    return true;
}


#endif /* SIMPLEJSON_HPP_ */

/*****************************************************************************
 *                          Field Type Definitions
 *****************************************************************************/

struct StringField: public FieldBase {
    typedef std::string ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = "";
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        value.assign(str,length);
        return true;
    }
};

struct TimeField: public FieldBase {
    typedef Time ValueType;
    Time value;

    TimeField(): value("") { } 

    virtual void Clear() {
        FieldBase::Clear();
        value.InitialiseFromString("",0);
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        value.InitialiseFromString(str,length);
        return true;
    }

    template <class Builder>
    void AddToJSON(Builder& builder, bool nullIfNotSupplied) {
        if ( supplied || !nullIfNotSupplied )
        {
            builder.Add(Name(),value.ISO8601Timestamp());
        }
        else
        {
            builder.AddNullField(Name());
        }
    }
};

struct StringArrayField: public FieldArrayBase<std::string> {

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        if (inArray) {
            value.emplace_back(str,length);
        } else {
            throw spJSON::WrongTypeError{Name()};
        }
        return true;
    }
};

struct TimeArrayField: public FieldArrayBase<Time> {

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        if (inArray) {
            value.emplace_back(str);
        } else {
            throw spJSON::WrongTypeError{Name()};
        }
        return true;
    }

    template <class Builder>
    void AddToJSON(Builder& builder, bool nullIfNotSupplied) {
        if ( supplied || !nullIfNotSupplied )
        {
        	std::vector<std::string> stringValues;
        	stringValues.reserve(value.size());
            for (Time& t: value) {
            	stringValues.emplace_back(std::move(t.ISO8601Timestamp()));
            }
            builder.Add(Name(), stringValues);
        }
        else
        {
            builder.AddNullField(Name());
        }

    }
};

struct IntField: public FieldBase {
    typedef int ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = 0;
    }

    bool Int(int i) {
        value = i;
        return true;
    }

    bool Uint(unsigned u) {
        if (u <= static_cast<unsigned>(std::numeric_limits<int>::max())) {
            value = u;
        } else {
            throw spJSON::ValueError{Name()};
        }
        return true;
    }
};

struct IntArrayField: public FieldArrayBase<int> {

    bool Int(int i) {
        value.push_back(i);
        return true;
    }

    bool Uint(unsigned u) {
        if (u <= static_cast<unsigned>(std::numeric_limits<int>::max())) {
            value.push_back(std::move(u));
        } else {
            throw spJSON::ValueError{Name()};
        }
        return true;
    }
};

struct I64Field: public FieldBase {
    typedef int64_t ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = 0;
    }

    bool Int(int i) {
        value = i;
        return true;
    }

    bool Int64(int64_t i) {
        value = i;
        return true;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }

    bool Uint64(uint64_t u) {
        if (u <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
        {
            value = u;
        } else {
            throw spJSON::ValueError{Name()};
        }
        return true;
    }
};

struct I64ArrayField: public FieldArrayBase<int64_t> {
    bool Int(int i) {
        value.push_back(std::move(i));
        return true;
    }

    bool Int64(int64_t i) {
        value.push_back(i);
        return true;
    }

    bool Uint(unsigned u) {
        value.push_back(std::move(u));
        return true;
    }

    bool Uint64(uint64_t u) {
        if (u <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
        {
            value.push_back(std::move(u));
        } else {
            throw spJSON::ValueError{Name()};
        }
        return true;
    }
};

struct UI64Field: public FieldBase {
    typedef uint64_t ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = 0;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }

    bool Uint64(uint64_t u) {
        value = u;
        return true;
    }
};

struct UI64ArrayField: public FieldArrayBase<uint64_t> {
    bool Uint(unsigned u) {
        value.push_back(std::move(u));
        return true;
    }

    bool Uint64(uint64_t u) {
        value.push_back(u);
        return true;
    }
};

struct UIntField: public FieldBase {
    typedef unsigned ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = 0;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }
};

struct UIntArrayField: public FieldArrayBase<unsigned> {

    bool Uint(unsigned u) {
        value.push_back(u);
        return true;
    }
};

struct DoubleField: public FieldBase {
    typedef float ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = 0.0;
    }

    bool Int(int i) {
        value = i;
        return true;
    }

    bool Int64(int64_t i) {
        value = i;
        return true;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }

    bool Uint64(uint64_t u) {
        value = u;
        return true;
    }

    bool Double(double d) {
        value = d;
        return true;
    }
};

struct DoubleArrayField: public FieldArrayBase<double> {
    bool Int(int i) {
        value.push_back(std::move(i));
        return true;
    }

    bool Int64(int64_t i) {
        value.push_back(std::move(i));
        return true;
    }

    bool Uint(unsigned u) {
        value.push_back(std::move(u));
        return true;
    }

    bool Uint64(uint64_t u) {
        value.push_back(std::move(u));
        return true;
    }

    bool Double(double d) {
        value.push_back(std::move(d));
        return true;
    }
};

struct BoolField: public FieldBase {
    typedef bool ValueType;
    ValueType value;

    virtual void Clear() {
        FieldBase::Clear();
        value = false;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }

    bool Bool(bool b) {
        value = b;
        return true;
    }
};

struct BoolArrayField: public FieldArrayBase<bool> {
    bool Uint(unsigned u) {
        value.push_back(u);
        return true;
    }

    bool Bool(bool b) {
        value.push_back(b);
        return true;
    }
};

template<class JSON>
struct EmbededObjectField: public FieldBase {
    /*******************************
     *         Properties
     *******************************/ 
    typedef JSON ValueType;
    ValueType value;

    size_t depth;

    /*******************************
     *         Utilities
     *******************************/ 

    virtual void Clear() {
        FieldBase::Clear();
        value.Clear();
        depth = 0;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        value.Key(str,length,copy);
        return true;
    }

    /**
     * Add this field to a JSON string which is being built up
     */
    template <class Builder>
    void AddToJSON(Builder& builder, bool nullIfNotSupplied) {
        builder.AddName(Name());
        builder.StartAnonymousObject();
        value.PrintAllFields(builder, nullIfNotSupplied);
        builder.EndObject();
    }

    /*******************************
     *     Rapid JSON Interface
     *******************************/ 

    bool Null() {
        if (depth > 0) {
            // Null found whilst procecssing a child object
            value.Null();
        } else {
            // Null is for us, not a child
            FieldBase::Null();
        }

        return true;
    }
    bool StartObject() {
        ++depth;
        value.StartObject();
        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        if (depth > 0) {
            value.EndObject(memberCount);
            --depth;
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        value.String(str,length,copy);
        return true;
    }

    bool Int(int i) {
        value.Int(i);
        return true;
    }

    bool Int64(int64_t i) {
        value.Int64(i);
        return true;
    }

    bool Uint(unsigned u) {
        value.Uint(u);
        return true;
    }

    bool Uint64(uint64_t u) {
        value.Uint64(u);
        return true;
    }

    bool Double(double d) {
        value.Double(d);
        return true;
    }

    bool Bool(bool b) {
        value.Bool(b);
        return true;
    }

    bool StartArray() {
        value.StartArray();
        return true;
    }

    bool EndArray(rapidjson::SizeType elementCount) {
        value.EndArray(elementCount);
        return true;
    }
};

/**
 * Note that we cannot derrive from the array base since we need special
 * handling for the embeded object which may have its own arrays
 */ 
template<class JSON>
struct ObjectArray: public FieldBase {
    /*******************************
     *         Properties
     *******************************/ 
    class pJSON {
    public:
        pJSON() : ptr(new JSON()) { }

        JSON* operator->() {
            return ptr.get();
        }

        JSON& operator*() {
            return *ptr;
        }
        
    private:
        std::unique_ptr<JSON> ptr;
    };
    typedef std::vector<pJSON> ValueType;
    ValueType value;

    int depth;

    /*******************************
     *         Utilities
     *******************************/ 
    ObjectArray() {
         Clear();
    }

    virtual void Clear() {
        FieldBase::Clear();
        value.clear();
        depth = -1;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        if (value.size() > 0)  {
            value.back()->Key(str,length,copy);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    /**
     * Add this field to a JSON string which is being built up
     */
    template <class Builder>
    void AddToJSON(Builder& builder, bool nullIfNotSupplied) {
        builder.StartArray(Name());
        for (pJSON& obj: value) {
            builder.StartAnonymousObject();
            obj->PrintAllFields(builder, nullIfNotSupplied);
            builder.EndObject();
        }
        builder.EndArray();
    }

    /*******************************
     *     Rapid JSON Interface
     *******************************/ 

    bool StartObject() {
        if (depth > 0) {
            ++depth;
            value.back()->StartObject();
        } else {
            depth = 1;
            value.emplace_back();
            value.back()->StartObject();
        }
        return true;
    }

    bool Null() {
        if (depth > 0) {
            // Null found whilst procecssing a child object
            value.back()->Null();
        } else {
            // Null is for us, not a child
            FieldBase::Null();
        }

        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        if (depth > 0) {
            value.back()->EndObject(memberCount);
            --depth;
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        if (depth > 0) {
            value.back()->String(str,length,copy);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Int(int i) {
        if (depth > 0) {
            value.back()->Int(i);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Int64(int64_t i) {
        if (depth > 0) {
            value.back()->Int64(i);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Uint(unsigned u) {
        if (depth > 0) {
            value.back()->Uint(u);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Uint64(uint64_t u) {
        if (depth > 0) {
            value.back()->Uint64(u);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Double(double d) {
        if (depth > 0) {
            value.back()->Double(d);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool Bool(bool b) {
        if (depth > 0) {
            value.back()->Bool(b);
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool StartArray() {
        if (depth > 0) {
            value.back()->StartArray();
        } else if (depth == -1) {
            depth = 0;
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }

    bool EndArray(rapidjson::SizeType elementCount) {
        if (depth > 0) {
            value.back()->EndArray(elementCount);
        } else if (depth == 0) {
            depth = -1;
        } else {
            throw spJSON::ParseError();
        }
        return true;
    }
};

/*****************************************************************************
 *                          Public Interface
 *****************************************************************************/

template <class...Fields>
SimpleParsedJSON<Fields...>::SimpleParsedJSON()
    : currentField(nullptr),
      depth(0)
{
    Clear();
    // Initialise the run-time map
    AddNextField<sizeof...(Fields)>();
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Parse(const char* json, std::string& errMsg) {
    class RapidJSONParser: public IParser {
        virtual void Parse(const char* json, SimpleParsedJSON<Fields...>& spj) {
            rapidjson::StringStream ss(json);
            rapidjson::Reader reader;
            constexpr unsigned parseFlags =
                    rapidjson::kParseDefaultFlags | rapidjson::kParseTrailingCommasFlag;
            rapidjson::ParseResult result = reader.Parse<parseFlags>(ss,spj);
            if (result.IsError()) {
                throw typename IParser::ParseError{rapidjson::GetParseError_En(result.Code())};
            }
        }
    } rjp;

    return Parse(json,errMsg,rjp);
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Parse(const char* json, std::string& errMsg, IParser& parser) {
    bool ok = false;

    try {
        parser.Parse(json,*this);
        ok = true;
    } catch (spJSON::UnknownFieldError extraField) {
        errMsg = "Unknown extra field: " + extraField.field;
    } catch (spJSON::ValueError& value) {
        errMsg = "Invalid value for field: " + value.field;
    } catch (spJSON::ParseError& parse) {
        errMsg = "Invalid JSON!";
    } catch (spJSON::WrongTypeError& type) {
        errMsg = "Invalid type for field: " + type.field;
    } catch (typename IParser::ParseError& error) {
        errMsg = "Failed to parse JSON: " + error.msg;
    }

    return ok;
}

template <class...Fields>
void SimpleParsedJSON<Fields...>::Clear() {
    depth = 0;
    currentField = nullptr;
    isArray = false;
    for (auto& item: fieldMap) {
        item.second.field->Clear();
    }
}

template <class...Fields>
template <class FIELD>
typename FIELD::ValueType& SimpleParsedJSON<Fields...>::Get() {
    return std::get<FIELD>(fields).value;
}

template <class...Fields>
template <class FIELD>
bool SimpleParsedJSON<Fields...>::Supplied() {
    return std::get<FIELD>(fields).supplied;
}

/*****************************************************************************
 *                          Rapid JSON Implementation
 *****************************************************************************/

/**
 * RapidJSON has found the start of a new object. Either this the start of our
 * object, or the start of an embeded object.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::StartObject() {
    if ( depth == 0) {
        ++depth;
    } else {
        if ( currentField != nullptr) {
            ++depth;
            currentField->field->StartObject();
        } else {
            throw spJSON::UnknownTypeError();
        }
    }
    return true;
}

/**
 * RapidJSON has found the end of the object. Check that we are currently
 * passing an object or throw an error.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::EndObject(rapidjson::SizeType memberCount) {
    if ( depth == 1) {
        --depth;
    } else if (depth > 1 && currentField) {
        currentField->field->EndObject(memberCount);
        --depth;
    } else {
        throw spJSON::ParseError();
    }
    return true;
}

/*
 * Rapid JSON has moved on to the next field. Check we know about it, mark it as
 * the current field so we can handle the value call-back correctly.
 *
 * NOTE: If we are currently parsing an embeded object we have to pass it down
 *       the chain.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Key(
    const char* str,
    rapidjson::SizeType length,
    bool copy)
{
    if (currentField && depth > 1 ) {
        currentField->field->Key(str,length,copy);
    } else {
        currentField = Get(str);

        if (!currentField) {
            throw spJSON::UnknownFieldError {str} ;
        }

        currentField->field->supplied = true;

        if (isArray) {
            throw spJSON::ParseError();
        }
    }

    return true;
}

/*
 * The current field has a string value. Check our current field can handle the
 * string, and if it can set the value.
 *
 * If no current field is selected, or it is of the wrong type, raise an error.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::String(
    const char* str,
    rapidjson::SizeType length,
    bool copy)
{
    if (currentField) {
        currentField->field->String(str,length,copy);
    } else {
        throw spJSON::ParseError();
    }

    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::RawNumber(const char *str, size_t len, bool copy) {
    return this->String(str, len, copy);
}
/**
 * The current field has an integer value. Check our current field is an int, or
 * can be converted to one, and set it.
 *
 * If there is no current field, or it is an incompatible type, raise an error.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Int(int i) {
    if (currentField) {
        currentField->field->Int(i);
    } else {
        throw spJSON::ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Int64(int64_t i) {
    if (currentField) {
        currentField->field->Int64(i);
    } else {
        throw spJSON::ParseError();
    }

    return true;
}


/*
 * The current field has an unsigned integer value. Check our current field is
 * an int, or can be converted to one, and set it.
 *
 * If there is no current field, or it is an incompatible type, raise an error.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Uint(unsigned u) {
    if (currentField) {
        currentField->field->Uint(u);
    } else {
        throw spJSON::ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Uint64(uint64_t u) {
    if (currentField) {
        currentField->field->Uint64(u);
    } else {
        throw spJSON::ParseError();
    }

    return true;
}

/*
 * The current field has a double value. Check it is a double and set the value.
 *
 * If there is no current field, or it is an incompatible type, raise an error.
 *
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Double(double d) {
    if (currentField) {
        currentField->field->Double(d);
    } else {
        throw spJSON::ParseError();
    }
    return true;
}


/*
 * The current field has a bool value. Check it is a bool and set it
 * accordingly.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Bool(bool b) {
    if (currentField) {
        currentField->field->Bool(b);
    } else {
        throw spJSON::ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::StartArray() {
    if (currentField) {
        currentField->field->StartArray();
    } else {
        throw spJSON::ParseError();
    }

    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::EndArray(rapidjson::SizeType elementCount) {
    if (currentField) {
        currentField->field->EndArray(elementCount);
    } else {
        throw spJSON::ParseError();
    }

    return true;
}

/*****************************************************************************
 *                      Rapid JSON Unsupported types
 *****************************************************************************/
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Null() {
    if (currentField) {
        currentField->field->Null();
    } else {
        throw spJSON::ParseError();
    }

    return true;
}

/*****************************************************************************
 *                  Runtime Initialisation
 *****************************************************************************/
template <class...Fields>
template <int idx>
void SimpleParsedJSON<Fields...>::AddField() {
    auto& field = std::get<idx>(fields);
    typename FieldMap::value_type item(
            field.Name(),
            { &field, field.Name() }
        );
    fieldMap.insert(std::move(item));
}

template<class ...Fields>
std::string SimpleParsedJSON<Fields...>::GetJSONString(bool nullIfNotSupplied) {
    SimpleJSONBuilder builder;

    PrintNextField<sizeof...(Fields)>(builder, nullIfNotSupplied);

    return builder.GetAndClear();
}

template<class ...Fields>
std::string SimpleParsedJSON<Fields...>::GetPrettyJSONString(bool nullIfNotSupplied) {
    SimpleJSONPrettyBuilder builder;

    PrintNextField<sizeof...(Fields)>(builder, nullIfNotSupplied);

    return builder.GetAndClear();
}

/**************************************************************************
*           Convert a field to its JSON representation
*   1) Call the type defined customer add command, if on the type
*   2) Provide a default implementation which calls straight through to 
*      the builder
**************************************************************************/

/**
 * Check if there is an override
 */
namespace SimpleParsedJSON_AddToJSON {
    template <typename T, class Builder>
    class HasAddToJSON
    {
        struct TTrue { };
        struct TFalse { };

        /**
         * This function can only be declared if the expression inside decltype
         * is valid. If it isn't this will fail, and the compiler will move onto 
         * the fallback implementation.
         *
         * The expresion inside decltype declares a pointer to the member function
         * called AddToJSON and which takes a Builder object. There are two cases:
         *    Function Exists:
         *       test will be declared as a function returning "true", and taking 
         *       a pointer to the member function.
         *    Function does not Exist:
         *       test will not be declared, but not a build error since template 
         *       substitution failure is not an error (SFINAE)
         *
         */
        template <typename C> 
        static TTrue test(decltype(static_cast<void (C::*)(Builder&, bool)>(&C::AddToJSON)));

        /**
         * Secondary implementation will work for all classes. 
         *
         * (The fall-back which returns "false")
         */
        template <typename C> 
        static TFalse test(...);

    public:
        /**
         * Only one of the two "test" functions above will be declared. These 
         * have different return types, which we extract with decltype, and then 
         * compare against "true" to return a bool.
         */
        static constexpr bool value = 
            std::is_same<decltype(test<T>(0)),TTrue>::value;
    };

    template <typename Field, class Builder>
    typename std::enable_if<HasAddToJSON<Field,Builder>::value, void>::type
    AddField(Builder& builder, Field& field, bool nullIfNotSupplied) {
        field.AddToJSON(builder, nullIfNotSupplied);
    }

    template <typename Field, class Builder>
    typename std::enable_if<!HasAddToJSON<Field,Builder>::value, void>::type
    AddField(Builder& builder, Field& field, bool nullIfNotSupplied) {
        builder.Add(field.Name(),field.value);
    }

}

/**
 * Implementation two (fall-back): Directly add the field to the builder.
 */
template<class ...Fields>
template<int idx, class Builder>
inline void SimpleParsedJSON<Fields...>::PrintField(Builder& builder, bool nullIfNotSupplied)
{
    auto& field = std::get<idx>(fields);
    if (field.supplied || !nullIfNotSupplied) {
        SimpleParsedJSON_AddToJSON::AddField(builder,field,nullIfNotSupplied);
    } else {
        builder.AddNullField(field.Name());
    }
}

/*****************************************************************************
 *                  Internal Utilities
 *****************************************************************************/

template <class...Fields>
typename SimpleParsedJSON<Fields...>::FieldInfo*
SimpleParsedJSON<Fields...>::Get(const char* fieldName)  {
    auto it = fieldMap.find(fieldName);
    if ( it != fieldMap.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

