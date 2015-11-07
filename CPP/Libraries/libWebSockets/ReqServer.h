#ifndef __DEV_TOOLS_CPP_LIBRARIES_WEBSOCKETS_REQ_SERVER_H__
#define __DEV_TOOLS_CPP_LIBRARIES_WEBSOCKETS_REQ_SERVER_H__

#include <map>
#include <memory>

class RequestReplyHandler {
public:
    virtual ~RequestReplyHandler() {}

    virtual std::string OnRequest(const char* request) = 0;

    struct InvalidRequestException {
        int code;
        std::string errMsg;
    };
};

class RequestServer {
public:
    void AddHandler(
             const std::string& requestName, 
             std::unique_ptr<RequestReplyHandler>&& handler) 
    {
        handlers[requestName].reset(handler.release());
    }

    std::string HandleMessage(const std::string& request);

    void HandleRequests(unsigned short port);
private:
    std::map<std::string,std::unique_ptr<RequestReplyHandler>> handlers;
};


#endif
