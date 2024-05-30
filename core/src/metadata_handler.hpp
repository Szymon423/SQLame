#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "utilities.hpp"
#include "configuration.hpp"
#include "directory_management.hpp"
#include "database_definitions.hpp"

namespace fs = std::filesystem;
namespace json = nlohmann;

/// @brief function which checks if table exists in metadata
/// @param table_name name of desired table
/// @return true if table exists, false if table does not exist
bool check_table_meta_exists(const std::string& table_name);

/// @brief Function which creates table metadata
/// @param table Table object
/// @return true if created succesfully, false if something went wrong
bool create_table_meta(const Table& table);

