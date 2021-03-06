#include <WorkerThread.h>
#include "tester.h"
#include <atomic>
#include <mutex>
#include <util_time.h>

#include <iostream>

int PostSingleTask(testLogger& log);
int ChainTasks(testLogger& log);
int ExternalAbort(testLogger& log);
int InternalAbort(testLogger& log);
int WaitForTask(testLogger& log);
int SingleClient(testLogger& log);
int TwoClients(testLogger& log); int SliceSize(testLogger& log);

int main(int argc, const char *argv[])
{
    Test("Posting a single task...",PostSingleTask).RunTest();
    Test("Posting tasks from within a task",ChainTasks).RunTest();
    Test("Aborting from the main thread",ExternalAbort).RunTest();
    Test("Aborting from within a task",InternalAbort).RunTest();
    Test("Posting a task synchronously",WaitForTask).RunTest();
    Test("Consume updates from a single client",SingleClient).RunTest();
    Test("Consume updates from two clients",TwoClients).RunTest();
    Test("Slice Size is respected",SliceSize).RunTest();
    return 0;
}

int PostSingleTask(testLogger& log) {
    WorkerThread worker;
    worker.Start();

    std::thread::id this_thread = std::this_thread::get_id();
    std::thread::id write_id;
    std::mutex target_mutex;
    std::atomic<size_t> target(0);

    auto f = [&] () -> void {
        std::unique_lock<std::mutex> lock(target_mutex);
        target = 1;
        write_id = std::this_thread::get_id();
    };

    std::unique_lock<std::mutex> lock(target_mutex);
    worker.PostTask(f);

    while (target == 0){
        lock.unlock();
        std::this_thread::yield();
        lock.lock();
    }

    if (target != 1) {
        log << "Failed to set target!" << endl;
        return 1;
    }

    if (write_id == this_thread) {
        log << "target written from the current thread!" << endl;
        return 1;
    }

    return 0;
}

int ChainTasks(testLogger& log) {
    std::thread::id this_thread = std::this_thread::get_id();
    WorkerThread worker;
    worker.Start();

    std::mutex target_mutex;
    std::vector<size_t> targets = { 0, 0 ,0};

    std::vector<std::thread::id> write_ids;
    write_ids.resize(3);

    std::vector<Time> write_times;
    write_times.resize(3);

    auto f1 = [&] () -> void {
        std::unique_lock<std::mutex> lock(target_mutex);
        targets[1] = 1;
        write_times[1].SetNow();
        write_ids[1] = std::this_thread::get_id();
    };

    auto f2 = [&] () -> void {
        std::unique_lock<std::mutex> lock(target_mutex);
        targets[2] = 2;
        write_times[2].SetNow();
        write_ids[2] = std::this_thread::get_id();
    };

    auto f0 = [&] () -> void {
        std::unique_lock<std::mutex> lock(target_mutex);
        worker.PostTask(f1);
        worker.PostTask(f2);
        targets[0] = 0;
        write_times[0].SetNow();
        write_ids[0] = std::this_thread::get_id();
    };

    std::unique_lock<std::mutex> lock(target_mutex);
    worker.PostTask(f0);

    while (targets[2] == 0){
        lock.unlock();
        std::this_thread::yield();
        lock.lock();
    }

    for (size_t i =0; i < 3; ++i) {
        size_t& target = targets[i];
        if (target != i) {
            log << "Failed to set target: " <<  i << endl;
            return 1;
        }
        std::thread::id& write_id = write_ids[i];

        if (write_id == this_thread) {
            log << "target written from the current thread!" << endl;
            return 1;
        }
    }

    if ( write_times[1].DiffUSecs(write_times[0]) < 0 ) {
        log << "F0: " << write_times[0].Timestamp() << endl;
        log << "F1: " << write_times[1].Timestamp() << endl;
        return 1;
    }

    if ( write_times[2].DiffUSecs(write_times[1]) < 0 ) {
        log << "F1: " << write_times[1].Timestamp() << endl;
        log << "F2: " << write_times[2].Timestamp() << endl;
        return 1;
    }

    return 0;
}

