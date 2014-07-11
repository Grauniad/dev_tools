#include "tester.h"
#include "regex.h"
int GroupMatch(testLogger& log);
int ArgMatch(testLogger& log);
int Replace(testLogger& log);
using namespace std;

int main (void) {
    Test("Testing group capture...",GroupMatch).RunTest();
    Test("Testing argument style capture...",ArgMatch).RunTest();
    Test("Testing pattern replacement ...",Replace).RunTest();
    return 0;
}

int GroupMatch(testLogger& log) {
    RegPattern e("a(.)c");
    if ( e.Group("abc",1) != "b") {
        log << "Failed to capture b: " << e.Group("abc",1);
        return 1;
    }

    if ( e.Group("a cat sat on abc",1) != " ") {
        log << "Failed to capture [space]: " << e.Group("a cat sat on abc",1);
        return 1;
    }
    return 0;
}

int ArgMatch(testLogger& log) {
    RegPattern argPattern("\\$\\{([0-9]+)\\}");
    if ( argPattern.Group("no substitutions here",1) != "" ) {
        log << " Group didn't return blank!" << endl;
        return 1;
    }
    if ( argPattern.Group("Simple arg subs (${1})",1) != "1" ) {
        log << " Group didn't return 1!" << endl;
        log << argPattern.Group("Simple arg subs (${1})",1) << endl;
        return 1;
    }
    return 0;
}

int Replace(testLogger& log) {
    RegPattern argPattern("\\$\\{([0-9]+)\\}");
    string res = argPattern.Replace("Result: (${1})","fourty two");
    if ( res != "Result: (fourty two)" ) {
        log << " Invalid replacement: " << res;
        return 1;
    }
    string res2 = argPattern.Replace("Result: (${1}) ${2}","fourty two");
    if ( res2 != "Result: (fourty two) ${2}" ) {
        log << " Invalid replacement (2): " << res2;
        return 1;
    }
    return 0;
}
