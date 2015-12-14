#include "tester.h"
#include <SimpleJSON.h>
#include <vector>
#include <string>

using namespace std;

//TODO : Embeded Objects!
int WriteStringArray(testLogger& log);

int ParseString(testLogger& log);
int ParseEmbededString(testLogger& log);
int ParseEmbededArrayString(testLogger& log);

int ParseStringArray(testLogger& log);
int ParseEmbededStringArray(testLogger& log);

int ParseInt(testLogger& log);
int ParseEmbededInt(testLogger& log);
int ParseEmbededArrayInt(testLogger& log);

int ParseIntArray(testLogger& log);

int ParseUnsigned(testLogger& log);
int ParseEmbededUnsigned(testLogger& log);
int ParseEmbededArrayUnsigned(testLogger& log);

int ParseUnsignedArray(testLogger& log);

int ParseDouble(testLogger& log);
int ParseEmbededDouble(testLogger& log);
int ParseEmbededArrayDouble(testLogger& log);

int ParseDoubleArray(testLogger& log);

int ParseBool(testLogger& log);
int ParseEmbededBool(testLogger& log);
int ParseEmbededArrayBool(testLogger& log);

int ParseBoolArray(testLogger& log);

int ParseI64(testLogger& log);
int ParseEmbededI64(testLogger& log);
int ParseEmbededArrayI64(testLogger& log);

int ParseI64Array(testLogger& log);

int ParseUI64(testLogger& log);
int ParseEmbededUI64(testLogger& log);
int ParseEmbededArrayUI64(testLogger& log);

int ParseUI64Array(testLogger& log);

int Clear(testLogger& log);
int EmbededClear(testLogger& log);
int EmbededArrayClear(testLogger& log);

int LargeInt(testLogger& log);
int LargeI64(testLogger& log);

int ExtraField(testLogger& log);
int EmbededObjectError(testLogger& log);

int TrippleEmbededObject(testLogger& log);

int EmbededObjectInArray(testLogger& log);

int Supplied(testLogger& log);
int SuppliedEmbeded(testLogger& log);
int SuppliedEmbededObjectInArray(testLogger& log);

//TODO Missing Field!

int main(int argc, const char *argv[])
{
    Test("Parsing a single string json",ParseString).RunTest();
    Test("Parsing a single emebded string json",ParseEmbededString).RunTest();
    Test("Parsing an array of emebded string jsons",ParseEmbededArrayString).RunTest();
    Test("Create a JSON object with a string array",WriteStringArray).RunTest();
    Test("Parsing a json string array",ParseStringArray).RunTest();
    Test("Parsing an embeded json string array",ParseEmbededStringArray).RunTest();
    Test("Parsing a single int json",ParseInt).RunTest();
    Test("Parsing a single embeded int json",ParseEmbededInt).RunTest();
    Test("Parsing an array of embeded ints json",ParseEmbededArrayInt).RunTest();
    Test("Parsing a json int array",ParseIntArray).RunTest();
    Test("Parsing a single unsigned json",ParseUnsigned).RunTest();
    Test("Parsing a json unsigned int array",ParseUnsignedArray).RunTest();
    Test("Parsing a single embeded unsigned json",ParseEmbededUnsigned).RunTest();
    Test("Parsing an array of embeded unsigneds json",ParseEmbededArrayUnsigned).RunTest();
    Test("Parsing a bool from json",ParseBool).RunTest();
    Test("Parsing a bool array from json",ParseBoolArray).RunTest();
    Test("Parsing a bools from embeded json",ParseEmbededBool).RunTest();
    Test("Parsing an array of bools from json",ParseEmbededArrayBool).RunTest();
    Test("Parsing a single double json",ParseDouble).RunTest();
    Test("Parsing a json double array",ParseDoubleArray).RunTest();
    Test("Parsing a single double json",ParseEmbededDouble).RunTest();
    Test("Parsing an array of doubles json",ParseEmbededArrayDouble).RunTest();
    Test("Parsing I64s...",ParseI64).RunTest();
    Test("Parsing array of I64s...",ParseI64Array).RunTest();
    Test("Parsing Embeded I64s...",ParseEmbededI64).RunTest();
    Test("Parsing Embeded I64 array...",ParseEmbededArrayI64).RunTest();
    Test("Parsing UI64s...",ParseUI64).RunTest();
    Test("Parsing array of UI64s...",ParseUI64Array).RunTest();
    Test("Parsing embeded UI64s...",ParseEmbededUI64).RunTest();
    Test("Parsing embeded UI64 array...",ParseEmbededArrayUI64).RunTest();
    Test("Too large INT",LargeInt).RunTest();
    Test("Too large I64",LargeI64).RunTest();
    Test("Checking Reset...",Clear).RunTest();
    Test("Checking Embeded Reset...",EmbededClear).RunTest();
    Test("Checking Embeded Array Reset...",EmbededArrayClear).RunTest();
    Test("Checking we get an error if an extra field is supplied",ExtraField).RunTest();
    Test("Checking we get an error if an unexpected embeded object is found",EmbededObjectError).RunTest();
    Test("Checking we handles objects within objects within objects",TrippleEmbededObject).RunTest();
    Test("Checking we handles objects within arrays within objects",EmbededObjectInArray).RunTest();
    Test("Checking supplied handling for fields in the root object",Supplied).RunTest();
    Test("Checking supplied handling for fields in embeded objects",SuppliedEmbeded).RunTest();
    Test("Checking supplied handling for fields in embeded objects in arrays",SuppliedEmbededObjectInArray).RunTest();
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

NewStringField(Field1)
NewStringField(Field2)
NewIntField(IntField1)
NewIntField(IntField2)
NewUIntField(UIntField1)
NewDoubleField(DoubleField1)
NewDoubleField(DoubleField2)
NewDoubleField(DoubleField3)
NewDoubleField(DoubleField4)
NewDoubleField(DoubleField5)
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
NewIntArrayField(IntArrayField1);
NewIntArrayField(IntArrayField2);
NewUIntArrayField(UIntArrayField1);
NewUIntArrayField(UIntArrayField2);
NewDoubleArrayField(DoubleArrayField1);
NewDoubleArrayField(DoubleArrayField2);
NewBoolArrayField(BoolArrayField1);
NewBoolArrayField(BoolArrayField2);
NewI64ArrayField(I64ArrayField1);
NewI64ArrayField(I64ArrayField2);
NewUI64ArrayField(UI64ArrayField1);
NewUI64ArrayField(UI64ArrayField2);

int ParseString(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "Field1": "Hello World!",
        "Field2": "",
    }
    )JSON";

    SimpleParsedJSON<Field1,Field2> json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

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

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<Field1>() != "Hello World!") {
        log << "2 Invalid value for field1: " << json3.Get<Field1>() << endl;
        return 1;
    }

    if ( json3.Get<Field2>() != "") {
        log << "2 Invalid value for field2: " << json3.Get<Field2>() << endl;
        return 1;
    }
    return 0;
}

