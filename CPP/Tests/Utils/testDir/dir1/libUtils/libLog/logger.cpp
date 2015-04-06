#include "logger.h"
#include "env.h"
#include <iostream>
#include <string>
#include <sstream>
#include <util_time.h>

using namespace std;

LogDevice::LogDevice() {
    static unsigned long nextId =0;
    this->LogDevice_Id = nextId;
    ++nextId;
}

/*
 * Log Factory
 */
class LogDevice_COUT: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            cout << message << endl;
        } else {
            cout << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _COUT;

class LogDevice_CERR: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            cerr << message << endl;
        } else {
            cerr << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _CERR;

class LogDevice_CLOG: public LogDevice {
    virtual void Log( const string& message,
                      const string& context, 
                      const Time& time,
                      LOG_LEVEL level) {

        if ( level == LOG_DEFAULT ) {
            clog << message << endl;
        } else {
            clog << GenericFormatLogger::Format(message,context,time,level);
        }
    }
} _CLOG;

LogDevice& LogFactory::COUT() {
    static LogDevice_COUT _cout;
    return _cout;
}

LogDevice& LogFactory::CERR() {
    static LogDevice_CERR _cerr;
    return _cerr;
}

LogDevice& LogFactory::CLOG() {
    static LogDevice_CLOG _clog;
    return _clog;
}

/*
 * Logger Implementation
 */

Logger::Logger() {
    logLevelNames.resize(__NUM_LOG_LEVELS);

    logLevelNames[LOG_DEFAULT] = "STDOUT";
    logLevelNames[LOG_OVERVIEW] = "DEBUG 1";
    logLevelNames[LOG_VERBOSE] = "DEBUG 2";
    logLevelNames[LOG_VERY_VERBOSE] = "DEBUG 3";
    logLevelNames[LOG_WARNING] = "WARNING";
    logLevelNames[LOG_ERROR] = "ERROR";
    logLevelNames[LOG_LOCKS] = "THREADING::LOCKS";
    logLevelNames[LOG_SCHEDULER] = "THREADING::SCHEDULER";
    logLevelNames[LOG_CHANNEL] = "THREADING::CHANNELS";

    enabled[LOG_DEFAULT]      = true;
    enabled[LOG_WARNING]      = true;
    enabled[LOG_ERROR]        = true;

    enabled[LOG_OVERVIEW]     = ENV::IsSet("DEV_TOOLS_LOG_OVERVIEW");
    enabled[LOG_VERBOSE]      = ENV::IsSet("DEV_TOOLS_LOG_VERBOSE");
    enabled[LOG_VERY_VERBOSE] = ENV::IsSet("DEV_TOOLS_LOG_VERY_VERBOSE");


    RegisterLog(LogFactory::CLOG());

}

void Logger::RegisterLog(LogDevice& log) {
    std::unique_lock<std::recursive_mutex> lock(loggingMutex);
    devices.insert(log);
}

void Logger::RemoveLog(LogDevice& log) {
    std::unique_lock<std::recursive_mutex> lock(loggingMutex);
    devices.erase(log);
}


void Logger::LogMessage( const string& message, 
                         LOG_LEVEL level,
                         const string& context) {
    // Only one thread may log at a time
    std::unique_lock<std::recursive_mutex> lock(loggingMutex);
    for(const LogDeviceKey& device: devices) {
        device.Log(message, context, Time(), level );
    }
}

void Logger::LogMessage( const BinaryReader& msg, 
                         LOG_LEVEL level,
                         const string& context) {
    string&& text = msg.ReadString();
    LogMessage(text,level, context);
}

void LogDevice_BinaryWriter::Log( const string& message,
                  const string& context, 
                  const Time& time,
                  LOG_LEVEL level) {
    log << GenericFormatLogger::Format(message,context,time,level);
}

string GenericFormatLogger::Format( const string& message,
                                    const string& context,
                                    const Time& time,
                                    LOG_LEVEL level) {
    stringstream log;
    log << "\n";
    log << "[" << Logger::Instance().GetName(level) << "] " 
        << Time().Timestamp() << " - " 
        << context << ": " << "\n";
    log << message << "\n";
    log << "\n";
    return log.str();
}

LOG_LEVEL Logger::GetValue(const std::string& level) {
	LOG_LEVEL levelKey = __NUM_LOG_LEVELS;
	for(size_t i = 0; i<logLevelNames.size() && levelKey==__NUM_LOG_LEVELS; ++i) {
		if (logLevelNames[i]==level)
		{
			levelKey = static_cast<LOG_LEVEL>(i);
		}
	}
	return levelKey;
}
