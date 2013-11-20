#ifndef __DEV_TOOLS_LIBRARIES_THREADS_GO_TASK_LISTS_H
#define __DEV_TOOLS_LIBRARIES_THREADS_GO_TASK_LISTS_H
#include "task.h"

template<class T>
class Local_Tasks {
private:
    std::list<rTask<T>> tasks;
public:
    Task_Ref Add(rTask<T> t) {
        tasks.insert(tasks.begin(),t);
        return t;
    }

    decltype(tasks.begin()) begin() {
        return tasks.begin();
    }

    decltype(tasks.end())  end() {
        return tasks.end();
    }

    ~Local_Tasks () {
        for ( rTask<T>& t : tasks) {
            t->Done();
        }
    }
};


#endif
