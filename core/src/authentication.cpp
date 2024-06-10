#include "authentication.hpp"


json::json Permissions::toJson() const {
    json::json j;
    j["create_table"] = create_table;
    j["drop_table"] = drop_table;
    j["insert_to_table"] = insert_to_table;
    j["select_from_table"] = select_from_table;
    return j;
}


Permissions Permissions::fromJson(const json::json& j) {
    Permissions p;
    try {
        p.create_table = j.at("create_table").get<bool>();

        if (j.contains("drop_table")) {
            std::vector<std::string> items;
            for (const auto& item : j.at("drop_table")) {
                items.push_back(item.get<std::string>());
            }
            p.drop_table = items;
        }

        if (j.contains("insert_to_table")) {
            std::vector<std::string> items;
            for (const auto& item : j.at("insert_to_table")) {
                items.push_back(item.get<std::string>());
            }
            p.insert_to_table = items;
        }

        if (j.contains("select_from_table")) {
            std::vector<std::string> items;
            for (const auto& item : j.at("select_from_table")) {
                items.push_back(item.get<std::string>());
            }
            p.select_from_table = items;
        }
    } 
    catch (const json::json::exception& e) {
        // TODO maybe do something here
        (void)e;
    }
    return p;
}


json::json User::toJson() const {
    json::json j;
    j["id"] = id;
    j["login"] = login;
    j["password"] = password;
    j["is_admin"] = is_admin;
    if (permissions.has_value()) {
        j["permissions"] = permissions->toJson();
    }
    return j;
}


User User::fromJson(const json::json& j) {
    User u;
    try {
        u.id = j.at("id").get<uint32_t>();
        u.login = j.at("login").get<std::string>();
        u.password = j.at("password").get<std::string>();
        u.is_admin = j.at("is_admin").get<bool>();

        if (j.contains("permissions")) {
            u.permissions = Permissions::fromJson(j.at("permissions"));
        }
    } 
    catch (const json::json::exception& e) {
        // TODO maybe do something here
        (void)e;
    }
    return u;
}


std::vector<User> load_users() {
    std::vector<User> users;
    json::json j;
    try {
        j = load_json_from_file(Configuration::base_path() / fs::path{ "data/metadata/users/users.meta" });
    }
    catch (UtilitiesException& e) {
        LOG_ERROR("Could not load users metadata file becouse of: {}", e.what());
        return users;
    }

    if (!j.is_array()) {
        return users;
    }

    for (const auto& item: j) {
        users.push_back(User::fromJson(item));
    }

    return users;
}


int authenticate(const std::string& login, const std::string& password) {
    LOG_TRACE("Authentication handling: User '{}', Password: '{}'", login, password);
    auto users = load_users();
    for (const auto& u: users) {
        if (u.login == login && u.password == password) {
            LOG_TRACE("User authorised.");
            return u.id;
        }
    }
    LOG_TRACE("User not authorised.");
    return -1;
}