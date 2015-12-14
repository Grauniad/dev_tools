#include "tester.h"
#include <SimpleJSON.h>

int SingleStringGen(testLogger& log);
int MultipleStringGen(testLogger& log);
int SingleUI64(testLogger& log);
int SingleI64(testLogger& log);
int SingleInt(testLogger& log);
int SingleUInt(testLogger& log);
int DoubleFields(testLogger& log);
int BooleanFields(testLogger& log);
int SingleEmbededObject(testLogger& log);
int IntArray(testLogger& log);
int I64Array(testLogger& log);
int UIntArray(testLogger& log);
int UI64Array(testLogger& log);
int DoubleArray(testLogger& log);
int StringArray(testLogger& log);
int SingleEmbededObjectWithArrays(testLogger& log);
int DoublyEmbededObject(testLogger& log);
int ArrayOfObjects(testLogger& log);
int ArrayOfObjects2(testLogger& log);

namespace {
}

int main(int argc, const char *argv[])
{
    Test("Testing generation of single string object",SingleStringGen).RunTest();
    Test("Testing generation of multiple string object",MultipleStringGen).RunTest();
    Test("Testing generation of single large unsigned int object",SingleUI64).RunTest();
    Test("Testing generation of single large int object",SingleI64).RunTest();
    Test("Testing generation of single int object",SingleInt).RunTest();
    Test("Testing generation of single unsigned int object",SingleUInt).RunTest();
    Test("Testing generation of double fields",DoubleFields).RunTest();
    Test("Testing generation of boolean fields in object",BooleanFields).RunTest();
    Test("Testing generation of a single embeded object",SingleEmbededObject).RunTest();
    Test("Testing generation of an object with an int array",IntArray).RunTest();
    Test("Testing generation of an object with a large int array",I64Array).RunTest();
    Test("Testing generation of an object with an unsigned int array",UIntArray).RunTest();
    Test("Testing generation of an object with a large unsigned int array",UI64Array).RunTest();
    Test("Testing generation of an object with a double array",DoubleArray).RunTest();
    Test("Testing generation of an object with a string array",StringArray).RunTest();
    Test("Testing generation of a single embeded object with arrays",SingleEmbededObjectWithArrays).RunTest();
    Test("Testing generation of a doubley embeded object",DoublyEmbededObject).RunTest();
    Test("Testing generation of array of objects",ArrayOfObjects).RunTest();
    Test("Testing generation of array of objects with more repeats",ArrayOfObjects2).RunTest();


    return 0;
}

int SingleStringGen(testLogger& log) {
    string input = R"RAW(
       {
           "stringField1": "Hello World!"
       }
    )RAW";
    string expected =
"    NewStringField(stringField1);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        stringField1\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int MultipleStringGen(testLogger& log) {
    string input = R"RAW(
       {
           "stringField1": "Hello World!",
           "stringField2": "Hello World!",
           "stringField3": "Hello World!",
       }
    )RAW";
    string expected =
"    NewStringField(stringField1);\n"
"    NewStringField(stringField2);\n"
"    NewStringField(stringField3);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        stringField1,\n"
"        stringField2,\n"
"        stringField3\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleUI64(testLogger& log) {
    string input = R"RAW(
       {
          "id": 505874924095815700
       }
    )RAW";
    string expected =
"    NewUI64Field(id);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        id\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleI64(testLogger& log) {
    string input = R"RAW(
       {
          "id": -505874924095815700
       }
    )RAW";
    string expected =
"    NewI64Field(id);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        id\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleUInt(testLogger& log) {
    string input = R"RAW(
       {
          "id": 50587
       }
    )RAW";
    string expected =
"    NewUIntField(id);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        id\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleInt(testLogger& log) {
    string input = R"RAW(
       {
          "id": -50587
       }
    )RAW";
    string expected =
"    NewIntField(id);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        id\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int IntArray(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              -5082,
              500
          ]
       }
    )RAW";
    string expected =
"    NewIntArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int I64Array(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              -5082000000000000,
              5000000000
          ]
       }
    )RAW";
    string expected =
"    NewI64ArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int DoubleArray(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              5082.0,
              -500
          ]
       }
    )RAW";
    string expected =
"    NewDoubleArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int StringArray(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              "ID 1",
              "ID 2",
          ]
       }
    )RAW";
    string expected =
"    NewStringArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int UIntArray(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              5082,
              -500
          ]
       }
    )RAW";
    string expected =
"    NewUIntArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int UI64Array(testLogger& log) {
    string input = R"RAW(
       {
          "ids": [
              5082000000000000,
              -5000000000
          ]
       }
    )RAW";
    string expected =
"    NewUI64ArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int DoubleFields(testLogger& log) {
    string input = R"RAW(
       {
          "d1": -50587.0,
          "d2":  50587.5,
          "d3":  1000000000000000000000000000000000
       }
    )RAW";
    string expected =
"    NewDoubleField(d1);\n"
"    NewDoubleField(d2);\n"
"    NewDoubleField(d3);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        d1,\n"
"        d2,\n"
"        d3\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int BooleanFields(testLogger& log) {
    string input = R"RAW(
       {
          "truncated": false,
          "profile_use_background_image": true
       }
    )RAW";
    string expected =
