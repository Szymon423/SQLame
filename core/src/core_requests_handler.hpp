#pragma once

#include "communication_requests_handler.hpp"
#include "tokenizer.hpp"
#include "operation_generator.hpp"
#include "log.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

namespace json = nlohmann;

/// @brief class for handling all requests based on IRequestHandler interface
class CoreRequestHandler : public IRequestHandler {
public:
    /// @brief overriden function to handle requests
    /// @param request incoming request
    /// @return processed data
    std::string handleRequest(const std::string& request) override;
};

/// @brief Resoult codes for request resolving
enum class ResoultCode {
    OK,     // everything went ok
    BAD     // something wen wrong
};