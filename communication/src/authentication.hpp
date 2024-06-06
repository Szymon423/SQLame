#pragma once

#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

namespace json = nlohmann;

class Permissions {
public:
    bool create_table;
    std::vector<std::string> drop_table;
    std::vector<std::string> insert_to_table;
    std::vector<std::string> select_from_table;

    /// @brief Function to allow creation json object from Permissions
    /// @return Json object
    json::json toJson() const;

    /// @brief Function to allow creation Permissions object from json
    /// @param j Refference to json object
    /// @return Permissions object
    static Permissions fromJson(const json::json& j);
};

class User {
public:
    uint32_t id;
    std::string login;
    std::string password;
    bool is_admin;
    std::optional<Permissions> permissions;

    /// @brief Function to allow creation json object from User
    /// @return Json object
    json::json toJson() const;

    /// @brief Function to allow creation User object from json
    /// @param j Refference to json object
    /// @return User object
    static User fromJson(const json::json& j);
};


std::vector<User> load_users();

/// @brief Function to validate login credentials
/// @param login user login
/// @param password user password
/// @return id of user or -1, when wrong credentials
int authenticate(const std::string& login, const std::string& password);
