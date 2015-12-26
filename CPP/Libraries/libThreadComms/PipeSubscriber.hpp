#include <PipePublisher.h>
#include <IPostable.h>

template <class Message>
PipeSubscriber<Message>::PipeSubscriber(
    PipePublisher<Message>* _parent,
    size_t maxSize)
        : notifyOnWrite(false),
          onNotify(nullptr),
          targetToNotify(nullptr),
          forwardMessage(false),
          onNewMessage(nullptr),
          read(0),
          written(0),
          messages(maxSize),
          parent(_parent)
{
}

template <class Message>
PipeSubscriber<Message>::~PipeSubscriber() {
    parent->RemoveClient(this);
}

template <class Message>
void PipeSubscriber<Message>::PushMessage(const Message& msg) {
    if ( forwardMessage ) {
        std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
        // No need to re-check post-lock since it is not possible to
        // unset the onNewMessage callback
        ++written;
        ++read;
        onNewMessage(msg);
    } else {
        if ( !messages.push(msg) ) {
            throw PushToFullQueueException {msg};
        }
        ++written;

        if (notifyOnWrite) {
            std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
            // No need to re-check post-lock since it is not possible to
            // unset the notify callback, and we are the only thread
            // allowed to publish.
            notifyOnWrite = false;
            if (targetToNotify) {
                targetToNotify->PostTask(onNotify);
            } else {
                onNotify();
            }
            onNotify = nullptr;
            targetToNotify = nullptr;
        }
    }
}

template <class Message>
bool PipeSubscriber<Message>::GetNextMessage(Message& msg) {
    bool gotMsg = false;
    if ( messages.pop(msg) ) {
        gotMsg = true;
        ++read;
    }

    return gotMsg;
}

template <class Message>
void PipeSubscriber<Message>::OnNextMessage(const NextMessageCallback& f) {
    std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
    if ( read < written ) {
        f();
        notifyOnWrite = false;
        onNotify = nullptr;
        targetToNotify = nullptr;
    } else {
        onNotify = f;
        notifyOnWrite = true;
        targetToNotify = nullptr;
    }
}

template <class Message>
void PipeSubscriber<Message>::OnNextMessage(
         const NextMessageCallback& f,
         IPostable* target)
{
    std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
    if ( read < written ) {
        target->PostTask(f);
        notifyOnWrite = false;
        onNotify = nullptr;
        targetToNotify = nullptr;
    } else {
        onNotify = f;
        notifyOnWrite = true;
        targetToNotify = target;
    }
}

template <class Message>
void PipeSubscriber<Message>::OnNewMessage(const NewMessasgCallback& f) {
    std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
    forwardMessage = true;
    onNewMessage = f;
    Message m;
    while (messages.pop(m)) {
        f(m);
    }
}
