#pragma once

#include <filesystem>

namespace fs = std::filesystem;

enum class CheckFor {
    FILE,
    DIRECTORY
};


/// @brief function to check if path exists
/// @param path target path 
/// @param target FILE | DIRECTORY
/// @return true if exist, false if does not exist
bool check_path_exist(fs::path path, CheckFor target = CheckFor::FILE);