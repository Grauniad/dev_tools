#ifndef __DEV_TOOLS_LIBCOMMANDS_COMMAND_H__
#define __DEV_TOOLS_LIBCOMMANDS_COMMAND_H__
#include <string>
#include <functional>
#include <map>


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

    virtual int Execute(const std::string& args);
private:
    std::function<int(T...)> proc;
};

template <>
class Function<>: public Func {
public:
    Function (int (*fptr)() );

    virtual int Execute(const std::string& args);
private:
    std::function<int()> proc;
};

class Commands {
public: 
    Commands();

    virtual ~Commands();

    template<class ...T>
    void AddCommand( const std::string& name, int (*fptr)(T... args ) );

    void AddCommand( const std::string& name, int (*fptr)() );

    int Execute(const std::string& command);

private:
    std::map<std::string,Func*> procs;
};

#include "Commands.hpp"

#endif
