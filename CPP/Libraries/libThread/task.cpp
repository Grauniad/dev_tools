#include "task.h"


// Stop all read / write activity on the channel
void Task_Base::Lock() {
    SLOG_FROM( LOG_LOCKS, 
              "Task::Lock",
              "Thread (" << Thread::MyId() 
                        << ") is seeking a lock on"
                        << " task " << Id()
              )

    infoMutex.lock();

    SLOG_FROM( LOG_LOCKS, 
              "Task::Lock",
              "Thread (" << Thread::MyId() 
                        << ") acquired a lock on "
                        << " task " << Id()
              )
}

void Task_Base::Unlock() {
    infoMutex.unlock();

    SLOG_FROM( LOG_LOCKS, 
              "Task::Unlock",
              "Thread (" << Thread::MyId() 
                        << ") released lock on task"
                        << Id()
              )
}
