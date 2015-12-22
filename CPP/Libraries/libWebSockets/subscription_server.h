#ifndef DEV_TOOLS_CPP_LIBS_WEBSOCKETS_SUBSCRIPTION_SERVER_H__
#define DEV_TOOLS_CPP_LIBS_WEBSOCKETS_SUBSCRIPTION_SERVER_H__

#include <websocketpp/config/asio_client.hpp>

#include <websocketpp/client.hpp>
#include <atomic>
#include <thread>

/**
 * Subscribes to a websocket and triggers a call-back for each message.
 *
 * The run function starts its own event loop, and would ordinarily
 * be called from a dedicated worker thread.
 */
class SubscriptionServer {
public:
    /**
     * C'tor - Start listening on the specified port
     */
    SubscriptionServer(const std::string& url);

    /**
     * Indicates if the query is currently live
     */
    bool Running();

protected: 
    /**
     * Stop the event loop
     */
    void Stop();
    
private:
    typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
    typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;

    /**
     * Call-back triggered when a new message is received.
     *
     * NOTE: This call-back is triggered from the IO thread.
     */
    virtual void OnMessage(const std::string& msg) = 0;

    /**
     * Call-back to intialise the encryption
     */
    context_ptr on_tls_init(websocketpp::connection_hdl);

    /**
     * Call-back to notify us of a successful connection
     */
    void on_open(websocketpp::connection_hdl hdl);

    /**
     * Call-back triggered by the arrival of a new message
     */
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

    std::atomic<bool>   running;
    client m_endpoint;
    client::connection_ptr con;

};

class SubscriptionServerThread: public SubscriptionServer {
public:
    SubscriptionServerThread(const std::string& url);

    ~SubscriptionServerThread();
private:
    std::thread io_thread;
};


#endif
