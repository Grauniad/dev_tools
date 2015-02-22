#ifndef LOGGER_PRE_PROCESS
#define LOGGER_PRE_PROCESS

#ifdef DEV_TOOLS_NO_LOGGING
/*
 * Optimise out logging
 */

#define IF_LOG(level,code) /* Logging has been optimised out! */
#define FLOG_FROM(level, context, code) /* Logging has been optimised out! */
#define PRINT(text) /* Logging has been optimised out! */
#define LOG_FROM(level, context, text) /* Logging has been optimised out! */
#define SPRINT(text) /* Logging has been optimised out! */
#define SLOG(level, text) /* Logging has been optimised out! */
#define SLOG_FROM(level, context, text) /* Logging has been optimised out! */

#else

#define IF_LOG(level,code) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        code \
    }

#define FLOG_FROM(level, context, code) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        Logger::Instance().LogMessage( [=] () -> std::string { \
            code; \
        }, level, context) \
    }

#define PRINT(text) \
    if ( Logger::Instance().LogEnabled(LOG_DEFAULT) ) { \
        Logger::Instance().LogMessage(text, LOG_DEFAULT); \
    }

#define LOG_FROM(level, context, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        Logger::Instance().LogMessage(text, level, context); \
   }

#define SPRINT(text) \
    if ( Logger::Instance().LogEnabled(LOG_DEFAULT) ) { \
        std::stringstream __s__; \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), LOG_DEFAULT); \
    }

#define SLOG(level, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        std::stringstream __s__; \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), level); \
   }

#define SLOG_FROM(level, context, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        std::stringstream __s__; \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), level, context); \
    }

#endif


#endif
