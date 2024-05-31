#include "configuration.hpp"

fs::path Configuration::_base_path;
int Configuration::_server_port;

fs::path Configuration::base_path() {
    return _base_path;
}

int Configuration::server_port() {
    return _server_port;
}

void Configuration::set_base_path(const fs::path& new_path) {
    _base_path = new_path;
}

void Configuration::set_server_port(int new_port) {
    _server_port = new_port;
}

bool Configuration::load_configuration(const fs::path& path) {
    std::ifstream json_file(path);

	if (!json_file.is_open()) {
		LOG_ERROR("Could not open '{}' with configuration.", path.string());
		return false;
	}

	json::json j;
	json_file >> j;
	json_file.close();

	set_base_path(j["base_path"].get<std::string>());
	set_server_port(j["server_port"].get<int>());

	return true;
}