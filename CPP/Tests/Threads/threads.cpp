#include "tester.h"
#include "go.h"

int IterateResults(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Doing a dumb LCM algo in parallel...",IterateResults).RunTest();
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
    )
    return t;
}

int IterateResults(testLogger& log ) {
    /*
     * Kick of background tasks
     */ 
    Task<long>& t23 = *GenerateMultiples(23);
    Task<long>& t34 = *GenerateMultiples(34);

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
            t23 >> m23;
        } else {
            t34 >> m34;
        }
        log << i << ": " << m23 << " , " << m34;
    }

    // Release our resources
    t23.Done();
    t34.Done();

    if ( m23 != 782 ) {
        return 1;
    } else {
        return 0;
    }
}
