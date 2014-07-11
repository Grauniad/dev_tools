#ifndef DEV_TOOLS_LIBRARIES_LIBSTRING_TOKENIZOR_H__
#define DEV_TOOLS_LIBRARIES_LIBSTRING_TOKENIZOR_H__

#include <string>
#include <vector>
#include <sstream>

class StringParts: private std::vector<std::string>  {
public:
    StringParts() : empty("") {}
   /*
    * Access without converstion 
    */
   const std::string& operator[](size_t idx);

   /*
    * Access with type conversion
    */
   template<class T> 
   T Get(size_t idx) {
       T tmp;
       std::stringstream buf((*this)[idx]);
       buf >> tmp;
       return tmp;
   }

   template<class T> 
   bool Get(size_t idx, T& out) {
       std::stringstream buf((*this)[idx]);
       buf >> out;
       return !buf.fail();
   }

   /*
    * Specialise the string fuction to prevent it corrupting the token
    */
   const std::string& Get( size_t idx) {
       return (*this)[idx];
   }
   bool Get(size_t idx, std::string& out) {
       out = (*this)[idx];
       return true;
   }

   using std::vector<std::string>::begin;
   using std::vector<std::string>::cbegin;
   using std::vector<std::string>::end;
   using std::vector<std::string>::cend;
   using std::vector<std::string>::size;
protected:
   using std::vector<std::string>::push_back;
private:
   std::string empty;
};

class Parts: public StringParts {
public:
   /* C'tor
    *
    * Break up the string by 
    */
   Parts(const std::string& input, 
          const std::string& sep_chars = " ");
};

class Tokens: public StringParts {
public:
   /* C'tor
    *
    * Break up the string into tokens
    */
   Tokens(const std::string& input, char sep = ' ');
};
#endif
