#include "http_request.h"

HTTPRequest::HTTPRequest(boost::asio::io_service& io_service,
                         const std::string& server,
                         const std::string& path)
   : AsyncHTTPSClient(REQUEST_MODE_GET, io_service, server, path, {}, ""),
     statusFuture(statusFlag.get_future())
{
}

HTTPRequest::~HTTPRequest() {
    WaitForMessage();
}

HTTPRequest::HTTPRequest(boost::asio::io_service& io_service,
                         const std::string& server,
                         const std::string& path,
                         const std::string& data,
                         const HeaderMap& headers)
   : AsyncHTTPSClient(REQUEST_MODE_POST, io_service, server, path, headers, data),
     statusFuture(statusFlag.get_future())
{
}

const HTTPMessage& HTTPRequest::WaitForMessage() {
    statusFuture.wait();
    return GetMessage();
}

void HTTPRequest::OnComplete(HTTPMessage& msg) {
    statusFlag.set_value(msg.status_code);
}
