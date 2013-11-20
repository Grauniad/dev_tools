#include "go.h"

#include "env.h"
#include <sstream>
#include "logger.h"
#include "thread_utils.h"


Task_Slave::Task_Slave(shared_ptr<Channel<Task_Ref> >& workQueue):
    theQueue(workQueue),
    threadId(Thread::MyId())

{ 
    SLOG_FROM( LOG_SCHEDULER, 
              "Task_Slave::Task_Slave",
              "Thread (" << Thread::MyId() 
                        << ") is going to create a new slave"
              )
    // Move assign the thread
    worker = new std::thread{&Task_Slave::DoWork,this};
}

Task_Slave::Task_Slave(Task_Ref t):
    theQueue(nullptr),
    threadId(Thread::MyId())
{ 
    SLOG_FROM( LOG_SCHEDULER, 
              "Task_Slave::Task_Slave",
              "Thread (" << Thread::MyId() 
                        << ") is going to start a new thread"
                        << " for a dedicated task"
              )
    worker = new std::thread{&Task_Slave::DoTask,this,t};
}

void Task_Slave::DoDedicated(Task_Ref t) {
    ThreadId() = Thread::MyId();
    DoTask(t);
}

void Task_Slave::DoWork() {
    try {
        // We've changed thread
        ThreadId() = Thread::MyId();

        Task_Ref t(nullptr);
        while (true) {
            SLOG_FROM( LOG_SCHEDULER, 
                      "Task_Slave::DoWork",
                      "Thread (" << Thread::MyId() 
                                << ") Is waiting for work"
                      )

            t = theQueue->Get();
            DoTask(t);
            // We don't need the task anymore
            t = nullptr;

        }
    } catch ( ChannelDeadException& deadChan ) {
        if ( deadChan.Id() == theQueue->Id() ) {
            // Someone has killed the task queue - indicating no
            // more work to do
              
            SLOG_FROM( LOG_SCHEDULER, 
                      "Task_Slave::DoWork",
                      "Thread (" << Thread::MyId() 
                       << ") has handled the death of the task queue."
                      )
            return;
        } else {
            // Some other channel has died!
            throw deadChan;
        }
    }
}


void Task_Slave::DoTask(Task_Ref t) {
    SLOG_FROM( LOG_SCHEDULER, 
              "Task_Slave::DoTask",
              "Thread (" << Thread::MyId() 
                        << ") Is doing work on " << t->Id()
              )

    t->Complete();

    SLOG_FROM( LOG_SCHEDULER, 
              "Task_Slave::DoTask",
              "Thread (" << Thread::MyId() 
                        << ") Has finished its work on " << t->Id();
              )
}


Task_Master::Task_Master(): theQueue(new Channel<Task_Ref>(0)) 
{
    // How many threads?
    if (ENV::IsSet("TASK_MASTER_NUM_THREADS")) {
        numThreads = ENV::GetEnvValue<size_t>( 
                         "TASK_MASTER_NUM_THREADS",
                         1);
    } else {
        // Fall back to the system default
        numThreads = thread::hardware_concurrency();
        if (numThreads == 0 ) {
            numThreads = 1;
        }
    }

    SLOG_FROM( LOG_SCHEDULER, 
              "Task_Master::Task_Master",
              "Initialising Scheduler with " << numThreads
              << " threads"
              )

    // Starting at 1: We're already in one thread
    for ( size_t i=1; i< numThreads; i++ ) {
        slaves.emplace_back(theQueue);
    }
}

void Task_Master::Schedule(Task_Ref t) {
    if ( numThreads == 1 ) {
        SLOG_FROM( LOG_SCHEDULER, 
                  "Task_Master::Schedule",
                  "Running in single threaded mode - starting work "
                  << "( Thread: " << Thread::MyId() << ")"
                  )

        t->Complete();

        SLOG_FROM( LOG_SCHEDULER, 
                  "Task_Master::Schedule",
                  "Work Complete "
                  << "( Thread: " << Thread::MyId() << ")"
                  )
    } else {
        SLOG_FROM( LOG_SCHEDULER, 
                  "Task_Master::Schedule",
                  "Scheduling new task with channel "
                  << t->Id()
                  << " ( Thread: " << Thread::MyId() << ")"
                  )

        (*theQueue) << Task_Ref(t);

        SLOG_FROM( LOG_SCHEDULER, 
                  "Task_Master::Schedule",
                  "New task (" << t->Id() << ") scheduled, current queue size: "
                  << theQueue->Size()
                  << " ( Thread: " << Thread::MyId() << ")"
                  )
    }
}

void Task_Master::StartDedicated(Task_Ref t) {
    if ( t->Started() ) {
        // If its already running we don't have to do anything
        return;
    }
    slaves.emplace_back(t);
}

void Task_Master::Promote(Task_Ref t) {
    if ( t->Started() ) {
        // If its already running we don't have to do anything
        return;
    }
    theQueue->Put(std::move(t),CHANNEL_LOC_FRONT);
}

void Task_Master::AddThreads(size_t n) {
    // Add threads to the pool
    for(size_t i =0; i< n; i++) {
        slaves.emplace_back(theQueue);
    }

    numThreads+=n;
}

void Task_Master::Kill() {
   SLOG_FROM( LOG_SCHEDULER, 
             "Task_Master::Kill",
             "Thread (" << Thread::MyId() 
              << ") is dispatching a kill to the task queue"
             )

   theQueue->Kill();

   SLOG_FROM( LOG_SCHEDULER, 
             "Task_Master::Kill",
             "kill has been dispatched, waiting for threads..."
             )

   for (Task_Slave& s: slaves) {
       SLOG_FROM( LOG_SCHEDULER, 
                 "Task_Master::Kill",
                 "Waiting for slave in thread ("
                 << s.ThreadId() << ")"
                 )

       s.Wait();

       SLOG_FROM( LOG_SCHEDULER, 
                 "Task_Master::Kill",
                 "Slave in thread (" << s.ThreadId()
                 << ") has completed"
                 )
   }

   SLOG_FROM( LOG_SCHEDULER, 
             "Task_Master::Kill",
             "All concurrent slaves are now dead, deleting handles"
             )

   slaves.clear();

   SLOG_FROM( LOG_SCHEDULER, 
             "Task_Master::Kill",
             "Clean up of concurrent slaves is now complete"
             )
}
