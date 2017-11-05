#include "gtest/gtest.h"
#include <SimpleJSON.h>
#include <util_time.h>
#include <iostream>

using namespace std;

const std::string EpochTimestamp = Time::EpochTimestamp;

int main(int argc, char **argv) {
    std::cout <<  EpochTimestamp << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
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
NewTimeField(TimeField1);
NewTimeField(TimeField2);
NewTimeArrayField(TimeArrayField1);
NewTimeArrayField(TimeArrayField2);


TEST(JSONParsing, SingleString) {
    std::string rawJson = R"JSON( 
    {
        "Field1": "Hello World!",
        "Field2": ""
    }
    )JSON";

    SimpleParsedJSON<Field1,Field2> json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    ASSERT_TRUE(ok);

    ASSERT_EQ( json.Get<Field1>(), "Hello World!");
    ASSERT_EQ( json.Get<Field2>() , "");

    std::string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ( json2.Get<Field1>() , "Hello World!");
    ASSERT_EQ( json2.Get<Field2>() , "");

    std::string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ( json3.Get<Field1>() , "Hello World!");
    ASSERT_EQ( json3.Get<Field2>() , "");
}

TEST(JSONParsing, ParseTime) {
    std::string rawJson = R"JSON( 
    {
        "TimeField1": "2015-07-13T05:38:17.33640392Z",
        "TimeField2": "",
    }
    )JSON";

    SimpleParsedJSON<TimeField1,TimeField2> json, json2, json3;

    std::string error;

    ASSERT_TRUE(json.Parse(rawJson.c_str(),error));

    const std::string expTime = "2015-07-13T05:38:17.336403Z";
    ASSERT_EQ(expTime, json.Get<TimeField1>().ISO8601Timestamp());
    ASSERT_EQ(EpochTimestamp, json.Get<TimeField2>().Timestamp());

    string newRawJson = json.GetJSONString();

    ASSERT_TRUE(json2.Parse(newRawJson.c_str(), error));

    ASSERT_EQ(json2.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json2.Get<TimeField2>().Timestamp(), EpochTimestamp);

    string newRawJson2 = json.GetPrettyJSONString();

    ASSERT_TRUE(json3.Parse(newRawJson2.c_str(), error));

    ASSERT_EQ(json3.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json3.Get<TimeField2>().Timestamp(), EpochTimestamp);
}

TEST(JSONParsing, ParseStringArray) {
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

    ASSERT_TRUE(ok);

    const vector<string>& v1 = json.Get<StringArrayField1>();
    const vector<string>& v2 = json.Get<StringArrayField2>();

    ASSERT_EQ(v1.size(),4 );

    ASSERT_EQ(v1[0],"String 1");

    ASSERT_EQ(v1[1],"String 2");

    ASSERT_EQ(v1[2],"");

    ASSERT_EQ(v1[3],"String 4");

    ASSERT_EQ(v2.size(),0 );


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<string>& v31 = json2.Get<StringArrayField1>();
    const vector<string>& v32 = json2.Get<StringArrayField2>();

    ASSERT_EQ(v31.size(),4 );

    ASSERT_EQ(v31[0],"String 1");

    ASSERT_EQ(v31[1],"String 2");

    ASSERT_EQ(v31[2],"");

    ASSERT_EQ(v31[3],"String 4");

    ASSERT_EQ(v32.size(),0 );


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<string>& v21 = json3.Get<StringArrayField1>();
    const vector<string>& v22 = json3.Get<StringArrayField2>();

    ASSERT_EQ(v21.size(),4 );

    ASSERT_EQ(v21[0],"String 1");

    ASSERT_EQ(v21[1],"String 2");

    ASSERT_EQ(v21[2],"");

    ASSERT_EQ(v21[3],"String 4");

    ASSERT_EQ(v22.size(),0 );

}

TEST(JSONParsing, ParseEmbededTime) {
    std::string rawJson = R"JSON(
    {
        "Object" : {
            "TimeField1": "2015-07-13T05:38:17.33640392Z",
            "TimeField2": "",
        }
    }
    )JSON";

    typedef SimpleParsedJSON<TimeField1,TimeField2> JSON; 
    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;


    std::string error;

    ASSERT_TRUE(parent.Parse(rawJson.c_str(),error));

    JSON& json = parent.Get<Object>();

    const std::string expTime = "2015-07-13T05:38:17.336403Z";
    ASSERT_EQ(json.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json.Get<TimeField2>().Timestamp(), EpochTimestamp);

    string newRawJson = parent.GetJSONString(true);
    ASSERT_TRUE(parent2.Parse(newRawJson.c_str(), error));

    JSON& json2 = parent2.Get<Object>();

    ASSERT_EQ(json2.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json2.Get<TimeField2>().Timestamp(), EpochTimestamp);

    string newRawJson2 = parent.GetPrettyJSONString();

    ASSERT_TRUE(parent3.Parse(newRawJson2.c_str(), error));

    JSON& json3 = parent3.Get<Object>();

    ASSERT_EQ(json3.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json3.Get<TimeField2>().Timestamp(), EpochTimestamp);
}

