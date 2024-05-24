#include <iostream>
#include "directory_management.hpp"

int main() {
    std::cout << "Hello from core" << std::endl;

    create_system_directories(fs::path{"../../../config/directories.json"});
}