#include <iostream>
#include <vector>
#include <memory>
#include "log.hpp"
#include "directory_management.hpp"
#include "core_requests_handler.hpp"
#include "communication_server.hpp"
#include "language_syntax.hpp"

int main() {
    Logger::init();

    LOG_INFO("Started SQLame Core.");
    validate_system_directories(fs::path{"../../config/directories.json"});

    CoreRequestHandler coreHandler;
    CommunicationServer server(&coreHandler, 9443);

    std::vector<std::unique_ptr<Operation>> operations;
    operations.emplace_back(std::make_unique<SelectOperation>());
    operations.emplace_back(std::make_unique<CreateOperation>());

    for (auto& operation : operations) {
        LOG_TRACE("{}", operation->resolve());
    }

    server.start();
    LOG_TRACE("Started communication server.");
    std::cin.get();

    LOG_TRACE("Stopping communication server.");
    server.stop();
}