#pragma once

#include <string>


/// @brief interface which will be used to create RequestsHandler class
class IRequestHandler {
public:
    /// @brief set default destructor
    virtual ~IRequestHandler() = default;

    /// @brief function interface which has to be overwritten
    /// @param request request string recieved by server
    /// @return processed data
    virtual std::string handleRequest(const std::string& request) = 0;
};