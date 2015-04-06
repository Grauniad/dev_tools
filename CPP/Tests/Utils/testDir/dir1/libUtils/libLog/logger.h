#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "binaryWriter.h"
#include <set>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <list>

/*
 * Provides a basic logging utility:
 * 
 *     LOG( LOG_OVERVIEW, "Hello World!")
 *
 * By default logging is sent to clog. This behaviour can be changed by removing 
 * the clog device, and adding your own:
 *
 *     OFStreamWriter logfile("MyLog.txt");
 *     LogDevice_BinaryWriter logger(logfile);
 *
 *     Logger.RemoveLog(LogFactory::CLOG());
 *     Logger.RegisterLog(logger);
 *
 *  Each registered device is passed a Time object, the log level, a context string,
 *  and the message itself.
 *
 *  The Factory objects, and the default Writer object print a standard timestamp header,
 *  and all messages sent to them
 *
 */

// Expressions for logging messages
#include "logger_preProcessor.h"

class Time;

// Valid Log Levels
using namespace std; 
enum LOG_LEVEL {
    LOG_CHANNEL,
    LOG_LOCKS,
    LOG_DEFAULT,
    LOG_ERROR,
    LOG_OVERVIEW,
    LOG_SCHEDULER,
    LOG_VERBOSE,
    LOG_VERY_VERBOSE,
    LOG_WARNING,
    __NUM_LOG_LEVELS
};


/*
 * Class to be implemented to add a new log device
 */
class LogDevice { 
    friend class Logger;
public:
    LogDevice();
    // The pointer is good only for the lifetime of the call
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) = 0;

    virtual ~LogDevice(){};


private:
    unsigned long LogDevice_Id;

    // Prevent more then one copy of the same ID
    LogDevice& operator=(const LogDevice& rhs) = delete;
    LogDevice(const LogDevice& rhs) = delete;
};

/*
 * Common instances
 */
class LogDevice_BinaryWriter: public LogDevice { 
    LogDevice_BinaryWriter(BinaryWriter& w): log(w){}

    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level);

private:
    BinaryWriter& log;
};

class LogFactory {
public:
    static LogDevice& COUT();
    static LogDevice& CERR();
    static LogDevice& CLOG();
private:
};

class GenericFormatLogger { 
public:
    static string Format( const string& message,
                          const string& context,
                          const Time& time,
                          LOG_LEVEL level);
};



/*
 * Main Logging Class
 */
class Logger {
public:
    Logger ();

    static Logger& Instance() {
        static Logger l;
        return l;
    }

    bool LogEnabled(LOG_LEVEL level) {
        return enabled[level];
    }

    bool LogEnabled(LOG_LEVEL level, bool on) {
        return enabled[level] = on;
    }

    void RegisterLog(LogDevice& device);
    void RemoveLog(LogDevice& device);

    void LogMessage( const string& message, 
                            LOG_LEVEL level,
                            const string& context="Log Message");
    void LogMessage( const BinaryReader& msg, 
                            LOG_LEVEL level,
                            const string& context="Log Message");
    const string& GetName( LOG_LEVEL level ) {
        return logLevelNames[level];
    }

    LOG_LEVEL GetValue(const std::string& level);

private:  
    /*
     * Wrapper class for use in set
     */
    class LogDeviceKey {

    public:
        LogDeviceKey(LogDevice& logDevice): log(&logDevice){}

        void Log( const string& message, 
                  const string& context, 
                  const Time& time,
                  LOG_LEVEL level) const {
            log->Log(message, context ,time, level);
        }

        // Comparator
        class Less {
        public:
            bool operator()( const LogDeviceKey& lhs, 
                             const LogDeviceKey& rhs) {
                return   lhs.log->LogDevice_Id 
                       < rhs.log->LogDevice_Id;
            }
        };
    private:
       LogDevice* log;
    };

    bool enabled[__NUM_LOG_LEVELS];
    set<LogDeviceKey,LogDeviceKey::Less> devices;
    vector<string>           logLevelNames;
    std::recursive_mutex     loggingMutex;
};




#endif
