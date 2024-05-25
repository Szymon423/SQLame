#pragma once

#include "communication_requests_handler.hpp"

class CoreRequestHandler : public IRequestHandler {
public:
    std::string handleRequest(const std::string& request) override;
};