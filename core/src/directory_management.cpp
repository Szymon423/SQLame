#include "directory_management.hpp"


bool check_path_exist(fs::path path, CheckFor target) {
	if (target == CheckFor::FILE) {
		return fs::exists(path) && !fs::is_directory(path);
	}
	return fs::exists(path) && fs::is_directory(path);
}

bool validate_system_directories(const fs::path& file_path) {
	std::ifstream json_file(file_path);

	if (!json_file.is_open()) {
		LOG_ERROR("Could not open {} with directories design.", file_path.string());
		return false;
	}

	json::json j;
	json_file >> j;
	json_file.close();

	iterate_through_directories(j["directories"], Configuration::base_path());

	return true;
}

void iterate_through_directories(const json::json& j, fs::path path) {
    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
			std::string key = it.key();
			fs::path new_path = path / fs::path{key};
			if (!check_path_exist(new_path, CheckFor::DIRECTORY)) {
				if (!fs::create_directory(new_path)) {
					LOG_ERROR("Could not create directory '{}", new_path.string());
				}
			}
			else {
				LOG_TRACE("Directory '{}' allready exists.", std::string{new_path});
			}
            iterate_through_directories(it.value(), new_path);
        }
    } 
}