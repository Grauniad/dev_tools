#ifndef __DEV_TOOLS_LIBRARIES_THREADS_CHANNEL_HPP
#define  __DEV_TOOLS_LIBRARIES_THREADS_CHANNEL_HPP

#include "logger.h"
#include <thread>
#include "thread_utils.h"

template <class T>
Channel<T>::Channel(size_t max): 
    state(CHANNEL_STATE_ALIVE),
    MAX_QUEUED(max),
    readers(0),
    writers(0)
{
}

template <class T>
void Channel<T>::Kill() {

    SLOG_FROM( LOG_CHANNEL, 
              Context("Kill"),
              "Thread (" << Thread::MyId() 
                        << ") is killing me!"
              )

    // If we don't lock the channel it is possible for a thread to
    // decide the channel is alive, but not start waiting until AFTER
    // we've killed the channel - leaving it waiting forever
    Lock();
        state = CHANNEL_STATE_DEAD;

        // Wake everyone up so they can handle the kill
        queueReadFlag.notify_all();
        queueWriteFlag.notify_all();
    Unlock();

    SLOG_FROM( LOG_CHANNEL, 
              Context("Kill"),
              "Thread (" << Thread::MyId() 
                        << ") has killed me!"
              )
}

template <class T>
size_t Channel<T>::Size() { 
    SLOG_FROM( LOG_LOCKS, 
              Context("Size"),
              "Thread (" << Thread::MyId() 
                        << ") is seeking a channel lock!"
              )

    // First get the queue lock
    std::unique_lock<std::recursive_mutex> queueLock(queueMutex);

    SLOG_FROM( LOG_LOCKS, 
              Context("Size"),
              "Thread (" << Thread::MyId() 
                        << ") acquired a channel lock!, "
                        << "it will now imediately release it"
              )

    return taskQueue.size(); 
}

// Stop all read / write activity on the channel
template<class T>
void Channel<T>::Lock() {
    SLOG_FROM( LOG_LOCKS, 
              Context("Lock"),
              "Thread (" << Thread::MyId() 
                        << ") is seeking a channel lock!"
              )

    queueMutex.lock();

    SLOG_FROM( LOG_LOCKS, 
              Context("Lock"),
              "Thread (" << Thread::MyId() 
                        << ") acquired a channel lock!"
              )
}

template <class T>
void Channel<T>::Unlock() {
    queueMutex.unlock();

    SLOG_FROM( LOG_LOCKS, 
              Context("Unlock"),
              "Thread (" << Thread::MyId() 
                        << ") released a channel lock!"
              )
}

template<class T>
void Channel<T>::Put( T&& item, CHANNEL_LOC loc) {
    SLOG_FROM( LOG_LOCKS, 
              Context("Put"),
              "Thread (" << Thread::MyId() 
                        << ") is seeking a channel lock!"
              )
    // First get the queue lock
    std::unique_lock<std::recursive_mutex> queueLock(queueMutex);

    SLOG_FROM( LOG_LOCKS, 
              Context("Put"),
              "Thread (" << Thread::MyId() 
                        << ") acquired a channel lock!"
              )

    while (    state != CHANNEL_STATE_ALIVE 
            || (MAX_QUEUED != 0 && taskQueue.size() >= MAX_QUEUED)  ) 
    {
        if ( state == CHANNEL_STATE_DEAD ) {
            SLOG_FROM( LOG_CHANNEL, 
                      Context("Put"),
                      "Thread (" << Thread::MyId() 
                       << ") is trying to write to a dead channel!"
                      )

            SLOG_FROM( LOG_LOCKS, 
                      Context("Put"),
                      "Thread (" << Thread::MyId() 
                       << ") is releasing channel lock (throwing)"
                      )
            throw ChannelDeadException(*this);
        }
        SLOG_FROM( LOG_LOCKS, 
                  Context("Put"),
                  "Thread (" << Thread::MyId() 
                            << ") is yielding a channel lock to wait()"
                  )
        // We now own the queue: but there is no room! We must release
        // our lock and wait for someone to make some room...
        writers++;
            queueReadFlag.wait(queueLock);
        writers--;
        // NOTE: wait will have re-aquired the queue for us; we're 
        //       back in sole control
        SLOG_FROM( LOG_LOCKS, 
                  Context("Put"),
                  "Thread ("<< Thread::MyId() 
                           << ") was awoken, and acquired a channel lock"
                  )
    }

    // Move construct the item on the queue
    if ( loc == CHANNEL_LOC_BACK ) {
        taskQueue.emplace_back(std::move(item));
    } else {
        taskQueue.emplace_front(std::move(item));
    }

    // Notify a waiting reader (if any)
    queueWriteFlag.notify_one();

    SLOG_FROM( LOG_CHANNEL, 
              Context("Put"),
              "Thread ("<< Thread::MyId() 
                       << ") Pushed an item into the channel" << endl
                       << "The queue is now of size: " << Size();
              )

    SLOG_FROM( LOG_LOCKS, 
              Context("Put"),
              "Thread ("<< Thread::MyId() 
                       << ") Is done with Channel, releasing lock..."
              )

    // queue released when queueLock is unwound
}

