#include "communication_server.hpp"
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <iostream>

class RequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    RequestHandler(IRequestHandler* handler) : _handler(handler) {}

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override {
        std::istream& inStream = request.stream();
        std::string requestBody(std::istreambuf_iterator<char>(inStream), {});
        
        std::string responseBody = _handler->handleRequest(requestBody);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        std::ostream& out = response.send();
        out << responseBody;
    }

private:
    IRequestHandler* _handler;
};

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    RequestHandlerFactory(IRequestHandler* handler) : _handler(handler) {}

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override {
        return new RequestHandler(_handler);
    }

private:
    IRequestHandler* _handler;
};

class CommunicationServer::Impl {
public:
    Impl(IRequestHandler* handler, unsigned short port) {
        Poco::Net::ServerSocket svs(port);
        Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;

        server = new Poco::Net::HTTPServer(new RequestHandlerFactory(handler), svs, pParams);
    }

    ~Impl() {
        delete server;
    }

    void start() {
        server->start();
    }

    void stop() {
        server->stop();
    }

private:
    Poco::Net::HTTPServer* server;
};

CommunicationServer::CommunicationServer(IRequestHandler* handler, unsigned short port)
    : pImpl(new Impl(handler, port))
{}

CommunicationServer::~CommunicationServer() = default;

void CommunicationServer::start() {
    pImpl->start();
}

void CommunicationServer::stop() {
    pImpl->stop();
}
