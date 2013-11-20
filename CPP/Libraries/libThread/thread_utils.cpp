#include "thread_utils.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;



long Thread::MyId() { 
    static std::mutex idLock;
    static unordered_map<std::thread::id,long> threadIds;
    static long nextId = 0;

    std::unique_lock<std::mutex> lock(idLock);

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

void Thread::Sleep(long us ) {
    chrono::microseconds dura(us);
    this_thread::sleep_for(dura);
}
