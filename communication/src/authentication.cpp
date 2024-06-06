#include "authentication.hpp"

bool authenticate(const std::string& login, const std::string& password) {
    return login == "admin" && password == "admin";
}