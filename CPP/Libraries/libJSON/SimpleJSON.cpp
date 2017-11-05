
#include "SimpleJSON.h"
#include "logger.h"

#include <sstream>
#include <algorithm>
#include <regex>

using namespace std;

/*****************************************************************************
 *                          Writers
 *****************************************************************************/
SimpleJSONBuilderCompactWriter::SimpleJSONBuilderCompactWriter()
    : rapidjson::Writer<rapidjson::StringBuffer>(
          static_cast<rapidjson::StringBuffer&>(*this))
{
}

void SimpleJSONBuilderCompactWriter::ResetAndClear() {
    rapidjson::StringBuffer::Clear();
    rapidjson::Writer<rapidjson::StringBuffer>::Reset(
        static_cast<rapidjson::StringBuffer&>(*this));
}

SimpleJSONBuilderPrettyWriter::SimpleJSONBuilderPrettyWriter()
    : rapidjson::PrettyWriter<rapidjson::StringBuffer>(
          static_cast<rapidjson::StringBuffer&>(*this))
{
}

void SimpleJSONBuilderPrettyWriter::ResetAndClear() {
    rapidjson::StringBuffer::Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer>::Reset(
        static_cast<rapidjson::StringBuffer&>(*this));
}

/*****************************************************************************
 *                          Base Scalar Field
 *****************************************************************************/
FieldBase::FieldBase() 
    : supplied(false)
{
}

void FieldBase::Clear() {
    supplied = false;
}

