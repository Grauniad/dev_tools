#include "ReqServer.h"

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <string>
#include <sstream>

#include <SimpleJSON.h>

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

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(RequestServer* s, server* raw_server, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get() << std::endl
              << " and message: " << msg->get_payload() << std::endl
              << " and header: " << msg->get_header()
              << std::endl;

    if (msg->get_payload() == "stop-listening") {
        raw_server->stop_listening();
    } else {
        const std::string& reply = s->HandleMessage(msg->get_payload());

        raw_server->send(hdl,reply,websocketpp::frame::opcode::TEXT);

    }
}


std::string RequestServer::HandleMessage(const std::string& request) {

    std::string response = "";
    std::stringstream strRequest(request);
    std::string reqName;
    strRequest >> reqName;

    auto it = handlers.find(reqName);

    if ( it != handlers.end()) {
        // "REQUEST_NAME { name: JSON_REQUEST, ...}"
        unsigned int offset = reqName.length() + 1;
        const char* jsonRequest = "";
        if (request.length() > offset) {
            jsonRequest = request.c_str() + offset;
        }
        try {
            response = it->second->OnRequest(jsonRequest);
        } catch (const RequestReplyHandler::InvalidRequestException& e) {
            response = ErrorMessage(e.errMsg);
        }
    } else {
        response = ErrorMessage("No such request: " + reqName);
    }
    return response;
}

void RequestServer::HandleRequests(unsigned short port) {
    server echo_server;

    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,this,&echo_server,::_1,::_2));

        echo_server.listen(port);
        std::cout << "Listening on 127.0.0.1:" << port << std::endl;

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
