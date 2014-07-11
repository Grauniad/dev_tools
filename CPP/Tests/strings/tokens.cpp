#include "tester.h"
#include "tokenizor.h"

int EmptyString(testLogger& log);
int Numbers(testLogger& log);
int Strings(testLogger& log);
int CustomSep(testLogger& log);
int InvalidOutput(testLogger& log);
int StringToParts(testLogger& log);
using namespace std;

int main (void) {
    Test("Tokenizing a blank string...",EmptyString).RunTest();
    Test("Tokenizing a string with numbers...",Numbers).RunTest();
    Test("Tokenizing a string with sub strings...",Strings).RunTest();
    Test("Tokenizing a string with invalid output...",InvalidOutput).RunTest();
    Test("Tokenizing a string with a custom separator...",CustomSep).RunTest();
    Test("Splitting a string into parts...",StringToParts).RunTest();
    return 0;
}

int EmptyString(testLogger& log) {
    Tokens tokens("");
    if ( tokens.size() != 0 ) {
        log << "Empty string yielded tokens: " << tokens.size();
        return 1;
    }
    if ( tokens[0] != "" || tokens[99] != "" ) {
        log << "Empty string did not yield blank tokens: " << tokens[0] << ", " << tokens[99];
        return 1;
    }
    return 0;
}

int Numbers(testLogger& log) {
    Tokens tokens("   123    4.56     9999978999999 ");
    if ( tokens.size() != 3 ) {
        log << "Wrong # of tokens!: " << tokens.size();
        return 1;
    }
    int ret0 = tokens.Get<int>(0);
    double ret1 = tokens.Get<double>(1);
    long ret2 = tokens.Get<long>(2);
    if ( ret0 != 123 ) {
        log << " Failed to pull the int: " << ret0;
        return 1;
    }

    if ( ret1 != 4.56 ) {
        log << " Failed to pull the float: " << ret1;
        return 1;
    }

    if ( ret2 != 9999978999999 ) {
        log << " Failed to pull the long: " << ret2;
        return 1;
    }
    ret0 = 0;
    ret1 = 0;
    ret2 = 0;

    tokens.Get(0,ret0);
    tokens.Get(1,ret1);
    tokens.Get(2,ret2);

    if ( ret0 != 123 ) {
        log << " Failed to get the int: " << ret0;
        return 1;
    }

    if ( ret1 != 4.56 ) {
        log << " Failed to get the float: " << ret1;
        return 1;
    }

    if ( ret2 != 9999978999999 ) {
        log << " Failed to get the long: " << ret2;
        return 1;
    }

    return 0;
}

int Strings(testLogger& log ) {
    Tokens tokens("   \"userName: Grauniad\"   \"password: let me in\"");

    if ( tokens.size() != 2 ) {
        log << "Wrong # of tokens!: " << tokens.size();
        return 1;
    }

    if (tokens[0] != "userName: Grauniad" ) {
        log << "Invalid token 1: " << tokens[0];
        return 1;
    }

    if (tokens[1] != "password: let me in" ) {
        log << "Invalid token 2: " << tokens[1];
        return 1;
    }

    std::string s0 = tokens.Get(0);
    std::string s1 = tokens.Get(1);

    if ( s0 != "userName: Grauniad" ) {
        log << "Invalid pull 1: " << tokens[0];
        return 1;
    }

    if (s1 != "password: let me in" ) {
        log << "Invalid pull 2: " << tokens[1];
        return 1;
    }

    std::string g0, g1;
    tokens.Get(0, g0);
    tokens.Get(1, g1);

    if ( g0 != "userName: Grauniad" ) {
        log << "Invalid get 1: " << tokens[0];
        return 1;
    }

    if ( g1 != "password: let me in" ) {
        log << "Invalid get 2: " << tokens[1];
        return 1;
    }

    return 0;
}

int InvalidOutput(testLogger& log ) {
    Tokens tokens( "one two_point_three four");
    int i;
    float f;
    string s;
    bool reti = tokens.Get(0,i);
    bool retf = tokens.Get(1,f);
    bool rets = tokens.Get(2,s);

    if ( reti ) {
        log << "Get failed to report fail for int pull!";
        return 1;
    }

    if ( retf ) {
        log << "Get failed to report fail for float pull!";
        return 1;
    }

    if ( !rets ) {
        log << "Get reported error on string pull!";
        return 1;
    }

    Tokens numbers("1 2.3 4");
    reti = numbers.Get(0,i);
    retf = numbers.Get(1,f);
    rets = numbers.Get(2,s);

    if ( !reti ) {
        log << "Get reported fail for int pull!";
        return 1;
    }

    if ( !retf ) {
        log << "Get reported fail for float pull!";
        return 1;
    }

    if ( !rets ) {
        log << "Get reported error on string pull!";
        return 1;
    }

    return 0;
}

int CustomSep(testLogger& log ) {
    Tokens tokens("   ;\"userName: Grauniad\";   ;\"password: let me in\";",';');

    if ( tokens.size() != 2 ) {
        log << "Wrong # of tokens!: " << tokens.size();
        return 1;
    }

    if (tokens[0] != "userName: Grauniad" ) {
        log << "Invalid token 1: " << tokens[0];
        return 1;
    }

    if (tokens[1] != "password: let me in" ) {
        log << "Invalid token 2: " << tokens[1];
        return 1;
    }
    return 0;
}

int StringToParts(testLogger& log) {
    Parts parts("string 1; string 2 \"has a sub string\"|string 3", ";|");
    if ( parts.size() != 3 ) {
        log << " Parts did not split into 3!: " << parts.size();
        return 1;
    }
    if ( parts[0] != "string 1" ) {
        log << " Invalid string 1: " << parts [0];
        return 1;
    }
    if ( parts[1] != " string 2 \"has a sub string\"" ) {
        log << " Invalid string 2: " << parts [1];
        return 1;
    }
    if ( parts[2] != "string 3" ) {
        log << " Invalid string 3: " << parts [2];
        return 1;
    }
    return 0;
}