TEST(JSONParsing, ParseEmbededStringArray) {
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    const vector<string>& v1 = json.Get<StringArrayField1>();
    const vector<string>& v2 = json.Get<StringArrayField2>();

    ASSERT_EQ(v1.size(),4 );

    ASSERT_EQ(v1[0],"String 1");

    ASSERT_EQ(v1[1],"String 2");

    ASSERT_EQ(v1[2],"");

    ASSERT_EQ(v1[3],"String 4");

    ASSERT_EQ(v2.size(),0 );


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = parent2.Get<Object>();

    const vector<string>& v21 = json2.Get<StringArrayField1>();
    const vector<string>& v22 = json2.Get<StringArrayField2>();

    ASSERT_EQ(v21.size(),4 );

    ASSERT_EQ(v21[0],"String 1");

    ASSERT_EQ(v21[1],"String 2");

    ASSERT_EQ(v21[2],"");

    ASSERT_EQ(v21[3],"String 4");

    ASSERT_EQ(v22.size(),0 );


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = parent3.Get<Object>();

    const vector<string>& v31 = json3.Get<StringArrayField1>();
    const vector<string>& v32 = json3.Get<StringArrayField2>();

    ASSERT_EQ(v31.size(),4 );

    ASSERT_EQ(v31[0],"String 1");

    ASSERT_EQ(v31[1],"String 2");

    ASSERT_EQ(v31[2],"");

    ASSERT_EQ(v31[3],"String 4");

    ASSERT_EQ(v32.size(),0 );

}

TEST(JSONParsing, ParseEmbededString) {
    std::string rawJson = R"JSON(
    {
        "Object" : {
            "Field1": "Hello World!",
            "Field2": ""
        }
    }
    )JSON";

    typedef SimpleParsedJSON<Field1,Field2> JSON; 
    NewEmbededObject(Object,JSON);
    SimpleParsedJSON<Object> parent, parent2, parent3;


    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    ASSERT_EQ(json.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json.Get<Field2>(),"");

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_EQ(json2.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json2.Get<Field2>(),"");

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_EQ(json3.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json3.Get<Field2>(),"");
}

TEST(JSONParsing, ParseEmbededArrayString) {
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

    ASSERT_TRUE(ok);

    JSON& json = *parent.Get<Objects>()[0];
    JSON& bjson = *parent.Get<Objects>()[1];

    ASSERT_EQ(json.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json.Get<Field2>(),"");

    ASSERT_EQ(bjson.Get<Field1>(),"Another string");

    ASSERT_EQ(bjson.Get<Field2>(),"Not a blank string");

    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(),error);

    ASSERT_TRUE(ok);

    JSON& json2 = *parent.Get<Objects>()[0];
    JSON& bjson2 = *parent.Get<Objects>()[1];

    ASSERT_EQ(json2.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json2.Get<Field2>(),"");

    ASSERT_EQ(bjson2.Get<Field1>(),"Another string");

    ASSERT_EQ(bjson2.Get<Field2>(),"Not a blank string");

    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(),error);

    ASSERT_TRUE(ok);

    JSON& json3 = *parent.Get<Objects>()[0];
    JSON& bjson3 = *parent.Get<Objects>()[1];

    ASSERT_EQ(json3.Get<Field1>(),"Hello World!");

    ASSERT_EQ(json3.Get<Field2>(),"");

    ASSERT_EQ(bjson3.Get<Field1>(),"Another string");

    ASSERT_EQ(bjson3.Get<Field2>(),"Not a blank string");

}

TEST(JSONParsing, ParseEmbededArrayTime) {
    std::string rawJson = R"JSON(
    {
        "Objects": [{
            "TimeField1": "2015-07-13T05:38:17.33640392Z",
            "TimeField2": ""
        }, {
            "TimeField1": "2016-07-13T05:38:17.33640392Z",
            "TimeField2": "2017-07-13T05:38:17.33640392Z"
        }]
    }
    )JSON";

    typedef SimpleParsedJSON<TimeField1,TimeField2> JSON; 
    NewObjectArray(Objects,JSON);
    SimpleParsedJSON<Objects> parent, parent2, parent3;

    std::string error;

    ASSERT_TRUE(parent.Parse(rawJson.c_str(),error));

    JSON& json = *parent.Get<Objects>()[0];
    JSON& bjson = *parent.Get<Objects>()[1];

    const std::string expTime = "2015-07-13T05:38:17.336403Z";
    const std::string expTimeb1 = "2016-07-13T05:38:17.336403Z";
    const std::string expTimeb2 = "2017-07-13T05:38:17.336403Z";

    ASSERT_EQ(json.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json.Get<TimeField2>().Timestamp(), EpochTimestamp);
    ASSERT_EQ(bjson.Get<TimeField1>().ISO8601Timestamp(), expTimeb1);
    ASSERT_EQ(bjson.Get<TimeField2>().ISO8601Timestamp(), expTimeb2);

    string newRawJson = parent.GetJSONString(true);

    ASSERT_TRUE(parent2.Parse(newRawJson.c_str(),error));

    JSON& json2 = *parent.Get<Objects>()[0];
    JSON& bjson2 = *parent.Get<Objects>()[1];

    ASSERT_EQ(json2.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json2.Get<TimeField2>().Timestamp(), EpochTimestamp);
    ASSERT_EQ(bjson2.Get<TimeField1>().ISO8601Timestamp(), expTimeb1);
    ASSERT_EQ(bjson2.Get<TimeField2>().ISO8601Timestamp(), expTimeb2);

    string newRawJson2 = parent.GetPrettyJSONString();

    ASSERT_TRUE(parent3.Parse(newRawJson2.c_str(),error));

    JSON& json3 = *parent.Get<Objects>()[0];
    JSON& bjson3 = *parent.Get<Objects>()[1];

    ASSERT_EQ(json3.Get<TimeField1>().ISO8601Timestamp(), expTime);
    ASSERT_EQ(json3.Get<TimeField2>().Timestamp(), EpochTimestamp);
    ASSERT_EQ(bjson3.Get<TimeField1>().ISO8601Timestamp(), expTimeb1);
    ASSERT_EQ(bjson3.Get<TimeField2>().ISO8601Timestamp(), expTimeb2);

}

