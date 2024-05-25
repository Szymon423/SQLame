#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/SecureServerSocket.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

#include <iostream>
#include <string>
#include <memory>

class MyHttpsServer {
public:
    MyHttpsServer(const std::string& certFile, const std::string& keyFile, const std::string& caFile, unsigned short port);
    ~MyHttpsServer();
    void start();
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};