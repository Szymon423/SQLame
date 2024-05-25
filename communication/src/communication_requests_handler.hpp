#pragma once

#include <string>

class IRequestHandler {
public:
    virtual ~IRequestHandler() = default;
    virtual std::string handleRequest(const std::string& request) = 0;
};