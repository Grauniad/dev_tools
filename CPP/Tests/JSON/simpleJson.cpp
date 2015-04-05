#include <iostream>
#include <tuple>
#include <map>
#include <string>
#include <type_traits>

using namespace std;

struct FieldBase {
    virtual void PrintValue() = 0;
};

struct StringField: public FieldBase {
    typedef std::string Value;
    Value value;
    virtual void PrintValue() { 
        cout << value << endl;
    }
};

struct Hello: public StringField {
    constexpr static const char* Name = "Hello";
};

struct World: public StringField {
    constexpr static const char* Name = "World";
};

template<class... Fields>
class ParsedJSON {
public:
   ParsedJSON() {
       AddNextField<sizeof...(Fields)>();
   }

   template <class T>
   T& Get() { return std::get<T>(fields); }

   int Size() { return sizeof...(Fields); } 
private:
   template<int idx>
   inline typename std::enable_if<idx!=0,void >::type
   AddNextField () { 
       AddField<idx-1>();
       AddNextField<idx-1>();
   }

   template<int idx>
   inline typename std::enable_if<idx==0,void >::type
   AddNextField () { }

   template<int idx> 
   void AddField() {
       auto& field= std::get<idx>(fields);
       fieldMap[field.Name] = &field;
   }

   std::tuple<Fields...> fields;
   std::map<std::string,FieldBase*> fieldMap;
};

int main(int argc, const char *argv[])
{
    ParsedJSON<Hello,World> json;
    cout << json.Get<Hello>().Name << " " << json.Get<World>().Name << endl;
    json.Get<Hello>().value = "Hello string";
    return 0;
}
