#include "stringStruct.h"
#include "tester.h"

using namespace std;

int InitialiseFactory(testLogger& log );
int InitialiseEmptyStruct(testLogger& log);
int InitialiseStruct(testLogger& log);
int InitialisePartialStruct(testLogger& log);

vector<StringStruct> buf1;
vector<StringStruct> buf2;
vector<StringStruct> buf3;
vector<StringStruct> buf4, buf5, buf6, buf7;

int main(int argc, const char *argv[])
{
    Test("Initialising factory...",InitialiseFactory).RunTest();
    Test("Initialising empty struct...",InitialiseEmptyStruct).RunTest();
    Test("Initialising struct...",InitialiseStruct).RunTest();
    Test("Initialising struct from partial string...",InitialisePartialStruct).RunTest();
    return 0;
}



int InitialiseFactory( testLogger& log ) {
    StringStructFactory factory("a1 a2 a3 a5");
    if ( factory.NumElements() != 4 ) {
        log << "Invalid element count: " << factory.NumElements() << endl;
        return 1;
    }
    if ( factory.GetIdx("a1") != 0 ) {
        log << "Invalid index for element a1: " << factory.GetIdx("a1") << endl;
        return 1;
    }
    if ( factory.GetIdx("a3") != 2 ) {
        log << "Invalid index for element a3: " << factory.GetIdx("a3") << endl;
        return 1;
    }
    if ( factory.GetIdx("a2") != 1 ) {
        log << "Invalid index for element a2: " << factory.GetIdx("a2") << endl;
        return 1;
    }
    if ( factory.GetIdx("a5") != 3 ) {
        log << "Invalid index for element a5: " << factory.GetIdx("a5") << endl;
        return 1;
    }
    return 0;
}

int InitialiseEmptyStruct(testLogger& log ) {
    StringStructFactory factory("a1 a2 a3 a5");
    StringStruct s = factory.New();
    if ( s[0] != 0 ) {
        log << "Element 0 not initialised to zero!: " << s[0] << endl;
        return 1;
    }
    if ( s[1] != 0 ) {
        log << "Element 1 not initialised to zero!: " << s[1] << endl;
        return 1;
    }
    if ( s[2] != 0 ) {
        log << "Element 2 not initialised to zero!: " << s[2] << endl;
        return 1;
    }
    if ( s[3] != 0 ) {
        log << "Element 3 not initialised to zero!: " << s[3] << endl;
        return 1;
    }

    StringStruct s2 = factory.New("");
    if ( s2[0] != 0 ) {
        log << "Element 0 not initialised to zero!: " << s2[0] << endl;
        return 1;
    }
    if ( s2[1] != 0 ) {
        log << "Element 1 not initialised to zero!: " << s2[1] << endl;
        return 1;
    }
    if ( s2[2] != 0 ) {
        log << "Element 2 not initialised to zero!: " << s2[2] << endl;
        return 1;
    }
    if ( s2[3] != 0 ) {
        log << "Element 3 not initialised to zero!: " << s2[3] << endl;
        return 1;
    }
    return 0;
}

int InitialiseStruct(testLogger& log ) {
    StringStructFactory factory("a1 a2 a3 a5");
    StringStruct s = factory.New("0235 2 0 -34234");
    if ( s[0] != 235 ) {
        log << "Failed to read 235: " << s[0] << endl;
        return 1;
    }
    if ( s[1] != 2 ) {
        log << "Failed to read 2: " << s[1] << endl;
        return 1;
    }
    if ( s[2] != 0 ) {
        log << "Failed to read 0: " << s[2] << endl;
        return 1;
    }
    if ( s[3] != -34234 ) {
        log << "Failed to read -34234: " << s[3] << endl;
        return 1;
    }
    return 0;
}

int InitialisePartialStruct(testLogger& log ) {
    StringStructFactory factory("a1 a2 a3 a5");
    StringStruct s = factory.New("0235 2");
    if ( s[0] != 235 ) {
        log << "Failed to read 235: " << s[0] << endl;
        return 1;
    }
    if ( s[1] != 2 ) {
        log << "Failed to read 2: " << s[1] << endl;
        return 1;
    }
    if ( s[2] != 0 ) {
        log << "Failed to read 0: " << s[2] << endl;
        return 1;
    }
    if ( s[3] != 0) {
        log << "Failed to read 0: " << s[3] << endl;
        return 1;
    }
    return 0;
}
