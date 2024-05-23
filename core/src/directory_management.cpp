#include "directory_management.hpp"


bool check_path_exist(fs::path path, CheckFor target = CheckFor::FILE) {
    if (target == CheckFor::FILE) {
        return fs::exists(path) && !fs::is_directory(path);
    }
    return fs::exists(path) && fs::is_directory(path);
}
