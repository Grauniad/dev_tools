
#include "tester.h"
#include "binaryData.h"
#include "dataLump.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <env.h>
#include <OSTools.h>

using namespace std;


int LS_DIR(testLogger& log );
int LS_LOCAL_DIR(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Get files in a specific directory...",LS_DIR).RunTest();
    Test("Get files in the current directory...",LS_LOCAL_DIR).RunTest();
    return 0;
}

int LS_DIR(testLogger& log) {
    std::string testDir = ENV::GetEnvValue("PROJET_ROOT_DIR",OS::PWD());
    testDir+= "/testDir";

    log << "Searching for: " << testDir << endl;
    std::vector<std::string> fnames = OS::Glob(testDir + "/*");

    log << "Found: " << endl;
    for (const std::string& fname: fnames ) {
        log << "  " << fname << endl;
    }


    vector<string> results = {
        "Makefile",
        "binaryData.cpp",
        "dir1/",
        "dynStruct.cpp",
        "search.cpp",
        "test.full_log",
        "time.cpp"
    };

    if (fnames.size() != results.size()) {
        log << "Invalid number of files: " << fnames.size() << endl;
        log << "Expected: " << results.size();
        return 1;
    }

    for (size_t i = 0; i<results.size(); ++i) {
        std::string exp = testDir + "/" + results[i];
        if ( fnames[i] != exp) {
            log << "Expected: " << exp << endl;
            log << "Got:      " << fnames[i] << endl;
            log << "Index: " <<  i << endl;
            return 1;
        }
    }

    return 0;
}


int LS_LOCAL_DIR(testLogger& log) {
    std::vector<std::string> fnames = OS::Glob("testDir/*");

    log << "Found: " << endl;
    for (const std::string& fname: fnames ) {
        log << "  " << fname << endl;
    }


    vector<string> results = {
        "Makefile",
        "binaryData.cpp",
        "dir1/",
        "dynStruct.cpp",
        "search.cpp",
        "test.full_log",
        "time.cpp"
    };

    if (fnames.size() != results.size()) {
        log << "Invalid number of files: " << fnames.size() << endl;
        log << "Expected: " << results.size();
        return 1;
    }

    for (size_t i = 0; i<results.size(); ++i) {
        std::string exp = "testDir/" + results[i];
        if ( fnames[i] != exp) {
            log << "Expected: " << exp << endl;
            log << "Got:      " << fnames[i] << endl;
            log << "Index: " <<  i << endl;
            return 1;
        }
    }

    return 0;
}