TEST(JSONParsing,ParseInt) {

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
        "DoubleField2": -500
    }
    )JSON";

    bool ok = json.Parse(rawJson.c_str(),error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<IntField1>() , 500);

    ASSERT_EQ(json.Get<IntField2>() , -500);

    ASSERT_EQ(json.Get<DoubleField1>() , 500);

    ASSERT_EQ(json.Get<DoubleField2>() , -500);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<IntField1>() , 500);

    ASSERT_EQ(json2.Get<IntField2>() , -500);

    ASSERT_EQ(json2.Get<DoubleField1>() , 500);

    ASSERT_EQ(json2.Get<DoubleField2>() , -500);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<IntField1>() , 500);

    ASSERT_EQ(json3.Get<IntField2>() , -500);

    ASSERT_EQ(json3.Get<DoubleField1>() , 500);

    ASSERT_EQ(json3.Get<DoubleField2>() , -500);

}

TEST(JSONParsing,ParseIntArray) {
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

    ASSERT_TRUE(ok);

    const vector<int>& v1 = json.Get<IntArrayField1>();
    const vector<int>& v2 = json.Get<IntArrayField2>();

    auto f = [&] (const vector<int>& v1, const vector<int>& v2) {
        ASSERT_EQ(v1.size() , 4 );

        ASSERT_EQ(v1[0] , 500);

        ASSERT_EQ(v1[1] , -134);

        ASSERT_EQ(v1[2] , 0);

        ASSERT_EQ(v1[3] , 23);

        ASSERT_EQ(v2.size() , 0 );
    };

    f(v1,v2);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<int>& v21 = json2.Get<IntArrayField1>();
    const vector<int>& v22 = json2.Get<IntArrayField2>();

    f(v21,v22);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<int>& v31 = json3.Get<IntArrayField1>();
    const vector<int>& v32 = json3.Get<IntArrayField2>();

    f(v31,v32);

}

TEST(JSONParsing,ParseEmbededInt) {
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
            "DoubleField2": -500
        }
    }
    )JSON";

    bool ok = parent.Parse(rawJson.c_str(),error);

    JSON& json = parent.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<IntField1>() , 500);

    ASSERT_EQ(json.Get<IntField2>() , -500);

    ASSERT_EQ(json.Get<DoubleField1>() , 500);

    ASSERT_EQ(json.Get<DoubleField2>() , -500);



    string newRawJson = parent.GetJSONString(true);


    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<IntField1>() , 500);

    ASSERT_EQ(json2.Get<IntField2>() , -500);

    ASSERT_EQ(json2.Get<DoubleField1>() , 500);

    ASSERT_EQ(json2.Get<DoubleField2>() , -500);


    string newRawJson2 = parent.GetPrettyJSONString();


    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<IntField1>() , 500);

    ASSERT_EQ(json3.Get<IntField2>() , -500);

    ASSERT_EQ(json3.Get<DoubleField1>() , 500);

    ASSERT_EQ(json3.Get<DoubleField2>() , -500);

}

TEST(JSONParsing,ParseEmbededArrayInt) {
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

    ASSERT_TRUE(ok);

    ASSERT_EQ(parent.Get<Objects>().size() , 3);

    ASSERT_EQ(json.Get<IntField1>() , 500);

    ASSERT_EQ(json.Get<IntField2>() , -500);

    ASSERT_EQ(json.Get<DoubleField1>() , 500);

    ASSERT_EQ(json.Get<DoubleField2>() , -500);



    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<IntField1>() , 500);

    ASSERT_EQ(json2.Get<IntField2>() , -500);

    ASSERT_EQ(json2.Get<DoubleField1>() , 500);

    ASSERT_EQ(json2.Get<DoubleField2>() , -500);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<IntField1>() , 500);

    ASSERT_EQ(json3.Get<IntField2>() , -500);

    ASSERT_EQ(json3.Get<DoubleField1>() , 500);

    ASSERT_EQ(json3.Get<DoubleField2>() , -500);

}

