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

#define LOG(level, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        Logger::Instance().LogMessage(text, level); \
    }

#define LOG_FROM(level, context, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        Logger::Instance().LogMessage(text, level, context); \
   }

#define SPRINT(text) \
    if ( Logger::Instance().LogEnabled(LOG_DEFAULT) ) { \
        std::stringstream& __s__ = LogFactory::Buf(); \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), LOG_DEFAULT); \
    }

#define SLOG(level, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        std::stringstream& __s__ = LogFactory::Buf(); \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), level); \
   }

#define SLOG_FROM(level, context, text) \
    if ( Logger::Instance().LogEnabled(level) ) { \
        std::stringstream& __s__ = LogFactory::Buf(); \
        __s__ << text; \
        Logger::Instance().LogMessage(__s__.str(), level, context); \
    }

