/*
 * Publisher updates to multiple consumers
 *
 *  Created on: 22nd December 2015
 *      Author: lhumphreys
 */

#ifndef DEV_TOOLS_CPP_LIBRARIES_LIB_THEAD_COMMS_PIPE_PUBLISHER_H__
#define DEV_TOOLS_CPP_LIBRARIES_LIB_THEAD_COMMS_PIPE_PUBLISHER_H__

#include <PipeSubscriber.h>
#include <mutex>
#include <set>

/**
 * Publishes copies of data to multiple consumers. The copy construction is
 * done on the publicatin thread, as in any locking, whilst the consumer is
 * lockless. Therefore the pipe is suitable for offloading work to a worker
 * thread, for consumption by one or more client threads.
 *
 * The client may either poll the consumer object periodically, or register
 * for an unread message notificaiton. To reduce overhead, the notification
 * will only be triggered once, at which point the client should drain the
 * queue, and re-configure the notification. See PipeSubscriber
 *
 */
template <class Message>
class PipePublisher { 
public:
    PipePublisher();

    virtual ~PipePublisher();

    /**
     * Create a new subscription to the publisher.
     *
     * @param  maxSize  Maximum number of unread messages
     */
    template <class Client = PipeSubscriber<Message>>
    Client* NewClient(size_t maxSize);

    /**
     * Publish a new message to all clients.
     */
    void Publish(const Message& msg);
private:
    friend class PipeSubscriber<Message>;

    void RemoveClient(PipeSubscriber<Message>* client);
    /*********************************
     *           Data
     *********************************/
    std::mutex                         subscriptionMutex;
    std::set<PipeSubscriber<Message>*> clients;
};


#include "PipePublisher.hpp"
#endif 
