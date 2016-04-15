#include "tester.h"
#include <PipePublisher.h>
#include <thread>
#include <IPostable.h>


using namespace std;

int PublishSingleConsumer(testLogger& log);
int PublishDoubleConsumer(testLogger& log);
int PublishNotify(testLogger& log);
int BatchNotify(testLogger& log);
int PublishPostNotify(testLogger& log);
int PublishForEachData(testLogger& log);
int BatchForEachData(testLogger& log);
int CustomHandler(testLogger& log);
int CustomBatchHandler(testLogger& log);
int CustomBatchReset(testLogger& log);
int CustomBatchAbort(testLogger& log);
int CustomBatchDone(testLogger& log);
int InstallCustomHandler(testLogger& log);
int PublishForEachDataUnread(testLogger& log);
int Abort(testLogger& log);
int AbortNotify(testLogger& log);
int AbortForEachData(testLogger& log);
int AbortForEachDataUnread(testLogger& log);
int AbortHandleDestruction(testLogger& log);
//int AbortHandleDestoructionUnreadData(testLogger& log);


int main(int argc, const char *argv[])
{
    Test("Publish to a single consumer",PublishSingleConsumer).RunTest();
    Test("Publish to two consumers",PublishDoubleConsumer).RunTest();
    Test("On Next Message Callback",PublishNotify).RunTest();
    Test("On Next Message Callback, batched",BatchNotify).RunTest();
    Test("On Next Message Callback, post variant",PublishPostNotify).RunTest();
    Test("On data callback",PublishForEachData).RunTest();
    Test("On data callback, batches",BatchForEachData).RunTest();
    Test("Custom on data callback",CustomHandler).RunTest();
    Test("Custom batch callbacks",CustomBatchHandler).RunTest();
    Test("Custom batch reset",CustomBatchReset).RunTest();
    Test("Install custom handler for on data callback",InstallCustomHandler).RunTest();
    Test("On data callback, unread data",PublishForEachDataUnread).RunTest();
    Test("On data callback, unread data",PublishForEachDataUnread).RunTest();
    Test("Abort prevents further publication",Abort).RunTest();
    Test("Abort from On Next Message callback",AbortNotify).RunTest();
    Test("Abort from on data callback",AbortForEachData).RunTest();
    Test("Abort from unread data, on data callback",AbortForEachDataUnread).RunTest();
    Test("Destruction of final handle results in abort",AbortHandleDestruction).RunTest();
    Test("Custom batch abort",CustomBatchAbort).RunTest();
    Test("Custom batch done",CustomBatchDone).RunTest();

    return 0;
}

struct Msg {
    std::string   message;
};

class Client {
    template <class F>
    Client (PipeSubscriber<Msg>* client, F task) 
       : subscription(client), worker(task)
    {
    }

    ~Client() {
        worker.join();
        delete subscription;
    }
public:
    PipeSubscriber<Msg>*  subscription;
    std::thread           worker;
};

bool MessagesMatch(testLogger& log,
                   std::vector<Msg>& sent,
                   std::vector<Msg>& got)
{
    bool match = true;
    if (sent.size() != got.size()) {
        log << "Invalid number of messages received: " << endl;
        log << "Expected: " << sent.size() << endl;
        log << "Got: " << got.size() << endl;
        match = false;
    }

    if ( match ) {
        for (size_t i = 0; match && i < sent.size(); ++i) {
            Msg& msg = sent[i];
            Msg& recvd = got[i];

            if ( msg.message != recvd.message ) {
                log << "Missmatch on message: " << i;
                log.ReportStringDiff(msg.message,recvd.message);
                match = false;
            }
        }
    }

    return match;
}

