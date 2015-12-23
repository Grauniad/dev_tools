/*
 * Subscribe to updates from the Pipe Publisher
 *
 *  Created on: 29 Oct 2015
 *      Author: lhumphreys
 */

#ifndef DEV_TOOLS_CPP_LIBRARIES_LIB_THEAD_COMMS_PIPE_SUBSCRUBER_H__
#define DEV_TOOLS_CPP_LIBRARIES_LIB_THEAD_COMMS_PIPE_SUBSCRUBER_H__

#include <boost/lockfree/spsc_queue.hpp>
#include <atomic>
#include <mutex>
#include <functional>

template <class Message>
class PipePublisher;

/**
 * Consumes data down a single-producer / single-consumer pipe.
 *
 * The pipe is lockless, except for the configuration and dispatch of the
 * onMessage notification. (Chossing not to use trigger results in a lockless
 * pipe.
 */
template <class Message>
class PipeSubscriber { 
public:

    ~PipeSubscriber();

    /** 
     * Pop the next message off the queue and populate
     * msg with the result.
     *
     * If there is no messasge to pop, msg is left unchanged
     *
     * @param msg   The message to populate.
     *
     * @returns true if msg was populated, false otherwise.
     */ 
    bool GetNextMessage(Message& msg);

    /**
     * Trigger a callback function ON **ETIHER** the publisher thread OR the
     * current thread when there is at least one unread message. This can be
     * used to trigger a post to the subscriber thread if desired, e.g using
     * boost::asio::io_service::post.
     *
     * This callback will be triggered exactly once. 
     *
     * NOTE: This callback may be triggered immediately (before returning) on
     *        the current thread if there is already unread data on the queue.
     */
    typedef std::function<void(void)> NextMessasgCallback;
    void OnNextMessasge(NextMessasgCallback&& f);

private:
    friend class PipePublisher<Message>;
    /*********************************
     *   Interface for Publisher
     *********************************/
    /**
     * Create a new consumer.
     *
     * @param maxSize  Maximum unread messages before an exception is
     *                 triggered on the producer.
     */
    PipeSubscriber(PipePublisher<Message>* parent, size_t maxSize);

     struct PushToFullQueueException {
         Message msg;
     };

     /**
      * Push a new message onto the queue. 
      * 
      * If the queue is full an instance of PushToFullQueueException is thrown.
      *
      * If there is an active OnNextMessage callback this will be triggered
      * before the function returns.
      *
      * @param msg  The message to add to the queue.
      */ 
     void PushMessage(const Message& msg);
    /*********************************
     *           Data
     *********************************/
    std::atomic<size_t>  read;
    std::atomic<size_t>  written;

    std::mutex           onNotifyMutex;
    std::atomic<bool>    notifyOnWrite;
    NextMessasgCallback  onNotify;

    boost::lockfree::spsc_queue<Message>  messages;

    PipePublisher<Message>*    parent;
};

#include "PipeSubscriber.hpp"

#endif 
