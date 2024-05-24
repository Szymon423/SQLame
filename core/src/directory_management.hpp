#pragma once

#include <filesystem>
#include <iostream>
#include <fstream> 
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
/// @param file_path path for JSON with configuration
/// @return true if all went ok, false if somethong went wrong
bool create_system_directories(fs::path file_path);

/// @brief function to create directory 
/// @param dir_path path to directory
/// @return true if succes, false if failure
bool create_new_directory(fs::path dir_path);

/// @brief function to go through entire json and create directories based on json
/// @param j json file
/// @param path base path on which directories system will be generated
void iterate_through_directories(const nlohmann::json& j, fs::path path);