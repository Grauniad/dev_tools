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

/**
 * TODO:
 *    We will get better performance (in the worst case) if we convert these if
 *    statements into a virtual function.
 *
 *    (Currently a UINT field initialised from a bool takes many more
 *    instructions, due to the number of comparisons. Profiling on a release
 *    build shows converting to a switch statement is no help.)
 */

/*****************************************************************************
 *                          Field Type Definitions
 *****************************************************************************/

struct StringField: public FieldBase {
    typedef std::string ValueType;
    ValueType value;

    virtual void Clear() {
        value = "";
    }

    virtual FieldType Type() {
        return STRING;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        value.assign(str,length);
        return true;
    }
};

struct StringArrayField: public FieldArrayBase {
    typedef std::vector<std::string> ValueType;
    ValueType value;

    virtual void Clear() {
        FieldArrayBase::Clear();
        value.clear();
    }

    virtual FieldType Type() {
        return STRING_ARRAY;
    }

    bool String(const char* str, rapidjson::SizeType length, bool copy) {
        if (inArray) {
            value.emplace_back(str,length);
        } else {
            throw spJSON::WrongTypeError{Name()};
        }
        return true;
    }
};

struct IntField: public FieldBase {
    typedef int ValueType;
    ValueType value;

    virtual void Clear() {
        value = 0;
    }

    virtual FieldType Type() {
        return INT;
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

struct I64Field: public FieldBase {
    typedef int64_t ValueType;
    ValueType value;

    virtual void Clear() {
        value = 0;
    }

    virtual FieldType Type() {
        return INT_64;
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

struct UI64Field: public FieldBase {
    typedef uint64_t ValueType;
    ValueType value;

    virtual void Clear() {
        value = 0;
    }

    virtual FieldType Type() {
        return UINT_64;
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

struct UIntField: public FieldBase {
    typedef unsigned ValueType;
    ValueType value;

    virtual void Clear() {
        value = 0;
    }

    virtual FieldType Type() {
        return UINT;
    }

    bool Uint(unsigned u) {
        value = u;
        return true;
    }
};

struct DoubleField: public FieldBase {
    typedef float ValueType;
    ValueType value;

    virtual void Clear() {
        value = 0.0;
    }

    virtual FieldType Type() {
        return DOUBLE;
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

struct BoolField: public FieldBase {
    typedef bool ValueType;
    ValueType value;

    virtual void Clear() {
        value = false;
    }

    virtual FieldType Type() {
        return BOOL;
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
        value.Clear();
        depth = 0;
    }

    bool Key(const char* str, rapidjson::SizeType length, bool copy) {
        value.Key(str,length,copy);
        return true;
    }

    virtual FieldType Type() {
        return OBJECT;
    }

    /**
     * Add this field to a JSON string which is being built up
     */
    virtual void AddToJSON(SimpleJSONBuilder& builder) {
        builder.AddName(Name());
        builder.StartAnonymousObject();
        value.PrintAllFields(builder);
        builder.EndObject();
    }

    /*******************************
     *     Rapid JSON Interface
     *******************************/ 

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

    virtual FieldType Type() {
        return OBJECT_ARRAY;
    }

    /**
     * Add this field to a JSON string which is being built up
     */
    virtual void AddToJSON(SimpleJSONBuilder& builder) {
        builder.StartArray(Name());
        for (pJSON& obj: value) {
            builder.StartAnonymousObject();
            obj->PrintAllFields(builder);
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
    bool ok = false;
    rapidjson::StringStream ss(json);
    rapidjson::Reader reader;

    try {
        reader.Parse(ss,*this);
        ok = true;
    } catch (UnknownFieldError& extraField) {
        errMsg = "Unknown extra field: " + extraField.field;
    } catch (spJSON::ValueError& value) {
        errMsg = "Invalid value for field: " + value.field;
    } catch (spJSON::ParseError& parse) {
        errMsg = "Invalid JSON!";
    } catch (spJSON::WrongTypeError& type) {
        errMsg = "Invalid type for field: " + type.field;
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
            throw UnknownFieldError {str} ;
        }

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
    throw spJSON::UnknownTypeError();
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
            { &field, field.Type(), field.Name() }
        );
    fieldMap.insert(std::move(item));
}

template<class ...Fields>
std::string SimpleParsedJSON<Fields...>::GetJSONString() {
    builder.Clear();

    PrintNextField<sizeof...(Fields)>(builder);

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
    template <typename T>
    class HasAddToJSON
    {
        struct TTrue { };
        struct TFalse { };

        template <typename C> 
        static TTrue test(decltype(&C::AddToJSON));

        template <typename C> 
        static TFalse test(...);

    public:
        static constexpr bool value = 
            std::is_same<decltype(test<T>(0)),TTrue>::value;
    };

    template <typename Field>
    typename std::enable_if<HasAddToJSON<Field>::value, void>::type
    AddField(SimpleJSONBuilder& builder, Field& field) {
        field.AddToJSON(builder);
    }

    template <typename Field>
    typename std::enable_if<!HasAddToJSON<Field>::value, void>::type
    AddField(SimpleJSONBuilder& builder, Field& field) {
        builder.Add(field.Name(),field.value);
    }

}

/**
 * Implementation two (fall-back): Directly add the field to the builder.
 */
template<class ...Fields>
template<int idx>
inline void SimpleParsedJSON<Fields...>::PrintField(SimpleJSONBuilder& builder)
{
    auto& field = std::get<idx>(fields);
    SimpleParsedJSON_AddToJSON::AddField(builder,field);
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

#endif /* SIMPLEJSON_HPP_ */
