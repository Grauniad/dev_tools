#include "tester.h"
#include "dataLump.h"
#include "OSTools.h"

using namespace std;

int Paths(testLogger& log);

int main(int argc, const char** argv) {
    Test("Check Path operations....", Paths).RunTest();
    return 0;
}

int Paths (testLogger& log) {
    if ( OS::Basename("/home/shusaku/wins.txt") != "wins.txt") {
        log << "Incorrect basename returned by OS::Basename" << endl;
        return 1;
    }

    if ( OS::Dirname("/home/shusaku/wins.txt") != "/home/shusaku") {
        log << "Incorrect directory returned by OS::Dirname" << endl;
        return 1;
    }

    if ( OS::Join("/home/shusaku", "wins.txt") != "/home/shusaku/wins.txt" ) {
        log << "Incorrect path returned by OS::Join" << endl;
        return 1;
    }

    return 0;
}