int ParseEmbededString(testLogger& log) {
    std::string rawJson = R"JSON(
    {
        "Object" : {
            "Field1": "Hello World!",
            "Field2": "",
        }
    }
    )JSON";

    typedef SimpleParsedJSON<Field1,Field2> JSON; 
    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;


    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object>();

    if ( json.Get<Field1>() != "Hello World!") {
        log << "Invalid value for field1: " << json.Get<Field1>() << endl;
        return 1;
    }

    if ( json.Get<Field2>() != "") {
        log << "Invalid value for field2: " << json.Get<Field2>() << endl;
        return 1;
    }

    string newRawJson = parent.GetJSONString(true);
    log << "Raw JSON:" << endl << newRawJson << endl;

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = parent2.Get<Object>();

    if ( json2.Get<Field1>() != "Hello World!") {
        log << "2 Invalid value for field1: " << json2.Get<Field1>() << endl;
        return 1;
    }

    if ( json2.Get<Field2>() != "") {
        log << "2 Invalid value for field2: " << json2.Get<Field2>() << endl;
        return 1;
    }

    string newRawJson2 = parent.GetPrettyJSONString();
    log << "Raw JSON:" << endl << newRawJson2 << endl;

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = parent3.Get<Object>();

    if ( json3.Get<Field1>() != "Hello World!") {
        log << "2 Invalid value for field1: " << json3.Get<Field1>() << endl;
        return 1;
    }

    if ( json3.Get<Field2>() != "") {
        log << "2 Invalid value for field2: " << json3.Get<Field2>() << endl;
        return 1;
    }
    return 0;
}

int ParseEmbededArrayString(testLogger& log) {
    std::string rawJson = R"JSON(
    {
        "Objects": [{
            "Field1": "Hello World!",
            "Field2": ""
        }, {
            "Field1": "Another string",
            "Field2": "Not a blank string"
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<Field1,Field2> JSON; 
    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[0];
    JSON& bjson = *parent.Get<Objects>()[1];

    if ( json.Get<Field1>() != "Hello World!") {
        log << "Invalid value for field1: " << json.Get<Field1>() << endl;
        return 1;
    }

    if ( json.Get<Field2>() != "") {
        log << "Invalid value for field2: " << json.Get<Field2>() << endl;
        return 1;
    }

    if ( bjson.Get<Field1>() != "Another string") {
        log << "Invalid value for field1: " << bjson.Get<Field1>() << endl;
        return 1;
    }

    if ( bjson.Get<Field2>() != "Not a blank string") {
        log << "Invalid value for field2: " << bjson.Get<Field2>() << endl;
        return 1;
    }

    string newRawJson = parent.GetJSONString(true);
    log << "Raw JSON:" << endl << newRawJson << endl;

    ok = parent2.Parse(newRawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = *parent.Get<Objects>()[0];
    JSON& bjson2 = *parent.Get<Objects>()[1];

    if ( json2.Get<Field1>() != "Hello World!") {
        log << "Invalid value for field1: " << json2.Get<Field1>() << endl;
        return 1;
    }

    if ( json2.Get<Field2>() != "") {
        log << "Invalid value for field2: " << json2.Get<Field2>() << endl;
        return 1;
    }

    if ( bjson2.Get<Field1>() != "Another string") {
        log << "Invalid value for field1: " << bjson2.Get<Field1>() << endl;
        return 1;
    }

    if ( bjson2.Get<Field2>() != "Not a blank string") {
        log << "Invalid value for field2: " << bjson2.Get<Field2>() << endl;
        return 1;
    }

    string newRawJson2 = parent.GetPrettyJSONString();
    log << "Raw JSON:" << endl << newRawJson2 << endl;

    ok = parent3.Parse(newRawJson2.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = *parent.Get<Objects>()[0];
    JSON& bjson3 = *parent.Get<Objects>()[1];

    if ( json3.Get<Field1>() != "Hello World!") {
        log << "Invalid value for field1: " << json3.Get<Field1>() << endl;
        return 1;
    }

    if ( json3.Get<Field2>() != "") {
        log << "Invalid value for field2: " << json3.Get<Field2>() << endl;
        return 1;
    }

    if ( bjson3.Get<Field1>() != "Another string") {
        log << "Invalid value for field1: " << bjson3.Get<Field1>() << endl;
        return 1;
    }

    if ( bjson3.Get<Field2>() != "Not a blank string") {
        log << "Invalid value for field2: " << bjson3.Get<Field2>() << endl;
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

    SimpleParsedJSON<StringArrayField1, StringArrayField2> json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<string>& v31 = json2.Get<StringArrayField1>();
    const vector<string>& v32 = json2.Get<StringArrayField2>();

    if ( v31.size() != 4 ) {
        log << "Invalid size for field1: " << v31.size() << endl;
        return 1;
    }

    if ( v31[0] != "String 1") {
        log << "Invalid value for string 0: " << v31[0] << endl;
        return 1;
    }

    if ( v31[1] != "String 2") {
        log << "Invalid value for string 1: " << v31[1] << endl;
        return 1;
    }

    if ( v31[2] != "") {
        log << "Invalid value for string 2: " << v31[2] << endl;
        return 1;
    }

    if ( v31[3] != "String 4") {
        log << "Invalid value for string 3: " << v31[3] << endl;
        return 1;
    }

    if ( v32.size() != 0 ) {
        log << "Invalid size for field2: " << v32.size() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<string>& v21 = json3.Get<StringArrayField1>();
    const vector<string>& v22 = json3.Get<StringArrayField2>();

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

int ParseEmbededStringArray(testLogger& log) {
    std::string rawJson = R"JSON({
            "Object": {
                "StringArrayField1": [
                    "String 1",
                    "String 2",
                    "",
                    "String 4"
                ],
                "StringArrayField2": []
            }
        }
    )JSON";

    typedef SimpleParsedJSON<StringArrayField1, StringArrayField2> JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object>();

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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    JSON& json2 = parent2.Get<Object>();

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    JSON& json3 = parent3.Get<Object>();

    const vector<string>& v31 = json3.Get<StringArrayField1>();
    const vector<string>& v32 = json3.Get<StringArrayField2>();

    if ( v31.size() != 4 ) {
        log << "Invalid size for field1: " << v31.size() << endl;
        return 1;
    }

    if ( v31[0] != "String 1") {
        log << "Invalid value for string 0: " << v31[0] << endl;
        return 1;
    }

    if ( v31[1] != "String 2") {
        log << "Invalid value for string 1: " << v31[1] << endl;
        return 1;
    }

    if ( v31[2] != "") {
        log << "Invalid value for string 2: " << v31[2] << endl;
        return 1;
    }

    if ( v31[3] != "String 4") {
        log << "Invalid value for string 3: " << v31[3] << endl;
        return 1;
    }

    if ( v32.size() != 0 ) {
        log << "Invalid size for field2: " << v32.size() << endl;
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
    > json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<IntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<IntField1>() << endl;
        return 1;
    }

    if ( json3.Get<IntField2>() != -500) {
        log << "Invalid value for field2: " << json3.Get<IntField2>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 500) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -500) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseIntArray(testLogger& log) {
    std::string rawJson = R"JSON( 
        {
            "IntArrayField1": [
                500,
                -134,
                0,
                23
            ],
            "IntArrayField2": []
        }
    )JSON";

    SimpleParsedJSON<IntArrayField1, IntArrayField2> json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    const vector<int>& v1 = json.Get<IntArrayField1>();
    const vector<int>& v2 = json.Get<IntArrayField2>();

    auto f = [&] (const vector<int>& v1, const vector<int>& v2) -> int {
        if ( v1.size() != 4 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != 500) {
            log << "Invalid value for int 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != -134) {
            log << "Invalid value for int 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != 0) {
            log << "Invalid value for int 2: " << v1[2] << endl;
            return 1;
        }

        if ( v1[3] != 23) {
            log << "Invalid value for int 3: " << v1[3] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<int>& v21 = json2.Get<IntArrayField1>();
    const vector<int>& v22 = json2.Get<IntArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<int>& v31 = json3.Get<IntArrayField1>();
    const vector<int>& v32 = json3.Get<IntArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseEmbededInt(testLogger& log) {
    typedef SimpleParsedJSON<
        IntField1,
        IntField2,
        DoubleField1,
        DoubleField2
    > JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    std::string rawJson = R"JSON({
        "Object": {
            "IntField1":     500,
            "IntField2":    -500,
            "DoubleField1":  500,
            "DoubleField2": -500,
        }
    }
    )JSON";

    bool ok = parent.Parse(rawJson.c_str(),error);

    JSON& json = parent.Get<Object>();

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

    string newRawJson = parent.GetJSONString(true);


    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();


    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<IntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<IntField1>() << endl;
        return 1;
    }

    if ( json3.Get<IntField2>() != -500) {
        log << "Invalid value for field2: " << json3.Get<IntField2>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 500) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -500) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseEmbededArrayInt(testLogger& log) {
    typedef SimpleParsedJSON<
        IntField1,
        IntField2,
        DoubleField1,
        DoubleField2
    > JSON;

    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    std::string rawJson = R"JSON({
        "Objects": [ {
            "IntField1":     0,
            "IntField2":     0,
            "DoubleField1":  1,
            "DoubleField2":  2
        },
        {
            "IntField1":     500,
            "IntField2":    -500,
            "DoubleField1":  500,
            "DoubleField2": -500
        },
        {
            "IntField1":     234,
            "IntField2":    -234,
            "DoubleField1":  324,
            "DoubleField2": -234
        }]
    }
    )JSON";

    bool ok = parent.Parse(rawJson.c_str(),error);

    JSON& json = *parent.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if (parent.Get<Objects>().size() != 3) {
        log << "Invalid size: " << parent.Get<Objects>().size() << endl;
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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<IntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<IntField1>() << endl;
        return 1;
    }

    if ( json3.Get<IntField2>() != -500) {
        log << "Invalid value for field2: " << json3.Get<IntField2>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 500) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -500) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
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
    > json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<I64Field1>() != -500) {
        log << "Invalid value for field1: " << json3.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field2>() != 500) {
        log << "Invalid value for field2: " << json3.Get<I64Field2>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field3>() != 5147483658) {
        log << "Invalid value for field3: " << json3.Get<I64Field3>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field4>() != -2147483658) {
        log << "Invalid value for field4: " << json3.Get<I64Field4>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5147483658) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -2147483658) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseI64Array(testLogger& log) {
    std::string rawJson = R"JSON( 
        {
            "I64ArrayField1": [
                -500,
                500,
                5147483658,
                -2147483658
            ],
            "I64ArrayField2": []
        }
    )JSON";

    SimpleParsedJSON<I64ArrayField1, I64ArrayField2> json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    const vector<int64_t>& v1 = json.Get<I64ArrayField1>();
    const vector<int64_t>& v2 = json.Get<I64ArrayField2>();

    auto f = [&] (const vector<int64_t>& v1, const vector<int64_t>& v2) -> int {
        if ( v1.size() != 4 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != -500) {
            log << "Invalid value for int 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != 500) {
            log << "Invalid value for int 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != 5147483658) {
            log << "Invalid value for int 2: " << v1[2] << endl;
            return 1;
        }

        if ( v1[3] != -2147483658) {
            log << "Invalid value for int 3: " << v1[3] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<int64_t>& v21 = json2.Get<I64ArrayField1>();
    const vector<int64_t>& v22 = json2.Get<I64ArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<int64_t>& v31 = json3.Get<I64ArrayField1>();
    const vector<int64_t>& v32 = json3.Get<I64ArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}


int ParseEmbededI64(testLogger& log) {
    std::string rawJson = R"JSON({
        "Object": {
            "I64Field1": -500,
            "I64Field2": 500,
            "I64Field3": 5147483658,
            "I64Field4": -2147483658,
            "DoubleField1": 5147483658,
            "DoubleField2": -2147483658,
        }
    }
    )JSON";

    typedef SimpleParsedJSON<
        I64Field1,
        I64Field2,
        I64Field3,
        I64Field4,
        DoubleField1,
        DoubleField2
    > JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object>();

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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = parent2.Get<Object>();

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = parent3.Get<Object>();

    if ( json3.Get<I64Field1>() != -500) {
        log << "Invalid value for field1: " << json3.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field2>() != 500) {
        log << "Invalid value for field2: " << json3.Get<I64Field2>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field3>() != 5147483658) {
        log << "Invalid value for field3: " << json3.Get<I64Field3>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field4>() != -2147483658) {
        log << "Invalid value for field4: " << json3.Get<I64Field4>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5147483658) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -2147483658) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
        return 1;
    }

    return 0;
}