int PublishSingleConsumer(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    std::thread worker(f);
    worker.join();

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int PublishNotify(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    
    client->OnNextMessage(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    client->OnNextMessage(f);

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int BatchNotify(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };

    client->OnNextMessage(f);

    publisher.StartBatch();

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> expected = { };

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    publisher.EndBatch();

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int PublishPostNotify(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };
    
    struct PostableTarget: public IPostable {
        PostableTarget() : posted_count(0) { } 
        void PostTask(const Task& task) {
            ++posted_count;
            task();
        }

        size_t posted_count;
    } dummy_event_loop;

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    
    client->OnNextMessage(f,&dummy_event_loop);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    if ( dummy_event_loop.posted_count != 1) {
        log << "Task was not posted!" << endl;
        return 1;
    }

    client->OnNextMessage(f);

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    if ( dummy_event_loop.posted_count != 1) {
        log << "Task was posted again!" << endl;
        return 1;
    }


    got.clear();
    publisher.Publish(expected[0]);

    client->OnNextMessage(f,&dummy_event_loop);

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    if ( dummy_event_loop.posted_count != 2) {
        log << "Task was not posted!" << endl;
        return 1;
    }

    return 0;
}

int PublishDoubleConsumer(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    std::thread worker(f);
    worker.join();

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    std::vector<Msg> toSend2 = {
        {"2nd Message 1"},
        {"2nd Mesasge 2"},
        {"2nd Hello World!"}
    };
    std::shared_ptr<PipeSubscriber<Msg>> client2(publisher.NewClient(1024));

    for (auto& msg : toSend2 ) {
        publisher.Publish(msg);
    }

    got.clear();
    auto f2 = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    std::vector<Msg> got2;
    auto f3 = [&] () -> void {
        Msg recvMsg;
        while(client2->GetNextMessage(recvMsg)) {
            got2.push_back(recvMsg);
        }
    };
    std::thread worker2(f2);
    std::thread worker3(f3);
    worker3.join();
    worker2.join();

    if (!MessagesMatch(log,toSend2,got)) {
        return 1;
    }

    if (!MessagesMatch(log,toSend2,got2)) {
        return 1;
    }


    return 0;
}

int PublishForEachData(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
    };
    
    client->OnNewMessage(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int BatchForEachData(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
    };

    client->OnNewMessage(f);
    publisher.StartBatch();

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int CustomHandler(testLogger& log) {
    PipePublisher<Msg> publisher;
    class Handler: public IPipeConsumer<Msg> {
    public:
        Handler(PipePublisher<Msg>* parent) { }

        virtual void PushMessage(const Msg& m) {
            got.push_back(m);
        }
        std::vector<Msg> got;
    };
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::shared_ptr<Handler> client(publisher.NewClient<Handler>());

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,toSend,client->got)) {
        return 1;
    }

    return 0;
}

int CustomBatchHandler(testLogger& log) {
    PipePublisher<Msg> publisher;
    class Handler: public IPipeConsumer<Msg> {
    public:
        Handler(PipePublisher<Msg>* parent) {
            startBatch = 0;
            endBatch = 0;
        }

        virtual void PushMessage(const Msg& m) {
            got.push_back(m);
        }

        virtual void StartBatch() {
            startBatch++;
        }

        virtual void EndBatch() {
            endBatch++;
        }
        std::vector<Msg> got;
        size_t           startBatch;
        size_t           endBatch;
    };
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::shared_ptr<Handler> client(publisher.NewClient<Handler>());

    publisher.StartBatch();

    if (client->startBatch != 1) {
        log << "Start Batch (1): " << client->startBatch;
        return 1;
    }

    if (client->endBatch != 0) {
        log << "End Batch (0): " << client->endBatch;
        return 1;
    }

    for (auto& msg : toSend ) {
        publisher.Publish(msg);

        if (client->startBatch != 1) {
            log << "Start Batch (1): " << client->startBatch;
            return 1;
        }

        if (client->endBatch != 0) {
            log << "End Batch (0): " << client->endBatch;
            return 1;
        }
    }

    publisher.EndBatch();

        if (client->startBatch != 1) {
            log << "Start Batch (1): " << client->startBatch;
            return 1;
        }

        if (client->endBatch != 1) {
            log << "End Batch (1): " << client->endBatch;
            return 1;
        }

    if (!MessagesMatch(log,toSend,client->got)) {
        return 1;
    }

    return 0;
}

int CustomBatchReset(testLogger& log) {
    std::unique_ptr<PipePublisher<Msg>> publisher(new PipePublisher<Msg>);
    class Handler: public IPipeConsumer<Msg> {
    public:
        Handler(PipePublisher<Msg>* parent) {
            startBatch = 0;
            endBatch = 0;
        }

        virtual void PushMessage(const Msg& m) { }

        virtual void StartBatch() {
            startBatch++;
        }

        virtual void EndBatch() {
            endBatch++;
        }
        size_t           startBatch;
        size_t           endBatch;
    };

    std::shared_ptr<Handler> client(publisher->NewClient<Handler>());

    publisher->StartBatch();

    if (client->startBatch != 1) {
        log << "Start Batch (1): " << client->startBatch;
        return 1;
    }

    if (client->endBatch != 0) {
        log << "End Batch (0): " << client->endBatch;
        return 1;
    }

    publisher->StartBatch();

    if (client->startBatch != 2) {
        log << "Start Batch (2): " << client->startBatch;
        return 1;
    }

    if (client->endBatch != 1) {
        log << "End Batch (1): " << client->endBatch;
        return 1;
    }

    publisher.reset(nullptr);

    if (client->startBatch != 2) {
        log << "Start Batch (2): " << client->startBatch;
        return 1;
    }

    if (client->endBatch != 2) {
        log << "End Batch (2): " << client->endBatch;
        return 1;
    }

    return 0;
}

int InstallCustomHandler(testLogger& log) {
    PipePublisher<Msg> publisher;
    class Handler: public IPipeConsumer<Msg> {
    public:
        virtual void PushMessage(const Msg& m) {
            got.push_back(m);
        }
        std::vector<Msg> got;
    };
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::shared_ptr<Handler> client(new Handler);

    publisher.InstallClient(client);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,toSend,client->got)) {
        return 1;
    }

    return 0;
}

