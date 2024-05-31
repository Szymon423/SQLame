#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include "log.hpp"
#include <fstream> 

namespace fs = std::filesystem;
namespace json = nlohmann;

/// @brief class containing definition of project configuration
class Configuration {
public:
    
    /// @brief Function which contains base path
    /// @return Base path
    static fs::path base_path();
    
    /// @brief Function which contains server port
    /// @return Server port
    static int server_port();

    /// @brief Function which loads configuration from configuration.json
    /// @param path path to configuration.json
    /// @return true if configuration was loaded succesfully, false if something went wrong
    static bool load_configuration(const fs::path& path);

private:
    static void set_base_path(const fs::path& new_path);
    static void set_server_port(int new_port);
    static fs::path _base_path;
    static int _server_port;
};

