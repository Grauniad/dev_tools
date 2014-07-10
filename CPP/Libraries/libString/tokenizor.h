#ifndef DEV_TOOLS_LIBRARIES_LIBSTRING_TOKENIZOR_H__
#define DEV_TOOLS_LIBRARIES_LIBSTRING_TOKENIZOR_H__

#include <string>
#include <vector>
#include <sstream>

class Tokens: private std::vector<std::string> {
public:
   /* C'tor
    *
    * Break up the string into tokens
    */
   Tokens(const std::string& input);

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
   void Get(size_t idx, T& out) {
       std::stringstream buf((*this)[idx]);
       buf >> out;
   }

   /*
    * Specialise the string fuction to prevent it corrupting the token
    */
   const std::string& Get( size_t idx) {
       return (*this)[idx];
   }
   void Get(size_t idx, std::string& out) {
       out = (*this)[idx];
   }

   using std::vector<std::string>::begin;
   using std::vector<std::string>::cbegin;
   using std::vector<std::string>::end;
   using std::vector<std::string>::cend;
   using std::vector<std::string>::size;
private:
   std::string empty;
};
#endif
