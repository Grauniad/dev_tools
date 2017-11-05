#include "gtest/gtest.h"
#include <SimpleJSON.h>

using namespace std;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(JSONParsing, SingleStringGen) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleTimeGen) {
    string input = R"RAW(
       {
           "timeField1": "2015-07-13T05:38:17.33640392Z"
       }
    )RAW";
    string expected =
"    NewTimeField(timeField1);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        timeField1\n"
"    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    ASSERT_EQ(output, expected);
}

TEST(JSONParsing, IgnoreNull) {
    string input = R"RAW(
       {
           "stringField1": "Hello World!",
           "dummyField": null
       }
    )RAW";
    string expected =
            "    NewStringField(stringField1);\n"
                    "\n"
                    "    typedef SimpleParsedJSON<\n"
                    "        stringField1\n"
                    "    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, MultipleStringGen) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleUI64) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleI64) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleUInt) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleInt) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, IntArray) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, IntArrayIgnoreNull) {
    string input = R"RAW(
       {
          "ids": [
              -5082,
              500,
              null
          ]
       }
    )RAW";
    string expected =
"    NewIntArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, ArrayIgnoreAllNull) {
    string input = R"RAW(
       {
          "ids": [-5, 10],
          "ignore": [null]
          "ignore2": [null, null]
       }
    )RAW";
    string expected =
            "    NewIntArrayField(ids);\n"
                    "\n"
                    "    typedef SimpleParsedJSON<\n"
                    "        ids\n"
                    "    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, ArrayIgnoreEmpty) {
    string input = R"RAW(
       {
          "ids": [-5, 10],
          "ignore": [ ]
       }
    )RAW";
    string expected =
            "    NewIntArrayField(ids);\n"
                    "\n"
                    "    typedef SimpleParsedJSON<\n"
                    "        ids\n"
                    "    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, IntArrayIgnoreNullFirst) {
    string input = R"RAW(
       {
          "ids": [
              null,
              -5082,
              500,
              null
          ]
       }
    )RAW";
    string expected =
            "    NewIntArrayField(ids);\n"
                    "\n"
                    "    typedef SimpleParsedJSON<\n"
                    "        ids\n"
                    "    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, I64Array) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, DoubleArray) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, StringArray) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, UIntArray) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, UI64Array) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, DoubleFields) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, BooleanFields) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, BooleanArrayFields) {
    string input = R"RAW(
       {
          "truncated": [false, true],
          "profile_use_background_image": [true]
       }
    )RAW";
    string expected =
            "    NewBoolArrayField(profile_use_background_image);\n"
                    "    NewBoolArrayField(truncated);\n"
                    "\n"
                    "    typedef SimpleParsedJSON<\n"
                    "        profile_use_background_image,\n"
                    "        truncated\n"
                    "    > OutputJSON;\n";
    string output = spJSON::Gen("OutputJSON", input);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, BooleanArrayFieldsWithNull) {
    string input = R"RAW(
       {
          "truncated": [false, true, null],
          "profile_use_background_image": [null, true]
       }
    )RAW";
    string expected =
            "    NewBoolArrayField(profile_use_background_image);\n"
            "    NewBoolArrayField(truncated);\n"
            "\n"
            "    typedef SimpleParsedJSON<\n"
            "        profile_use_background_image,\n"
            "        truncated\n"
            "    > OutputJSON;\n";
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, SingleEmbededObject) {
    string input = R"RAW(
       {
          "id": 505874924095815700,
          "metadata": {
            "result_type": "recent",
            "iso_language_code": "ja",
            "dummy": null
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
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing, DoublyEmbededObject) {
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
                "aliases": ["alias1", "alias2"],
                "dummy": null
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

    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONParsing,SingleEmbededObjectWithArrays) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects_NoMerge) {
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
              "display_url": "ask.fm/KATANA77",
              "new_string": "This shouldn't be int the parse"
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

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects_Merge) {
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
              "display_url": "ask.fm/KATANA77",
              "new_string": "This shouldn't be int the parse"
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
                    "        NewStringField(new_string);\n"
                    "        NewStringField(url);\n"
                    "\n"
                    "        typedef SimpleParsedJSON<\n"
                    "            display_url,\n"
                    "            expanded_url,\n"
                    "            indices,\n"
                    "            new_string,\n"
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

    spJSON::GeneratorOptions options;
    options.mergeFields = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects_MergeNull) {
string input = R"RAW(
       {
          "id": 505874924095815700,
          "urls": [
            null,
            {
            },
            {
              "url": null,
              "expanded_url": null,
              "display_url": null,
              "indices": null
            },
            {
              "indices": [null]
            },
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
              "display_url": "ask.fm/KATANA77",
              "new_string": "This shouldn't be int the parse"
            },
            {
              "url": null,
              "expanded_url": null,
              "display_url": null,
              "indices": null
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
                "        NewStringField(new_string);\n"
                "        NewStringField(url);\n"
                "\n"
                "        typedef SimpleParsedJSON<\n"
                "            display_url,\n"
                "            expanded_url,\n"
                "            indices,\n"
                "            new_string,\n"
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

spJSON::GeneratorOptions options;
options.mergeFields = true;
options.ignoreNull = true;
string output = spJSON::Gen("OutputJSON", input, options);

ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects2) {
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

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects2_NoMerge) {
string input = R"RAW(
       {
            "OuterObjects": [
                {
                    "Objects": [
                        {
                            "IntField1": 1
                        },
                        {
                            "IntField2": 2
                        }
                    ]
                },
                {
                    "Objects": [
                        {
                            "IntField1": 3
                        },
                        {
                            "IntField2": 4
                        },
                        {
                            "IntField3": 5
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
    spJSON::GeneratorOptions options;
    options.mergeFields = false;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects2_Merge) {
    string input = R"RAW(
           {
                "OuterObjects": [
                    {
                        "Objects": [
                            {
                                "IntField1": 1
                            },
                            {
                                "IntField2": 2
                            }
                        ]
                    },
                    {
                        "Objects": [
                            {
                                "IntField1": 3
                            },
                            {
                                "IntField2": 4
                            },
                            {
                                "IntField3": 5
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
            NewUIntField(IntField2);
            NewUIntField(IntField3);

            typedef SimpleParsedJSON<
                IntField1,
                IntField2,
                IntField3
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
    spJSON::GeneratorOptions options;
    options.mergeFields = true;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects2_MultiMerge) {
    string input = R"RAW(
           {
                "OuterObjects": [
                    {
                        "Objects": [
                            {
                                "IntField1": 1
                            },
                            {
                                "IntField2": 2
                            }
                        ],
                        "Objects2": [
                            {
                                "IntField21": 1
                            },
                            {
                                "IntField22": 2
                            }
                        ]
                    },
                    {
                        "Dummy": {
                            "dummyField": "String"
                        },
                        "Objects": [
                            {
                                "IntField1": 3
                            },
                            {
                                "IntField2": 4
                            },
                            {
                                "IntField3": 5
                            }
                        ]
                    }
                ],
                "Objects": [
                    {
                        "outer1": 1
                    },
                    {
                        "outer2": 2
                    }
                ]
           }
        )RAW";
    string expected =
            R"RAW(
    namespace Objects_fields {
        NewUIntField(outer1);
        NewUIntField(outer2);

        typedef SimpleParsedJSON<
            outer1,
            outer2
        > JSON;
    }
    NewObjectArray(Objects, Objects_fields::JSON);

    namespace OuterObjects_fields {

        namespace Dummy_fields {
            NewStringField(dummyField);

            typedef SimpleParsedJSON<
                dummyField
            > JSON;
        }
        NewEmbededObject(Dummy, Dummy_fields::JSON);

        namespace Objects_fields {
            NewUIntField(IntField1);
            NewUIntField(IntField2);
            NewUIntField(IntField3);

            typedef SimpleParsedJSON<
                IntField1,
                IntField2,
                IntField3
            > JSON;
        }
        NewObjectArray(Objects, Objects_fields::JSON);

        namespace Objects2_fields {
            NewUIntField(IntField21);
            NewUIntField(IntField22);

            typedef SimpleParsedJSON<
                IntField21,
                IntField22
            > JSON;
        }
        NewObjectArray(Objects2, Objects2_fields::JSON);

        typedef SimpleParsedJSON<
            Dummy,
            Objects,
            Objects2
        > JSON;
    }
    NewObjectArray(OuterObjects, OuterObjects_fields::JSON);

    typedef SimpleParsedJSON<
        Objects,
        OuterObjects
    > OutputJSON;
)RAW";
    spJSON::GeneratorOptions options;
    options.mergeFields = true;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfObjects2WithNulls) {
    string input = R"RAW(
       {
            "OuterObjects": [
                null,
                {
                    "Objects": [
                        null,
                        {
                            "IntField1": 1
                        },
                        {
                            "IntField1": 2
                        },
                        null
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
                        },
                        null
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
    spJSON::GeneratorOptions options;
    options.ignoreNull = true;
    string output = spJSON::Gen("OutputJSON", input, options);

    ASSERT_EQ(output , expected);
}

TEST(JSONGen, ArrayOfTimestamps) {
    string input = R"RAW(
       {
          "ids": [
              "2015-07-13T05:38:17.33640392Z",
              ""
          ]
       }
    )RAW";
    string expected =
"    NewTimeArrayField(ids);\n"
"\n"
"    typedef SimpleParsedJSON<\n"
"        ids\n"
"    > OutputJSON;\n";

    string output = spJSON::Gen("OutputJSON", input);

    ASSERT_EQ(output, expected);
}
