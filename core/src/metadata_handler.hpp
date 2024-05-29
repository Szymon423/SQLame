#pragma once

#include <filesystem>
#include <string>
#include "directory_management.hpp"
#include "configuration.hpp"

namespace fs = std::filesystem;

/// @brief function which checks if table exists in metadata
/// @param table_name name of desired table
/// @return true if table exists, false if table does not exist
bool check_table_exists(const std::string& table_name);

