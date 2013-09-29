#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_TASKS_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_TASKS_H

/*
 * Common interface, so we can store in a homogenous array
 */
class Task_Base {
    public:
        virtual void Complete() = 0;
        virtual void Done() = 0;
        virtual ~Task_Base() {}
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

/*
 * Templated Task implementation
 */
template<class T>
class Task: public Task_Base {
public:
    using F = std::function<void(Channel<T>&)>;
    Task(F f, size_t bufferSize = 0) :
        resultStream(bufferSize),
        work(f)
        { }

    virtual ~Task() {
        // Release any blocking threads
        Done();
    }

    // Used by the task owner to get results in real
    // time
    // Will THROW if the resulsStream has been killed
    Task& operator >> (T& item) {
        resultStream >> item;
        return *this;
    }

    virtual void Done() {
        resultStream.Kill();
    }

    virtual void Complete() {
        try {
            //TODO:
            //std::call_once(runFlag,work, resultStream);
            work(resultStream);
        } catch (ChannelDeadException& deadChan) {
            if ( deadChan.Id() == resultStream.Id() ) {
                // Someone has killed the results stream - indicating no
                // more work to do
                return;
            } else {
                // Some other channel has died!
                throw deadChan;
            }
        }
    }
private:
    Channel<T>         resultStream;
    F                  work;
    std::once_flag     runFlag;
};

// Specialise the Task_Ref
template<class T>
class rTask: public Task_Ref {
public:
    // Transfers ownership
    rTask(rTask<T>&& rhs): Task_Ref(std::move(rhs)) {}

    // Now both the lhs and rhs own the object
    rTask(const Task_Ref& rhs): Task_Ref(rhs) {}

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
        Task_Ref::operator=(static_cast<Task_Ref>(task));
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
};

#endif
