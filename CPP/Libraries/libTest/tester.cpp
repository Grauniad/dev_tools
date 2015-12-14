#include <iostream>
#include "tester.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>
#include "util_time.h"
#include "stdWriter.h"
#include "env.h"
#include <algorithm>

const size_t MAX_QUEUE_SIZE = 1e7;

DefaultTestLogger DefaultTestLogger::GLOBAL_LOG;

using namespace std;
DefaultTestLogger& DefaultTestLogger::RunTimeLog() {
    return GLOBAL_LOG;
}

DefaultTestLogger::DefaultTestLogger()
   : ffull_log("test.full_log"),
     use_full_log(false), use_capture_from(false)
{
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

    use_full_log = ENV::IsSet("DEV_TOOLS_TEST_FULL_TEST_LOG");
    use_capture_from = ENV::IsSet("DEV_TOOLS_TEST_CAPTURE_FROM");
    Logger::Instance().LogEnabled( LOG_ERROR, true);
}

DefaultTestLogger::~DefaultTestLogger() {
    Logger::Instance().RemoveLog(*this);
}

void DefaultTestLogger::WriteLog(const string& fname, const Time& time) {
	FlushQueue();
    OFStreamWriter("OverviewLog_" + time.FileTimestamp() + fname) << overview_log.str();
    OFStreamWriter("FullLog_" + time.FileTimestamp() + fname) << full_log.str();
}

void DefaultTestLogger::Log( const string& message,
                             const string& context, 
                             const Time& time,
                             LOG_LEVEL level) {
	messageQueue.push({time,message,context,level});

	if ( messageQueue.size() > MAX_QUEUE_SIZE) {
     	cout << Time().Timestamp()
     	     << "***WARNING: MAX MESSAGE QUEUE SIZE REACHED!, FLUSHING QUEUE ****"
     	     << std::endl;
		FlushQueue();
        cout << Time().Timestamp() << "**** FLUSH COMPLETE ****" << std::endl;
	}

}

Test::Test(string description, std::function<int(testLogger& log)> test):
    runTime(0)
{
    this->testLogged = test;
    this->testType = LOGGED;
    this->description = description;
}

void Test::RunTest(FAILURE_ACTION action) {
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
        log.FlushQueue();
        cout << log.str() << endl;
        log.WriteLog(description, startTime);
        DefaultTestLogger::RunTimeLog().WriteLog("TestRuntimeLog", startTime);
        if ( ENV::IsSet("DEV_TOOLS_TEST_FULL_TEST_LOG") ) {
            cout << DefaultTestLogger::RunTimeLog().str() << endl;
        }
        switch(action)
        {
            case PRINT_LOG_AND_EXIT:
                exit(result);
                break;
            case PRINT_LOG_AND_THROW:
                throw TestFailureException{description};
                break;
            default:
                cout << "WARNING: Unknown action, I will now abort" << endl;
                abort();

        }
    }
}

void DefaultTestLogger::FlushQueue() {
	while (!messageQueue.empty()) {
		LogMessage(messageQueue.front());
		messageQueue.pop();
	}
}

void DefaultTestLogger::LogMessage(const Message& message) {
	const string messageText = GenericFormatLogger::Format(
			                       message.message,
								   message.context,
								   message.time,
								   message.level);
    if ( message.level == LOG_DEFAULT ) {
        testoutput << message.message;
        overview_log << messageText;
        full_log << messageText;

    } else if (    message.level == LOG_ERROR
                || message.level == LOG_WARNING
                || message.level == LOG_OVERVIEW)
    {
        testoutput << message.message;
        overview_log << messageText;
        full_log << messageText;
    } else {
        full_log << messageText;

        if ( use_full_log ) {
            testoutput << message.message;
        }
    }

    if ( use_full_log ) {
        ffull_log << messageText;
        ffull_log.flush();
    } else if (    use_capture_from
    		    && ENV::IsInList("DEV_TOOLS_TEST_CAPTURE_FROM", message.context) )
    {
        ffull_log << messageText;
        ffull_log.flush();
    }
}

void DefaultTestLogger::ReportStringDiff(
          const string& expected,
          const string actual)
{
    size_t len = 0;
    size_t diffIndex = 0;
    if ( expected != actual ) {
        (*this) << "Expected:" << expected << std::endl;
        (*this) << "Actual:" << actual << std::endl;
        (*this) << "String Miss-Match!" << std::endl;
        (*this) << "Expected Length: " << expected.length() << std::endl;
        (*this) << "Actual Length: " << actual.length() << std::endl;

        if ( expected.length() < actual.length() ) {
            len = expected.length();
        } else {
            len = actual.length();
        }

        for (; 
             diffIndex < len && (expected[diffIndex] == actual[diffIndex]);
             ++diffIndex) 
        {
        }

        if (diffIndex == len ) {
            (*this) << "No miss-match in shared section" << std::endl;
        } else {
            (*this) << "Diff detected: " << std::endl;
            size_t diffStart = len -25;
            size_t diffEnd = len +25;

            if ( diffStart < 0 ) {
                diffStart = 0;
            }

            if ( diffEnd > len ) {
                diffEnd = len;
            }

            (*this) << ">> " << expected.substr(diffStart,(diffEnd-diffStart)) << std::endl;
            (*this) << "<< " << actual.substr(diffStart,(diffEnd-diffStart)) << std::endl;
        }
    }
}