TEST(JSONParsing,ParseI64) {
    std::string rawJson = R"JSON( 
    {
        "I64Field1": -500,
        "I64Field2": 500,
        "I64Field3": 5147483658,
        "I64Field4": -2147483658,
        "DoubleField1": 5147483658,
        "DoubleField2": -2147483658
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

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<I64Field1>() , -500);

    ASSERT_EQ(json.Get<I64Field2>() , 500);

    ASSERT_EQ(json.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json.Get<DoubleField2>() , -2147483658);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<I64Field1>() , -500);

    ASSERT_EQ(json2.Get<I64Field2>() , 500);

    ASSERT_EQ(json2.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json2.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json2.Get<DoubleField2>() , -2147483658);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<I64Field1>() , -500);

    ASSERT_EQ(json3.Get<I64Field2>() , 500);

    ASSERT_EQ(json3.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json3.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json3.Get<DoubleField2>() , -2147483658);

}

TEST(JSONParsing,ParseI64Array) {
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

    ASSERT_TRUE(ok);

    const vector<int64_t>& v1 = json.Get<I64ArrayField1>();
    const vector<int64_t>& v2 = json.Get<I64ArrayField2>();

    auto f = [&] (const vector<int64_t>& v1, const vector<int64_t>& v2) {
        ASSERT_EQ(v1.size() , 4 );

        ASSERT_EQ(v1[0] , -500);

        ASSERT_EQ(v1[1] , 500);

        ASSERT_EQ(v1[2] , 5147483658);

        ASSERT_EQ(v1[3] , -2147483658);

        ASSERT_EQ(v2.size() , 0 );

    };

    f(v1,v2);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<int64_t>& v21 = json2.Get<I64ArrayField1>();
    const vector<int64_t>& v22 = json2.Get<I64ArrayField2>();

    f(v21,v22);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<int64_t>& v31 = json3.Get<I64ArrayField1>();
    const vector<int64_t>& v32 = json3.Get<I64ArrayField2>();

    f(v31,v32);

}


TEST(JSONParsing,ParseEmbededI64) {
    std::string rawJson = R"JSON({
        "Object": {
            "I64Field1": -500,
            "I64Field2": 500,
            "I64Field3": 5147483658,
            "I64Field4": -2147483658,
            "DoubleField1": 5147483658,
            "DoubleField2": -2147483658
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    ASSERT_EQ(json.Get<I64Field1>() , -500);

    ASSERT_EQ(json.Get<I64Field2>() , 500);

    ASSERT_EQ(json.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json.Get<DoubleField2>() , -2147483658);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_EQ(json2.Get<I64Field1>() , -500);

    ASSERT_EQ(json2.Get<I64Field2>() , 500);

    ASSERT_EQ(json2.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json2.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json2.Get<DoubleField2>() , -2147483658);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_EQ(json3.Get<I64Field1>() , -500);

    ASSERT_EQ(json3.Get<I64Field2>() , 500);

    ASSERT_EQ(json3.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json3.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json3.Get<DoubleField2>() , -2147483658);

}

TEST(JSONParsing,ParseEmbededArrayI64) {
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

    ASSERT_TRUE(ok);

    JSON& json = *parent.Get<Objects>()[1];

    ASSERT_EQ(json.Get<I64Field1>() , -500);

    ASSERT_EQ(json.Get<I64Field2>() , 500);

    ASSERT_EQ(json.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json.Get<DoubleField2>() , -2147483658);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_EQ(json2.Get<I64Field1>() , -500);

    ASSERT_EQ(json2.Get<I64Field2>() , 500);

    ASSERT_EQ(json2.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json2.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json2.Get<DoubleField2>() , -2147483658);


    string newRawJson22 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson22.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_EQ(json3.Get<I64Field1>() , -500);

    ASSERT_EQ(json3.Get<I64Field2>() , 500);

    ASSERT_EQ(json3.Get<I64Field3>() , 5147483658);

    ASSERT_EQ(json3.Get<I64Field4>() , -2147483658);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5147483658);

    ASSERT_EQ(json3.Get<DoubleField2>() , -2147483658);

}

TEST(JSONParsing,ParseUI64) {
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

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<UI64Field1>() , value1);

    ASSERT_EQ(json.Get<UI64Field2>() , value2);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<UI64Field1>() , value1);

    ASSERT_EQ(json2.Get<UI64Field2>() , value2);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<UI64Field1>() , value1);

    ASSERT_EQ(json3.Get<UI64Field2>() , value2);

}

