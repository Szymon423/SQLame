#include "utilities.hpp"

UtilitiesException::UtilitiesException(const std::string msg): message(msg) {

}

// const char* UtilitiesException::what() {
//     return message.c_str();
// }
const std::string UtilitiesException::what() {
    return message;
}

void save_json_to_file(const fs::path& path, const json::json& j) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw UtilitiesException("Unable to open file for writing: " + path.string());
    }

    file << j.dump(4);
    file.close();
}

json::json load_json_from_file(const fs::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw UtilitiesException("Unable to open file for reading: " + path.string());
    }

    json::json j;
    file >> j;
    file.close();
    return j;
}