int ParseEmbededArrayI64(testLogger& log) {
    std::string rawJson = R"JSON({
        "Objects": [{
            "I64Field1": -0,
            "I64Field2": 100,
            "I64Field3": 1,
            "I64Field4": -1,
            "DoubleField1": 0,
            "DoubleField2": -1
        },
        {
            "I64Field1": -500,
            "I64Field2": 500,
            "I64Field3": 5147483658,
            "I64Field4": -2147483658,
            "DoubleField1": 5147483658,
            "DoubleField2": -2147483658
        },
        {
            "I64Field1": -1,
            "I64Field2": 2,
            "I64Field3": 2,
            "I64Field4": -2,
            "DoubleField1": 3,
            "DoubleField2": -4
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<
        I64Field1,
        I64Field2,
        I64Field3,
        I64Field4,
        DoubleField1,
        DoubleField2
    > JSON;

    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[1];

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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = *parent2.Get<Objects>()[1];

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

    log << "Re-building JSON>..." << endl;

    string newRawJson22 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson22.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = *parent3.Get<Objects>()[1];

    if ( json3.Get<I64Field1>() != -500) {
        log << "Invalid value for field1: " << json3.Get<I64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field2>() != 500) {
        log << "Invalid value for field2: " << json3.Get<I64Field2>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field3>() != 5147483658) {
        log << "Invalid value for field3: " << json3.Get<I64Field3>() << endl;
        return 1;
    }

    if ( json3.Get<I64Field4>() != -2147483658) {
        log << "Invalid value for field4: " << json3.Get<I64Field4>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5147483658) {
        log << "Invalid value for double 1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    if ( json3.Get<DoubleField2>() != -2147483658) {
        log << "Invalid value for double 2: " << json3.Get<DoubleField2>() << endl;
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
    > json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json3.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json3.Get<UI64Field2>() << endl;
        return 1;
    }

    return 0;
}

int ParseUI64Array(testLogger& log) {
    std::string rawJson = R"JSON( 
        {
            "UI64ArrayField1": [
                500,
                12514748365008,
                0
            ],
            "UI64ArrayField2": []
        }
    )JSON";

    SimpleParsedJSON<UI64ArrayField1, UI64ArrayField2> json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    const vector<uint64_t>& v1 = json.Get<UI64ArrayField1>();
    const vector<uint64_t>& v2 = json.Get<UI64ArrayField2>();

    auto f = [&] (const vector<uint64_t>& v1, const vector<uint64_t>& v2) -> int {
        if ( v1.size() != 3 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != 500) {
            log << "Invalid value for int 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != 12514748365008) {
            log << "Invalid value for int 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != 0) {
            log << "Invalid value for int 1: " << v1[2] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<uint64_t>& v21 = json2.Get<UI64ArrayField1>();
    const vector<uint64_t>& v22 = json2.Get<UI64ArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse JSON!" << endl;
        return 1;
    }

    const vector<uint64_t>& v31 = json3.Get<UI64ArrayField1>();
    const vector<uint64_t>& v32 = json3.Get<UI64ArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseEmbededUI64(testLogger& log) {
    uint64_t value1 = 500;
    uint64_t value2 = static_cast<uint64_t>(
                          std::numeric_limits<unsigned>::max())
                      + 1;
    stringstream rawJson;

    rawJson << "{ \"Object\": {"
            << "\"UI64Field1\": " << value1 << ","
            << "\"UI64Field2\": " << value2
            << "}" << "}";

    typedef SimpleParsedJSON<
        UI64Field1,
        UI64Field2
    > JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.str().c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object>();

    if ( json.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json.Get<UI64Field2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = parent2.Get<Object>();

    if ( json2.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json2.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json2.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json2.Get<UI64Field2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = parent3.Get<Object>();

    if ( json3.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json3.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json3.Get<UI64Field2>() << endl;
        return 1;
    }

    return 0;
}

int ParseEmbededArrayUI64(testLogger& log) {
    uint64_t value1 = 500;
    uint64_t value2 = static_cast<uint64_t>(
                          std::numeric_limits<unsigned>::max())
                      + 1;
    stringstream rawJson;

    rawJson << "{ \"Objects\": [{"
            <<     "\"UI64Field1\": 0,"
            <<     "\"UI64Field2\": 1" 
            <<     "},"
            <<     "{"
            <<     "\"UI64Field1\": " << value1 << ","
            <<     "\"UI64Field2\": " << value2
            <<     "},"
            <<     "{"
            <<     "\"UI64Field1\": 1,"
            <<     "\"UI64Field2\": 2" 
            <<     "}]" 
            << "}";

    typedef SimpleParsedJSON<
        UI64Field1,
        UI64Field2
    > JSON;

    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.str().c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[1];

    if ( json.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json.Get<UI64Field2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json2 = *parent2.Get<Objects>()[1];

    if ( json2.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json2.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json2.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json2.Get<UI64Field2>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson22 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson22.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json3 = *parent3.Get<Objects>()[1];

    if ( json3.Get<UI64Field1>() != value1) {
        log << "Invalid value for field1: " << json3.Get<UI64Field1>() << endl;
        return 1;
    }

    if ( json3.Get<UI64Field2>() != value2) {
        log << "Invalid value for field2: " << json3.Get<UI64Field2>() << endl;
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

    SimpleParsedJSON<UIntField1> json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json2.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<UIntField1>() << endl;
        return 1;
    }

    return 0;
}

int ParseUnsignedArray(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "UIntArrayField1": [
           500,
           0,
           123
        ],
        "UIntArrayField2": []

    }
    )JSON";

    SimpleParsedJSON<UIntArrayField1, UIntArrayField2> json1, json2, json3;

    std::string error;

    bool ok = json1.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    auto f = [&] (const vector<unsigned>& v1, const vector<unsigned>& v2) -> int {
        if ( v1.size() != 3 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != 500) {
            log << "Invalid value for int 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != 0) {
            log << "Invalid value for int 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != 123) {
            log << "Invalid value for int 2: " << v1[2] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    const vector<unsigned>& v1 = json1.Get<UIntArrayField1>();
    const vector<unsigned>& v2 = json1.Get<UIntArrayField2>();

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<unsigned>& v21 = json2.Get<UIntArrayField1>();
    const vector<unsigned>& v22 = json2.Get<UIntArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<unsigned>& v31 = json3.Get<UIntArrayField1>();
    const vector<unsigned>& v32 = json3.Get<UIntArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseBoolArray(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "BoolArrayField1": [
            true,
            false,
            1,
            0
        ],
        "BoolArrayField2": []

    }
    )JSON";

    SimpleParsedJSON<BoolArrayField1, BoolArrayField2> json1, json2, json3;

    std::string error;

    bool ok = json1.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    auto f = [&] (const vector<bool>& v1, const vector<bool>& v2) -> int {
        if ( v1.size() != 4 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != true) {
            log << "Invalid value for int 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != false) {
            log << "Invalid value for int 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != true) {
            log << "Invalid value for int 2: " << v1[2] << endl;
            return 1;
        }

        if ( v1[3] != false) {
            log << "Invalid value for int 3: " << v1[3] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    const vector<bool>& v1 = json1.Get<BoolArrayField1>();
    const vector<bool>& v2 = json1.Get<BoolArrayField2>();

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<bool>& v21 = json2.Get<BoolArrayField1>();
    const vector<bool>& v22 = json2.Get<BoolArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<bool>& v31 = json3.Get<BoolArrayField1>();
    const vector<bool>& v32 = json3.Get<BoolArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseDoubleArray(testLogger& log) {
    std::string rawJson = R"JSON( 
    {
        "DoubleArrayField1": [
           5.5,
           5,
           -5,
           2000000000000,
          -2000000000001
        ],
        "DoubleArrayField2": []

    }
    )JSON";

    SimpleParsedJSON<DoubleArrayField1, DoubleArrayField2> json1, json2, json3;

    std::string error;

    bool ok = json1.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    auto f = [&] (const vector<double>& v1, const vector<double>& v2) -> int {
        if ( v1.size() != 5 ) {
            log << "Invalid size for field1: " << v1.size() << endl;
            return 1;
        }

        if ( v1[0] != 5.5) {
            log << "Invalid value for item 0: " << v1[0] << endl;
            return 1;
        }

        if ( v1[1] != 5) {
            log << "Invalid value for item 1: " << v1[1] << endl;
            return 1;
        }

        if ( v1[2] != -5) {
            log << "Invalid value for item 2: " << v1[2] << endl;
            return 1;
        }

        if ( v1[3] != 2000000000000L) {
            log << "Invalid value for item 3: " << v1[3] << endl;
            return 1;
        }

        if ( v1[4] != -2000000000001L) {
            log << "Invalid value for item 4: " << v1[4] << endl;
            return 1;
        }

        if ( v2.size() != 0 ) {
            log << "Invalid size for field2: " << v2.size() << endl;
            return 1;
        }

        return 0;
    };

    const vector<double>& v1 = json1.Get<DoubleArrayField1>();
    const vector<double>& v2 = json1.Get<DoubleArrayField2>();

    if ( f(v1,v2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<double>& v21 = json2.Get<DoubleArrayField1>();
    const vector<double>& v22 = json2.Get<DoubleArrayField2>();

    if ( f(v21,v22) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<double>& v31 = json3.Get<DoubleArrayField1>();
    const vector<double>& v32 = json3.Get<DoubleArrayField2>();

    if ( f(v31,v32) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseEmbededUnsigned(testLogger& log) {
    std::string rawJson = R"JSON({
        "Object": {
            "UIntField1": 500,
        }
    }
    )JSON";

    typedef SimpleParsedJSON<UIntField1> JSON;
    NewEmbededObject(Object,JSON);

    SimpleParsedJSON<Object> parent, parent2, parent3;
    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    JSON& json = parent.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json2.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<UIntField1>() << endl;
        return 1;
    }

    return 0;
}

int ParseEmbededArrayUnsigned(testLogger& log) {
    std::string rawJson = R"JSON({
        "Objects": [{
            "UIntField1": 0
        },
        {
            "UIntField1": 500
        },
        {
            "UIntField1": 123
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<UIntField1> JSON;
    NewObjectArray(Objects,JSON);

    SimpleParsedJSON<Objects> parent, parent2, parent3;
    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( parent.Get<Objects>().size() != 3 ) {
        log << "Invalid size: " << parent.Get<Objects>().size();
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[1];

    if ( json.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json2.Get<UIntField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<UIntField1>() != 500) {
        log << "Invalid value for field1: " << json3.Get<UIntField1>() << endl;
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


int ParseEmbededBool(testLogger& log) {
    std::string rawJson = R"JSON({
        "Object": {
            "BoolField1": false,
            "BoolField2": true,
            "BoolField3": 0,
            "BoolField4": 1
        }
    }
    )JSON";

    typedef SimpleParsedJSON<
        BoolField1,
        BoolField2,
        BoolField3,
        BoolField4
    > JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json = parent.Get<Object>();

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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json2 = parent2.Get<Object>();

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json3 = parent3.Get<Object>();

    if (json3.Get<BoolField1>()) {
        log << "Field1 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField2>()) {
        log << "Field2 returned false!";
        return 1;
    }

    if (json3.Get<BoolField3>()) {
        log << "Field3 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField4>()) {
        log << "Field4 returned false!";
        return 1;
    }

    return 0;
}

int ParseEmbededArrayBool(testLogger& log) {
    std::string rawJson = R"JSON({
        "Objects": [{
            "BoolField1": false,
            "BoolField2": true,
            "BoolField3": 0,
            "BoolField4": 1
        },
        {
            "BoolField1": false,
            "BoolField2": true,
            "BoolField3": 0,
            "BoolField4": 1
        },
        {
            "BoolField1": false,
            "BoolField2": true,
            "BoolField3": 0,
            "BoolField4": 1
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<
        BoolField1,
        BoolField2,
        BoolField3,
        BoolField4
    > JSON;

    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[1];

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

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json2 = *parent2.Get<Objects>()[1];

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    JSON& json3 = *parent3.Get<Objects>()[1];

    if (json3.Get<BoolField1>()) {
        log << "Field1 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField2>()) {
        log << "Field2 returned false!";
        return 1;
    }

    if (json3.Get<BoolField3>()) {
        log << "Field3 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField4>()) {
        log << "Field4 returned false!";
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
    > json, json2, json3;

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

    string newRawJson = json.GetJSONString(true);

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

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if ( !ok ) {
        log << "Failed to parse bools!" << endl;
        log << error << endl;
        return 1;
    }

    if (json3.Get<BoolField1>()) {
        log << "Field1 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField2>()) {
        log << "Field2 returned false!";
        return 1;
    }

    if (json3.Get<BoolField3>()) {
        log << "Field3 returned true!";
        return 1;
    }

    if (!json3.Get<BoolField4>()) {
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
        "DoubleField4": 2000000000000,
        "DoubleField5": -2000000000001,
    }
    )JSON";

    typedef SimpleParsedJSON<
        DoubleField1,
        DoubleField2,
        DoubleField3,
        DoubleField4,
        DoubleField5
        > JSON;
        
    JSON json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    auto f = [&] (JSON& json) -> int {
        if ( json.Get<DoubleField1>() != 5.5) {
            log << "Invalid value for field1: " << json.Get<DoubleField1>() << endl;
            return 1;
        }

        if ( json.Get<DoubleField2>() != 5) {
            log << "Invalid value for field2: " << json.Get<DoubleField2>() << endl;
            return 1;
        }

        if ( json.Get<DoubleField3>() != -5) {
            log << "Invalid value for field3: " << json.Get<DoubleField3>() << endl;
            return 1;
        }

        if ( json.Get<DoubleField4>() != 2000000000000L) {
            log << "Invalid value for field4: " << json.Get<DoubleField4>() << endl;
            return 1;
        }

        if ( json.Get<DoubleField5>() != -2000000000001L) {
            log << "Invalid value for field5: " << json.Get<DoubleField5>() << endl;
            return 1;
        }

        return 0;
    };

    if ( f(json) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( f(json2) != 0 ) {
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( f(json3) != 0 ) {
        return 1;
    }

    return 0;
}

int ParseEmbededDouble(testLogger& log) {
    std::string rawJson = R"JSON({
        "Object": {
            "DoubleField1": 5.5,
            "DoubleField2": 5,
            "DoubleField3": -5,
        }
    }
    )JSON";

    typedef SimpleParsedJSON<
        DoubleField1,
        DoubleField2,
        DoubleField3
        > JSON;
    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object>();

    if ( json.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    return 0;
}

int ParseEmbededArrayDouble(testLogger& log) {
    std::string rawJson = R"JSON({
        "Objects": [{
            "DoubleField1": 0.5,
            "DoubleField2": 0,
            "DoubleField3": -1
        },
        {
            "DoubleField1": 5.5,
            "DoubleField2": 5,
            "DoubleField3": -5
        },
        {
            "DoubleField1": 2.5,
            "DoubleField2": 3,
            "DoubleField3": -4
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<
        DoubleField1,
        DoubleField2,
        DoubleField3
        > JSON;
    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = *parent.Get<Objects>()[1];

    if ( json.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json3.Get<DoubleField1>() << endl;
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

    json.Get<IntField1>() = 23;
    json.Get<UIntField1>() = 23;
    json.Get<I64Field1>() = 2304;
    json.Get<UI64Field1>() = 23984304;
    json.Get<DoubleField1>() = 3.141;
    json.Get<BoolField1>() = true;
    json.Get<Field1>() = "Hello!";
    json.Get<StringArrayField1>().push_back("Hello!");


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

    if ( json.Supplied<IntField1>()) {
        log << "Int supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<UIntField1>()) {
        log << "UInt supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<I64Field1>()) {
        log << "I64 supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<UI64Field1>()) {
        log << "UI64 supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<DoubleField1>()) {
        log << "Double supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<BoolField1>()) {
        log << "Bool supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<Field1>()) {
        log << "String supplied!" << endl;
        return 1;
    }

    if ( json.Supplied<StringArrayField1>()) {
        log << "String Array supplied!" << endl;
        return 1;
    }

    string ExpNotNull =
R"RAW({
    "StringArrayField1": [],
    "Field1": "",
    "BoolField1": false,
    "DoubleField1": 0.0,
    "UI64Field1": 0,
    "I64Field1": 0,
    "UIntField1": 0,
    "IntField1": 0
})RAW";
    string notNull = json.GetPrettyJSONString(false);
    if ( notNull != ExpNotNull) {
        log << "Incorrect JSON found for not-null print!" << endl;
        log << "Expected: " << endl;
        log << ExpNotNull << endl;
        log << "Actual: " << endl;
        log << notNull << endl;
        return 1;
    }

    string ExpNull =
R"RAW({
    "StringArrayField1": null,
    "Field1": null,
    "BoolField1": null,
    "DoubleField1": null,
    "UI64Field1": null,
    "I64Field1": null,
    "UIntField1": null,
    "IntField1": null
})RAW";

    string null = json.GetPrettyJSONString(true);
    if ( null != ExpNull) {
        log << "Incorrect JSON found for null print!" << endl;
        log << "Expected: " << endl;
        log << ExpNull << endl;
        log << "Actual: " << endl;
        log << null << endl;
        return 1;
    }

    return 0;
}

int EmbededClear(testLogger& log) {
    typedef SimpleParsedJSON<
        IntField1,
        UIntField1,
        I64Field1,
        UI64Field1,
        DoubleField1,
        BoolField1,
        Field1,
        StringArrayField1
        > JSON;

    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent;

    JSON& json = parent.Get<Object>();
    json.Get<IntField1>() = 23;
    json.Get<UIntField1>() = 23;
    json.Get<I64Field1>() = 2304;
    json.Get<UI64Field1>() = 23984304;
    json.Get<DoubleField1>() = 3.141;
    json.Get<BoolField1>() = true;
    json.Get<Field1>() = "Hello!";
    json.Get<StringArrayField1>().push_back("Hello!");

    parent.Clear();


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

    string ExpNotNull =
R"RAW({
    "Object": {
        "StringArrayField1": [],
        "Field1": "",
        "BoolField1": false,
        "DoubleField1": 0.0,
        "UI64Field1": 0,
        "I64Field1": 0,
        "UIntField1": 0,
        "IntField1": 0
    }
})RAW";
    string notNull = parent.GetPrettyJSONString(false);
    if ( notNull != ExpNotNull) {
        log << "Incorrect JSON found for not-null print!" << endl;
        log << "Expected: " << endl;
        log << ExpNotNull << endl;
        log << "Actual: " << endl;
        log << notNull << endl;
        return 1;
    }

    string ExpNull =
R"RAW({
    "Object": null
})RAW";

    string null = parent.GetPrettyJSONString(true);
    if ( null != ExpNull) {
        log << "Incorrect JSON found for null print!" << endl;
        log << "Expected: " << endl;
        log << ExpNull << endl;
        log << "Actual: " << endl;
        log << null << endl;
        return 1;
    }

    return 0;
}

int EmbededArrayClear(testLogger& log) {
    typedef SimpleParsedJSON<
        IntField1,
        UIntField1,
        I64Field1,
        UI64Field1,
        DoubleField1,
        BoolField1,
        Field1,
        StringArrayField1
        > JSON;

    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent;

    parent.Get<Objects>().emplace_back();
    JSON& json = *parent.Get<Objects>()[0];

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

    parent.Clear();

    if (parent.Get<Objects>().size() != 0 ) {
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


int TrippleEmbededObject(testLogger& log) {
    std::string rawJson = R"JSON({
        "Object3": {
            "Object2": { 
                "Object": {
                    "DoubleField1": 5.5,
                    "DoubleField2": 5,
                    "DoubleField3": -5
                }
            }
        }
    }
    )JSON";

    typedef SimpleParsedJSON<
        DoubleField1,
        DoubleField2,
        DoubleField3
        > JSON;
    NewEmbededObject(Object,JSON);

    typedef SimpleParsedJSON<Object> JSON2;
    NewEmbededObject(Object2,JSON2);

    typedef SimpleParsedJSON<Object2> JSON3;
    NewEmbededObject(Object3,JSON3);

    SimpleParsedJSON<Object3> parent, parent2, parent3;

    std::string error;
    bool ok = parent.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    JSON& json = parent.Get<Object3>().Get<Object2>().Get<Object>();

    if ( json.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object3>().Get<Object2>().Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json2.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json2.Get<DoubleField1>() << endl;
        return 1;
    }

    log << "Re-building JSON>..." << endl;

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object3>().Get<Object2>().Get<Object>();

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    }

    if ( json3.Get<DoubleField1>() != 5.5) {
        log << "Invalid value for field1: " << json3.Get<DoubleField1>() << endl;
        return 1;
    }

    return 0;
}

int EmbededObjectInArray(testLogger& log) {
    std::string rawJson = R"JSON({
        "OuterObjects": [
            {
                "Objects": [ 
                    {
                        "IntField1": 1
                    },
                    {
                        "IntField1": 2
                    }
                ]
            },
            {
                "Objects": [ 
                    {
                        "IntField1": 3
                    },
                    {
                        "IntField1": 4
                    },
                    {
                        "IntField1": 5
                    }
                ]
            }
        ]
    }
    )JSON";

    typedef SimpleParsedJSON<IntField1> Object;
    NewObjectArray(Objects,Object);

    NewObjectArray(OuterObjects,SimpleParsedJSON<Objects>);;

    typedef SimpleParsedJSON<OuterObjects> JSON;

    JSON json;

    std::string error;
    bool ok = json.Parse(rawJson.c_str(),error);

    if (!ok) {
        log << "Failed to parse: " << error;
        return 1;
    } else {
        log << ">> " << rawJson << endl;
        log << "<< " << json.GetJSONString(true) << endl;
    }

    auto& outerObjects = json.Get<OuterObjects>();

    if ( outerObjects.size() != 2) {
        log << "Invalid OuterObjects size: " << outerObjects.size() << endl;
        return 1;
    }

    auto& outer1 = *outerObjects[0];
    auto& outer2 = *outerObjects[1];

    auto& objects1 = outer1.Get<Objects>();
    auto& objects2 = outer2.Get<Objects>();

    if (objects1.size() != 2 ) {
        log << "Invalid size for objects1: " << objects1.size() << endl;
        return 1;
    }

    auto& object1 = *objects1[0];
    auto& object2 = *objects1[1];

    if ( object1.Get<IntField1>() != 1 ) {
        log << "Invalid value for object1: " << object1.Get<IntField1>() << endl;
        return 1;
    }

    if ( object2.Get<IntField1>() != 2 ) {
        log << "Invalid value for object2: " << object2.Get<IntField1>() << endl;
        return 1;
    }

    if (objects2.size() != 3 ) {
        log << "Invalid size for objects2: " << objects2.size() << endl;
        return 1;
    }

    auto& object3 = *objects2[0];
    auto& object4 = *objects2[1];
    auto& object5 = *objects2[2];

    if ( object3.Get<IntField1>() != 3 ) {
        log << "Invalid value for object3: " << object3.Get<IntField1>() << endl;
        return 1;
    }

    if ( object4.Get<IntField1>() != 4 ) {
        log << "Invalid value for object4: " << object4.Get<IntField1>() << endl;
        return 1;
    }

    if ( object5.Get<IntField1>() != 5 ) {
        log << "Invalid value for object5: " << object5.Get<IntField1>() << endl;
        return 1;
    }

    return 0;
}

namespace Supplied_JSON {
    NewBoolField(BoolField1);
    NewDoubleArrayField(DoubleArrayField1);
    NewDoubleField(DoubleField1);
    NewStringField(Field1);
    NewI64ArrayField(I64ArrayField1);
    NewUIntField(I64Field1);
    NewIntArrayField(IntArrayField1);
    NewUIntField(IntField1);
    NewStringArrayField(StringArrayField1);
    NewUI64ArrayField(UI64ArrayField1);
    NewUIntField(UI64Field1);
    NewUIntArrayField(UIntArrayField1);
    NewUIntField(UIntField1);
    NewBoolField(BoolField2);
    NewDoubleArrayField(DoubleArrayField2);
    NewDoubleField(DoubleField2);
    NewStringField(Field2);
    NewI64ArrayField(I64ArrayField2);
    NewUIntField(I64Field2);
    NewIntArrayField(IntArrayField2);
    NewUIntField(IntField2);
    NewStringArrayField(StringArrayField2);
    NewUI64ArrayField(UI64ArrayField2);
    NewUIntField(UI64Field2);
    NewUIntArrayField(UIntArrayField2);
    NewUIntField(UIntField2);
    NewBoolField(BoolField3);
    NewDoubleArrayField(DoubleArrayField3);
    NewDoubleField(DoubleField3);
    NewStringField(Field3);
    NewI64ArrayField(I64ArrayField3);
    NewUIntField(I64Field3);
    NewIntArrayField(IntArrayField3);
    NewUIntField(IntField3);
    NewStringArrayField(StringArrayField3);
    NewUI64ArrayField(UI64ArrayField3);
    NewUIntField(UI64Field3);
    NewUIntArrayField(UIntArrayField3);
    NewUIntField(UIntField3);

    typedef SimpleParsedJSON<
        BoolField1,
        DoubleArrayField1,
        DoubleField1,
        Field1,
        I64ArrayField1,
        I64Field1,
        IntArrayField1,
        IntField1,
        StringArrayField1,
        UI64ArrayField1,
        UI64Field1,
        UIntArrayField1,
        UIntField1,
        BoolField2,
        DoubleArrayField2,
        DoubleField2,
        Field2,
        I64ArrayField2,
        I64Field2,
        IntArrayField2,
        IntField2,
        StringArrayField2,
        UI64ArrayField2,
        UI64Field2,
        UIntArrayField2,
        UIntField2,
        BoolField3,
        DoubleArrayField3,
        DoubleField3,
        Field3,
        I64ArrayField3,
        I64Field3,
        IntArrayField3,
        IntField3,
        StringArrayField3,
        UI64ArrayField3,
        UI64Field3,
        UIntArrayField3,
        UIntField3
    > OutputJSON;

};
int Supplied(testLogger& log) {
    using namespace Supplied_JSON;
    string inputJSON =
R"RAW({
    "UIntField1": 1,
    "UIntArrayField1": [
        100
    ],
    "UI64Field1": 1,
    "UI64ArrayField1": [
        3423423432487
    ],
    "StringArrayField1": [
        "initial value"
    ],
    "IntField1": 1,
    "IntArrayField1": [
        -100
    ],
    "I64Field1": 1,
    "I64ArrayField1": [
        -3423423432487
    ],
    "Field1": "field1",
    "DoubleField1": 5.5,
    "DoubleArrayField1": [
        0.5
    ],
    "BoolField1": true,
    "UIntField2": null,
    "UIntArrayField2": null,
    "UI64Field2": null,
    "UI64ArrayField2": null,
    "StringArrayField2": null,
    "IntField2": null,
    "IntArrayField2": null,
    "I64Field2": null,
    "I64ArrayField2": null,
    "Field2": null,
    "DoubleField2": null,
    "DoubleArrayField2": null,
    "BoolField2": null,
})RAW";
    string expOutput =
R"RAW({
    "UIntField3": null,
    "UIntArrayField3": null,
    "UI64Field3": null,
    "UI64ArrayField3": null,
    "StringArrayField3": null,
    "IntField3": null,
    "IntArrayField3": null,
    "I64Field3": null,
    "I64ArrayField3": null,
    "Field3": null,
    "DoubleField3": null,
    "DoubleArrayField3": null,
    "BoolField3": null,
    "UIntField2": null,
    "UIntArrayField2": null,
    "UI64Field2": null,
    "UI64ArrayField2": null,
    "StringArrayField2": null,
    "IntField2": null,
    "IntArrayField2": null,
    "I64Field2": null,
    "I64ArrayField2": null,
    "Field2": null,
    "DoubleField2": null,
    "DoubleArrayField2": null,
    "BoolField2": null,
    "UIntField1": 1,
    "UIntArrayField1": [
        100
    ],
    "UI64Field1": 1,
    "UI64ArrayField1": [
        3423423432487
    ],
    "StringArrayField1": [
        "initial value"
    ],
    "IntField1": 1,
    "IntArrayField1": [
        -100
    ],
    "I64Field1": 1,
    "I64ArrayField1": [
        -3423423432487
    ],
    "Field1": "field1",
    "DoubleField1": 5.5,
    "DoubleArrayField1": [
        0.5
    ],
    "BoolField1": true
})RAW";
    string allNull =
R"RAW({
    "UIntField3": null,
    "UIntArrayField3": null,
    "UI64Field3": null,
    "UI64ArrayField3": null,
    "StringArrayField3": null,
    "IntField3": null,
    "IntArrayField3": null,
    "I64Field3": null,
    "I64ArrayField3": null,
    "Field3": null,
    "DoubleField3": null,
    "DoubleArrayField3": null,
    "BoolField3": null,
    "UIntField2": null,
    "UIntArrayField2": null,
    "UI64Field2": null,
    "UI64ArrayField2": null,
    "StringArrayField2": null,
    "IntField2": null,
    "IntArrayField2": null,
    "I64Field2": null,
    "I64ArrayField2": null,
    "Field2": null,
    "DoubleField2": null,
    "DoubleArrayField2": null,
    "BoolField2": null,
    "UIntField1": null,
    "UIntArrayField1": null,
    "UI64Field1": null,
    "UI64ArrayField1": null,
    "StringArrayField1": null,
    "IntField1": null,
    "IntArrayField1": null,
    "I64Field1": null,
    "I64ArrayField1": null,
    "Field1": null,
    "DoubleField1": null,
    "DoubleArrayField1": null,
    "BoolField1": null
})RAW";
    OutputJSON json;
    string error;
    if ( !json.Parse(inputJSON.c_str(), error) ) {
        log << "Failed to parse JSON: " << error << endl;
        return 1;
    }
    string output = json.GetPrettyJSONString(true);

    if ( output != expOutput ) {
        log << "Invalid output after parse" << endl;
        log << "Expected: " << endl << expOutput << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }

    json.Clear();
    output = json.GetPrettyJSONString(true);

    if ( output != allNull ) {
        log << "Invalid output after clear" << endl;
        log << "Expected: " << endl << allNull << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }

    return 0;
}

namespace Supplied_Embeded {

    namespace ObjectA1_fields {

        namespace ObjectB1_fields {

            namespace ObjectC1_fields {
                NewDoubleField(DoubleFieldA1);
                NewUIntField(DoubleFieldB1);
                NewIntField(DoubleFieldC1);

                typedef SimpleParsedJSON<
                    DoubleFieldA1,
                    DoubleFieldB1,
                    DoubleFieldC1
                > JSON;
            }
            NewEmbededObject(ObjectC1, ObjectC1_fields::JSON);

            typedef SimpleParsedJSON<
                ObjectC1
            > JSON;
        }
        NewEmbededObject(ObjectB1, ObjectB1_fields::JSON);

        typedef SimpleParsedJSON<
            ObjectB1
        > JSON;
    }
    NewEmbededObject(ObjectA1, ObjectA1_fields::JSON);

    namespace ObjectA2_fields {

        namespace ObjectB2_fields {

            namespace ObjectC2_fields {
                NewDoubleField(DoubleFieldA2);
                NewUIntField(DoubleFieldB2);
                NewIntField(DoubleFieldC2);

                typedef SimpleParsedJSON<
                    DoubleFieldA2,
                    DoubleFieldB2,
                    DoubleFieldC2
                > JSON;
            }
            NewEmbededObject(ObjectC2, ObjectC2_fields::JSON);

            typedef SimpleParsedJSON<
                ObjectC2
            > JSON;
        }
        NewEmbededObject(ObjectB2, ObjectB2_fields::JSON);

        typedef SimpleParsedJSON<
            ObjectB2
        > JSON;
    }
    NewEmbededObject(ObjectA2, ObjectA2_fields::JSON);

    namespace ObjectA3_fields {

        namespace ObjectB3_fields {

            namespace ObjectC3_fields {
                NewDoubleField(DoubleFieldA3);
                NewUIntField(DoubleFieldB3);
                NewIntField(DoubleFieldC3);

                typedef SimpleParsedJSON<
                    DoubleFieldA3,
                    DoubleFieldB3,
                    DoubleFieldC3
                > JSON;
            }
            NewEmbededObject(ObjectC3, ObjectC3_fields::JSON);

            typedef SimpleParsedJSON<
                ObjectC3
            > JSON;
        }
        NewEmbededObject(ObjectB3, ObjectB3_fields::JSON);

        typedef SimpleParsedJSON<
            ObjectB3
        > JSON;
    }
    NewEmbededObject(ObjectA3, ObjectA3_fields::JSON);

    namespace ObjectA4_fields {

        namespace ObjectB4_fields {

            namespace ObjectC4_fields {
                NewDoubleField(DoubleFieldA4);
                NewUIntField(DoubleFieldB4);
                NewIntField(DoubleFieldC4);

                typedef SimpleParsedJSON<
                    DoubleFieldA4,
                    DoubleFieldB4,
                    DoubleFieldC4
                > JSON;
            }
            NewEmbededObject(ObjectC4, ObjectC4_fields::JSON);

            typedef SimpleParsedJSON<
                ObjectC4
            > JSON;
        }
        NewEmbededObject(ObjectB4, ObjectB4_fields::JSON);

        typedef SimpleParsedJSON<
            ObjectB4
        > JSON;
    }
    NewEmbededObject(ObjectA4, ObjectA4_fields::JSON);

    typedef SimpleParsedJSON<
        ObjectA1,
        ObjectA2,
        ObjectA3,
        ObjectA4
    > OutputJSON;
};

int SuppliedEmbeded(testLogger& log) {
    using namespace Supplied_Embeded;
    string input =
R"RAW({
    "ObjectA1": {
        "ObjectB1": { 
            "ObjectC1": {
                "DoubleFieldA1": 5.5,
                "DoubleFieldB1": null
            }
        }
    },
    "ObjectA2": {
    },
    "ObjectA3": {
        "ObjectB3": null
    },
    "ObjectA4": null
})RAW";
    string expOutput =
R"RAW({
    "ObjectA4": null,
    "ObjectA3": {
        "ObjectB3": null
    },
    "ObjectA2": {
        "ObjectB2": null
    },
    "ObjectA1": {
        "ObjectB1": {
            "ObjectC1": {
                "DoubleFieldC1": null,
                "DoubleFieldB1": null,
                "DoubleFieldA1": 5.5
            }
        }
    }
})RAW";
    string allNull =
