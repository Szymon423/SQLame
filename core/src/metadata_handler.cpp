#include "metadata_handler.hpp"

bool check_table_exists(const std::string& table_name) {
    fs::path tables_metadata_path = Configuration::base_path() / fs::path("data/metadata/tables/" + table_name + ".meta"); 
    return check_path_exist(tables_metadata_path, CheckFor::FILE);
}
