#include "tester.h"
#include "Commands.h"
#include <sstream>

using namespace std;

int Function_NoArgs(testLogger& log);
int Function_OneArg(testLogger& log);
int Commands_NoArgs(testLogger& log);
int Commands_OneArg(testLogger& log);
int Commands_StringArgs(testLogger& log);

int One () {
    return 1;
}

int Double ( int x ) {
    return x + x;
}

int Basic_Authenticate ( string user, string password) {
    if ( user != "Torajiro" ) {
        return 1;
    } else if ( password != "Shusaku" ) {
        return 2;
    } else {
        return 0;
    }
}

int Authenticate ( string user, string password) {
    SLOG(LOG_OVERVIEW,"user: " << user)
    if ( user != "Kuwabara Torajiro" ) {
        return 1;
    } else if ( password != "Shusaku" ) {
        return 2;
    } else {
        return 0;
    }
}

int main(void) {
    Test("Constructing a wrapper around a no args function...",Function_NoArgs).RunTest();
    Test("Constructing a wrapper around a an int function...",Function_OneArg).RunTest();
    Test("Adding a no-args function to commands...",Commands_NoArgs).RunTest();
    Test("Adding an int function to commands...",Commands_OneArg).RunTest();
    // Known failure - fixed once tokenizor is written...
    //Test("Adding an strings function to commands...",Commands_StringArgs).RunTest();
    return 0;
}

int Function_NoArgs(testLogger& log) {
    Function<> f(One);
    stringstream input;
    if ( f.Execute(input) != 1 ) {
        log << "One did not return 1! " << f.Execute(input);
        return 1;
    }
    return 0;
}

int Function_OneArg(testLogger& log) {
    Function<int> f(Double);
    stringstream input;
    input << 2;
    int result = f.Execute(input);
    if (  result != 4 ) {
        log << "Double 2 was not 4! " << result;
        return 1;
    };
    return 0;
}


int Commands_NoArgs(testLogger& log) {
    Commands dispatcher;
    dispatcher.AddCommand("One",One);
    int ret = dispatcher.Execute("One");
    int ret2 = dispatcher.Execute("  One");
    int ret3 = dispatcher.Execute("     One   ");
    if ( ret != 1 ) {
        log << "One did not return 1! " << ret;
        return 1;
    }
    if ( ret2 != 1 ) {
        log << "One (2) did not return 1! " << ret;
        return 1;
    }
    if ( ret3 != 1) {
        log << "One (3) did not return 1! " << ret;
        return 1;
    }
    return 0;
}


int Commands_OneArg(testLogger& log) {
    Commands dispatcher;
    dispatcher.AddCommand("double",Double);
    int ret = dispatcher.Execute("double 2");
    int ret2 = dispatcher.Execute("     double         3     ");
    if ( ret != 4 ) { 
        log << "Double 2 was not 4! " << ret;
        return 1;
    }

    if ( ret2 != 6 ) { 
        log << "Double 3 was not 6! " << ret;
        return 1;
    }

    return 0;
}

int Commands_StringArgs(testLogger& log ) {
    Commands dispatcher;
    dispatcher.AddCommand("connect",Basic_Authenticate);
    dispatcher.AddCommand("login",Authenticate);
    int ret = dispatcher.Execute("connect Torajiro Shusaku");
    int ret2 = dispatcher.Execute("login 'Kuwabara Torajiro' Shusaku");

    if ( ret != 0 ) {
        log << " Failed to connect: " << ret;
        return 1;
    }

    if ( ret2 != 0 ) {
        log << " Failed to login: " << ret2;
        return 1;
    }
    return 0;
}
