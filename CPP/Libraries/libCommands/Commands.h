#ifndef __DEV_TOOLS_LIBCOMMANDS_COMMAND_H__
#define __DEV_TOOLS_LIBCOMMANDS_COMMAND_H__
#include <string>
#include <functional>
#include <map>

class Tokens;


class Func {
public:
   virtual int Execute(const std::string& args) = 0;

   virtual ~Func() {}

   struct ExecutionError {
       ExecutionError(const std::string& errMsg): msg(errMsg) {}
       std::string msg;
   };
};

template<class ...T>
class Function: public Func {
public:
    // C'tors
    Function ( int (*fptr)(T...) );
    Function (const std::function<int(T...)>& f);

    virtual int Execute(const std::string& args);
private:
    std::function<int(T...)> proc;
};

template <>
class Function<>: public Func {
public:
    Function (int (*fptr)() );
    Function (const std::function<int()>& f);

    virtual int Execute(const std::string& args);
private:
    std::function<int()> proc;
};

class Aliases {
public:
    Aliases() = default;

    void AddAlias(const std::string& name, 
                  const std::string& skeleton);

    std::string Expand(const std::string& name, 
                       const std::string& args);
    bool IsAlias(const std::string& name);
private:
    std::map<std::string,std::string> aliases;
};

/*
 * Map command strings to function calls
 */
class Commands {
public: 
    Commands();

    virtual ~Commands();

    template<class ...T>
    void AddCommand( const std::string& name, int (*fptr)(T... args ) );
    template<class ...T>
    void AddCommand( const std::string& name, const std::function<int(T...)>& f);

    void AddCommand( const std::string& name, const std::function<int()>& f);
    void AddCommand(const std::string& name, int(*fptr)());

    /* 
     * Entry point for handling a command line from the user.
     *
     * splits up the string into individual commands and then forwards to 
     * ExecuteSingleCommand
     */ 
    int Execute(const std::string& input);

private:

    /*
     *
     */
    int ExecuteSingleCommand(const std::string& command);

    /*
     * Handler to install a new alias, triggered by an "alias" command given to Exxecute
     */
    int ExecuteAliasCommand(const Tokens& args);

    std::map<std::string,Func*> procs;
    Aliases aliasTable;
};

#include "Commands.hpp"

#endif
