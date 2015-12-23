#include "tester.h"
#include <PipePublisher.h>
#include <thread>


using namespace std;

int PublishSingleConsumer(testLogger& log);
int PublishDoubleConsumer(testLogger& log);
int PublishNotify(testLogger& log);


int main(int argc, const char *argv[])
{
    Test("Publish to a single consumer",PublishSingleConsumer).RunTest();
    Test("Publish to two consumers",PublishDoubleConsumer).RunTest();
    Test("On Next Message Callback",PublishNotify).RunTest();
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
    std::unique_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
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
    std::unique_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
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
    
    client->OnNextMessasge(f);

    for (auto& msg : toSend ) {
        publisher.Publish(msg);
    }

    std::vector<Msg> expected = {
        {"Message 1"}
    };

    if (!MessagesMatch(log,expected,got)) {
        return 1;
    }

    client->OnNextMessasge(f);

    if (!MessagesMatch(log,toSend,got)) {
        return 1;
    }

    return 0;
}

int PublishDoubleConsumer(testLogger& log) {
    PipePublisher<Msg> publisher;
    std::unique_ptr<PipeSubscriber<Msg>> client(publisher.NewClient(1024));
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
    std::unique_ptr<PipeSubscriber<Msg>> client2(publisher.NewClient(1024));

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