int PublishForEachDataUnread(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"}
    };

    std::vector<Msg> toSend2 = {
        {"Hello World!"}
    };

    std::vector<Msg> end_result = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
    };
    
    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    client->OnNewMessage(f);

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    for (auto& msg : toSend2 ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,end_result,got)) {
        return 1;
    }

    return 0;
}

int Abort(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    client->Abort();

    Msg finalMessage = {"This should not be published"};
    publisher.Publish(finalMessage);

    std::vector<Msg> got;
    auto f = [&] () -> void {
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
    };
    std::thread worker(f);
    worker.join();

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int CustomBatchAbort(testLogger& log) {
    PipePublisher<Msg> publisher;

    class Handler: public IPipeConsumer<Msg> {
    public:
        virtual void PushMessage(const Msg& m) {
            got.push_back(m);
        }
        std::vector<Msg> got;
    };

    std::shared_ptr<Handler> client(new Handler);

    publisher.InstallClient(client);

    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    publisher.StartBatch();
    client->Abort();

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    publisher.EndBatch();

    Msg finalMessage = {"This should not be published"};
    publisher.Publish(finalMessage);

    if (!MessagesMatch(log,toSend,client->got)) {
        return 1;
    }

    return 0;
}

int CustomBatchDone(testLogger& log) {
    PipePublisher<Msg> publisher;

    class Handler: public IPipeConsumer<Msg> {
    public:
        Handler() : endBatchCalls(0) { }
        virtual void PushMessage(const Msg& m) {
            got.push_back(m);
        }
        std::vector<Msg> got;

        // Track calls to the OnStateChange callback
        void OnStateChange() {
            stateChanges.push_back(State());
        }

        // Track calls to endBatchCalls
        void EndBatch() {
            endBatchCalls++;
        }

        bool ValidateStateChanges(testLogger& log) {
            bool ok = true;
            if (stateChanges.size() != 1)
            {
                log << "Invalid # of state changes: " << stateChanges.size() << std::endl;
                ok = false;
            }

            if (stateChanges[0] != Handler::FINSIHED)
            {
                log << "Invalid state!" << std::endl;
                ok = false;
            }
            return ok;
        }

        std::vector<STATE> stateChanges;
        size_t             endBatchCalls;
    };

    std::shared_ptr<Handler> client(new Handler);

    publisher.InstallClient(client);

    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    publisher.StartBatch();

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    publisher.Done();

    if (!MessagesMatch(log,toSend,client->got)) {
        return 1;
    }

    if (client->endBatchCalls != 1)
    {
        log << "End batch was not called!" << std::endl;
        return 1;
    }

    if (!client->ValidateStateChanges(log))
    {
        return 1;
    }


    return 0;
}

int AbortNotify(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> got;
    size_t callCount = 0;
    auto f = [&] () -> void {
        ++callCount;
        Msg recvMsg;
        while(client->GetNextMessage(recvMsg)) {
            got.push_back(recvMsg);
        }
        client->Abort();
    };
    
    client->OnNextMessage(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    client->OnNextMessage(f);

    f();
    // No more messages should have been published...
    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    if (callCount != 2 ) {
        log << "Extra NextMessage callback: " << callCount << endl;
        return 1;
    }

    return 0;
}

int AbortForEachData(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
        client->Abort();
    };
    
    client->OnNewMessage(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    return 0;
}

int AbortForEachDataUnread(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"}
    };

    std::vector<Msg> toSend2 = {
        {"Hello World!"}
    };

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
        client->Abort();
    };
    
    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    client->OnNewMessage(f);

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    for (auto& msg : toSend2 ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    return 0;
}

int AbortHandleDestruction(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::shared_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
    std::vector<Msg> toSend = {
        {"Message 1"},
        {"Mesasge 2"},
        {"Hello World!"}
    };

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    std::vector<Msg> got;
    auto f = [&] (const Msg& newMsg) -> void {
        got.push_back(newMsg);
        client.reset();
    };
    
    client->OnNewMessage(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    return 0;
}
