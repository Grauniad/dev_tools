/*
 * SimpleJSON.hpp
 *
 *  Created on: 1 Apr 2015
 *      Author: lhumphreys
 */

#ifndef SIMPLEJSON_HPP_
#define SIMPLEJSON_HPP_

#include <limits>

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
};

struct StringArrayField: public FieldBase {
    typedef std::vector<std::string> ValueType;
    ValueType value;

    virtual void Clear() {
        value.clear();
    }

    virtual FieldType Type() {
        return STRING_ARRAY;
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
    } catch (ValueError& value) {
        errMsg = "Invalid value for field: " + value.field;
    } catch (ParseError& parse) {
        errMsg = "Invalid JSON!";
    } catch (WrongTypeError& type) {
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
 *
 * We don't support embeded objects, so if this a second call to StartObject
 * raise an error.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::StartObject() {
    if ( depth == 0) {
        ++depth;
    } else {
        if ( currentField != nullptr) {
            throw WrongTypeError{currentField->name};
        } else {
            throw UnknownTypeError();
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
    } else {
        throw ParseError();
    }
    return true;
}

/*
 * Rapid JSON has moved on to the next field. Check we know about it, mark it as
 * the current field so we can handle the value call-back correctly.
 */
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Key(
    const char* str,
    rapidjson::SizeType length,
    bool copy)
{
    currentField = Get(str);

    if (!currentField) {
        throw UnknownFieldError {str} ;
    }

    if (isArray) {
        throw ParseError();
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

        FieldInfo& info = *currentField;

        if (info.type == FieldBase::STRING) {

            StringField& field = static_cast<StringField&>(*info.field);
            /*
             * Profiling shows that a reserve before the assign is
             * counterproductive - the = operator strlen's and assigns anyway.
             */
            field.value = str;

        } else if (info.type == FieldBase::STRING_ARRAY) {
            StringArrayField& field =
                    static_cast<StringArrayField&>(*info.field);
            field.value.push_back(str);
        } else {
            throw WrongTypeError{info.name};
        }

    } else {
        throw ParseError();
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
        FieldInfo& info = *currentField;

        if (info.type == FieldBase::INT) {
            IntField& field = static_cast<IntField&>(*info.field);
            field.value = i;

        } else if (info.type == FieldBase::DOUBLE) {
            DoubleField& field = static_cast<DoubleField&>(*info.field);
            field.value = i;

        } else if (info.type == FieldBase::INT_64) {
            I64Field& field = static_cast<I64Field&>(*info.field);
            field.value = i;

        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Int64(int64_t i) {
    if (currentField) {
        FieldInfo& info = *currentField;

        if (info.type == FieldBase::INT_64) {
            I64Field& field = static_cast<I64Field&>(*info.field);
            field.value = i;

        } else if (info.type == FieldBase::DOUBLE) {
            DoubleField& field = static_cast<DoubleField&>(*info.field);
            field.value = i;

        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
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
        FieldInfo& info = *currentField;

        switch(info.type)
        {
        case FieldBase::UINT:
            static_cast<UIntField&>(*info.field).value = u;
            break;

        case FieldBase::INT:
            if (u <= static_cast<unsigned>(std::numeric_limits<int>::max())) {
                static_cast<IntField&>(*info.field).value = static_cast<int>(u);
            } else {
                throw ValueError{info.name};
            }
            break;

        case FieldBase::INT_64:
            static_cast<I64Field&>(*info.field).value = u;
            break;

        case FieldBase::UINT_64:
            static_cast<UI64Field&>(*info.field).value = u;
            break;

        case FieldBase::DOUBLE:
            static_cast<DoubleField&>(*info.field).value = u;
            break;

        case FieldBase::BOOL:
            static_cast<BoolField&>(*info.field).value = u;
            break;

        default:
            throw WrongTypeError{info.name};
            break;
        }

    } else {
        throw ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::Uint64(uint64_t u) {
    if (currentField) {
        FieldInfo& info = *currentField;

        if (info.type == FieldBase::INT_64) {
            if (u <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
            {
                I64Field& field = static_cast<I64Field&>(*info.field);
                field.value = u;
            } else {
                throw ValueError{info.name};
            }
        } else if (info.type == FieldBase::UINT_64) {
                UI64Field& field = static_cast<UI64Field&>(*info.field);
                field.value = u;

        } else if (info.type == FieldBase::DOUBLE) {
            DoubleField& field = static_cast<DoubleField&>(*info.field);
            field.value = u;

        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
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
        FieldInfo& info = *currentField;
        if (info.type == FieldBase::DOUBLE) {
            DoubleField& field = static_cast<DoubleField&>(*info.field);
            field.value = d;
        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
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
        FieldInfo& info = *currentField;
        if (info.type == FieldBase::BOOL) {
            BoolField& field = static_cast<BoolField&>(*info.field);
            field.value = b;
        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
    }
    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::StartArray() {
    if (currentField) {
        FieldInfo& info = *currentField;
        if (info.type == FieldBase::STRING_ARRAY) {
            isArray = true;
        } else {
            throw WrongTypeError{info.name};
        }
    } else {
        throw ParseError();
    }

    return true;
}

template <class...Fields>
bool SimpleParsedJSON<Fields...>::EndArray(rapidjson::SizeType elementCount) {
    if ( isArray) {
        isArray = false;
    } else {
        throw ParseError();
    }

    return true;
}

/*****************************************************************************
 *                      Rapid JSON Unsupported types
 *****************************************************************************/
template <class...Fields>
bool SimpleParsedJSON<Fields...>::Null() {
    throw UnknownTypeError();
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

    PrintNextField<sizeof...(Fields)>();

    return builder.GetAndClear();
}

/**************************************************************************
*           Convert each field to its JSON representation
**************************************************************************/

template<class ...Fields>
template<int idx>
inline void SimpleParsedJSON<Fields...>::PrintField() {
    auto& field = std::get<idx>(fields);
    builder.Add(field.Name(),field.value);
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
