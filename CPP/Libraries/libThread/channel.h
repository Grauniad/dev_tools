#ifndef __DEV_TOOLS_LIBRARIES_THREADS_CHANNEL_H
#define  __DEV_TOOLS_LIBRARIES_THREADS_CHANNEL_H

#include <forward_list>
#include <atomic>
#include <mutex>
#include <queue>
#include <deque>
#include <atomic>
#include <condition_variable>
#include <string>
#include <sstream>

/* * Inter-thread communication mechanism, losely inspired by Go's
 * * channels
 *
 * The items must support MOVE SEMANTICS.
 *
 * One or more threads may push items into the queue until MAX_QUEUED 
 * is reached. At this point, further pushes will block there is 
 * room in the QUEUE. 
 *
 * One or more (different or same) threads may also pull from the queue.
 * When there are no more elements in the queue, the pull blocks until
 * there is an element in the queue.
 * 
 * Pushing and pulling is thread safe, but may be blocking
 *
 * Any thread may kill the channel at any time. Anyone currently (or in
 * the future) trying to read / write to the channel will be thrown an
 * instance of ChannelDeadException
 *
 *
 *
 */

enum CHANNEL_STATE {
    CHANNEL_STATE_ALIVE,
    CHANNEL_STATE_DEAD,
    __NUM_CHANNEL_STATES
};

enum CHANNEL_LOC {
    CHANNEL_LOC_FRONT,
    CHANNEL_LOC_BACK,
    __NUM_CHANNEL_LOCS
};


long ChannelId();

class Channel_Base {
public:
    Channel_Base (): myId(ChannelId()) { }
    long Id() {
        return myId;
    }

    virtual std::atomic_size_t& QueueSize() =0;

    virtual ~Channel_Base() {}
private:
    std::atomic_long   myId;
};

template <class T>
class Channel: public Channel_Base {
public:
    Channel(size_t max); 

    size_t Size();

    // Stop all read / write activity on the channel
    void Lock();

    // Resume read / write activity on the channel
    void Unlock();

    // Pushes an item into the queue. If there is no room 
    // it must wait
    Channel<T>& operator<< (T&& item);

    Channel<T>& operator>> (T& pitem) {
        pitem = std::move(Get()); 
        return *this;
    }

    void Put(T&& item, CHANNEL_LOC loc = CHANNEL_LOC_BACK);

    T Get();

    void Kill();

    bool Alive();

    virtual std::atomic_size_t& QueueSize() {
        return MAX_QUEUED;
    }

    std::atomic_size_t& BlockedReaders() {
        return readers;
    }

    std::atomic_size_t& BlockedWriters() {
        return writers;
    }

private:
    //  Uses MUTEX - Debug use only!
    std::string Context(std::string f) {
        static std::mutex contextLock;
        std::unique_lock<std::mutex> lock(contextLock);

        std::stringstream s;
        s << "Channel[" << Id() << "]::" << f;
        return s.str();
    }

    std::atomic<CHANNEL_STATE>   state;
    std::deque<T>                taskQueue;
    std::atomic_size_t           MAX_QUEUED;

    // Those waiting to do something
    std::atomic_size_t           readers;
    std::atomic_size_t           writers;

    // Only one thread may touch the queue at a time
    std::recursive_mutex         queueMutex;

    std::condition_variable_any  queueReadFlag;
    std::condition_variable_any  queueWriteFlag;
};

class ChannelException {
public:
    virtual long Id() = 0;
};

class ChannelDeadException: public ChannelException {
public:
    ChannelDeadException(Channel_Base& chan): id(chan.Id()) {}

    virtual long Id() {
        return id;
    }
private:
    long id;
};

#endif
