#include <PipePublisher.h>

template <class Message>
PipeSubscriber<Message>::PipeSubscriber(
    PipePublisher<Message>* _parent,
    size_t maxSize)
        : read(0),
          written(0),
          notifyOnWrite(false),
          onNotify(nullptr),
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
    if ( !messages.push(msg) ) {
        throw PushToFullQueueException {msg};
    }
    ++written;

    if (notifyOnWrite) {
        std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
        notifyOnWrite = false;
        onNotify();
        onNotify = nullptr;
    }
}

template <class Message>
bool PipeSubscriber<Message>::GetNextMessage(Message& msg) {
    bool read = false;
    if ( messages.pop(msg) ) {
        read = true;
        ++read;
    }

    return read;
}

template <class Message>
void PipeSubscriber<Message>::OnNextMessasge(NextMessasgCallback&& f) {
    std::unique_lock<std::mutex> notifyLock(onNotifyMutex);
    if ( read < written ) {
        f();
        notifyOnWrite = false;
        onNotify = nullptr;
    } else {
        onNotify = f;
        notifyOnWrite = true;
    }
}
