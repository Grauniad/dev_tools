#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_TASKS_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_TASKS_H

/*
 * Common interface, so we can store in a homogenous array
 */
class Task_Base {
public:
    Task_Base(): started(false) {}
    virtual void Complete() = 0;
    virtual void Done() = 0;
    virtual ~Task_Base() {}

    atomic_bool& Started() {
        return started;
    }

    virtual long Id() = 0;
private:
    atomic_bool    started;
};


enum TASK_POLICY {
    TASK_POLICY_DEDICATED, /* Force the scheduler to create a new thread */
    TASK_POLICY_IMMEDIATE, /* Start immediately (create a new thread if existing pool is busy) */
    TASK_POLICY_ON_DEMAND, /* Queue the task, if it is not running at the time of first read, run it in a new thread*/
    TASK_POLICY_STANDARD,  /* FIFO semantics ** BEWARE DEADLOCKS! ** */
};

/*
 * Implements reference counting so that the task is not destroyed until
 * all interested threads are done with it
 */
class Task_Ref {
public:
    // Transfers ownership
    Task_Ref(Task_Ref&& rhs): tsk(std::move(rhs.tsk)) {}

    // Now both the lhs and rhs own the object
    Task_Ref(const Task_Ref& rhs): tsk(rhs.tsk) {}

    // Manages a new task
    Task_Ref(Task_Base* task): tsk(task) {}

    ~Task_Ref() {
        tsk.reset();
    }

    // Transfers ownership
    Task_Ref& operator=(Task_Ref&& rhs) {
         tsk.swap(rhs.tsk);
         return *this;
    }

    // Shares ownership
    Task_Ref& operator=(const Task_Ref& rhs) {
        tsk.reset(rhs.tsk.get());
        return *this;
    }

    // Takes ownership of a new Task
    Task_Ref& operator=(Task_Base* task) {
        tsk.reset(task);
        return *this;
    }

    Task_Base* operator& () {
        return tsk.get();
    }

    Task_Base* operator-> () {
        return tsk.get();
    }


protected:
    shared_ptr<Task_Base>    tsk;
};

template<class T>
class rTask;

/*
 * Templated Task implementation
 */
template<class T>
class Task: public Task_Base {
public:
    friend class rTask<T>;
    using F = std::function<void(Channel<T>&)>;
    Task( F f, 
          size_t bufferSize = 0, 
          TASK_POLICY pol = TASK_POLICY_ON_DEMAND) :
            resultStream(bufferSize),
            work(f),
            policy(pol)
            { }

    virtual ~Task() {
        SLOG_FROM( LOG_SCHEDULER, 
                  "Task::~Task",
                  "Thread (" << Thread::MyId() 
                            << ") is killing task "
                            << Id() 
                            << ", declaring done..."
                  )
        /*
         *  - Stop any new threads starting to wait
         *  - Throws out the executing thread (if any)
         */
        Done();

        // Lock down the task...
        std::unique_lock<std::mutex> runLock(runMutex);

        // No-one is executing us, and no one can start waiting, but
        // we must wait for anyone currently waiting on the channel to
        // receive the death signal
        while (    resultStream.BlockedReaders() > 0 
                || resultStream.BlockedWriters() > 0 ) 
        {
            SLOG_FROM( LOG_SCHEDULER, 
                      "Task::~Task",
                      "Thread (" << Thread::MyId() 
                                << ") is yielind as there are still "
                                << "blocked readers / writers on task "
                                << Id() 
                      )
            this_thread::yield();
        }

        SLOG_FROM( LOG_SCHEDULER, 
                  "Task::~Task",
                  "Thread (" << Thread::MyId() 
                            << ") is killing task "
                            << Id() 
                            << ", locking down results..."
                  )

        // no-one is still waiting on us (and no one can start)

        // Finally, we lock the channel - When this returns we know
        // the final thread must have left exeuction in the class
        resultStream.Lock();

        // No one has a reference to the class, no-one is in the
        // class, and no-one is executing the work. We can now die!
        resultStream.Unlock();

        SLOG_FROM( LOG_SCHEDULER, 
                  "Task::~Task",
                  "Thread (" << Thread::MyId() 
                            << ") has destroyed task "
                            << Id() 
                  )

    }


    virtual void Done() {
        resultStream.Kill();
        // Wait for a running application to release the task
        std::unique_lock<std::mutex> runLock(runMutex);
    }

    virtual void Complete() {
        // Make sure we are in so control of the task...
        std::unique_lock<std::mutex> runLock(runMutex);

        if ( Started() ) {
            SLOG_FROM( LOG_SCHEDULER, 
                      "Task::Complete",
                      "Thread (" << Thread::MyId() 
                                << ") did not start task "
                                << Id() 
                                << " because it has already started"
                      )
            // Prevent a double run
            return;
        }

        // Make sure we haven't been "Done"
        if ( !resultStream.Alive() ) {
            SLOG_FROM( LOG_SCHEDULER, 
                      "Task::Complete",
                      "Thread (" << Thread::MyId() 
                                << ") did not start task "
                                << Id() 
                                << " because the result stream is dead"
                      )
            // Prevent running a dead task
            return;
        }

        try {
            Started() = true;

            SLOG_FROM( LOG_SCHEDULER, 
                      "Task::Complete",
                      "Thread (" << Thread::MyId() 
                                << ") started task "
                                << Id() 
                      )
            work(resultStream);

            SLOG_FROM( LOG_SCHEDULER, 
                      "Task::Complete",
                      "Thread (" << Thread::MyId() 
                                << ") completed task "
                                << Id() 
                      )
        } catch (ChannelDeadException& deadChan) {
            if ( deadChan.Id() == resultStream.Id() ) {
                SLOG_FROM( LOG_SCHEDULER, 
                          "Task::Complete",
                          "Thread (" << Thread::MyId() 
                                    << ") stopped - channel is dead"
                                    << Id() 
                          )
                // Someone has killed the results stream - indicating no
                // more work to do
                return;
            } else {
                // Some other channel has died!
                throw deadChan;
            }
        }
    }

    virtual long Id() {
        return resultStream.Id();
    }

    void Lock();

    void Unlock();


private:
    Channel<T>         resultStream;
    F                  work;
    std::once_flag     runFlag;
    TASK_POLICY        policy;
    std::mutex         runMutex;
};


// Specialise the Task_Ref
template<class T>
class rTask: public Task_Ref {
public:
    // Transfers ownership
    rTask(rTask<T>&& rhs): Task_Ref(std::move(rhs)) {}

    // Now both the lhs and rhs own the object
    rTask(const rTask<T>& rhs): Task_Ref(rhs) {}

    // Manages a new task
    rTask(Task<T>* task): Task_Ref(task) {}

    // Transfers ownership
    rTask<T>& operator=(rTask<T>&& rhs) {
        Task_Ref::operator=(std::move(rhs));
        return *this;
    }

    // Shares ownership
    rTask<T>& operator=(const rTask<T>& rhs) {
        Task_Ref::operator=(rhs);
        return *this;
    }

    // Takes ownership of a new Task
    rTask<T>& operator=(Task<T>* task) {
        Task_Ref::operator=(task);
        return *this;
    }

    Task<T>& operator* () {
        return *static_cast<Task<T>* >(tsk.get());
    }

    Task<T>* operator& () {
        return static_cast<Task<T>* >(tsk.get());
    }

    Task<T>* operator-> () {
        return static_cast<Task<T>* >(tsk.get());
    }

    Channel<T>& Results(); 
    
    // Used by the task owner to get results in real
    // time
    // Will THROW if the resulsStream has been killed!
    rTask& operator >> (T& item);
};
#endif
