
#include "SimpleJSON.h"

using namespace std;

SimpleJSONBuilder::SimpleJSONBuilder() 
    : writer(buf)
{
    writer.StartObject();
}

void SimpleJSONBuilder::Clear() {
    buf.Clear();
    writer.Reset(buf);

    writer.StartObject();
}

std::string SimpleJSONBuilder::GetAndClear() {
    writer.EndObject();

    std::string result = buf.GetString();

    Clear();

    return result;
}

void SimpleJSONBuilder::Add(
    const string& name,
    const vector<string>& stringArray)
{
    writer.String(name.c_str());
    writer.StartArray();
    for (const std::string& item: stringArray) {
        writer.String(item.c_str());
    }
    writer.EndArray();
}

void SimpleJSONBuilder::Add(const std::string& name, const std::string& value) {
    writer.String(name.c_str());
    writer.String(value.c_str());
}

void SimpleJSONBuilder::Add(const std::string& name, const int& value) {
    writer.String(name.c_str());
    writer.Int(value);
}

void SimpleJSONBuilder::Add(const std::string& name, const int64_t& value) {
    writer.String(name.c_str());
    writer.Int64(value);
}

void SimpleJSONBuilder::Add(const std::string& name, const unsigned & value) {
    writer.String(name.c_str());
    writer.Uint(value);
}

void SimpleJSONBuilder::Add(const std::string& name, const uint64_t& value) {
    writer.String(name.c_str());
    writer.Uint64(value);
}

void SimpleJSONBuilder::Add(const std::string& name, const double& value) {
    writer.String(name.c_str());
    writer.Double(value);
}

void SimpleJSONBuilder::Add(const std::string& name, const bool& value) {
    writer.String(name.c_str());
    writer.Bool(value);
}

void SimpleJSONBuilder::StartArray(const std::string& name) {
    writer.String(name.c_str());
    writer.StartArray();
}

void SimpleJSONBuilder::EndArray() {
    writer.EndArray();
}

void SimpleJSONBuilder::StartAnonymousObject() {
    writer.StartObject();
}

void SimpleJSONBuilder::EndObject() {
    writer.EndObject();
}
