#include "io_thread.h"
#include <iostream>

IOThread::IOThread()
   : io_continue(true),
     io_thread(&IOThread::IOLoop, this)
{
}

IOThread::~IOThread() {
    Stop();
}

void IOThread::Stop() {
    io_continue = false;
    NewIO();
    io_service.stop();
    io_thread.join();
}

std::shared_ptr<HTTPRequest> IOThread::HTTPSRequest(
    const std::string& server,
    const std::string& path)
{
    std::shared_ptr<HTTPRequest> req(
       new HTTPRequest(io_service,server,path));

    NewIO();

    return req;
}

std::shared_ptr<HTTPRequest> IOThread::HTTPSPOST(
    const std::string& server,
    const std::string& path,
    const std::string& data,
    const AsyncHTTPSClient::HeaderMap& headers)
{
    std::shared_ptr<HTTPRequest> req(
       new HTTPRequest(io_service,server,path,data,headers));

    NewIO();

    return req;
}

void IOThread::NewIO() {
    io_trigger.notify_all();
}

bool IOThread::IOContinue() {
    std::cout << "IOContinue: Waiting for trigger..." << std::endl;
    IOLock lock(io_mutex);
    io_trigger.wait(lock);
    std::cout << "IOContinue: Got Trigger..." << std::endl;
    return io_continue;
}

void IOThread::IOLoop() {
    while (IOContinue()) {
        try
        {
            std::cout << "IOLoop: start" << std::endl;
            io_service.run();
            io_service.reset();
            std::cout << "IOLoop: done" << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << "Exception: " << e.what() << "\n";
        }
    }
}
     
