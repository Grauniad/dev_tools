#include "stringUtils.h"
#include "tokenizor.h"
#include <string>
#include "regex_utils.h"
#include "logger.h"
#include <sstream>
#include <boost/algorithm/string/trim.hpp>

using namespace std;


/*
 * Utility class for replacing a 
 * single argument in the string
 */
class ArgToken {
public:
   ArgToken(const std::string& input)
     : matched(false), wild(false), quoted(false), n(-1), processed(""), data(input)
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
           wild = argPattern.Group(data,2) != "";
           quoted = argPattern.Group(data,2) == "@";
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

   bool IsWildCard() {
       return wild;
   }

   bool IsQuote() {
       return quoted;
   }

   void Replace(const Tokens& args) {
       if ( !wild ) {
           processed += args[ArgNumber()-1];
       } else {
           for (size_t i = ArgNumber()-1; i< args.size(); ++i) {
               if ( i != static_cast<size_t>(ArgNumber()) -1 ) {
                   processed += " ";
               }
               if ( quoted) {
                   processed += "\"";
               }
               processed += args[i];
               if ( quoted) {
                   processed += "\"";
               }
           }
       }
       token = "";
   }

   string Result() {
       return processed;
   }
private:
   static RegPattern argPattern;
   bool matched;
   bool wild;
   bool quoted;
   int n;
   string processed;
   string data;
   string token;
};

RegPattern ArgToken::argPattern("\\$\\{([1-9][0-9]*)([*@]?)\\}");

std::string StringUtils::Substitute(
                const std::string& skeleton, 
                const std::string& input)
{
    Tokens argTokens(input);
    ArgToken arg(skeleton);
    for ( ; arg.Matched(); arg.Next() ) {
        arg.Replace(argTokens);
    }
    return arg.Result();
}

void StringUtils::Trim(std::string& toTrim) {
        boost::algorithm::trim(toTrim);
}

void StringUtils::FastPrintLong(long n, size_t max, char* out) {
    --out;
    // Catch negative numbers
    if ( n < 0 ) {
        *(++out) = '-';
        n*=-1;
        --max;
    }

    // 0 is a special case - it would skip the whole loop
    if ( n == 0 ) {
        *(++out) = '0';
    } else {
        char tmp[50];

        // Print each digit, smallest first...
        char *p = tmp-1;
        while ( n > 0 ) {
            *(++p) = '0' + n % 10;
            n /= 10;
        }

        char * stop = tmp;
        size_t len = 1 + (p-tmp);
        if ( len+1 > max ) {
            stop = (p - max) +2;
        }
        // Reverse the number
        for ( char * t=p; t>=stop; --t) {
            *(++out) = *t;
        }
    }

    // Null terminate - it's a proper string...
    *(++out) = '\0';
}

