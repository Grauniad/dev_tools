#include "ReqServer.h"


#include <iostream>
#include <string>
#include <sstream>

#include <SimpleJSON.h>

/**************************************************************
 *                Utilities
 **************************************************************/

namespace {
    NewIntField(errorNumber);
    NewStringField(errorText);
    typedef SimpleParsedJSON<errorNumber,errorText> ErrorMsg;

    std::string ErrorMessage(const std::string& msg) {
        std::string response;
        static ErrorMsg errJSON;
        errJSON.Clear();

        errJSON.Get<errorNumber>() = 0;
        errJSON.Get<errorText>() = msg;
        response = "ERROR ";
        response += errJSON.GetJSONString();
        return response;
    }

}

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(RequestServer* s, server* raw_server, websocketpp::connection_hdl hdl, message_ptr msg) {
    if (msg->get_payload() == "stop-listening") {
        raw_server->stop_listening();
    } else {
        const std::string& reply = s->HandleMessage(msg->get_payload(),raw_server,hdl);

        raw_server->send(hdl,reply,websocketpp::frame::opcode::TEXT);
    }
}

/**************************************************************
 *                Data Subscriptions
 **************************************************************/
class Request: public SubscriptionHandler::SubRequest {
public:
    Request(
        const std::string& req,
        server* s,
        websocketpp::connection_hdl c)
    : request(req), serv(s), conn(c)
    {
    }

    const char* RequestMessasge() {
        return request.c_str();
    }

    /**
     * Send a data update, a JSON message, down the pipe
     */
    void SendMessage(const std::string& msg) {
        serv->send(conn,msg,websocketpp::frame::opcode::TEXT);
    }

private:
    std::string                 request;
    server*                     serv;
    websocketpp::connection_hdl conn;
};

/**************************************************************
 *                Request Serve
 **************************************************************/

RequestServer::RequestServer() {
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    echo_server.init_asio();
}

std::string RequestServer::HandleMessage(
    const std::string& request,
    server* raw_server,
    websocketpp::connection_hdl hdl)
{

    std::string response = "";
    std::stringstream strRequest(request);
    std::string reqName;
    strRequest >> reqName;

    auto req_it = req_handlers.find(reqName);

    if ( req_it != req_handlers.end()) {
        response = HandleRequestReplyMessage(reqName,request,*(req_it->second));
    } else {
        auto sub_it = sub_handlers.find(reqName);
        if ( sub_it != sub_handlers.end()) {
            response = 
                HandleSubscriptionMessage(reqName, request, *(sub_it->second),raw_server,hdl);
        } else {
            response = ErrorMessage("No such request: " + reqName);
        }
    }
    return response;
}

std::string RequestServer::HandleRequestReplyMessage(
                    const std::string& reqName,
                    const std::string& request,
                    RequestReplyHandler& handler) 
{
    std::string response = "";
    // "REQUEST_NAME { name: JSON_REQUEST, ...}"
    unsigned int offset = reqName.length() + 1;
    const char* jsonRequest = "";
    if (request.length() > offset) {
        jsonRequest = request.c_str() + offset;
    }
    try {
        response = handler.OnRequest(jsonRequest);
    } catch (const RequestReplyHandler::InvalidRequestException& e) {
        response = ErrorMessage(e.errMsg);
    }

    return response;
}

std::string RequestServer::HandleSubscriptionMessage(
                    const std::string& reqName,
                    const std::string& request,
                    SubscriptionHandler& handler,
                    server*  raw_server,
                    websocketpp::connection_hdl hdl)
{
    std::string response = "";
    // "REQUEST_NAME { name: JSON_REQUEST, ...}"
    unsigned int offset = reqName.length() + 1;
    const char* jsonRequest = "";
    if (request.length() > offset) {
        jsonRequest = request.c_str() + offset;
    }
    try {
        SubscriptionHandler::RequestHandle reqHdl(
            new Request(jsonRequest,raw_server,hdl));
        response = handler.OnRequest(reqHdl);
    } catch (const SubscriptionHandler::InvalidRequestException& e) {
        response = ErrorMessage(e.errMsg);
    }

    return response;
}


void RequestServer::HandleRequests(unsigned short port) {
    try {
        echo_server.set_message_handler(bind(&on_message,this,&echo_server,::_1,::_2));

        echo_server.listen(port);

        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

void RequestServer::PostTask(const RequestServer::InteruptHandler& f) {
    boost::asio::io_service& serv = echo_server.get_io_service();
    
    serv.post(f);
}
