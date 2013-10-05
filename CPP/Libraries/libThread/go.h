#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_H
#include "channel.h"
#include "channel.hpp"
#include <thread>
#include <atomic>
#include <list>
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

    // Dedicated mode
    Task_Slave(Task_Ref t);  

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
    void DoTask(Task_Ref t);
    void DoDedicated(Task_Ref t);
    shared_ptr<Channel<Task_Ref> >  theQueue;
    atomic_long                     threadId;
    std::thread                     worker;
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

   // Default case:
   //   - ON_DEMAND
   //   - STANDARD
   template<class T>
   static rTask<T,POLICY> Schedule( typename Task<T,POLICY>::F f,
                                    TASK_POLICY policy = TASK_POLICY_ON_DEMAND,
                                    size_t resultQueueSize =0 );


   void Kill();
private:
   Task_Master(); // Spawn the slave threads, and build the wait queues

   shared_ptr<Channel<Task_Ref> >       theQueue;

   std::list<Task_Slave>   slaves;
   std::atomic_size_t      numThreads;
};

template<class T>
rTask<T,POLICY> Task_Master::Schedule_NORMAL ( typename Task<T,POLICY>::F f,
                                               size_t resultQueueSize) 
{
   rTask<T,POLICY> t(new Task<T,POLICY>(f,resultQueueSize));
   Scheduler().Schedule(t);
   return std::move(t);
}

// Dedicated: Always start a new thread
template<class T>
rTask<T,TASK_POLICY_DEDICATED> 
Task_Master::Schedule<T,TASK_POLICY_DEDICATED>
    ( typename Task<T,TASK_POLICY_DEDICATED>::F f,
      size_t resultQueueSize) 
{
   rTask<T,TASK_POLICY_DEDICATED> 
       t(new Task<T,TASK_POLICY_DEDICATED>(f,resultQueueSize));
   //Scheduler().StartDedicated(t);
   return std::move(t);
}

// Immediate: Start a new thread if none available
template<class T>
static rTask<T,TASK_POLICY_IMMEDIATE> 
Task_Master::Schedule<T,TASK_POLICY_IMMEDIATE>
     ( typename Task<T,TASK_POLICY_IMMEDIATE>::F f,
       size_t resultQueueSize =0 ) 
{
   rTask<T,TASK_POLICY_IMMEDIATE> 
       t(new Task<T,TASK_POLICY_IMMEDIATE>(f,resultQueueSize));

   // This needs to be an atomic action
   theQueue->Lock();
   if ( theQueue->BlockedReaders() > 0 )  {
       Promote(t);

       // We had to hold the lock until we were head of the queue
       theQueue->Unlock();
   } else {
       // No point locking, we're going to spawn a new 
       // thread anyway
       theQueue->Unlock();
       StartDedicated(t);
   }

   return std::move(t);
}


#endif
