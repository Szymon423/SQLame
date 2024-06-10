#include "communication_server.hpp"


bool validate_JWT(const std::string& token, int& userId) {
    try {
        Poco::JWT::Token t;
        Poco::JWT::Signer signer(TOKEN_SIGNER);

        t = signer.verify(token);

        if (t.payload().has("uid")) {
            userId = t.payload().get("uid").convert<int>();
            return true;
        }
    } catch (const Poco::Exception& e) {
        return false;
    }
    return false;
}


QuerryRequestHandler::QuerryRequestHandler(IRequestHandler* handler) 
    : _handler(handler) {}


void QuerryRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::string authHeader = request.get("Authorization", "");

    if (authHeader.empty() || authHeader.find("Bearer ") != 0) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        response.setContentType("application/json");
        std::ostream& out = response.send();
        out << "{\"error\":\"Unauthorized\"}";
        return;
    }

    int userId;
    std::string token = authHeader.substr(7);
    if (!validate_JWT(token, userId)) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        response.setContentType("application/json");
        std::ostream& out = response.send();
        out << "{\"error\":\"Invalid token\"}";
        return;
    }

    std::istream& inStream = request.stream();
    std::string requestBody(std::istreambuf_iterator<char>(inStream), {});
    std::string responseBody = _handler->handleRequest(requestBody, userId);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    std::ostream& out = response.send();
    out << responseBody;
}


AuthorisationRequestHandler::AuthorisationRequestHandler(IAuthorisationHandler* handler) 
    : _handler(handler) {}


void AuthorisationRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::istream& inStream = request.stream();
    std::string requestBody(std::istreambuf_iterator<char>(inStream), {});
    
    response.setContentType("application/json");
    // handle response when no credentials provided
    if (!request.has("login") || !request.has("password")) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        std::ostream& out = response.send();
        out << "{\"error\": \"Missing authentication data.\"}";
        return;
    }

    std::string login = request.get("login");
    std::string password = request.get("password");

    int user_id = _handler->handle_authorisation(login, password);
    if (user_id == -1) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        std::ostream& out = response.send();
        out << "{\"error\": \"Authentication faliure.\"}";
        return;
    }

    Poco::JWT::Token token;
    token.setType("JWT");
    token.setSubject("1234567890");

    // set inside of token information about logged user id
    token.payload().set("uid", std::to_string(user_id));
    token.setIssuedAt(Poco::Timestamp());

    Poco::JWT::Signer signer(TOKEN_SIGNER);
    std::string jwt = signer.sign(token, Poco::JWT::Signer::ALGO_HS256);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.add("Authentication", "Bearer " + jwt);
    std::ostream& out = response.send();
    out << "{\"message\": \"Authentication ok.\"}";
}


void UndefiniedRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
    std::istream& inStream = request.stream();
    std::string requestBody(std::istreambuf_iterator<char>(inStream), {});

    response.setStatus(Poco::Net::HTTPResponse::HTTP_NO_CONTENT);
    response.setContentType("application/json");
    std::ostream& out = response.send();
    std::string responseBody = "{\"message\": \"Not supported action.\"}";
    out << responseBody;
}


RequestHandlerFactory::RequestHandlerFactory(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler)
    : _auth_handler(auth_handler), _request_handler(request_handler) {}


Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
    const std::string& uri = request.getURI();

    if (uri == "/authorisation") {
        return new AuthorisationRequestHandler(_auth_handler);
    }
    if (uri == "/request") {
        return new QuerryRequestHandler(_request_handler);
    }
    return new UndefiniedRequestHandler;
}


CommunicationServer::Impl::Impl(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler, unsigned short port) {
    Poco::Net::ServerSocket svs(port);
    Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
    server = new Poco::Net::HTTPServer(new RequestHandlerFactory(auth_handler, request_handler), svs, pParams);
}


CommunicationServer::Impl::~Impl() {
    delete server;
}


void CommunicationServer::Impl::start() {
    server->start();
}


void CommunicationServer::Impl::stop() {
    server->stop();
}


CommunicationServer::CommunicationServer(IAuthorisationHandler* auth_handler, IRequestHandler* request_handler, unsigned short port)
    : pImpl(new Impl(auth_handler, request_handler, port))
{}

CommunicationServer::~CommunicationServer() = default;

void CommunicationServer::start() {
    pImpl->start();
}

void CommunicationServer::stop() {
    pImpl->stop();
}
