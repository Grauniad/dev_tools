#include "tester.h"
#include "dataLump.h"
#include "OSTools.h"
#include "file.h"

using namespace std;

int Paths(testLogger& log);

int OpenDirs(testLogger& log);

int main(int argc, const char** argv) {
    Test("Check Path operations....", Paths).RunTest();
    Test("Check Path operations....", OpenDirs).RunTest();
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

int OpenDirs(testLogger& log) {
    FilePointer f = FilePointer::Open("ROOT/home/test/");
    if ( !f.IsOk() ) {
        log << "Failed to open directory :(" <<endl;
        return 1;
    }

    if ( f->ParentDirectory() != "ROOT/home" ) {
        log << "Failed to identify parent directory: " << f->ParentDirectory() << endl;
        return 2;
    }

    if ( f->Name() != "test" ) {
        log << "Failed to identify directory name: " << f->Name() << endl;
        return 3;
    }

    return 0;
}
