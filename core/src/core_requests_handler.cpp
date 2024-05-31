#include "core_requests_handler.hpp"

std::string CoreRequestHandler::handleRequest(const std::string& request) {

    json::json j = json::json::parse(request);

    auto token = tokenize(j);

    LOG_TRACE("{}", print_token(*token));

    std::unique_ptr<Operation> operation = nullptr;
    try {
        operation = generate_operation(token);
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception: {}", e.what());
        return std::string{ "{\"message\": \"Error resolving request: " } + e.what() + "\"}";
    }

    return std::string{ "{\"message\": \"" } + operation->resolve() + "\"}";
}
