#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
namespace json = nlohmann;

/// @brief enum defining if path is checked for file od directory
enum class CheckFor {
    FILE,
    DIRECTORY
};

/// @brief function to check if path exists
/// @param path target path 
/// @param target FILE | DIRECTORY
/// @return true if exist, false if does not exist
bool check_path_exist(fs::path path, CheckFor target = CheckFor::FILE);

/// @brief function which load's system configuration file from path and based on it create directories
/// @param directories_config_file_path path for JSON with configuration
/// @return true if all went ok, false if somethong went wrong
bool create_system_directories(fs::path directories_config_file_path);