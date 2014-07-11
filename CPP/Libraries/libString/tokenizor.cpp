#include "tokenizor.h"
#include<boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

typedef boost::escaped_list_separator<char>  Separator;
typedef boost::char_separator<char>  Splitter;
typedef boost::tokenizer<Separator> Tokeniser;
typedef boost::tokenizer<Splitter> Divider;

const std::string& StringParts::operator[](size_t idx) const {
    if ( idx >= size() ) {
        return empty;
    } else {
        return vector<string>::operator[](idx);
    }
}

Tokens::Tokens(const string& input, char sep)
{
    string buf(input);
    string working(input);
    boost::algorithm::trim(buf);

    Tokeniser tokeniser(buf,Separator('\\', sep));
    for ( auto it = tokeniser.begin(); it!=tokeniser.end(); ++it) {
        // Skip blank values...
        working = *it;
        boost::algorithm::trim(working);
        if ( working != "" ) {
            push_back(*it);
        }
    }
}

Parts::Parts(const string& input, const string& sep_chars)
{
    string buf(input);
    string working(input);
    boost::algorithm::trim(buf);

    Divider tokeniser(buf,Splitter(sep_chars.c_str()));
    for ( auto it = tokeniser.begin(); it!=tokeniser.end(); ++it) {
        push_back(*it);
    }
}

