/*
 * CEFJSLogging.cpp
 *
 *  Created on: 15 Feb 2015
 *      Author: lhumphreys
 */


#include "CEFJSLogging.h"
#include <logger.h>

#include <CEFJSUtils.h>
#include <util_time.h>

class FormatFunc: public CEFJSUtils::JSFunction {
public:
	FormatFunc(): JSFunction("DEV_TOOLS_Log_Format") { }

	virtual void Execute(JSFunctionContext& context) {
		static std::string input;
		static std::string log_context;
		static std::string log_level;
		input.clear();
		log_context.clear();
		log_level.clear();
		if (context.GetArg(0,input) &&
			context.GetArg(1,log_context) &&
			context.GetArg(2,log_level))
		{
			LOG_LEVEL logLevelKey =
					Logger::Instance().GetValue(log_level);
			if (logLevelKey!=__NUM_LOG_LEVELS)
			{
				context.SetRetVal(
                    GenericFormatLogger::Format(
                        input,
                        log_context,
                        Time(),
                        logLevelKey)
				);
			}
			else
			{
				LOG_FROM(LOG_VERBOSE,"FormatFunc","Unknown level" + log_level);
			}
		}
		else
		{
            LOG_FROM(LOG_VERBOSE,"FormatFunc","Failed to parse args :(");
		}
	}
};

bool CEFJSLogging::AddFormater(CefRefPtr<CefV8Context> context) {
	bool ok = false;
	FormatFunc::Install(
	    context,
	    std::unique_ptr<CEFJSUtils::JSFunction>(new FormatFunc));
	return ok;
}