int ExternalAbort(testLogger& log) {
    WorkerThread worker;
    size_t target1 = 0;
    size_t target2 = 0;
    std::mutex write_mutex;
    std::condition_variable permission_to_continue;
    std::condition_variable write_complete;

    auto f1 = [&] () -> void {
        std::unique_lock<std::mutex> lock(write_mutex);
        target1 = 1;
        write_complete.notify_all();
        permission_to_continue.wait(lock);
    };

    auto f2 = [&] () -> void {
        log << "Executing F2" << endl;
        std::unique_lock<std::mutex> lock(write_mutex);
        target2 = 1;
    };


    std::unique_lock<std::mutex> lock(write_mutex);

    worker.PostTask(f1);
    worker.PostTask(f2);

    worker.Start();

    write_complete.wait(lock);
    worker.Abort();
    permission_to_continue.notify_all();
    lock.unlock();
    worker.Join();

    if ( target1 != 1 ) {
        log << "Failed to write target!" << endl;
        return 1;
    }

    if ( target2 != 0 ) {
        log << "Additional write to target2!" << endl;
        return 1;
    }
    return 0;
}

int InternalAbort (testLogger& log ) {
    WorkerThread worker;
    size_t target1 = 0;
    size_t target2 = 0;
    std::mutex write_mutex;

    auto f1 = [&] () -> void {
        std::unique_lock<std::mutex> lock(write_mutex);
        target1 = 1;
        worker.Abort();
    };

    auto f2 = [&] () -> void {
        std::unique_lock<std::mutex> lock(write_mutex);
        target2 = 1;
    };


    std::unique_lock<std::mutex> lock(write_mutex);

    worker.PostTask(f1);
    worker.PostTask(f2);

    worker.Start();
    lock.unlock();
    worker.Join();

    if ( target1 != 1 ) {
        log << "Failed to write target!" << endl;
        return 1;
    }

    if ( target2 != 0 ) {
        log << "Additional write to target2!" << endl;
        return 1;
    }
    return 0;
    return 1;
}

int WaitForTask(testLogger& log) {
    WorkerThread worker;
    worker.Start();

    std::thread::id this_thread = std::this_thread::get_id();
    std::thread::id write_id;
    std::mutex target_mutex;
    std::atomic<size_t> target(0);

    auto f = [&] () -> void {
        std::unique_lock<std::mutex> lock(target_mutex);
        target = 1;
        write_id = std::this_thread::get_id();
    };

    if (!worker.DoTask(f)) {
        log << "Do task failed!" << endl;
        return 1;
    }

    std::unique_lock<std::mutex> lock(target_mutex);

    if (target != 1) {
        log << "Failed to set target!" << endl;
        return 1;
    }

    if (write_id == this_thread) {
        log << "target written from the current thread!" << endl;
        return 1;
    }

    return 0;
}

struct Msg {
    Time t;
    std::string message;
};

bool MessagesMatch(testLogger& log,
                   const std::vector<Msg>& sent,
                   const std::vector<Msg>& got)
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
            const Msg& msg = sent[i];
            const Msg& recvd = got[i];

            if ( msg.message != recvd.message ) {
                log << "Missmatch on message: " << i;
                log.ReportStringDiff(msg.message,recvd.message);
                match = false;
            }

            if (msg.t.DiffUSecs(recvd.t) != 0 ) {
                log << "Failed to match time stamps on message : " << i;
                log.ReportStringDiff(msg.t.Timestamp(),recvd.t.Timestamp());
                match = false;
            }
        }
    }

    return match;
}

int SingleClient(testLogger& log ) {
    PipePublisher<Msg> publisher;
    WorkerThread worker;
    std::vector<Msg> messages;
    std::mutex  comms_mutex;
    std::condition_variable wait_for_complete;

    std::vector<Msg> toSend {
        {Time(), "Hello"},
        {Time(), "World!"}
    };

    std::function<void (Msg&)> push = 
        [&comms_mutex, &wait_for_complete, &toSend, &messages] (Msg& m) -> void {
            messages.push_back(m);
            if ( messages.size() == toSend.size()) {
                std::unique_lock<std::mutex> lock(comms_mutex);
                wait_for_complete.notify_all();
            }
        };

    worker.Start();

    worker.ConsumeUpdates(publisher,push,1000,1000);

    std::unique_lock<std::mutex> lock(comms_mutex);

    for (Msg& m: toSend) {
        publisher.Publish(m);
    }

    wait_for_complete.wait(lock);

    if (!MessagesMatch(log,toSend,messages)) {
        return 1;
    }

    return 0;
}