TEST(JSONParsing,ParseUI64Array) {
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

    ASSERT_TRUE(ok);

    const vector<uint64_t>& v1 = json.Get<UI64ArrayField1>();
    const vector<uint64_t>& v2 = json.Get<UI64ArrayField2>();

    auto f = [&] (const vector<uint64_t>& v1, const vector<uint64_t>& v2) -> void {
        ASSERT_EQ(v1.size() , 3 );

        ASSERT_EQ(v1[0] , 500);

        ASSERT_EQ(v1[1] , 12514748365008);

        ASSERT_EQ(v1[2] , 0);

        ASSERT_EQ(v2.size() , 0 );

    };

    f(v1,v2);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<uint64_t>& v21 = json2.Get<UI64ArrayField1>();
    const vector<uint64_t>& v22 = json2.Get<UI64ArrayField2>();

    f(v21,v22);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    const vector<uint64_t>& v31 = json3.Get<UI64ArrayField1>();
    const vector<uint64_t>& v32 = json3.Get<UI64ArrayField2>();

    f(v31,v32);

}

TEST(JSONParsing,ParseEmbededUI64) {
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    ASSERT_EQ(json.Get<UI64Field1>() , value1);

    ASSERT_EQ(json.Get<UI64Field2>() , value2);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_EQ(json2.Get<UI64Field1>() , value1);

    ASSERT_EQ(json2.Get<UI64Field2>() , value2);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_EQ(json3.Get<UI64Field1>() , value1);

    ASSERT_EQ(json3.Get<UI64Field2>() , value2);

}

TEST(JSONParsing,ParseEmbededArrayUI64) {
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

    ASSERT_TRUE(ok);

    JSON& json = *parent.Get<Objects>()[1];

    ASSERT_EQ(json.Get<UI64Field1>() , value1);

    ASSERT_EQ(json.Get<UI64Field2>() , value2);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_EQ(json2.Get<UI64Field1>() , value1);

    ASSERT_EQ(json2.Get<UI64Field2>() , value2);


    string newRawJson22 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson22.c_str(), error);

    ASSERT_TRUE(ok);

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_EQ(json3.Get<UI64Field1>() , value1);

    ASSERT_EQ(json3.Get<UI64Field2>() , value2);

}

TEST(JSONParsing,ParseUnsigned) {
    std::string rawJson = R"JSON( 
    {
        "UIntField1": 500
    }
    )JSON";

    SimpleParsedJSON<UIntField1> json, json2, json3;

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<UIntField1>() , 500);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<UIntField1>() , 500);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<UIntField1>() , 500);

}

TEST(JSONParsing,ParseUnsignedArray) {
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

    ASSERT_TRUE(ok);

    auto f = [&] (const vector<unsigned>& v1, const vector<unsigned>& v2) -> void {
        ASSERT_EQ(v1.size() , 3 );

        ASSERT_EQ(v1[0] , 500);

        ASSERT_EQ(v1[1] , 0);

        ASSERT_EQ(v1[2] , 123);

        ASSERT_EQ(v2.size() , 0 );

    };

    const vector<unsigned>& v1 = json1.Get<UIntArrayField1>();
    const vector<unsigned>& v2 = json1.Get<UIntArrayField2>();

    f(v1,v2);


    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<unsigned>& v21 = json2.Get<UIntArrayField1>();
    const vector<unsigned>& v22 = json2.Get<UIntArrayField2>();

    f(v21,v22);


    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<unsigned>& v31 = json3.Get<UIntArrayField1>();
    const vector<unsigned>& v32 = json3.Get<UIntArrayField2>();

    f(v31,v32);

}

TEST(JSONParsing,ParseBoolArray) {
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

    ASSERT_TRUE(ok);

    auto f = [&] (const vector<bool>& v1, const vector<bool>& v2) -> void {
        ASSERT_EQ(v1.size() , 4 );

        ASSERT_EQ(v1[0] , true);

        ASSERT_EQ(v1[1] , false);

        ASSERT_EQ(v1[2] , true);

        ASSERT_EQ(v1[3] , false);

        ASSERT_EQ(v2.size() , 0 );

    };

    const vector<bool>& v1 = json1.Get<BoolArrayField1>();
    const vector<bool>& v2 = json1.Get<BoolArrayField2>();

    f(v1,v2);


    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<bool>& v21 = json2.Get<BoolArrayField1>();
    const vector<bool>& v22 = json2.Get<BoolArrayField2>();

    f(v21,v22);


    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<bool>& v31 = json3.Get<BoolArrayField1>();
    const vector<bool>& v32 = json3.Get<BoolArrayField2>();

    f(v31,v32);

}

TEST(JSONParsing,ParseDoubleArray) {
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

    ASSERT_TRUE(ok);

    auto f = [&] (const vector<double>& v1, const vector<double>& v2) -> void {
        ASSERT_EQ(v1.size() , 5 );

        ASSERT_EQ(v1[0] , 5.5);

        ASSERT_EQ(v1[1] , 5);

        ASSERT_EQ(v1[2] , -5);

        ASSERT_EQ(v1[3] , 2000000000000L);

        ASSERT_EQ(v1[4] , -2000000000001L);

        ASSERT_EQ(v2.size() , 0 );

    };

    const vector<double>& v1 = json1.Get<DoubleArrayField1>();
    const vector<double>& v2 = json1.Get<DoubleArrayField2>();

    f(v1,v2);


    string newRawJson = json1.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    const vector<double>& v21 = json2.Get<DoubleArrayField1>();
    const vector<double>& v22 = json2.Get<DoubleArrayField2>();

    f(v21,v22);


    string newRawJson2 = json1.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    const vector<double>& v31 = json3.Get<DoubleArrayField1>();
    const vector<double>& v32 = json3.Get<DoubleArrayField2>();

    f(v31,v32);

}

