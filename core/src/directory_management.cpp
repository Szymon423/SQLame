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
    
    std::cout << json_object.dump(4) << std::endl;

    return true;
}