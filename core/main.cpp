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

    // load test request
    fs::path file_path = "../../test/request.json";
    std::ifstream json_file(file_path);

	if (!json_file.is_open()) {
		LOG_ERROR("Could not open {}.", std::string{file_path});
		return 1;
	}

	json::json j;
	json_file >> j;
	json_file.close();

    auto resoult = tokenize(j);

    std::unique_ptr<Operation> operation = nullptr;
    try {
        operation = generate_operation(resoult);
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception: {}", e.what());
        return 1;
    }

    // std::cout << print_token(*resoult) << std::endl;

    std::cout << operation->resolve() << std::endl;


    server.start();
    LOG_TRACE("Started communication server.");
    std::cin.get();

    LOG_TRACE("Stopping communication server.");
    server.stop();
}