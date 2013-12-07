#include "task.hpp"

template<class T>
rTask<T> Task_Master::Schedule( typename Task<T>::F f,
                                 TASK_POLICY policy,
                                 size_t resultQueueSize) 
{
   rTask<T> t(new Task<T>(f,resultQueueSize,policy));
    switch (policy) {

    /*
     * In the simple cases all we have to is add to the back
     * of the queue
     */
    case TASK_POLICY_STANDARD:
    case TASK_POLICY_ON_DEMAND:
       Scheduler().Schedule(t);
       return std::move(t);

    /*
     * We MUST fork a new thread
     */
    case TASK_POLICY_DEDICATED:
       Scheduler().StartDedicated(t);
       return std::move(t);

    /*
     * If we can't run it immediately we must fork a new thread
     */
    case TASK_POLICY_IMMEDIATE:
       // This needs to be an atomic action
       Scheduler().theQueue->Lock();
       if ( Scheduler().theQueue->BlockedReaders() > 0 )  {
           Scheduler().Promote(t);

           // We had to hold the lock until we were head of the queue
           Scheduler().theQueue->Unlock();
       } else {
           // No point locking, we're going to spawn a new 
           // thread anyway
           Scheduler().theQueue->Unlock();
           Scheduler().StartDedicated(t);
       }
       return std::move(t);

    }
    return std::move(t);
}
