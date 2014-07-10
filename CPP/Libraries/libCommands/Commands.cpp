#include "Commands.h"

using namespace std;

int Func::Execute(const std::string& args) {
    stringstream input(args);
    return Execute(input);
}

Function<>::Function(int (*fptr)() ) 
    : proc (fptr)
{
}

int Function<>::Execute(std::stringstream& input ) {
    return proc();
}

Commands::Commands() {
}

Commands::~Commands() {
    for (auto& pair : procs ) {
        delete pair.second;
    }
}

void Commands::AddCommand(const std::string& name, int (*fptr)() ) {

    auto it = procs.find(name);
    if ( it != procs.end() ) {
        // Already exists - update the pointer
        delete it->second;
        it->second = new Function<>(fptr);
    } else {
        // New function, create the map:
        procs.emplace(name, new Function<>(fptr));
    }
}

int Commands::Execute(const std::string& command) {
    int ret = 0;
    string procName;
    stringstream input(command);
    input >> procName;
    auto it = procs.find(procName);
    if ( it != procs.end() ) {
        ret = it->second->Execute(input);
    } else {
        throw "NOT YET IMPLEMENTED!";
    }

    return ret;
}
