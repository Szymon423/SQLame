#pragma once

#include "communication_requests_handler.hpp"

/// @brief class for handling all requests based on IRequestHandler interface
class CoreRequestHandler : public IRequestHandler {
public:
    /// @brief overriden function to handle requests
    /// @param request incoming request
    /// @return processed data
    std::string handleRequest(const std::string& request) override;
};