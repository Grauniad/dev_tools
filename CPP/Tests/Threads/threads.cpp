#include "tester.h"
#include "go.h"
#include "thread_utils.h"
#include "taskList.h"

int IterateResults(testLogger& log );
int QueueTasks(testLogger& log );
int RunImmediately(testLogger& log);
int RunOnDemand(testLogger& log);
int FireAndForget(testLogger& log);

int main(int argc, const char *argv[])
{
    Test("Basic Task Healtch Check",FireAndForget).RunTest();
    //Test("Doing a dumb LCM algo in parallel...",IterateResults).RunTest();
    //Test("Testing Run Immediately mechanism",RunImmediately).RunTest();
    //Test("Testing on-demand mechanism",RunOnDemand).RunTest();
    //Test("Testing Queue mechanism",QueueTasks).RunTest();
    return 0;
}

int FireAndForget( testLogger& log ) {
    // Trivial task
    rTask<long> t = GO (long, 
        results << 42;
    );

    log << "Waiting for a while, to make sure no one tries to kill the thread...";
    Thread::Sleep(1000);

    long l;
    t >> l;

    if ( l != 42 ) {
        log << "Execpted l to be 42, but got " << l << endl;
        return 1;
    }
    return 0;
}

rTask<long> GenerateMultiples(long mul) {
    rTask<long> t = GO (long, 
        // Go to sleep once we have 20 elements queued
        results.QueueSize() = 20;

        // Generate elements
        for (long i =1; true; i++) {
            results << i*mul;
        }
    );
    return t;
}

int QueueTasks (testLogger& log ) {
    /*
     * Queue 50 Tasks up, they should fire out (roughly) in order:
     */
    Channel<bool> go_flags(0);
    Channel<int> work_done(0);

    Channel<bool>* readToGo = &go_flags;
    Channel<int>*  writeToFinish = &work_done;

    bool got[50];

    // Auto frees resources
    Local_Tasks<bool> taskList;

    this_thread::yield();
    Thread::Sleep(1000);

    size_t NUM_WORKERS = Task_Master::Scheduler().FreeWorkers();

    // Queue everything up
    for ( int i=0; i<50; ++i) {
        got [i] = false;

        taskList.Add( QUEUE(bool,
            bool go =   readToGo->Get();
            writeToFinish->Put(i+0);

            // Otherwise we get an used variable warning
            results << std::move(go);
        ));
    }

    for ( size_t i =0; i< 2*NUM_WORKERS; ++i ) {
        this_thread::yield();
        Thread::Sleep(100);
    }

    // Now the go_flags should have NUM_WORKERS waiting on it
    if ( go_flags.BlockedReaders() != NUM_WORKERS ) {
        log << "The scheduler has not started the correct number of jobs!";
        log << "NUM_WORKERS: " << NUM_WORKERS;
        log << "Jobs: " << go_flags.BlockedReaders();
        return 1;
    }

    // FIRE!
    for ( int i=0; i<50; ++i) {
        go_flags << true;
    }

    // Collect the results
    for ( int i=0; i<50; i++ ) {
        int j;
        work_done >> j;
        log << "Got: " << j << " :(";
        got[j] = true;
    }

    // Did we get everything?
    for ( int i =0 ; i< 50; i++ ) {
        if ( !got[i] ) {
            log << "Failed to get job slot: " << i << " :(";
            return 1;
        }
    }
    return 0;
}

