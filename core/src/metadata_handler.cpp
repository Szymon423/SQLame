#include "metadata_handler.hpp"

bool check_table_meta_exists(const std::string& table_name) {
    fs::path tables_metadata_path = Configuration::base_path() / fs::path("data/metadata/tables/" + table_name + ".meta"); 
    return check_path_exist(tables_metadata_path, CheckFor::FILE);
}

bool create_table_meta(const Table& table) {
    try {
        save_json_to_file(Configuration::base_path() / fs::path("data/metadata/tables/" + table.name + ".meta"), table.toJson());
    }
    catch (const UtilitiesException& e) {
        LOG_ERROR("Unable to open file for writing: {}.meta", table.name);
        return false;
    }
    return true;
}

bool delete_table_meta(const std::string& table_name) {
    try {
        if (fs::remove(Configuration::base_path() / fs::path("data/metadata/tables/" + table_name + ".meta"))) {
            return true;
        } 
        else {
            return false;
        }
    } 
    catch (const fs::filesystem_error& e) {
        LOG_ERROR("Could not delete file '{}', becouse of: {}", table_name, e.what());
        return false;
    }
}