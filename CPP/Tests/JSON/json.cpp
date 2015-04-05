#include "tester.h"
#include <SimpleJSON.h>
#include <vector>
#include <string>

using namespace std;

int WriteStringArray(testLogger& log);
int ParseString(testLogger& log);
int ParseStringArray(testLogger& log);
int ParseInt(testLogger& log);
int ParseUnsigned(testLogger& log);
int ParseDouble(testLogger& log);
int ParseBool(testLogger& log);
int LargeInt(testLogger& log);
int LargeI64(testLogger& log);
int ParseI64(testLogger& log);
int ParseUI64(testLogger& log);
int ExtraField(testLogger& log);
int EmbededObjectError(testLogger& log);
int Clear(testLogger& log);

//TODO Missing Field!

int main(int argc, const char *argv[])
{
    Test("Create a JSON object with a string array",WriteStringArray).RunTest();
    Test("Parsing a single string json",ParseString).RunTest();
    Test("Parsing a json string array",ParseStringArray).RunTest();
    Test("Parsing a single int json",ParseInt).RunTest();
    Test("Parsing a single unsigned json",ParseUnsigned).RunTest();
    Test("Parsing a bools from json",ParseBool).RunTest();
    Test("Parsing a single double json",ParseDouble).RunTest();
    Test("Parsing I64s...",ParseI64).RunTest();
    Test("Parsing UI64s...",ParseUI64).RunTest();
    Test("Too large INT",LargeInt).RunTest();
    Test("Too large I64",LargeI64).RunTest();
    Test("Checking Reset...",Clear).RunTest();
    Test("Checking we get an error if an extra field is supplied",ExtraField).RunTest();
    Test("Checking we get an error if an embeded object is found",EmbededObjectError).RunTest();
    return 0;
}

int WriteStringArray(testLogger& log) {
    SimpleJSONBuilder builder;
    vector<string> strings = {
        "string 1",
        "A string with lots and lots of words...or something",
        "A string with an embded \"quote from someone or other\"...",
        R"RAW(
          A raw, multi-line string
        )RAW"
    };

    vector<string> strings2 = {
        "string 2",
        "Another random string",
        "",
        "More strings",
        "\""
    };

    log << "Array 1..." << endl;

    builder.Add("testArray", strings);
    std::string result = builder.GetAndClear();
    std::string expected =
        R"JSON({"testArray":["string 1","A string with lots and lots of words...or something","A string with an embded \"quote from someone or other\"...","\n          A raw, multi-line string\n        "]})JSON";

    if (  expected != result ) {
        log << "Expected: " << expected << endl;
        log << "Got:      " << result << endl;
        return 1;
    }
    log << "Array 2..." << endl;

    builder.Add("testArray2",strings2);
    std::string result2 = builder.GetAndClear();
    std::string expected2 =
        R"JSON({"testArray2":["string 2","Another random string","","More strings","\""]})JSON";

    if (  expected2 != result2 ) {
        log << "Expected: " << expected2 << endl;
        log << "Got:      " << result2 << endl;
        return 1;
    }

    builder.Add("testArray2",strings2);
    builder.Add("testArray",strings);

    std::string result3 = builder.GetAndClear();
    std::string expected3 =
        R"JSON({"testArray2":["string 2","Another random string","","More strings","\""],"testArray":["string 1","A string with lots and lots of words...or something","A string with an embded \"quote from someone or other\"...","\n          A raw, multi-line string\n        "]})JSON";

    if (  expected3 != result3 ) {
        log << "Expected: " << expected3 << endl;
        log << "Got:      " << result3 << endl;
        return 1;
    }
    return 0;
}

//struct Field1: public StringField { const char * Name() { return "Field1"; } };

