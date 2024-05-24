#include "directory_management.hpp"

#include <iostream>


bool check_path_exist(fs::path path, CheckFor target) {
    if (target == CheckFor::FILE) {
        return fs::exists(path) && !fs::is_directory(path);
    }
    return fs::exists(path) && fs::is_directory(path);
}

bool create_system_directories(fs::path directories_config_file_path) {
    json::json j;
    j["pi"] = 3.14159;
    j["happy"] = true;
    j["name"] = "Niels";
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    j["list"] = { 1, 0, 2 };
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };

    std::cout << j.dump(4) << std::endl;

    return true;
}