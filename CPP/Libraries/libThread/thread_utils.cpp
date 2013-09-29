#include "thread_utils.h"
#include <unordered_map>
#include <thread>

using namespace std;


long Thread::MyId() { 
    static unordered_map<std::thread::id,long> threadIds;
    static long nextId = 0;

    auto id =  this_thread::get_id();
    auto it = threadIds.find(id) ;

    if ( it != threadIds.end() )  {
        return it->second;
    } else {
        threadIds[id] = nextId;
        ++nextId;
        return nextId-1;
    }
}
