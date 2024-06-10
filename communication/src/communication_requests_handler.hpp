#pragma once

#include <string>


/// @brief interface for hnadling authorisation
class IAuthorisationHandler {
public:
    /// @brief interface for logging function
    /// @param login user login
    /// @param password user password
    /// @return id of logged user or -1 when authorisation failed
    virtual int handle_authorisation(const std::string& login, const std::string& password) = 0;
};


/// @brief interface which will be used to create RequestsHandler class
class IRequestHandler {
public:
    /// @brief set default destructor
    virtual ~IRequestHandler() = default;

    /// @brief function interface which has to be overwritten
    /// @param request request string recieved by server
    /// @return processed data
    virtual std::string handleRequest(const std::string& request, const int& uID) = 0;
};