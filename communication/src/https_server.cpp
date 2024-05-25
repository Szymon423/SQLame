#include "https_server.hpp"


class MyRequestHandler: public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("text/html");
        std::ostream& out = response.send();
        out << "<html><head><title>HTTPS Server</title></head>";
        out << "<body><h1>Hello, HTTPS!</h1></body></html>";
    }
};

class MyRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override {
        return new MyRequestHandler;
    }
};

class MyHttpsServer::Impl {
public:
    Impl(const std::string& certFile, const std::string& keyFile, const std::string& caFile, unsigned short port) {
        Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::SERVER_USE, certFile, keyFile, caFile));
        Poco::Net::SecureServerSocket svs(port, 64, context);
        Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;

        server = new Poco::Net::HTTPServer(new MyRequestHandlerFactory, svs, pParams);
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

MyHttpsServer::MyHttpsServer(const std::string& certFile, const std::string& keyFile, const std::string& caFile, unsigned short port)
    : pImpl(new Impl(certFile, keyFile, caFile, port))
{
}

MyHttpsServer::~MyHttpsServer() = default;

void MyHttpsServer::start() {
    pImpl->start();
}

void MyHttpsServer::stop() {
    pImpl->stop();
}
