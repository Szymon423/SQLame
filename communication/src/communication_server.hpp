#pragma once

#include "communication_requests_handler.hpp"
#include <Poco/Net/HTTPServer.h>
#include <memory>

class CommunicationServer {
public:
    CommunicationServer(IRequestHandler* handler, const std::string& certFile, const std::string& keyFile, const std::string& caFile, unsigned short port);
    ~CommunicationServer();
    void start();
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};