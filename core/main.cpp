#include <iostream>
#include <vector>
#include <memory>
#include "log.hpp"
#include "directory_management.hpp"
#include "core_requests_handler.hpp"
#include "communication_server.hpp"
#include "language_syntax.hpp"
#include "tokenizer.hpp"

int main() {
    Logger::init();

    LOG_INFO("Started SQLame Core.");
    validate_system_directories(fs::path{"../../config/directories.json"});

    CoreRequestHandler coreHandler;
    CommunicationServer server(&coreHandler, 9443);

    std::vector<std::unique_ptr<Operation>> operations;
    operations.emplace_back(std::make_unique<SelectOperation>());
    operations.emplace_back(std::make_unique<CreateTableOperation>());

    for (auto& operation : operations) {
        LOG_TRACE("{}", operation->resolve());
    }

    fs::path file_path = "../../test/request.json";
    std::ifstream json_file(file_path);

	// Sprawdź, czy plik został otwarty poprawnie
	if (!json_file.is_open()) {
		LOG_ERROR("Could not open {}.", std::string{file_path});
		return 1;
	}

	// Parsuj plik JSON do obiektu nlohmann::json
	json::json j;
	json_file >> j;

	// Zamknij plik
	json_file.close();

    auto resoult = tokenize(j);

    std::cout << print_token(*resoult) << std::endl;

    // server.start();
    // LOG_TRACE("Started communication server.");
    // std::cin.get();

    // LOG_TRACE("Stopping communication server.");
    // server.stop();
}