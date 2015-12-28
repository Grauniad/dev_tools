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
class IPostable;

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

    virtual ~PipeSubscriber();

    /** 
     * Pop the next message off the queue and populate
     * msg with the result.
     *
     * If there is no message to pop, msg is left unchanged
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
     * NOITE: No unread message notifications will be triggered whilst message
     *        forwarding has been enabled.
     *
     * NOTE: This callback may be triggered immediately (before returning) on
     *        the current thread if there is already unread data on the queue.
     */
    typedef std::function<void(void)> NextMessageCallback;
    void OnNextMessage(const NextMessageCallback& f);

    /**
     * Variant of the OnNextMessage callback which posts the task to another 
     * event loop.
     *
     *  @param  f          The callback to trigger
     *  @param  target     The object to post the task to.
     */
     void OnNextMessage(const NextMessageCallback& f, IPostable* target);

    /**
     * Trigger a callback function for each new message received by the
     * subsciber. The function will be called from the **PUBLISHER THREAD**.
     *
     * If there are any unread message currently in the queue, these will be 
     * triggered on the **CURRENT THREAD** before this function returns. 
     *
     * NOITE: Setting up a message forward via OnNewMessage implicitly
     *        suprresses the OnNextMessage callback since there will never be
     *        any unread data.
     *
     * NOTE: To preserve ordering this function will lock out the publisher thread
     *       until call-backss for all existing messages haeve been completed.
     */
    typedef std::function<void(const Message&)> NewMessasgCallback;
    void OnNewMessage(const NewMessasgCallback&  f);

protected:
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
     virtual void PushMessage(const Message& msg);
private:

    /***********************************
     * Unread data Notification
     ***********************************/
    std::atomic<bool>    notifyOnWrite;
    NextMessageCallback  onNotify;
    IPostable*           targetToNotify;

    /***********************************
     * Forward Messages
     ***********************************/
    std::atomic<bool>    forwardMessage;
    NewMessasgCallback   onNewMessage;

    /*********************************
     *           Data
     *********************************/
    std::atomic<size_t>  read;
    std::atomic<size_t>  written;

    std::mutex           onNotifyMutex;

    boost::lockfree::spsc_queue<Message>  messages;

    PipePublisher<Message>*    parent;
};

#include "PipeSubscriber.hpp"

#endif 
