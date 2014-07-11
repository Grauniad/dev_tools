#include "Commands.h"
#include "logger.h"

using namespace std;

Function<>::Function(int (*fptr)() ) 
    : proc (fptr)
{
}

int Function<>::Execute(const std::string& input ) {
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

int Commands::Execute(const std::string& input) {
    SLOG_FROM(LOG_VERBOSE,"Commands::Execute",
              "Executing script: " << input)
    int ret = 0;
    Parts commands(input,";");

    for ( string& command : commands ) {
        ret = ExecuteSingleCommand(command);
    }

    return ret;
}

int Commands::ExecuteSingleCommand(const std::string& command) {
    SLOG_FROM(LOG_VERBOSE,"Commands::ExecuteSingleCommand",
              "Executing command: " << command)
    int ret = 0;
    string procName;
    stringstream input(command);
    input >> procName;
    string args = input.str().substr(input.str().find(procName) + procName.length());
    auto it = procs.find(procName);
    if ( it != procs.end() ) {
        ret = it->second->Execute(args);
    } else {
        throw "NOT YET IMPLEMENTED!";
    }
    return ret;
}
