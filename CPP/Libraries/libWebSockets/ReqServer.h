#ifndef __DEV_TOOLS_CPP_LIBRARIES_WEBSOCKETS_REQ_SERVER_H__
#define __DEV_TOOLS_CPP_LIBRARIES_WEBSOCKETS_REQ_SERVER_H__

#include <map>
#include <memory>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <IPostable.h>

class RequestReplyHandler {
public:
    virtual ~RequestReplyHandler() {}

protected:
    friend class RequestServer;
    virtual std::string OnRequest(const char* request) = 0;

    struct InvalidRequestException {
        int code;
        std::string errMsg;
    };
};


class SubscriptionHandler {
public:
    virtual ~SubscriptionHandler() {}

    class SubRequest {
    public:
        virtual void SendMessage(const std::string& msg) = 0;

        virtual const char* RequestMessasge() = 0;
    };

    typedef std::shared_ptr<SubRequest> RequestHandle;

    struct InvalidRequestException {
        int code;
        std::string errMsg;
    };

    virtual void OnRequest(RequestHandle hdl) = 0;
};

typedef websocketpp::server<websocketpp::config::asio> server;

/**
 *  The request server 
 */
class RequestServer: public IPostable {
public:
    /**************************************************************
     *                Initialisation Interface
     **************************************************************/
    /**
     * Construct the request server.
     */
    RequestServer();

    /**
     * Install a new handler, which will be availabel when HandleRequests is
     * called
     */
    void AddHandler(
             const std::string& requestName, 
             std::unique_ptr<RequestReplyHandler>&& handler);

    void AddHandler(
             const std::string& requestName, 
             std::unique_ptr<SubscriptionHandler>&& handler);

    /**
     * Run the event loop, handle any incoming requests or posted tasks
     */
    void HandleRequests(unsigned short port);

    /**************************************************************
     *                Event Loop Interface
     **************************************************************/

    /**
     * Queue a task to be performed on the event loop thread.
     */
    typedef IPostable::Task InteruptHandler;
    void PostTask(const InteruptHandler& f);

    /**************************************************************
     *                Utility Methods
     **************************************************************/

    std::string HandleMessage(
         const std::string& request,
         server*  raw_server,
         websocketpp::connection_hdl hdl);

private:
    std::string HandleRequestReplyMessage(
                    const std::string& reqName,
                    const std::string& request,
                    RequestReplyHandler& handler);

    std::string HandleSubscriptionMessage(
                    const std::string& reqName,
                    const std::string& request,
                    SubscriptionHandler& handler,
                    server*  raw_server,
                    websocketpp::connection_hdl hdl);

    server echo_server;

    std::map<std::string,std::unique_ptr<RequestReplyHandler>> req_handlers;
    std::map<std::string,std::unique_ptr<SubscriptionHandler>> sub_handlers;
};


#endif