template<class T>
Channel<T>& Channel<T>::operator<< (T&& item) {
    Put(std::move(item));
    return *this;
}

template<class T>
T Channel<T>::Get() {
    SLOG_FROM( LOG_LOCKS, 
              Context("Get"),
              "Thread (" << Thread::MyId() 
                        << ") is seeking a channel lock!"
              )

    // First get the queue lock 
    std::unique_lock<std::recursive_mutex> queueLock(queueMutex);

    SLOG_FROM( LOG_LOCKS, 
              Context("Get"),
              "Thread (" << Thread::MyId() 
                        << ") acquired a channel lock!"
              )

    while (    state != CHANNEL_STATE_ALIVE
            || taskQueue.size() == 0 ) 
    {

        if ( state == CHANNEL_STATE_DEAD ) {
            SLOG_FROM( LOG_CHANNEL, 
                      Context("Get"),
                      "Thread (" << Thread::MyId() 
                       << ") is trying to read from a dead channel!"
                      )

            SLOG_FROM( LOG_LOCKS, 
                      Context("Get"),
                      "Thread (" << Thread::MyId() 
                       << ") is releasing channel lock (throwing)"
                      )

            throw ChannelDeadException(*this);
        }

        SLOG_FROM( LOG_LOCKS, 
                  Context("Get"),
                  "Thread (" << Thread::MyId() 
                            << ") is yielding a channel lock to wait()"
                  )
        // We now own the queue, but there is no data! We must release 
        // the lock and wait for someonen to put some there...
        readers++;
            queueWriteFlag.wait(queueLock);
            // NOTE: wait will have re-aquired the queue for us; we're 
            //       back in sole control
        readers--;
        SLOG_FROM( LOG_LOCKS, 
                  Context("Get"),
                  "Thread ("<< Thread::MyId() 
                           << ") was awoken, and acquired a channel lock"
                  )
    }

    // Move the item;
    T item(taskQueue.front());

    // Pop the queue
    taskQueue.pop_front();

    // Notify a waiting writer (if any)
    queueReadFlag.notify_one();

    SLOG_FROM( LOG_CHANNEL, 
              Context("Get"),
              "Thread ("<< Thread::MyId() 
                       << ") Pulled an item from a channel." << endl
                       << "The queue is now of size: " << Size();
              )

    SLOG_FROM( LOG_LOCKS, 
              Context("Get"),
              "Thread ("<< Thread::MyId() 
                       << ") Is done with Channel, releasing lock..."
              )

    return item;
}

template <class T>
bool Channel<T>::Alive() {
    return state == CHANNEL_STATE_ALIVE;
}

#endif