TEST(JSONParsing,ParseEmbededUnsigned) {
    std::string rawJson = R"JSON({
        "Object": {
            "UIntField1": 500
        }
    }
    )JSON";

    typedef SimpleParsedJSON<UIntField1> JSON;
    NewEmbededObject(Object,JSON);

    SimpleParsedJSON<Object> parent, parent2, parent3;
    std::string error;

    bool ok = parent.Parse(rawJson.c_str(),error);

    JSON& json = parent.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json.Get<UIntField1>() , 500);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<UIntField1>() , 500);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<UIntField1>() , 500);

}

TEST(JSONParsing,ParseEmbededArrayUnsigned) {
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

    ASSERT_TRUE(ok);

    ASSERT_EQ(parent.Get<Objects>().size() , 3 );

    JSON& json = *parent.Get<Objects>()[1];

    ASSERT_EQ(json.Get<UIntField1>() , 500);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<UIntField1>() , 500);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<UIntField1>() , 500);

}

TEST(JSONParsing,LargeInt) {
    stringstream rawJson;
    rawJson << "{\"IntField1\": "
            << std::numeric_limits<unsigned>::max()
            << "}";

    SimpleParsedJSON<IntField1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error , "Invalid value for field: IntField1" );
}

TEST(JSONParsing,LargeI64) {
    stringstream rawJson;
    rawJson << "{\"I64Field1\": "
            << std::numeric_limits<uint64_t>::max()
            << "}";

    SimpleParsedJSON<I64Field1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error , "Invalid value for field: I64Field1" );

}

TEST(JSONParsing,ExtraField) {

    std::string rawJson = R"JSON( 
    {
        "Field1": "Hello World!",
        "Field2": "Hello World!"
    }
    )JSON";

    SimpleParsedJSON<Field1> json;

    std::string error;

    bool ok  = json.Parse(rawJson.c_str(),error);

    if (ok) {
    }

    ASSERT_EQ(error , "Unknown extra field: Field2" );
}


TEST(JSONParsing,ParseEmbededBool) {
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    ASSERT_FALSE(json.Get<BoolField1>());

    ASSERT_TRUE(json.Get<BoolField2>());

    ASSERT_FALSE(json.Get<BoolField3>());

    ASSERT_TRUE(json.Get<BoolField4>());



    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok );

    JSON& json2 = parent2.Get<Object>();

    ASSERT_FALSE(json2.Get<BoolField1>());

    ASSERT_TRUE(json2.Get<BoolField2>());

    ASSERT_FALSE(json2.Get<BoolField3>());

    ASSERT_TRUE(json2.Get<BoolField4>());


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok );

    JSON& json3 = parent3.Get<Object>();

    ASSERT_FALSE(json3.Get<BoolField1>());

    ASSERT_TRUE(json3.Get<BoolField2>());

    ASSERT_FALSE(json3.Get<BoolField3>());

    ASSERT_TRUE(json3.Get<BoolField4>());

}

TEST(JSONParsing,ParseEmbededArrayBool) {
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

    ASSERT_TRUE(ok );

    JSON& json = *parent.Get<Objects>()[1];

    ASSERT_FALSE(json.Get<BoolField1>());

    ASSERT_TRUE(json.Get<BoolField2>());

    ASSERT_FALSE(json.Get<BoolField3>());

    ASSERT_TRUE(json.Get<BoolField4>());



    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok );

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_FALSE(json2.Get<BoolField1>());

    ASSERT_TRUE(json2.Get<BoolField2>());

    ASSERT_FALSE(json2.Get<BoolField3>());

    ASSERT_TRUE(json2.Get<BoolField4>());


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok );

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_FALSE(json3.Get<BoolField1>());

    ASSERT_TRUE(json3.Get<BoolField2>());

    ASSERT_FALSE(json3.Get<BoolField3>());

    ASSERT_TRUE(json3.Get<BoolField4>());

}
TEST(JSONParsing,ParseBool) {
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

    ASSERT_TRUE(ok );

    ASSERT_FALSE(json.Get<BoolField1>());

    ASSERT_TRUE(json.Get<BoolField2>());

    ASSERT_FALSE(json.Get<BoolField3>());

    ASSERT_TRUE(json.Get<BoolField4>());



    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok );

    ASSERT_FALSE(json2.Get<BoolField1>());

    ASSERT_TRUE(json2.Get<BoolField2>());

    ASSERT_FALSE(json2.Get<BoolField3>());

    ASSERT_TRUE(json2.Get<BoolField4>());


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok );

    ASSERT_FALSE(json3.Get<BoolField1>());

    ASSERT_TRUE(json3.Get<BoolField2>());

    ASSERT_FALSE(json3.Get<BoolField3>());

    ASSERT_TRUE(json3.Get<BoolField4>());
}

