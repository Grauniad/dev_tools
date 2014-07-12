#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <type_traits>
#include <tuple>

#include "populate_tuple_from_tokens.hpp"
#include "call_from_tupple.hpp"

template<class ...T>
Function<T...>::Function(int (*fptr)(T...)) 
    : proc(fptr)
{
}

template<class ...T>
Function<T...>::Function(const std::function<int(T...)>& f)
    : proc (f)
{
}

template<class ...T>
int Function<T...>::Execute(const std::string& input ) {
    Tokens tokens(input);
    std::tuple<T...> args;
    int ret = 0;
    ret = TupleForEach::PopulateTuple<T...>(tokens,args);

    if ( ret ) {
        ret = CallFromTuple::CallFromTuple(&proc,args); 
    } else {
        throw ExecutionError("Failed to parse arguments");
    }
    return ret;
}

template<class ...T>
void Commands::AddCommand(const std::string& name, const std::function<int(T...)>& f) {

    auto it = procs.find(name);
    if ( it != procs.end() ) {
        // Already exists - update the pointer
        delete it->second;
        it->second = new Function<T...>(f);
    } else {
        // New function, create the map:
        procs.emplace(name, new Function<T...>(f));
    }
}

template<class ...T>
void Commands::AddCommand(const std::string& name, int (*fptr)(T...) ) {

    auto it = procs.find(name);
    if ( it != procs.end() ) {
        // Already exists - update the pointer
        delete it->second;
        it->second = new Function<T...>(fptr);
    } else {
        // New function, create the map:
        procs.emplace(name, new Function<T...>(fptr));
    }
}
