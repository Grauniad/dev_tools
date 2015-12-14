#ifndef DEV_TOOLS_CPP_LIBS_WEBSOCKETS_IO_THREAD_H__
#define DEV_TOOLS_CPP_LIBS_WEBSOCKETS_IO_THREAD_H__

#include <http_request.h>
#include <stream_client.h>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>


/**
 * Spawns a new IO thread, which can be used to create
 * new requests
 */
class IOThread {
public:
    /**
     * Start the new thread
     */ 
    IOThread();

    virtual ~IOThread();

    /**
     * Request content via a one-shot HTTP request.
     */
    std::shared_ptr<HTTPRequest> HTTPSRequest(
        const std::string& server,
        const std::string& path);

    /**
     * Request content via a one-shot HTTP request.
     */
    std::shared_ptr<HTTPRequest> HTTPSPOST(
        const std::string& server,
        const std::string& path,
        const std::string& data,
        const AsyncHTTPSClient::HeaderMap& headers);

    /**
     * Spawn a new thread to handle a websocket stream
     */

private:
    void IOLoop();

    /**
     * Stop the thread. 
     *
     * This function call will block until the IO thread as been
     * terminated.
     */
    void Stop();

    /**
     * Check if the IO thread should continue
     */
    bool IOContinue();

    /**
     * Notifies the IO thread that new has been scheduled.
     */
    void NewIO();

    std::atomic<bool> io_continue;
    std::condition_variable io_trigger;
    std::mutex io_mutex;
    typedef std::unique_lock<std::mutex> IOLock;
    boost::asio::io_service io_service;
    std::thread io_thread;
};

#endif
