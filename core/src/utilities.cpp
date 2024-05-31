#include "utilities.hpp"


UtilitiesException::UtilitiesException(const std::string msg): message(msg) {

}


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


void save_to_file(const std::vector<uint8_t>& byte_vector, const fs::path& path) {
    std::ofstream output_file(path, std::ios::binary);
    if (!output_file) {
        throw UtilitiesException("Unable to open file for writing: " + path.string());
    }

    output_file.write(reinterpret_cast<const char*>(byte_vector.data()), byte_vector.size());
    if (!output_file) {
        throw UtilitiesException("Unable to write to file: " + path.string());
    }

    output_file.close();
}