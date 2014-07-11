#include "tester.h"
#include "Commands.h"
#include <sstream>

using namespace std;

int Function_NoArgs(testLogger& log);
int Function_OneArg(testLogger& log);
int Commands_NoArgs(testLogger& log);
int Commands_OneArg(testLogger& log);
int Commands_StringArgs(testLogger& log);
int MultiCalls(testLogger& log);

int main(void) {
    Test("Constructing a wrapper around a no args function...",Function_NoArgs).RunTest();
    Test("Constructing a wrapper around a an int function...",Function_OneArg).RunTest();
    Test("Adding a no-args function to commands...",Commands_NoArgs).RunTest();
    Test("Adding an int function to commands...",Commands_OneArg).RunTest();
    Test("Adding an strings function to commands...",Commands_StringArgs).RunTest();
    Test("Running multiple commands...",MultiCalls).RunTest();
    return 0;
}

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

class Thing {
public: 
   static int Set(int newVal) { return val = newVal; }
   static int Get() { return val; }
private:
   static int val;
};
int Thing::val = 0;


int Function_NoArgs(testLogger& log) {
    Function<> f(One);
    stringstream input;
    if ( f.Execute(input.str()) != 1 ) {
        log << "One did not return 1! " << f.Execute(input.str());
        return 1;
    }
    return 0;
}

int Function_OneArg(testLogger& log) {
    Function<int> f(Double);
    stringstream input;
    input << 2;
    int result = f.Execute(input.str());
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
    int ret2 = dispatcher.Execute("login \"Kuwabara Torajiro\" Shusaku");

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

int MultiCalls(testLogger& log) {

    Commands dispatcher;
    dispatcher.AddCommand("set",Thing::Set);
    dispatcher.AddCommand("get",Thing::Get);
    dispatcher.AddCommand("one",One);
    int val = dispatcher.Execute("set 5; set 3; one; get");
    if ( val != 3 ) {
        log << " Failed to get correct value: " << val;
        return 1;
    }

    if ( Thing::Get() != 3 ) {
        log << " Failed to set correct value: " << Thing::Get();
        return 1;
    }

    dispatcher.AddCommand("login",Authenticate);
    val = dispatcher.Execute("set 85        ; login \"Kuwabara Torajiro\" Shusaku;");
    if ( val != 0 ) {
        log << " Failed to authenticate!: " << val;
        return 1;
    }

    if ( Thing::Get() != 85 ) {
        log << " Failed to set correct value: " << Thing::Get();
        return 1;
    }

    return 0;
}