TEST(JSONParsing,ParseDouble) {
    std::string rawJson = R"JSON( 
    {
        "DoubleField1": 5.5,
        "DoubleField2": 5,
        "DoubleField3": -5,
        "DoubleField4": 2000000000000,
        "DoubleField5": -2000000000001
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

    ASSERT_TRUE(ok);

    auto f = [&] (JSON& json) -> void {
        ASSERT_EQ(json.Get<DoubleField1>() , 5.5);

        ASSERT_EQ(json.Get<DoubleField2>() , 5);

        ASSERT_EQ(json.Get<DoubleField3>() , -5);

        ASSERT_EQ(json.Get<DoubleField4>() , 2000000000000L);

        ASSERT_EQ(json.Get<DoubleField5>() , -2000000000001L);

    };

    f(json);


    string newRawJson = json.GetJSONString(true);

    ok = json2.Parse(newRawJson.c_str(), error);

    ASSERT_TRUE(ok);

    f(json2);


    string newRawJson2 = json.GetPrettyJSONString();

    ok = json3.Parse(newRawJson2.c_str(), error);

    ASSERT_TRUE(ok);

    f(json3);

}

TEST(JSONParsing,ParseEmbededDouble) {
    std::string rawJson = R"JSON({
        "Object": {
            "DoubleField1": 5.5,
            "DoubleField2": 5,
            "DoubleField3": -5
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object>();

    ASSERT_EQ(json.Get<DoubleField1>() , 5.5);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5.5);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5.5);

}

TEST(JSONParsing,ParseEmbededArrayDouble) {
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

    ASSERT_TRUE(ok);

    JSON& json = *parent.Get<Objects>()[1];

    ASSERT_EQ(json.Get<DoubleField1>() , 5.5);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = *parent2.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5.5);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = *parent3.Get<Objects>()[1];

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5.5);

}

TEST(JSONParsing,Clear) {
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

    ASSERT_EQ(json.Get<IntField1>() , 0);

    ASSERT_EQ(json.Get<UIntField1>() , 0);

    ASSERT_EQ(json.Get<I64Field1>() , 0);

    ASSERT_EQ(json.Get<UI64Field1>() , 0);

    ASSERT_EQ(json.Get<DoubleField1>() , 0);

    ASSERT_EQ(json.Get<BoolField1>() , false);

    ASSERT_EQ(json.Get<Field1>() , "");

    ASSERT_EQ(json.Get<StringArrayField1>().size() , 0);

    ASSERT_FALSE(json.Supplied<IntField1>());

    ASSERT_FALSE(json.Supplied<UIntField1>());

    ASSERT_FALSE(json.Supplied<I64Field1>());

    ASSERT_FALSE(json.Supplied<UI64Field1>());

    ASSERT_FALSE(json.Supplied<DoubleField1>());

    ASSERT_FALSE(json.Supplied<BoolField1>());

    ASSERT_FALSE(json.Supplied<Field1>());

    ASSERT_FALSE(json.Supplied<StringArrayField1>());

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
    ASSERT_EQ(notNull , ExpNotNull);

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
    ASSERT_EQ(null , ExpNull);

}

TEST(JSONParsing,EmbededClear) {
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


    ASSERT_EQ(json.Get<IntField1>() , 0);

    ASSERT_EQ(json.Get<UIntField1>() , 0);

    ASSERT_EQ(json.Get<I64Field1>() , 0);

    ASSERT_EQ(json.Get<UI64Field1>() , 0);

    ASSERT_EQ(json.Get<DoubleField1>() , 0);

    ASSERT_EQ(json.Get<BoolField1>() , false);

    ASSERT_EQ(json.Get<Field1>() , "");

    ASSERT_EQ(json.Get<StringArrayField1>().size() , 0);

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
    ASSERT_EQ(notNull , ExpNotNull);

    string ExpNull =
R"RAW({
    "Object": null
})RAW";

    string null = parent.GetPrettyJSONString(true);
    ASSERT_EQ(null , ExpNull);

}

TEST(JSONParsing,EmbededArrayClear) {
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

    ASSERT_EQ(json.Get<IntField1>() , 0);

    ASSERT_EQ(json.Get<UIntField1>() , 0);

    ASSERT_EQ(json.Get<I64Field1>() , 0);

    ASSERT_EQ(json.Get<UI64Field1>() , 0);

    ASSERT_EQ(json.Get<DoubleField1>() , 0);

    ASSERT_EQ(json.Get<BoolField1>() , false);

    ASSERT_EQ(json.Get<Field1>() , "");

    ASSERT_EQ(json.Get<StringArrayField1>().size() , 0);

    parent.Clear();

    ASSERT_EQ(parent.Get<Objects>().size() , 0);
}

TEST(JSONParsing,EmbededObjectError) {
    stringstream rawJson;
    rawJson << "{\"IntField1\": { \"field1\": 1 } } ";

    SimpleParsedJSON<IntField1> json;

    std::string error;

    bool ok = json.Parse(rawJson.str().c_str(),error);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error , "Invalid type for field: IntField1" );
}


