#include "core_requests_handler.hpp"
#include <nlohmann/json.hpp>

std::string CoreRequestHandler::handleRequest(const std::string& request) {

    json::json j = json::json::parse(request);

    auto token = tokenize(j);

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
