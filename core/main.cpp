#include <iostream>
#include <vector>
#include <memory>
#include "log.hpp"
#include "configuration.hpp"
#include "directory_management.hpp"
#include "core_requests_handler.hpp"
#include "communication_server.hpp"
#include "operation_generator.hpp"
#include "tokenizer.hpp"
#include "metadata_handler.hpp"

int main() {
    Logger::init();
    LOG_INFO("Started SQLame Core.");

    fs::path configuration_path = "../../config/configuration.json";
    Configuration::load_configuration(configuration_path);
    validate_system_directories(configuration_path);

    CoreRequestHandler coreHandler;
    CommunicationServer server(&coreHandler, 9443);

    server.start();
    LOG_TRACE("Started communication server.");
    std::cin.get();

    LOG_TRACE("Stopping communication server.");
    server.stop();
}