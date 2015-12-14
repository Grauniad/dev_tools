#ifndef DEV_TOOLS_CPP_LIBS_WEBSOCKETS_HTTP_REQUEST_H__
#define DEV_TOOLS_CPP_LIBS_WEBSOCKETS_HTTP_REQUEST_H__

#include <http_client.h>
#include <future>

class HTTPRequest: public AsyncHTTPSClient
{
public: 
    /**
     * Perform a standard GET request
     */
    HTTPRequest(boost::asio::io_service& io_service,
                const std::string& server,
                const std::string& path);

    ~HTTPRequest();

    /**
     * Perform a standard POST request
     */
    HTTPRequest(boost::asio::io_service& io_service,
                const std::string& server,
                const std::string& path,
                const std::string &data,
                const HeaderMap&  headers);

    /**
     * Waits for request to complete before returning the result
     */
    const HTTPMessage& WaitForMessage();


private:
    virtual void OnComplete(HTTPMessage& msg);

    std::promise<int> statusFlag;
    std::future<int>  statusFuture;
};

#endif
