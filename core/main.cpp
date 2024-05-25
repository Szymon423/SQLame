#include <iostream>
#include "directory_management.hpp"
#include "log.hpp"

int main() {
    Logger::init();

    LOG_INFO("Started SQLame Core.");
    validate_system_directories(fs::path{"../../config/directories.json"});
}