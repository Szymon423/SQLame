#include "tokenizer.hpp"


void iterate_through_request(const json::json& j) {
    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
			std::string key = it.key();
			fs::path new_path = path / fs::path{key};
			if (!check_path_exist(new_path, CheckFor::DIRECTORY)) {
				if (!create_new_directory(new_path)) {
					// tu coś zrób
				}
			}
			else {
				LOG_TRACE("Directory {} allready exists.", std::string{new_path});
			}
            iterate_through_directories(it.value(), new_path);
        }
    } 
    else if (j.is_array()) {
        
    }
}


std::vector<Token> tokenize(json::json j) {

}

