#include <logger.h>

template <class Message>
PipePublisher<Message>::PipePublisher() 
{
}

template <class Message>
PipePublisher<Message>::~PipePublisher() 
{
    if (clients.size() > 0) {
        LOG_FROM(LOG_ERROR,"PipePublisher<Message>::~PipePublisher",
            "Destruction will leave one or more orphaned clients");
    }
}

template <class Message>
void PipePublisher<Message>::Publish(const Message& msg) {
    /**
     * This lock is not expected to be noticable on a profile, since it is not
     * expected that clients will be created / destroyed reguarly, and we are
     * a single-publisher model.
     *
     * Therefore on almost every call there will be no contention for the
     * mutex, and on the modern Linux systems we are targeting, the futex
     * will lock with a single atomic "test and set" operation, without the
     * need for a system call.
     */
    std::unique_lock<std::mutex> clientLock(subscriptionMutex);

    for (PipeSubscriber<Message>* client : clients) {
        client->PushMessage(msg);
    }
}

template <class Message>
PipeSubscriber<Message>* PipePublisher<Message>::NewClient(size_t maxSize) {
    // Lock out the publisher thread until we have finished with the client
    // list
    std::unique_lock<std::mutex> clientLock(subscriptionMutex);

    PipeSubscriber<Message>* client = 
        new PipeSubscriber<Message>(this,maxSize);

    clients.insert(client);

    return client;

}

template <class Message>
void PipePublisher<Message>::RemoveClient(PipeSubscriber<Message>* client) {
    // Lock out the publisher thread until we have finished with the client
    // list
    std::unique_lock<std::mutex> clientLock(subscriptionMutex);

    clients.erase(client);
}
