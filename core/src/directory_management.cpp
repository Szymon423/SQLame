#include "directory_management.hpp"


bool check_path_exist(fs::path path, CheckFor target) {
	if (target == CheckFor::FILE) {
		return fs::exists(path) && !fs::is_directory(path);
	}
	return fs::exists(path) && fs::is_directory(path);
}

bool validate_system_directories(fs::path file_path) {
	std::ifstream json_file(file_path);

	// Sprawdź, czy plik został otwarty poprawnie
	if (!json_file.is_open()) {
		LOG_ERROR("Could not open {} with directories design.", std::string{file_path});
		return false;
	}

	// Parsuj plik JSON do obiektu nlohmann::json
	json::json json_object;
	json_file >> json_object;

	// Zamknij plik
	json_file.close();

	fs::path base_path = json_object["base"].template get<std::string>();
	iterate_through_directories(json_object["directories"], base_path);

	return true;
}

bool create_new_directory(fs::path dir_path) {
	if (fs::create_directory(dir_path)) {
		LOG_TRACE("Created {} directory.", std::string{dir_path});
		return true;
	} 
	else {
		LOG_ERROR("Could not create {} directory.", std::string{dir_path});
		return false;
	}

}

void iterate_through_directories(const json::json& j, fs::path path) {
    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
			std::string key = it.key();
			fs::path new_path = path / fs::path{key};
			if (!check_path_exist(new_path, CheckFor::DIRECTORY)) {
				if (!create_new_directory(new_path)) {
					// tu coś zrób
				}
			}
			else {
				LOG_TRACE("Directory {} allready exists.", std::string{new_path});
			}
            iterate_through_directories(it.value(), new_path);
        }
    } 
}