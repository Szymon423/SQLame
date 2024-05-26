#pragma once

#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>

namespace json = nlohmann;

/// @brief enum containing all acceptable tokens
enum class TokenType {
    CREATE,
    TABLE,
    NAME,
    COLUMNS,
    TYPE,
    UNIQUE,
    PRIMARY_KEY,
    AUTOINCREMENT,
    WHAT,
    FROM,
    WHERE,
    IN,
    IS_SMALLER,
    IS_GREATER,
    IS_EQUAL,
    ORDER,
    BY,
    HOW,
    ASCENDING,
    DESCENDING,
    UNKNOWN
};

class Token {
public:
    TokenType type;
    std::optional<std::variant<std::unique_ptr<Token>, std::vector<std::unique_ptr<Token>>>> child;
};

/// @brief function creating array of tokens from request
/// @param j request body
/// @return vector of tokens
std::vector<Token> tokenize(json::json j);


/// @brief function going through each element in request
/// @param j request body
void iterate_through_request(const json::json& j)