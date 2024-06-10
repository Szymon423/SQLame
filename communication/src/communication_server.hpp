#pragma once

#include "communication_requests_handler.hpp"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <iostream>
#include <memory>

/// @brief class for handling querries to SQLame engine
class QuerryRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    /// @brief Constructor with RequestHandler
    /// @param handler class with implementation of way of handling querries
    QuerryRequestHandler(IRequestHandler* handler);

    /// @brief Function which actually handles request
    /// @param request request object
    /// @param response response object
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
    /// @brief request handler interface element
    IRequestHandler* _handler;
};


/// @brief class for handling authorisation request
class AuthorisationRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    /// @brief default constructor
    AuthorisationRequestHandler(IAuthorisationHandler* _handler);

    /// @brief function which actually handles authorisation request
    /// @param request request object
    /// @param response response object
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
    /// @brief authorisation handler interface element
    IAuthorisationHandler* _handler;
};


/// @brief class for handling not supported api paths
class UndefiniedRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    /// @brief default constructor
    UndefiniedRequestHandler() = default;

    /// @brief function which actually handles unsupported request
    /// @param request request object
    /// @param response response object
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};


class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    RequestHandlerFactory(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler);

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

private:
    IAuthorisationHandler* _auth_handler;
    IRequestHandler* _request_handler;
};


/// @brief clas handling http communication server
class CommunicationServer {
public:
    /// @brief constructor
    /// @param handler object responsible for handling requests processing
    /// @param port port for communication
    CommunicationServer(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler, unsigned short port);

    /// @brief destructor
    ~CommunicationServer();

    /// @brief function to run server in background
    void start();

    /// @brief function to stop server
    void stop();

private:
    class Impl {
    public:
        Impl(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler, unsigned short port);
        ~Impl();
        void start();
        void stop();
    private:
        Poco::Net::HTTPServer* server;
    };
    std::unique_ptr<Impl> pImpl;
};