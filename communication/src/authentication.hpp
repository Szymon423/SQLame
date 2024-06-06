#pragma once

#include <string>

/// @brief Function to validate login credentials
/// @param login user login
/// @param password user password
/// @return true when login succesfull, false when failure
bool authenticate(const std::string& login, const std::string& password);