NewStringField(Field1)
NewStringField(Field2)
NewIntField(IntField1)
NewIntField(IntField2)
NewUIntField(UIntField1)
NewDoubleField(DoubleField1)
NewDoubleField(DoubleField2)
NewDoubleField(DoubleField3)
NewBoolField(BoolField1)
NewBoolField(BoolField2)
NewBoolField(BoolField3)
NewBoolField(BoolField4)
NewI64Field(I64Field1)
NewI64Field(I64Field2)
NewI64Field(I64Field3)
NewI64Field(I64Field4)
NewUI64Field(UI64Field1)
NewUI64Field(UI64Field2)
NewStringArrayField(StringArrayField1);
NewStringArrayField(StringArrayField2);

int ParseString(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "Field1": "Hello World!",
        "Field2": "",
    }
    )JSON";

    SimpleParsedJSON<Field1,Field2> json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<Field1>() != "Hello World!") {
        log << "Invalid value for field1: " << json.Get<Field1>() << endl;
        return 1;
    }

    if ( json.Get<Field2>() != "") {
        log << "Invalid value for field2: " << json.Get<Field2>() << endl;
        return 1;
    }

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<Field1>() != "Hello World!") {
        log << "2 Invalid value for field1: " << json2.Get<Field1>() << endl;
        return 1;
    }

    if ( json2.Get<Field2>() != "") {
        log << "2 Invalid value for field2: " << json2.Get<Field2>() << endl;
        return 1;
    }
    return 0;
}

int ParseStringArray(testLogger& log) {
    std::string rawJson = R"JSON( 
        {
            "StringArrayField1": [
                "String 1",
                "String 2",
                "",
                "String 4"
            ],
            "StringArrayField2": []
        }
    )JSON";

    SimpleParsedJSON<StringArrayField1, StringArrayField2> json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    const vector<string>& v1 = json.Get<StringArrayField1>();
    const vector<string>& v2 = json.Get<StringArrayField2>();

    if ( v1.size() != 4 ) {
        log << "Invalid size for field1: " << v1.size() << endl;
        return 1;
    }

    if ( v1[0] != "String 1") {
        log << "Invalid value for string 0: " << v1[0] << endl;
        return 1;
    }

    if ( v1[1] != "String 2") {
        log << "Invalid value for string 1: " << v1[1] << endl;
        return 1;
    }

    if ( v1[2] != "") {
        log << "Invalid value for string 2: " << v1[2] << endl;
        return 1;
    }

    if ( v1[3] != "String 4") {
        log << "Invalid value for string 3: " << v1[3] << endl;
        return 1;
    }

    if ( v2.size() != 0 ) {
        log << "Invalid size for field2: " << v2.size() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<string>& v21 = json2.Get<StringArrayField1>();
    const vector<string>& v22 = json2.Get<StringArrayField2>();

    if ( v21.size() != 4 ) {
        log << "Invalid size for field1: " << v21.size() << endl;
        return 1;
    }

    if ( v21[0] != "String 1") {
        log << "Invalid value for string 0: " << v21[0] << endl;
        return 1;
    }

    if ( v21[1] != "String 2") {
        log << "Invalid value for string 1: " << v21[1] << endl;
        return 1;
    }

    if ( v21[2] != "") {
        log << "Invalid value for string 2: " << v21[2] << endl;
        return 1;
    }

    if ( v21[3] != "String 4") {
        log << "Invalid value for string 3: " << v21[3] << endl;
        return 1;
    }

    if ( v22.size() != 0 ) {
        log << "Invalid size for field2: " << v22.size() << endl;
        return 1;
    }

    return 0;
}

