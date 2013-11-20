#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#include "channel.h"
#include "channel.hpp"
#include <thread>
#include <atomic>
#include <list>
#include "task.h"

// Create a new on-demand task
#define GO(TYPE,CODE) \
Task_Master::Schedule<TYPE> ([=] (Channel<TYPE>& results) -> void { \
    CODE \
}, TASK_POLICY_ON_DEMAND)

#define QUEUE(TYPE,CODE) \
Task_Master::Schedule<TYPE> ([=] (Channel<TYPE>& results) -> void { \
    CODE \
}, TASK_POLICY_STANDARD)

#define QUEUE_CAPTURE(TYPE, CODE) \
Task_Master::Schedule<TYPE> ([&] (Channel<TYPE>& results) -> void { \
    CODE \
}, TASK_POLICY_STANDARD)

#define RUN(TYPE,CODE) \
Task_Master::Schedule<TYPE> ([=] (Channel<TYPE>& results) -> void { \
    CODE \
}, TASK_POLICY_IMMEDIATE)

#define RUN_CAPTURE(TYPE, CODE) \
Task_Master::Schedule<TYPE> ([&] (Channel<TYPE>& results) -> void { \
    CODE \
}, TASK_POLICY_IMMEDIATE)

/*
 * Simple Task Scheduler 
 */

class Task_Slave {
public:
    Task_Slave(shared_ptr<Channel<Task_Ref> >& workQueue);

    // Dedicated mode
    Task_Slave(Task_Ref t);  

    Task_Slave(Task_Slave&& source): 
        theQueue(std::move(source.theQueue)),
        worker(std::move(source.worker)) {}

    ~Task_Slave() {
        delete worker;
    }

    atomic_long& ThreadId () {
        return threadId;
    }

    void Wait() {
        worker->join();
    }
private:
    void DoWork();
    void DoTask(Task_Ref t);
    void DoDedicated(Task_Ref t);
    shared_ptr<Channel<Task_Ref> >  theQueue;
    atomic_long                     threadId;
    std::thread*                    worker;
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

   void StartDedicated(Task_Ref t);
   void Promote(Task_Ref t);

   void AddThreads(size_t n);

   size_t FreeWorkers() {
       return theQueue->BlockedReaders();
   }

   template<class T>
   static rTask<T> Schedule( typename Task<T>::F f,
                             TASK_POLICY policy = TASK_POLICY_ON_DEMAND,
                             size_t resultQueueSize =0 );


   void Kill();
private:
   Task_Master(); // Spawn the slave threads, and build the wait queues

   shared_ptr<Channel<Task_Ref> >       theQueue;

   std::list<Task_Slave>   slaves;
   std::atomic_size_t      numThreads;
};
#include "go.hpp"

#endif
