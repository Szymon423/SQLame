#pragma once

#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

namespace json = nlohmann;

/// @brief enum containing all acceptable tokens
enum class TokenType {
    NOT_SET,
    CREATE,
    SELECT,
    INSERT,
    INTO,
    TABLE,
    NAME,
    COLUMNS,
    TYPE,
    UNIQUE,
    PRIMARY_KEY,
    AUTOINCREMENT,
    NOT_NULL,
    ATTRIBUTES,
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
    REQUEST,
    ARRAY_ELEMENT,
    NUMBER,
    BOOLEAN,
    INT,
    DOUBLE,
    TEXT,
    UNIX_TIME,
    UNIX_TIME_MS,
    BLOB,
    UNKNOWN,
    LABEL,
    DROP,
    VALUES
};

class Token {
public:
    TokenType type;
    std::optional<std::string> label;
    std::optional<double> value_double;
    std::optional<int64_t> value_int;
    std::optional<uint64_t> value_unix_time;
    std::optional<bool> value_boolean;
    std::optional<std::vector<std::unique_ptr<Token>>> children;

    /// @brief Function which ckecks if one of child type is wanted type
    /// @param type wanted type
    /// @return true if child with proper type is found, false if not found
    bool has_child(TokenType type);

    /// @brief Function which returns child with wanted type
    /// @param type wanted type
    /// @return found child or nullptr when did not found child with proper type
    std::unique_ptr<Token>* get_child(TokenType type);

    /// @brief Function which tells how many children does token has
    /// @return number of childred of token, or -1 when children are not present
    int get_children_number();
};

/// @brief function to map key to token
/// @param key keyword in language 
/// @return assigned token or TokenType::UNKNOWN
TokenType mapKeyToToken(const std::string& key);

/// @brief function creating array of tokens from request
/// @param j request body
/// @return request token
std::unique_ptr<Token> tokenize(const json::json& j);

/// @brief function going through each element in request
/// @param j request body
void iterate_through_request(const json::json& j, std::unique_ptr<Token>& t);

/// @brief function which converts token type to string
/// @param type token type
/// @return string representation of token
std::string tokenTypeToString(TokenType type);

/// @brief function to print token
/// @param t token to print
/// @param depth current depth for printing
/// @return string with token representation
std::string print_token(const Token& t, int depth = 0);