int TwoClients(testLogger& log ) {
    PipePublisher<Msg> publisher;
    WorkerThread worker;
    std::vector<Msg> messages;
    std::vector<Msg> messages2;
    std::mutex  comms_mutex;
    std::mutex  comms_mutex2;
    std::condition_variable wait_for_complete;
    std::atomic<bool> done1(false);
    std::atomic<bool> done2(false);

    std::vector<Msg> toSend1 {
        {Time(), "Hello"},
        {Time(), "World!"}
    };

    std::vector<Msg> toSend2 {
        {Time(), "Another String"},
        {Time(), "And Another"}
    };

    std::function<void (Msg&)> push = [&] (Msg& m) -> void {
			std::unique_lock<std::mutex> lock(comms_mutex);
            messages.push_back(m);
            if ( messages.size() == 2) {
                std::unique_lock<std::mutex> lock2(comms_mutex2);
                done1 = true;
                wait_for_complete.notify_all();
            }
        };

    std::function<void (Msg&)> push2 = [&] (Msg& m) -> void {
			std::unique_lock<std::mutex> lock(comms_mutex);
            messages2.push_back(m);
            if ( messages2.size() == 2) {
                std::unique_lock<std::mutex> lock2(comms_mutex2);
                done2 = true;
                wait_for_complete.notify_all();
            }
        };

    worker.Start();

    worker.ConsumeUpdates(publisher,push,1000,1000);

    std::unique_lock<std::mutex> lock(comms_mutex);

    for (Msg& m: toSend1) {
        publisher.Publish(m);
    }

    wait_for_complete.wait(lock);

    if (!MessagesMatch(log,toSend1,messages)) {
        return 1;
    }

    if (!MessagesMatch(log,{},messages2)) {
        return 1;
    }

    messages.clear();

    done1 = false;
    done2 = false;

    worker.ConsumeUpdates(publisher,push2,1000,1000);


    for (Msg& m: toSend2) {
        publisher.Publish(m);
    }

    wait_for_complete.wait(lock);
    if (!done1 || !done2) {
        wait_for_complete.wait(lock);
    }

    if (!MessagesMatch(log,toSend2,messages)) {
        return 1;
    }

    if (!MessagesMatch(log,toSend2,messages2)) {
        return 1;
    }

    return 0;
}

int SliceSize(testLogger& log ) {
    PipePublisher<Msg> publisher;
    WorkerThread worker;
    std::vector<Msg> messages;
    std::mutex  comms_mutex;
    std::condition_variable wait_for_complete;

    std::vector<Msg> toSend {
        {Time(), ""},
        {Time(), ""},
        {Time(), ""},
        {Time(), ""},
        {Time(), ""}
    };
    const size_t toGet = toSend.size();

    std::vector<Msg> expected;
    for (Msg& m: toSend) {
        expected.push_back({m.t,"ONE"});
        expected.push_back({m.t,"TWO"});
    }

    size_t count1 = 0;
    size_t count2 = 0;
    auto f = [&comms_mutex, &messages]
             (size_t& count, Msg& m, std::condition_variable& doneFlag, size_t toGet, std::string name) -> void {
                std::unique_lock<std::mutex> lock(comms_mutex);
                messages.push_back({m.t,name});
                ++count;
                if (count == toGet) {
                    doneFlag.notify_all();
                }
            };

    std::function<void(Msg&)> task1 = [&f, &comms_mutex, &messages, &count1, &wait_for_complete, &toGet]
                 (Msg& m) -> void {
                     f(count1, m,wait_for_complete,toGet,"ONE");
                 };

    std::function<void(Msg&)> task2 = [&f, &comms_mutex, &messages, &count2, &wait_for_complete, &toGet]
                 (Msg& m) -> void {
                     f(count2, m,wait_for_complete,toGet,"TWO");
                 };

    worker.Start();
    std::unique_lock<std::mutex> lock(comms_mutex);

    worker.ConsumeUpdates(publisher,task1,100,1);
    worker.ConsumeUpdates(publisher,task2,100,1);

    for (Msg& m: toSend) {
        publisher.Publish(m);
    }

    while (count1 != toGet || count2 != toGet) {
        wait_for_complete.wait(lock);
    }

    if (!MessagesMatch(log,expected,messages)) {
        return 1;
    }

    return 0;
}