bool FieldBase::StartObject() {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::EndObject(rapidjson::SizeType memberCount) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::String(const char* str, rapidjson::SizeType length, bool copy) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Key(const char* str, rapidjson::SizeType length, bool copy) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Int(int i) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Int64(int64_t i) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Uint(unsigned u) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Uint64(uint64_t u) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Double(double d) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Bool(bool b) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::StartArray() {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::EndArray(rapidjson::SizeType elementCount) {
   throw spJSON::WrongTypeError{Name()};
}

bool FieldBase::Null() {
   supplied = false;
   return true;
}

/*****************************************************************************
 *                          SimpleParsedJSON - Auto-generate
 *****************************************************************************/

class SimpleParsedJSON_Generator {
private:

    class IFieldType {
    public:
        virtual ~IFieldType() {}

        virtual string GetDefinition() = 0;

        virtual std::shared_ptr<SimpleParsedJSON_Generator> ObjectDefn() {
            return nullptr;
        }

    };

    class BaseField: public IFieldType {
    public:
        BaseField(
                const std::string& type,
                const std::string& name,
                const std::string& indent)
                : type(type)
                , name(name)
                , indent(indent) { }

        virtual string GetDefinition() {
            std::string def =  indent + type + "(" + name + ");";
            return def;
        }

        static std::unique_ptr<BaseField> I64Type(
                bool isArray,
                const std::string& name,
                const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray? "NewI64ArrayField" : "NewI64Field",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> UI64Type(
            bool isArray,
            const std::string& name,
            const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray? "NewUI64ArrayField" : "NewUI64Field",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> IntType(
            bool isArray,
            const std::string& name,
            const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray? "NewIntArrayField" : "NewIntField",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> UIntType(
            bool isArray,
            const std::string& name,
            const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray? "NewUIntArrayField" : "NewUIntField",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> BoolType(
            bool isArray,
            const std::string& name,
            const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray? "NewBoolArrayField" : "NewBoolField",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> DoubleType(
            bool isArray,
            const std::string& name,
            const std::string& indent) {
            return std::make_unique<BaseField>(
                    isArray ? "NewDoubleArrayField" : "NewDoubleField",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> StringType(
            bool isArray,
            const std::string& name,
            const std::string& indent)
        {
            return std::make_unique<BaseField>(
                    isArray ? "NewStringArrayField" : "NewStringField",
                    name,
                    indent);
        }

        static std::unique_ptr<BaseField> TimeType(
            bool isArray,
            const std::string& name,
            const std::string& indent)
        {
            return std::make_unique<BaseField>(
                    isArray ? "NewTimeArrayField" : "NewTimeField",
                    name,
                    indent);
        }
    private:
        std::string type;
        std::string name;
        std::string indent;
    };

    class ObjectField: public IFieldType {
    public:
        ObjectField(bool isArray,
                    const std::string& name,
                    const std::string& indent,
                    const spJSON::GeneratorOptions& options)
                : isArray(isArray)
                , indent(indent)
                , name(name)
        {
            string childNamespace = name + "_fields";
            objDefn = std::make_unique<SimpleParsedJSON_Generator>(
                        childNamespace,
                        indent + "    ",
                        options);
        }

        virtual string GetDefinition() {
            string jsonName = name + "_fields::JSON";

            stringstream buf;
            buf << endl;
            buf << objDefn->GetCode("JSON");

            if (isArray) {
                buf << indent << "NewObjectArray(";
            } else {
                buf << indent << "NewEmbededObject(";
            }
            buf << name << ", " << jsonName << ");";
            return buf.str();
        }

        static std::unique_ptr<ObjectField> ObjectType(
                bool isArray,
                const std::string& name,
                const std::string& indent,
                const spJSON::GeneratorOptions& options)
        {
            return std::make_unique<ObjectField>(isArray, name, indent, options);
        }

        virtual std::shared_ptr<SimpleParsedJSON_Generator> ObjectDefn() override {
            return objDefn;
        }

    public:
        bool isArray;
        std::string indent;
        std::string name;
        shared_ptr<SimpleParsedJSON_Generator> objDefn;
    };
public:

    SimpleParsedJSON_Generator(
            const std::string _namespaceName = "",
            const std::string _indent = "    ",
            spJSON::GeneratorOptions opts = spJSON::GeneratorOptions())
            : started(false),
              isArray(false),
              childObject(nullptr),
              pendingObject(nullptr),
              indent(_indent),
              namespaceName(_namespaceName),
              options(opts) {
        int nsIndentSize = indent.length() - 4;
        if (nsIndentSize < 0) {
            nsIndentSize = 0;
        }
        nsIndent = indent.substr(0, nsIndentSize);
    }

    bool IsIsoTimestamp(const std::string& str) {
        bool isTime = false;

        static std::regex timestamp("[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]T[0-9][0-9]:[0-9][0-9]:[0-9][0-9]\\.[0-9]*Z");
        std::smatch match;

        if (std::regex_match(str,match,timestamp)) {
            isTime = true;
        }

        return isTime;
    }

    SimpleParsedJSON_Generator &ActiveObject() {
        SimpleParsedJSON_Generator *obj = childObject.get();

        if (obj) {
            return obj->ActiveObject();
        } else {
            return *this;
        }
    }

    /*
     * Check if the parser is currently parsing an object that is a child of this
     * parsed object.
     *
     * NOTE: This does NOT guarentee that childObject is not null, since we may
     *       be ignoring the object.
     */
    bool ParsingChildObject() {
        bool parserIsBelowUs = false;
        if (childObject.get()) {
            // Yup - directly in an object below us.
            parserIsBelowUs = true;
        } else if ( isArray) {
            // Yup - We have an unclosed array, must be below us...
            parserIsBelowUs = true;
        }

        return parserIsBelowUs;
    }

    bool KnownType() {
        return (current->second.get() != nullptr);
    }

    bool Key(const char *str, rapidjson::SizeType length, bool copy) {
        auto &active = ActiveObject();
        if (active.ParsingChildObject()) {
            // The parser is below the current "active" level, which means
            // we are not interested in the field...
            //
            // (For this to have happened we must have actively decided to
            //  ignore a descent. This may happen for example when parsing
            //  arrays of objects).
        } else {
            string field = str;
            auto it = active.keys.find(field);
            if (it == active.keys.end()) {
                it = active.keys.emplace(field, unique_ptr<IFieldType>(nullptr)).first;
            }
            active.current = it;
        }

        return true;
    }

    bool String(const char *str, rapidjson::SizeType length, bool copy) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else if (IsIsoTimestamp(str)) {
            active.current->second = BaseField::TimeType(active.isArray, active.current->first, active.indent);
        } else {
            active.current->second = BaseField::StringType(active.isArray, active.current->first, active.indent);
        }
        return true;
    }

    bool Double(double d) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::DoubleType(active.isArray, active.current->first, active.indent);
        }

        return true;
    }

    bool Int(int i) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::IntType(active.isArray, active.current->first, active.indent);
        }
        return true;
    }

    bool Int64(int64_t i) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::I64Type(active.isArray, active.current->first, active.indent);
        }
        return true;
    }

    bool Uint(unsigned u) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::UIntType(active.isArray, active.current->first, active.indent);
        }

        return true;
    }

    bool Uint64(uint64_t u) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::UI64Type(active.isArray, active.current->first, active.indent);
        }

        return true;
    }

    bool Bool(bool b) {
        auto &active = ActiveObject();
        if (active.KnownType()) {
            // We already know what this is...
        } else {
            active.current->second = BaseField::BoolType(active.isArray, active.current->first, active.indent);
        }
        return true;
    }

    virtual bool StartArray() {
        auto &active = ActiveObject();
        active.isArray = true;
        return true;
    }

    virtual bool EndArray(rapidjson::SizeType elementCount) {
        auto &active = ActiveObject();
        active.isArray = false;

        return true;
    }

    bool StartObject() {
        if (started == false) {
            LOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::StartObject",
                    "Starting initial object");
            started = true;
        } else if (childObject.get()) {
            LOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::StartObject",
                    "forwarding to existing child object...");
            childObject->StartObject();
        } else if (KnownType() && options.mergeFields == false) {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::StartObject",
                    "Skipping object, we already know the type: "
                            << namespaceName << "::" << current->first)
        } else if (KnownType() && options.mergeFields == true) {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::StartObject",
                    "Resuming known object: "
                            << namespaceName << "::" << current->first)
            childObject = current->second->ObjectDefn();
        } else {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::StartObject",
                    "Starting a new child object: " << current->first);

            auto objPtr = ObjectField::ObjectType(isArray, current->first, indent, options);
            childObject = objPtr->ObjectDefn();
            childObject->StartObject();
            pendingObject = std::move(objPtr);
        }
        return true;
    }

    bool EndObject(rapidjson::SizeType memberCount) {
        if (started == false) {
            SLOG_FROM(
                    LOG_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "End of non-existent object!");
        }
        else if (childObject.get() == nullptr) {
            LOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "Terminated the object itself");
        } else if (childObject->ParsingChildObject()) {
            LOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "Forwarding to child object...");
            childObject->EndObject(memberCount);
        } else if (KnownType() && options.mergeFields == false) {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "Skipping end object, for object of known type..."
                            << namespaceName << "::" << current->first)
        } else if (KnownType() && options.mergeFields == true) {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "Completed object, again: "
                            << namespaceName << "::" << current->first)
            childObject.reset((SimpleParsedJSON_Generator*)nullptr);
        } else {
            SLOG_FROM(
                    LOG_VERY_VERBOSE,
                    "SimpleParsedJSON_Generator::EndObject",
                    "Finished processing child object, " << current->first);
            childObject->EndObject(memberCount);

            if (pendingObject.get()) {
                current->second.reset(pendingObject.release());
                pendingObject.reset(nullptr);
            }
            childObject.reset((SimpleParsedJSON_Generator*)nullptr);
        }
        return true;
    }

    bool Null() {
        if (options.ignoreNull == false) {
            throw "TODO!";
        } else {
            // Ignore the null - there might be another type to come...
        }

        return true;
    }

    bool RawNumber(const char *str, size_t len, bool copy) {
        return this->String(str, len, copy);
    }

    string GetCode(const std::string &jsonName) {
        stringstream result;
        stringstream fields;
        if (namespaceName != "") {
            result << nsIndent << "namespace " << namespaceName << " {" << endl;
        }

        auto it = keys.begin();
        while (it != keys.end()) {
            if (it->second.get() == nullptr) {
                it = keys.erase(it);
            } else {
                ++it;
            }
        }
        it = keys.begin();
        while (it != keys.end()) {
            result << it->second->GetDefinition() << endl;

            fields << indent << "    " << it->first;

            ++it;
            if (it == keys.end()) {
                fields << endl;
            } else {
                fields << "," << endl;
            }
        }

        result << endl;
        result << indent << "typedef SimpleParsedJSON<" << endl;
        result << fields.str();
        result << indent << "> " << jsonName << ";" << endl;
        if (namespaceName != "") {
            result << nsIndent << "}" << endl;
        }
        return result.str();
    }

private:
    bool started;     // Indicates if we have found the start of the outer object yet
    bool isArray;     // Indicates if the field currently being scanned is an array

    shared_ptr<SimpleParsedJSON_Generator> childObject;
    std::unique_ptr<ObjectField> pendingObject;

    typedef std::map<std::string,std::unique_ptr<IFieldType>> Keys;
    Keys keys;
    Keys::iterator current;
    std::string indent;
    std::string nsIndent;
    std::string namespaceName;
    spJSON::GeneratorOptions options;
};


std::string spJSON::Gen(
        const string& className,
        const string& exampleJson,
        spJSON::GeneratorOptions options )
{
    SimpleParsedJSON_Generator gen("", "    ", options);

    rapidjson::StringStream ss(exampleJson.c_str());
    rapidjson::Reader reader;

    reader.Parse(ss,gen);

    return gen.GetCode(className);
}
