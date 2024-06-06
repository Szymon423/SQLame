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

    // TODO fix path
    std::ifstream file();
    if (!file.is_open()) {
        // throw UtilitiesException("Unable to open file for reading: " + path.string());
        return users;
    }

    json::json j;
    file >> j;
    file.close();

    if (!j.is_array()) {
        // TODO maybe throw error here
        return users;
    }

    for (const auto& item: j) {
        users.push_back(User::fromJson(item));
    }

    return users;
}


int authenticate(const std::string& login, const std::string& password) {
    auto users = load_users();
    for (const auto& u: users) {
        if (u.login == login && u.password == password) {
            return u.id;
        }
    }
    return -1;
}
