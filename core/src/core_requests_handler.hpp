#pragma once

#include "communication_requests_handler.hpp"
#include "operation_generator.hpp"
#include "tokenizer.hpp"
#include "log.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>


namespace json = nlohmann;


/// @brief core class for performing authorisation
class CoreAuthorisationHandler : public IAuthorisationHandler {
    /// @brief core authorisation function for logging
    /// @param login user login
    /// @param password user password
    /// @return id of logged user or -1 when authorisation failed
    int handle_authorisation(const std::string& login, const std::string& password) override;
};


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