R"RAW({
    "ObjectA4": null,
    "ObjectA3": null,
    "ObjectA2": null,
    "ObjectA1": null
})RAW";
    OutputJSON json;
    string error;
    if ( !json.Parse(input.c_str(), error) ) {
        log << "Failed to parse JSON: " << error << endl;
        return 1;
    }

    string output = json.GetPrettyJSONString(true);

    if ( output != expOutput ) {
        log << "Invalid output after parse" << endl;
        log << "Expected: " << endl << expOutput << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }

    json.Clear();
    output = json.GetPrettyJSONString(true);

    if ( output != allNull ) {
        log << "Invalid output after clear" << endl;
        log << "Expected: " << endl << allNull << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }
    return 0;
}

namespace Supplied_EmbededInArray {
    namespace OuterObjects_fields {
        namespace Objects_fields {
            NewUIntField(IntField1);

            typedef SimpleParsedJSON<
                IntField1
            > JSON;
        }
        NewObjectArray(Objects, Objects_fields::JSON);

        typedef SimpleParsedJSON<
            Objects
        > JSON;
    }
    NewObjectArray(OuterObjects, OuterObjects_fields::JSON);

    typedef SimpleParsedJSON<
        OuterObjects
    > OutputJSON;
};

int SuppliedEmbededObjectInArray(testLogger& log) {
    using namespace Supplied_EmbededInArray;

    string input = R"RAW(
       {
            "OuterObjects": [
                {
                    "Objects": [ 
                        {
                            "IntField1": 1
                        },
                        {
                            "IntField1": null
                        },
                        {
                        }
                    ]
                },
                {
                    "Objects": null
                },
                {
                }
            ]
       }
    )RAW";

    string expOutput =
R"RAW({
    "OuterObjects": [
        {
            "Objects": [
                {
                    "IntField1": 1
                },
                {
                    "IntField1": null
                },
                {
                    "IntField1": null
                }
            ]
        },
        {
            "Objects": null
        },
        {
            "Objects": null
        }
    ]
})RAW";
    string allNull =
R"RAW({
    "OuterObjects": null
})RAW";
    OutputJSON json;
    string error;
    if ( !json.Parse(input.c_str(), error) ) {
        log << "Failed to parse JSON: " << error << endl;
        return 1;
    }

    string output = json.GetPrettyJSONString(true);

    if ( output != expOutput ) {
        log << "Invalid output after parse" << endl;
        log << "Expected: " << endl << expOutput << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }

    json.Clear();
    output = json.GetPrettyJSONString(true);

    if ( output != allNull ) {
        log << "Invalid output after clear" << endl;
        log << "Expected: " << endl << allNull << endl;
        log << "Got:      " << endl << output << endl;
        return 1;
    }

    return 0;
}
