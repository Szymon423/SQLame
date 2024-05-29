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
    static const fs::path base_path();
    static const int server_port();
    static bool load_configuration(const fs::path& path);

private:
    static void set_base_path(const fs::path& new_path);
    static void set_server_port(int new_port);
    static fs::path _base_path;
    static int _server_port;
};