TEST(JSONParsing,TrippleEmbededObject) {
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

    ASSERT_TRUE(ok);

    JSON& json = parent.Get<Object3>().Get<Object2>().Get<Object>();

    ASSERT_EQ(json.Get<DoubleField1>() , 5.5);


    string newRawJson = parent.GetJSONString(true);

    ok = parent2.Parse(newRawJson.c_str(), error);

    JSON& json2 = parent2.Get<Object3>().Get<Object2>().Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json2.Get<DoubleField1>() , 5.5);


    string newRawJson2 = parent.GetPrettyJSONString();

    ok = parent3.Parse(newRawJson2.c_str(), error);

    JSON& json3 = parent3.Get<Object3>().Get<Object2>().Get<Object>();

    ASSERT_TRUE(ok);

    ASSERT_EQ(json3.Get<DoubleField1>() , 5.5);

}

TEST(JSONParsing,EmbededObjectInArray) {
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

    ASSERT_TRUE(ok);

    auto& outerObjects = json.Get<OuterObjects>();

    ASSERT_EQ(outerObjects.size() , 2);

    auto& outer1 = *outerObjects[0];
    auto& outer2 = *outerObjects[1];

    auto& objects1 = outer1.Get<Objects>();
    auto& objects2 = outer2.Get<Objects>();

    ASSERT_EQ(objects1.size() , 2 );

    auto& object1 = *objects1[0];
    auto& object2 = *objects1[1];

    ASSERT_EQ(object1.Get<IntField1>() , 1 );

    ASSERT_EQ(object2.Get<IntField1>() , 2 );

    ASSERT_EQ(objects2.size() , 3 );

    auto& object3 = *objects2[0];
    auto& object4 = *objects2[1];
    auto& object5 = *objects2[2];

    ASSERT_EQ(object3.Get<IntField1>() , 3 );

    ASSERT_EQ(object4.Get<IntField1>() , 4 );

    ASSERT_EQ(object5.Get<IntField1>() , 5 );

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
TEST(JSONParsing,Supplied) {
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
    "BoolField2": null
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
    ASSERT_TRUE(json.Parse(inputJSON.c_str(), error) );
    string output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , expOutput);

    json.Clear();
    output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , allNull);

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

TEST(JSONParsing,SuppliedEmbeded) {
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
    ASSERT_TRUE(json.Parse(input.c_str(), error) );

    string output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , expOutput);

    json.Clear();
    output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , allNull);
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

TEST(JSONParsing,SuppliedEmbededObjectInArray) {
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
    ASSERT_TRUE(json.Parse(input.c_str(), error) );

    string output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , expOutput);

    json.Clear();
    output = json.GetPrettyJSONString(true);

    ASSERT_EQ(output , allNull);

}

TEST(JSONParsing,ParseTimeArray) {
    std::string rawJson = R"JSON( 
        {
            "TimeArrayField1": [
                "2015-07-13T05:38:17.33640392Z",
                "",
                "2016-07-13T05:38:17.33640392Z",
                "2017-07-13T05:38:17.33640392Z"
            ],
            "TimeArrayField2": []
        }
    )JSON";

    SimpleParsedJSON<TimeArrayField1, TimeArrayField2> json, json2, json3;

    std::string error;

    ASSERT_TRUE(json.Parse(rawJson.c_str(),error));

    auto checker = [] (const vector<Time>& v1, const vector<Time>& v2) -> void
    {
        ASSERT_EQ(v1.size(), 4);
        const std::string expTime = "2015-07-13T05:38:17.336403Z";
        const std::string expTime1 = "2016-07-13T05:38:17.336403Z";
        const std::string expTime2 = "2017-07-13T05:38:17.336403Z";

        ASSERT_EQ(v1[0].ISO8601Timestamp(), expTime);
        ASSERT_EQ(v1[1].Timestamp(), EpochTimestamp);
        ASSERT_EQ(v1[2].ISO8601Timestamp(), expTime1);
        ASSERT_EQ(v1[3].ISO8601Timestamp(), expTime2);

        ASSERT_EQ(v2.size(), 0 );
    };

    const vector<Time>& v1 = json.Get<TimeArrayField1>();
    const vector<Time>& v2 = json.Get<TimeArrayField2>();

    checker(v1,v2);

    string newRawJson = json.GetJSONString(true);

    ASSERT_TRUE(json2.Parse(newRawJson.c_str(), error));

    const vector<Time>& v21 = json2.Get<TimeArrayField1>();
    const vector<Time>& v22 = json2.Get<TimeArrayField2>();

    checker(v21,v22);

    string newRawJson2 = json.GetPrettyJSONString();

    ASSERT_TRUE(json3.Parse(newRawJson2.c_str(), error));

    const vector<Time>& v31 = json3.Get<TimeArrayField1>();
    const vector<Time>& v32 = json3.Get<TimeArrayField2>();

    checker(v31,v32);
}



