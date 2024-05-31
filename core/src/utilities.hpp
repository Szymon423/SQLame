#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "log.hpp"

namespace json = nlohmann;
namespace fs = std::filesystem;

class UtilitiesException : public std::exception {
private:
    std::string message;

public:
    UtilitiesException(const std::string msg);
    // const char* what ();
    const std::string what ();
};

/// @brief Function which saves JSON data to file
/// @param path Path under which JSON object will be saved
/// @param j JSON object
/// @throws UtilitiesException Unable to open file for writing
void save_json_to_file(const fs::path& path, const json::json& j);

/// @brief Function which loads JSON data from file
/// @param path Path from which JSON object will be loaded
/// @return JSON object
/// @throws UtilitiesException Unable to open file for reading
json::json load_json_from_file(const fs::path& path);