int RunOnDemand (testLogger& log ) {
    LOG( LOG_OVERVIEW, "Starting On Demand")
    /*
     * Queue 50 Tasks up, they should fire out (roughly) in order:
     */
    Channel<bool> go_flags(0);
    Channel<int> work_done(0);

    Channel<bool>* readToGo = &go_flags;
    Channel<int>*  writeToFinish = &work_done;

    bool got[50];

    // Auto frees resources
    Local_Tasks<bool> taskList;

    this_thread::yield();
    Thread::Sleep(1000);

    size_t NUM_WORKERS = Task_Master::Scheduler().FreeWorkers();

    // Queue everything up
    for ( int i=0; i<50; ++i) {
        got [i] = false;

        taskList.Add( GO(bool,
            results.QueueSize() = 1;

            bool go =   readToGo->Get();
            writeToFinish->Put(i+0);

            while ( true ) {
                // Blocks the thread 
                results <<  (go ? 1 : 0);
            }

        ));
    }
    LOG( LOG_OVERVIEW, "Queued everything up")

    for ( size_t i =0; i< 2*NUM_WORKERS; ++i ) {
        this_thread::yield();
        Thread::Sleep(100);
    }

    // Now the go_flags should have NUM_WORKERS waiting on it
    if ( go_flags.BlockedReaders() != NUM_WORKERS ) {
        log << "The scheduler has not started the correct number of jobs!";
        log << "NUM_WORKERS: " << NUM_WORKERS;
        log << "Jobs: " << go_flags.BlockedReaders();
        return 1;
    }

    if ( Task_Master::Scheduler().FreeWorkers() != 0 ) {
        log << "The scheduler has failed to use all of its resources!";
        log << "Free Workers: " << Task_Master::Scheduler().FreeWorkers();
        log << "Jobs: " << go_flags.BlockedReaders();
        return 1;
    }

    LOG( LOG_OVERVIEW, "Queued everything up")

    // FIRE!
    for ( int i=0; i<50; ++i) {
        go_flags << true;
    }

    LOG( LOG_OVERVIEW, "Sent the triggers")

    /*
     * Force the scheduler to start the queued threads
     */
    for  ( rTask<bool>& t : taskList) {
        bool forceStart;
        t.Results() >> forceStart;
    }

    LOG( LOG_OVERVIEW, "Tried to get info from each task")

    // Collect the results
    for ( int i=0; i<50; i++ ) {
        SLOG( LOG_OVERVIEW, "Collecting data from " << i)
        int j;
        work_done >> j;
        log << "Got: " << j << " :(";
        got[j] = true;
    }

    // Did we get everything?
    for ( int i =0 ; i< 50; i++ ) {
        if ( !got[i] ) {
            log << "Failed to get job slot: " << i << " :(";
            return 1;
        }
    }
    LOG( LOG_OVERVIEW, "Finishing On Demand")

    return 0;
}

int RunImmediately (testLogger& log ) {
    
    LOG( LOG_OVERVIEW, "Starting Immediate Run!")
    /*
     * Queue 50 Tasks up, they should fire out (roughly) in order:
     */
    Channel<bool> go_flags(0);
    Channel<int> work_done(0);

    Channel<bool>* readToGo = &go_flags;
    Channel<int>*  writeToFinish = &work_done;

    bool got[50];

    // Auto frees resources
    Local_Tasks<bool> taskList;

    this_thread::yield();
    Thread::Sleep(1000);

    size_t NUM_WORKERS = Task_Master::Scheduler().FreeWorkers();

    // Queue everything up
    for ( int i=0; i<50; ++i) {
        got [i] = false;

        taskList.Add( RUN(bool,
            results.QueueSize() = 1;

            bool go =   readToGo->Get();
            writeToFinish->Put(i+0);

            while ( true ) {
                // Blocks the thread 
                results <<  (go ? 1 : 0);
            }

        ));
    }


    for ( size_t i =0; i< 2*NUM_WORKERS; ++i ) {
        this_thread::yield();
        Thread::Sleep(100);
    }

    // Now the go_flags should have NUM_WORKERS waiting on it
    if ( go_flags.BlockedReaders() != 50 ) {
        log << "The scheduler has not started the correct number of jobs!";
        log << "NUM_WORKERS: " << NUM_WORKERS;
        log << "Jobs: " << go_flags.BlockedReaders();
        return 1;
    }

    if ( Task_Master::Scheduler().FreeWorkers() != 0 ) {
        log << "The scheduler has failed to use all of its resources!";
        log << "Free Workers: " << Task_Master::Scheduler().FreeWorkers();
        log << "Jobs: " << go_flags.BlockedReaders();
        return 1;
    }

    // FIRE!
    for ( int i=0; i<50; ++i) {
        go_flags << true;
    }

    // Collect the results
    for ( int i=0; i<50; i++ ) {
        int j;
        work_done >> j;
        log << "Got: " << j << " :(";
        got[j] = true;
    }

    // Did we get everything?
    for ( int i =0 ; i< 50; i++ ) {
        if ( !got[i] ) {
            log << "Failed to get job slot: " << i << " :(";
            return 1;
        }
    }
    LOG( LOG_OVERVIEW, "Completing Immediate Run!")

    return 0;
}

int IterateResults(testLogger& log ) {
    /*
     * Kick of background tasks
     */ 
    rTask<long> t23 = GenerateMultiples(23);
    rTask<long> t34 = GenerateMultiples(34);

    /*
     * Look for a common pair
     */ 
    long m23 = 1;
    long m34 = 2;
    for ( int i=1; m23!=m34; i++ ) {
        /*
         * Extracts data from other threads, blocks if 
         * the other threads are yet to post any more results
         */
        if ( m23 < m34 ) {
            t23.Results() >> m23;
        } else {
            t34.Results() >> m34;
        }
        log << i << ": " << m23 << " , " << m34;
    }

    // Release our resources
    t23->Done();
    t34->Done();

    if ( m23 != 782 ) {
        return 1;
    } else {
        return 0;
    }
}
