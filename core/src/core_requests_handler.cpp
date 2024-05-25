#include "core_requests_handler.hpp"

std::string CoreRequestHandler::handleRequest(const std::string& request) {
    std::string response = "{\"message\": \"Request processed successfully\"}";
    return response;
}
