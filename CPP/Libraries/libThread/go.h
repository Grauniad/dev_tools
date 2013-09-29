#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#include "channel.h"
#include "channel.hpp"
#include <thread>
#include <atomic>
#include "task.h"

#define GO(TYPE,CODE) \
Task_Master::Schedule<TYPE> ( [=] (Channel<TYPE>& results) -> void { \
    CODE \
});

/*
 * Simple Task Scheduler 
 */

class Task_Slave {
public:
    Task_Slave(shared_ptr<Channel<Task_Ref> >& workQueue);

    Task_Slave(Task_Slave&& source): 
        theQueue(std::move(source.theQueue)),
        worker(std::move(source.worker)) {}

    atomic_long& ThreadId () {
        return threadId;
    }

    void Wait() {
        worker.join();
    }
private:
    void DoWork();
    shared_ptr<Channel<Task_Ref> >  theQueue;
    std::thread                     worker;
    atomic_long                     threadId;
};


// Manages a thread pool
class Task_Master {
public:
   static Task_Master& Scheduler() {
       static Task_Master master;
       return master;
   }

   virtual ~Task_Master() {
       // release the threads
       Kill();
   }

   void Schedule(Task_Ref t);

   template<class T>
   static rTask<T> Schedule( typename Task<T>::F f,
                         size_t resultQueueSize =0 ) {
       rTask<T> t(new Task<T>(f,resultQueueSize));
       Scheduler().Schedule(t);
       return std::move(t);
   }

   void Kill();
private:
   Task_Master(); // Spawn the slave threads, and build the wait queues

   shared_ptr<Channel<Task_Ref> >       theQueue;

   std::vector<Task_Slave> slaves;
   std::atomic_size_t      numThreads;
};


#endif
