#include "tester.h"
#include "stringUtils.h"
#include <string>
using namespace std;

int NoSubstitute(testLogger& log);
int SingleArgSubstitutions(testLogger& log);
int MultiArgSubs(testLogger& log);
int QuotedArgSubs(testLogger& log);

int main (void) {
    Test("Applying a substitution with no match...",NoSubstitute).RunTest();
    Test("Applying a substitution with single args...",SingleArgSubstitutions).RunTest();
    Test("Applying a substitution with multiple args...",MultiArgSubs).RunTest();
    Test("Applying a substitution with quoted multiple args...",QuotedArgSubs).RunTest();
    return 0;
}


int NoSubstitute(testLogger& log) {
    string input = "There ${a}are no subt$ { dsf } itutions in {1}this s${2 }tring";
    string result = StringUtils::Substitute(input , "");
    if ( result != input ) {
        log << " non-requested substitution: " << result;
        return 1;
    }
    return 0;
}

int SingleArgSubstitutions(testLogger& log) {
    string input = "${3} ${0} ${2} ${1} ${0}";
    string result = StringUtils::Substitute(input , "one two three");
    if ( result != "three ${0} two one ${0}" ) {
        log << "Invalid substitution!" << result << endl;
        return 1;
    }

    string result2 = StringUtils::Substitute(input , "${1} ${2} ${3}");

    if ( result2 != "${3} ${0} ${2} ${1} ${0}" ) {
        log << "Recursive substitution!" << ">" << result2 << "<" << endl;
        return 1;
    }

    return 0;
}


int MultiArgSubs(testLogger& log) {
    string input = "pushd ${1}; ${2*}; popd";
    string result = StringUtils::Substitute(input , "ROOT sl 20 \"^Node::Node(const std\"");
    string expected = "pushd ROOT; sl 20 ^Node::Node(const std; popd"; 
    if ( result != expected ) {
        log << " Bad substs: >" << result << "<" << endl;
        log << " Execpted:   >" << expected << "<" << endl;
        return 1;
    }
    // Not yet implemented...
    return 0;
}

int QuotedArgSubs(testLogger& log) {
    string input = "pushd \"${1}\"; ${2@}; popd";
    string result = StringUtils::Substitute(input , "ROOT sl 20 \"^Node::Node(const std\"");
    string expected = "pushd \"ROOT\"; \"sl\" \"20\" \"^Node::Node(const std\"; popd"; 
    if ( result != expected ) {
        log << " Bad substs: >" << result << "<" << endl;
        log << " Execpted:   >" << expected << "<" << endl;
        return 1;
    }
    // Not yet implemented...
    return 0;
}
