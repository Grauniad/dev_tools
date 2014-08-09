#include <cstdlib>
#include <cstdio>
#include "util_time.h"
#include <iostream>
#include "stringUtils.h"
#include "tester.h"
#include <cstring>
#include <limits>

using namespace std;

const long TARGET = 100000;
const long MAX = numeric_limits<long>::max();
const long BIG_MUL = 100000;
const size_t MAX_PRINT=8;

namespace benchmark {
    void stdprintf() {
        char buf [50];
        for ( long i = 0; i< TARGET; ++i ) {
            snprintf(buf,MAX_PRINT,"%ld",i);
        }
    }
    void stdprintf_big() {
        char buf [MAX_PRINT];
        for ( long i = 0; i< TARGET; ++i ) {
            snprintf(buf,MAX_PRINT,"%ld",i*BIG_MUL);
        }
    }
    void stdprintf_huge() {
        char buf [MAX_PRINT];
        for ( long i = MAX; i> MAX - TARGET; --i ) {
            snprintf(buf,MAX_PRINT,"%ld",i);
        }
    }
    void Fast() {
        char buf [MAX_PRINT];
        for ( long i = 0; i< TARGET; ++i ) {
            StringUtils::FastPrintLong(i,MAX_PRINT,buf);
        }
    }
    void Fast_big() {
        char buf [MAX_PRINT];
        for ( long i = 0; i< TARGET; ++i ) {
            StringUtils::FastPrintLong(i*BIG_MUL,MAX_PRINT,buf);
        }
    }
    void Fast_huge() {
        char buf [MAX_PRINT];
        for ( long i = MAX; i> MAX - TARGET; --i ) {
            StringUtils::FastPrintLong(i,MAX_PRINT,buf);
        }
    }
};

int CheckLongs(testLogger& log );

int main(int argc, const char *argv[])
{
    Test("Checking longs...",CheckLongs).RunTest();
    /*
     * sprintf benchmarks
     */
    Time start;
    benchmark::stdprintf();
    Time stop;
    cout << "snprintf: " << stop.DiffUSecs(start) << "us" << endl;
    start.SetNow();
    benchmark::stdprintf_big();
    stop.SetNow();
    cout << "snprintf (big N): " << stop.DiffUSecs(start) << "us" << endl;
    start.SetNow();
    benchmark::stdprintf_huge();
    stop.SetNow();
    cout << "snprintf_huge (huge N): " << stop.DiffUSecs(start) << "us" << endl;

    /*
     * FastPrintLong benchmarks
     */
    start.SetNow();
    benchmark::Fast();
    stop.SetNow();
    cout << "StringUtils::FastPrintLong: " << stop.DiffUSecs(start) << "us" << endl;
    start.SetNow();
    benchmark::Fast_big();
    stop.SetNow();
    cout << "StringUtils::FastPrintLong (big N): " << stop.DiffUSecs(start) << "us" << endl;
    start.SetNow();
    benchmark::Fast_huge();
    stop.SetNow();
    cout << "StringUtils::FastPrintLong (huge N): " << stop.DiffUSecs(start) << "us" << endl;
    return 0;
}

int CheckLongs(testLogger& log ) {
    char sbuf[50];
    char fbuf[50];
    for ( long i = -100000; i < 100000; ++i ) {
        snprintf(sbuf,5,"%ld",i);
        StringUtils::FastPrintLong(i,5,fbuf);
        if (  strncmp(sbuf,fbuf,50) != 0 ) {
            log << "Failed long: " << i << endl;
            log << "expected: " << sbuf << endl;
            log << "actual: " << fbuf << endl;
            return 1;
        }
    }
    return 0;
}
