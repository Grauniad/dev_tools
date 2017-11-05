#include "gtest/gtest.h"
#include <SimpleJSON.h>

using namespace std;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

NewStringArrayField(StringArrayField1);

typedef SimpleParsedJSON<StringArrayField1> StringArrayJson;
TEST(JSONParsing, UnexpectedEmbededArray ) {
    StringArrayJson json;
    std::string rawJson = R"JSON(
        {
            "StringArrayField1": [
                ["String 1", "String 2"],
                ["", "String 4"]
            ],
        }
    )JSON";

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error, "Invalid type for field: StringArrayField1");
}

TEST(JSONParsing, InvalidArray ) {
    StringArrayJson json;

    std::string rawJson = R"JSON(
        {
            "StringArrayField1": ]
        }
    )JSON";

    std::string error;

    bool ok = json.Parse(rawJson.c_str(),error);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error, "Failed to parse JSON: Invalid value.");
}

TEST(JSONParsing, ArrayInvalidParser ) {
    StringArrayJson json;

    class DodgeyStringParser: public StringArrayJson::IParser {
    public:
        virtual void Parse(const char* json, StringArrayJson& spj) {
            spj.StartObject();
            std::string key = "StringArrayField1";
            spj.Key(key.c_str(), key.length(), false);
            spj.EndArray(0);
        }
    } dodgeyParser;

    std::string error;

    std::string rawJson = R"JSON(
        {
            "StringArrayField1": ]
        }
    )JSON";

    bool ok = json.Parse(rawJson.c_str(),error, dodgeyParser);

    ASSERT_FALSE(ok);

    ASSERT_EQ(error, "Invalid JSON!");
}
