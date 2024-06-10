#include "core_requests_handler.hpp"
#include <nlohmann/json.hpp>
#include "authentication.hpp"


int CoreAuthorisationHandler::handle_authorisation(const std::string& login, const std::string& password) {
    return authenticate(login, password);
}


std::string CoreRequestHandler::handleRequest(const std::string& request, const int& userId) {

    json::json j = json::json::parse(request);

    auto token = tokenize(j);
    LOG_TRACE("User with ID: {} querried:", userId);
    LOG_TRACE("{}", print_token(*token));

    std::unique_ptr<Operation> operation = nullptr;
    ResoultCode rc = ResoultCode::OK;
    std::string message;
    try {
        operation = generate_operation(token);
        message = operation->resolve();
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception: {}", e.what());
        rc = ResoultCode::BAD;
        message = "Error resolving request: " + e.what();
    }

    json::json response;
    response["code"] = (int)rc;
    response["message"] = message;

    return response.dump();
}
