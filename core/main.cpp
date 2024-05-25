#include <iostream>
#include "log.hpp"
#include "directory_management.hpp"
#include "core_requests_handler.hpp"
#include "communication_server.hpp"

int main() {
    Logger::init();

    LOG_INFO("Started SQLame Core.");
    validate_system_directories(fs::path{"../../config/directories.json"});

    CoreRequestHandler coreHandler;
    CommunicationServer server(&coreHandler, 9443);

    server.start();
    LOG_TRACE("Started communication server.");
    std::cin.get();

    LOG_TRACE("Stopping communication server.");
    server.stop();
}