int ParseInt(testLogger& log) {

    SimpleParsedJSON<
        IntField1,
        IntField2,
        DoubleField1,
        DoubleField2
    > json, json2;

    std::string error;

    std::string rawJson = R"JSON( 
    {
        "IntField1":     500,
        "IntField2":    -500,
        "DoubleField1":  500,
        "DoubleField2": -500,
    }
    )JSON";

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<IntField1>() != 500) {
        log << "Invalid value for field1: " << json.Get<IntField1>() << endl;
        return 1;
    }

    if ( json.Get<IntField2>() != -500) {
        log << "Invalid value for field2: " << json.Get<IntField2>() << endl;
        return 1;
    }

    if ( json.Get<DoubleField1>() != 500) {
        log << "Invalid value for double 1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json.Get<DoubleField2>() != -500) {
        log << "Invalid value for double 2: " << json.Get<DoubleField2>() << endl;
        return 1;
    }


    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<IntField1>() != 500) {
        log << "Invalid value for field1: " << json2.Get<IntField1>() << endl;
        return 1;
    }

    if ( json2.Get<IntField2>() != -500) {
        log << "Invalid value for field2: " << json2.Get<IntField2>() << endl;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 500) {
        log << "Invalid value for double 1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json2.Get<DoubleField2>() != -500) {
        log << "Invalid value for double 2: " << json2.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseI64(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "I64Field1": -500,
        "I64Field2": 500,
        "I64Field3": 5147483658,
        "I64Field4": -2147483658,
        "DoubleField1": 5147483658,
        "DoubleField2": -2147483658,
    }
    )JSON";

    SimpleParsedJSON<
        I64Field1,
        I64Field2,
        I64Field3,
        I64Field4,
        DoubleField1,
        DoubleField2
    > json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<I64Field1>() != -500) {
        log << "Invalid value for field1: " << json.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json.Get<I64Field2>() != 500) {
        log << "Invalid value for field2: " << json.Get<I64Field2>() << endl;
        return 1;
    }

    if ( json.Get<I64Field3>() != 5147483658) {
        log << "Invalid value for field3: " << json.Get<I64Field3>() << endl;
        return 1;
    }

    if ( json.Get<I64Field4>() != -2147483658) {
        log << "Invalid value for field4: " << json.Get<I64Field4>() << endl;
        return 1;
    }

    if ( json.Get<DoubleField1>() != 5147483658) {
        log << "Invalid value for double 1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json.Get<DoubleField2>() != -2147483658) {
        log << "Invalid value for double 2: " << json.Get<DoubleField2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<I64Field1>() != -500) {
        log << "Invalid value for field1: " << json2.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json2.Get<I64Field2>() != 500) {
        log << "Invalid value for field2: " << json2.Get<I64Field2>() << endl;
        return 1;
    }

    if ( json2.Get<I64Field3>() != 5147483658) {
        log << "Invalid value for field3: " << json2.Get<I64Field3>() << endl;
        return 1;
    }

    if ( json2.Get<I64Field4>() != -2147483658) {
        log << "Invalid value for field4: " << json2.Get<I64Field4>() << endl;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 5147483658) {
        log << "Invalid value for double 1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json2.Get<DoubleField2>() != -2147483658) {
        log << "Invalid value for double 2: " << json2.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseUI64(testLogger& log) {
    uint64_t value1 = 500;
    uint64_t value2 = static_cast<uint64_t>(
                          std::numeric_limits<unsigned>::max())
                      + 1;
    stringstream rawJson;

    rawJson << "{"
            << "\"UI64Field1\": " << value1 << ","
            << "\"UI64Field2\": " << value2
            << "}";

    SimpleParsedJSON<
        UI64Field1,
        UI64Field2
    > json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json.Get<UI64Field2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json2.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json2.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json2.Get<UI64Field2>() << endl;
        return 1;
    }

    return 0;
}

int ParseUnsigned(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "UIntField1": 500,
    }
    )JSON";

    SimpleParsedJSON<UIntField1> json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json2.Get<UIntField1>() << endl;
        return 1;
    }

    return 0;
}

int LargeInt(testLogger& log) {
    stringstream rawJson;
    rawJson << "{\"IntField1\": "
            << std::numeric_limits<unsigned>::max()
            << "}";

    SimpleParsedJSON<IntField1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    if (ok) {
        log << "Large int parsed!" << endl;
        return 1;
    }

    if ( error != "Invalid value for field: IntField1" ) {
        log << "Invalid error messsage: " << error << endl;
        return 1;
    }
    return 0;
}