"    NewBoolField(profile_use_background_image);\n"
"    NewBoolField(truncated);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        profile_use_background_image,\n"
"        truncated\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleEmbededObject(testLogger& log) {
    string input = R"RAW(
       {
          "id": 505874924095815700,
          "metadata": {
            "result_type": "recent",
            "iso_language_code": "ja"
          },
          "created_at": "Sun Aug 31 00:29:15 +0000 2014"
       }
    )RAW";
    string expected =
"    NewStringField(created_at);\n"
"    NewUI64Field(id);\n"
"\n"
"    namespace metadata_fields {\n"
"        NewStringField(iso_language_code);\n"
"        NewStringField(result_type);\n"
"\n"
"        typedef SimpleParsedJSON<\n"
"            iso_language_code,\n"
"            result_type\n"
"        > JSON;\n"
"    }\n"
"    NewEmbededObject(metadata, metadata_fields::JSON);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        created_at,\n"
"        id,\n"
"        metadata\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int DoublyEmbededObject(testLogger& log) {
    string input = R"RAW(
       {
          "id": 505874924095815700,
          "metadata": {
            "result_type": "recent",
            "iso_language_code": "ja",
            "child": {
                "result_type": "recent",
                "alias_ids": [1,2],
                "iso_language_code": "ja",
                "aliases": ["alias1", "alias2"]
            }
          },
          "created_at": "Sun Aug 31 00:29:15 +0000 2014"
       }
    )RAW";
    string expected =
"    NewStringField(created_at);\n"
"    NewUI64Field(id);\n"
"\n"
"    namespace metadata_fields {\n"
"\n"
"        namespace child_fields {\n"
"            NewUIntArrayField(alias_ids);\n"
"            NewStringArrayField(aliases);\n"
"            NewStringField(iso_language_code);\n"
"            NewStringField(result_type);\n"
"\n"
"            typedef SimpleParsedJSON<\n"
"                alias_ids,\n"
"                aliases,\n"
"                iso_language_code,\n"
"                result_type\n"
"            > JSON;\n"
"        }\n"
"        NewEmbededObject(child, child_fields::JSON);\n"
"        NewStringField(iso_language_code);\n"
"        NewStringField(result_type);\n"
"\n"
"        typedef SimpleParsedJSON<\n"
"            child,\n"
"            iso_language_code,\n"
"            result_type\n"
"        > JSON;\n"
"    }\n"
"    NewEmbededObject(metadata, metadata_fields::JSON);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        created_at,\n"
"        id,\n"
"        metadata\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int SingleEmbededObjectWithArrays(testLogger& log) {
    string input = R"RAW(
       {
          "id": 505874924095815700,
          "metadata": {
            "result_type": "recent",
            "alias_ids": [1,2],
            "iso_language_code": "ja",
            "aliases": ["alias1", "alias2"]
          },
          "created_at": "Sun Aug 31 00:29:15 +0000 2014"
       }
    )RAW";
    string expected =
"    NewStringField(created_at);\n"
"    NewUI64Field(id);\n"
"\n"
"    namespace metadata_fields {\n"
"        NewUIntArrayField(alias_ids);\n"
"        NewStringArrayField(aliases);\n"
"        NewStringField(iso_language_code);\n"
"        NewStringField(result_type);\n"
"\n"
"        typedef SimpleParsedJSON<\n"
"            alias_ids,\n"
"            aliases,\n"
"            iso_language_code,\n"
"            result_type\n"
"        > JSON;\n"
"    }\n"
"    NewEmbededObject(metadata, metadata_fields::JSON);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        created_at,\n"
"        id,\n"
"        metadata\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int ArrayOfObjects(testLogger& log ) {
    string input = R"RAW(
       {
          "id": 505874924095815700,
          "urls": [
            {
              "url": "http://t.co/QMLJeFmfMT",
              "expanded_url": "http://www.pixiv.net/member.php?id=4776",
              "display_url": "pixiv.net/member.php?id=…",
              "indices": [
                58,
                80
              ]
            },
            {
              "url": "http://t.co/LU8T7vmU3h",
              "expanded_url": "http://ask.fm/KATANA77",
              "display_url": "ask.fm/KATANA77"
            }
          ],
          "created_at": "Sun Aug 31 00:29:15 +0000 2014"
       }
    )RAW";
    string expected =
"    NewStringField(created_at);\n"
"    NewUI64Field(id);\n"
"\n"
"    namespace urls_fields {\n"
"        NewStringField(display_url);\n"
"        NewStringField(expanded_url);\n"
"        NewUIntArrayField(indices);\n"
"        NewStringField(url);\n"
"\n"
"        typedef SimpleParsedJSON<\n"
"            display_url,\n"
"            expanded_url,\n"
"            indices,\n"
"            url\n"
"        > JSON;\n"
"    }\n"
"    NewObjectArray(urls, urls_fields::JSON);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        created_at,\n"
"        id,\n"
"        urls\n"
"    > OutputJSON;\n";

    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}

int ArrayOfObjects2(testLogger& log ) {
    string input = R"RAW(
       {
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
    )RAW";
    string expected =
R"RAW(
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
)RAW";
    string output = spJSON::Gen("OutputJSON", input);

    if (output != expected) {
        log << "Missmatch in generated code" << endl;
        log << "Expected: " << endl;
        log << expected << endl;
        log << endl;
        log << "Got: " << endl;
        log << output << endl;
        return 1;
    }
    return 0;
}
