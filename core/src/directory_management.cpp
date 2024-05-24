#include "directory_management.hpp"


bool check_path_exist(fs::path path, CheckFor target) {
	if (target == CheckFor::FILE) {
		return fs::exists(path) && !fs::is_directory(path);
	}
	return fs::exists(path) && fs::is_directory(path);
}

bool create_system_directories(fs::path file_path) {

	std::ifstream json_file(file_path);

	// Sprawdź, czy plik został otwarty poprawnie
	if (!json_file.is_open()) {
		std::cerr << "Nie można otworzyć pliku: " << file_path << std::endl;
		return 1;
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
		std::cout << "Folder: " << dir_path << " został pomyślnie utworzony" << std::endl;
		return true;
	} 
	else {
		std::cerr << "Nie udało się utworzyć folderu: " << dir_path << std::endl;
		return false;
	}

}

void iterate_through_directories(const json::json& j, fs::path path) {
    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
			std::string key = it.key();
			fs::path new_path = path / fs::path{key};
        	std::cout << new_path << std::endl;
			if (!check_path_exist(new_path, CheckFor::DIRECTORY)) {
				if (!create_new_directory(new_path)) {
					std::cout << "Nie udało się utworzyć folderu.";
				}
			}
            iterate_through_directories(it.value(), new_path);
        }
    } 
	else {
        std::cout << path << std::endl;
    }
}