int LargeI64(testLogger& log) {
    stringstream rawJson;
    rawJson << "{\"I64Field1\": "
            << std::numeric_limits<uint64_t>::max()
            << "}";

    SimpleParsedJSON<I64Field1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    if (ok) {
        log << "Large i64 parsed!" << endl;
        return 1;
    }

    if ( error != "Invalid value for field: I64Field1" ) {
        log << "Invalid error messsage: " << error << endl;
        return 1;
    }

    return 0;
}

int ExtraField(testLogger& log) {

    std::string rawJson = R"JSON( 
    {
        "Field1": "Hello World!",
        "Field2": "Hello World!",
    }
    )JSON";

    SimpleParsedJSON<Field1> json;

    std::string error;

    bool ok  = json.Parse(rawJson.c_str(),error);

    if (ok) {
        log << "Extra field parsed!" << endl;
    }

    if ( error != "Unknown extra field: Field2" ) {
        log << "Invalid error message: " << error;
        return 1;
    }
    return 0;
}


int ParseBool(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "BoolField1": false,
        "BoolField2": true,
        "BoolField3": 0,
        "BoolField4": 1
    }
    )JSON";

    SimpleParsedJSON<
        BoolField1,
        BoolField2,
        BoolField3,
        BoolField4
    > json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    if (json.Get<BoolField1>()) {
        log << "Field1 returned true!";
        return 1;
    }

    if (!json.Get<BoolField2>()) {
        log << "Field2 returned false!";
        return 1;
    }

    if (json.Get<BoolField3>()) {
        log << "Field3 returned true!";
        return 1;
    }

    if (!json.Get<BoolField4>()) {
        log << "Field4 returned false!";
        return 1;
    }


    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    if (json2.Get<BoolField1>()) {
        log << "Field1 returned true!";
        return 1;
    }

    if (!json2.Get<BoolField2>()) {
        log << "Field2 returned false!";
        return 1;
    }

    if (json2.Get<BoolField3>()) {
        log << "Field3 returned true!";
        return 1;
    }

    if (!json2.Get<BoolField4>()) {
        log << "Field4 returned false!";
        return 1;
    }

    return 0;
}

int ParseDouble(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "DoubleField1": 5.5,
        "DoubleField2": 5,
        "DoubleField3": -5,
    }
    )JSON";

    SimpleParsedJSON<
        DoubleField1,
        DoubleField2,
        DoubleField3
        > json, json2;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString();

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    return 0;
}

int Clear(testLogger& log) {
    SimpleParsedJSON<
        IntField1,
        UIntField1,
        I64Field1,
        UI64Field1,
        DoubleField1,
        BoolField1,
        Field1,
        StringArrayField1
        > json;

    json.Clear();

    if ( json.Get<IntField1>() != 0) {
        log << "Invalid value for int: " << json.Get<IntField1>() << endl;
        return 1;
    }

    if ( json.Get<UIntField1>() != 0) {
        log << "Invalid value for uint: " << json.Get<UIntField1>() << endl;
        return 1;
    }

    if ( json.Get<I64Field1>() != 0) {
        log << "Invalid value for int64_t: " << json.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json.Get<UI64Field1>() != 0) {
        log << "Invalid value for uint64_t: " << json.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json.Get<DoubleField1>() != 0) {
        log << "Invalid value for double: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json.Get<BoolField1>() != false) {
        log << "Invalid value for bool: " << json.Get<BoolField1>() << endl;
        return 1;
    }

    if ( json.Get<Field1>() != "") {
        log << "Invalid value for bool: " << json.Get<Field1>() << endl;
        return 1;
    }

    if ( json.Get<StringArrayField1>().size() != 0) {
        log << "Invalid size for strign array: "
            << json.Get<StringArrayField1>().size() << endl;
        return 1;
    }
    return 0;
}

int EmbededObjectError(testLogger& log) {
    stringstream rawJson;
    rawJson << "{\"IntField1\": { \"field1\": 1 } } ";

    SimpleParsedJSON<IntField1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    if (ok) {
        log << "Embeded object parsed!" << endl;
        return 1;
    }

    if ( error != "Invalid type for field: IntField1" ) {
        log << "Invalid error messsage: " << error << endl;
        return 1;
    }
    return 0;
}

