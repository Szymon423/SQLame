#pragma once

#include "communication_requests_handler.hpp"
#include <Poco/Net/HTTPServer.h>
#include <memory>

/// @brief clas handling http communication server
class CommunicationServer {
public:

    /// @brief constructor
    /// @param handler object responsible for handling requests processing
    /// @param port port for communication
    CommunicationServer(IRequestHandler* handler, unsigned short port);

    /// @brief destructor
    ~CommunicationServer();

    /// @brief function to run server in background
    void start();

    /// @brief function to stop server
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};