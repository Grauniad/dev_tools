#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include "util_time.h"
#include "stdWriter.h"
#include "env.h"

DefaultTestLogger DefaultTestLogger::GLOBAL_LOG;

using namespace std;
DefaultTestLogger& DefaultTestLogger::RunTimeLog() {
    return GLOBAL_LOG;
}

DefaultTestLogger::DefaultTestLogger(): ffull_log("test.full_log") {
    Logger::Instance().RegisterLog(*this);

    // Prevent Logging directly to stdout / stderr
    Logger::Instance().RemoveLog(LogFactory::CERR());
    Logger::Instance().RemoveLog(LogFactory::CLOG());
    Logger::Instance().RemoveLog(LogFactory::COUT());

    Logger::Instance().LogEnabled( LOG_DEFAULT, true);
    Logger::Instance().LogEnabled( LOG_OVERVIEW, true);
    if ( !ENV::IsSet("NO_LOG_VERBOSE") ) {
        Logger::Instance().LogEnabled( LOG_VERBOSE, true);
    }

    if ( !ENV::IsSet("NO_LOG_VERY_VERBOSE") ) {
        Logger::Instance().LogEnabled( LOG_VERY_VERBOSE, true);
    }

    if ( !ENV::IsSet("NO_LOG_LOCKS") ) {
        Logger::Instance().LogEnabled( LOG_LOCKS, true);
    }

    if ( !ENV::IsSet("NO_LOG_CHANNEL") ) {
        Logger::Instance().LogEnabled( LOG_CHANNEL, true);
    }

    if ( !ENV::IsSet("NO_LOG_SCHEDULER") ) {
        Logger::Instance().LogEnabled( LOG_SCHEDULER, true);
    }
    Logger::Instance().LogEnabled( LOG_WARNING, true);
    Logger::Instance().LogEnabled( LOG_ERROR, true);
}

DefaultTestLogger::~DefaultTestLogger() {
    Logger::Instance().RemoveLog(*this);
}

void DefaultTestLogger::WriteLog(const string& fname, const Time& time) {
    OFStreamWriter("OverviewLog_" + time.FileTimestamp() + fname) << overview_log.str();
    OFStreamWriter("FullLog_" + time.FileTimestamp() + fname) << full_log.str();
}

void DefaultTestLogger::Log( const string& message,
                             const string& context, 
                             const Time& time,
                             LOG_LEVEL level) {
    if ( level == LOG_DEFAULT ) {
        testoutput << message;
        overview_log << message;
        full_log << message;

    } else if (    level == LOG_ERROR
                || level == LOG_WARNING
                || level == LOG_OVERVIEW) 
    {
        testoutput << GenericFormatLogger::Format(message,context,time,level);
        overview_log << GenericFormatLogger::Format(message,context,time,level);
        full_log << GenericFormatLogger::Format(message,context,time,level);
    } else {
        full_log << GenericFormatLogger::Format(message,context,time,level);

        if ( ENV::IsSet("DEV_TOOLS_TEST_FULL_TEST_LOG") ) {
            testoutput << GenericFormatLogger::Format(message,context,time,level);
        }
    }

    if ( ENV::IsSet("DEV_TOOLS_TEST_FULL_TEST_LOG") ) {
        ffull_log << GenericFormatLogger::Format(message,context,time,level);
        ffull_log.flush();
    } else if ( ENV::IsInList("DEV_TOOLS_TEST_CAPTURE_FROM", context) ) {
        ffull_log << GenericFormatLogger::Format(message,context,time,level);
        ffull_log.flush();
    }
}

Test::Test(string description, std::function<int(testLogger& log)> test):
    runTime(0)
{
    this->testLogged = test;
    this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest() {
    cout << description << ": ";
    testLogger log;
    startTime.SetNow();
    auto start = clock();
    int result = testLogged(log);
    auto stop = clock();
    #ifdef __PROFILE__TESTS
        // we don't want to mess around in the invoke function
        using TestPtr = int(*)(testLogger&);
        TestPtr f = *testLogged.target<TestPtr>();
        int count =1;
        while ( clock() == start ) {
            f(log);
            count++;
        }
        stop = clock();
        if (result == 0 ) {
            long TICKS_PER_SECOND = CLOCKS_PER_SEC;
            double duration =double( stop +1 - start);
            long target = __PROFILE_CLOCK_SECS__ * TICKS_PER_SECOND; 

            cout << duration << " (x"  << count << "), " << target;
            cout << " , " << TICKS_PER_SECOND;
            cout << "( " << stop << " - " << start << " )" << endl;

            for ( long t = duration; t < target; t+=duration ) {
                for ( int i=0; i<count; i++) {
                    f(log);
                }
            }
        }
    #endif
    runTime = double(stop - start) / CLOCKS_PER_SEC;
    if (result == 0 ) {
        cout << "TEST PASSED in " << runTime << " seconds" <<  endl;
    } else {
        cout << "TEST FAILED in " << runTime << " seconds" <<  endl;
        cout << "Run Time Log:" << endl;
        cout << "Test Log follows: " << endl;
        cout << log.str() << endl;
        DefaultTestLogger::RunTimeLog().WriteLog("TestRuntimeLog", startTime);
        log.WriteLog(description, startTime);
        if ( ENV::IsSet("DEV_TOOLS_TEST_FULL_TEST_LOG") ) {
            cout << DefaultTestLogger::RunTimeLog().str() << endl;
        }
        exit(result);
    }
}
