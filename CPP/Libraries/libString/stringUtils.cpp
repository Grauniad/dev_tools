#include "stringUtils.h"
#include "tokenizor.h"
#include <string>
#include "regex.h"
#include "logger.h"
#include <sstream>

using namespace std;


/*
 * Utility class for replacing a 
 * single argument in the string
 */
class ArgToken {
public:
   ArgToken(const std::string& input)
     : matched(false), n(-1), processed(""), data(input)
   {
       Next();
   }

   bool Next() {
       SLOG_FROM(LOG_VERBOSE,"ArgToken::Next",
                 "Start" << endl
                 << "processed: " << processed << endl
                 << "data: " << data << endl)
       token = argPattern.Group(data,0);
       matched = (token != "");
       if ( matched) {
           stringstream s(argPattern.Group(data,1));
           s >> n;
       } else {
           n = -1;
       }

       if ( matched ) {
           // find the token - everything up until the 
           // token is processed
            size_t start = data.find(token);
            if ( start > 0 ) {
                processed += data.substr(0,start);
            }
            data = data.substr(start + token.length());
       } else {
           // No more matches - 
           processed += data;
       }
       SLOG_FROM(LOG_VERBOSE,"ArgToken::Next",
                 "End" << endl
                 << "processed: " << processed << endl
                 << "data: " << data << endl)
       return matched;
   }

   // Is there an arg to replace?
   bool Matched() {
       return matched;
   }

   // Return the arg number being requested
   int ArgNumber() {
       return n;
   }

   void Replace(const std::string& arg) {
       processed += arg;
       token = "";
   }

   string Result() {
       return processed;
   }
private:
   static RegPattern argPattern;
   bool matched;
   int n;
   string processed;
   string data;
   string token;
};

RegPattern ArgToken::argPattern("\\$\\{([1-9][0-9]*)\\}");

std::string StringUtils::Substitute(
                const std::string& skeleton, 
                const std::string& input)
{
    Tokens argTokens(input);
    ArgToken arg(skeleton);
    for ( ; arg.Matched(); arg.Next() ) {
        arg.Replace(argTokens[arg.ArgNumber()-1]);
    }
    return arg.Result();
}
