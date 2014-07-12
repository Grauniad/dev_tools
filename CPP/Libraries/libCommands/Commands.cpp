#include "Commands.h"
#include "logger.h"
#include "stringUtils.h"
#include "tokenizor.h"

using namespace std;

Function<>::Function(int (*fptr)() ) 
    : proc (fptr)
{
}

Function<>::Function(const function<int()>& f)
    : proc (f)
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

void Aliases::AddAlias(const std::string& name, const std::string& skeleton) {
    aliases[name] = skeleton;
}

bool Aliases::IsAlias(const std::string& name) {
    auto it = aliases.find(name);
    return it != aliases.end();
}

std::string Aliases::Expand(const std::string& name, 
                            const std::string& args) 
{
    auto it = aliases.find(name);
    if ( it != aliases.end() ) {
        return StringUtils::Substitute(it->second, args);
    } else {
        return args;
    }
}

void Commands::AddCommand(const std::string& name, int(*fptr)()) {
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

void Commands::AddCommand(const std::string& name, const std::function<int()>& f) {
    auto it = procs.find(name);
    if ( it != procs.end() ) {
        // Already exists - update the pointer
        delete it->second;
        it->second = new Function<>(f);
    } else {
        // New function, create the map:
        procs.emplace(name, new Function<>(f));
    }
}

int Commands::Execute(const std::string& input) {
    SLOG_FROM(LOG_VERBOSE,"Commands::Execute",
              "Executing script: " << input)
    int ret = 0;
    // Split into individual commands
    Tokens commands(input,';',true);

    for ( string& command : commands ) {
        ret = ExecuteSingleCommand(command);
    }

    return ret;
}

int Commands::ExecuteSingleCommand(const std::string& command) {
    SLOG_FROM(LOG_VERBOSE,"Commands::ExecuteSingleCommand",
              "Executing command: " << command)
    int ret = 0;
    Tokens tokens(command);

    string procName = tokens[0];

    if ( aliasTable.IsAlias(procName) ) {
        /* This command is a user alias. This may expand into
         * multiple commands so execute it as a new command line
         *
         * We pass the un-parsed string through since to preserve
         * quotes etc during string expansion.
         */
        stringstream ss(command);
        ss >> procName;
        string args;
        getline(ss,args);
        Execute(aliasTable.Expand(procName,args));
    } else if ( procName == "alias" ) {
        // The user wants to define a new alias
        ret = ExecuteAliasCommand(tokens);
    } else {
        // A standard function call
        auto it = procs.find(procName);
        if ( it != procs.end() ) {
            // Execute will tokenise itself..
            stringstream ss(command);
            ss >> procName;
            string args;
            getline(ss,args);

            // TODO: we're parsing the string twice here. In most case
            //       refactor if it becomes a bottle neck.
            ret = it->second->Execute(args);
        } else {
            throw "NOT YET IMPLEMENTED!";
        }
    }
    return ret;
}

int Commands::ExecuteAliasCommand(const Tokens& args) {
    // args 0 was alias
    string name(args[1]);
    string skel(args[2]);
    if ( name != "" && skel != "" ) {
        aliasTable.AddAlias(name,skel);
    }
    return 0;
}
