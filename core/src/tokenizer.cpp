#include "tokenizer.hpp"
#include <sstream>

TokenType mapKeyToToken(const std::string& key) {
    static std::unordered_map<std::string, TokenType> keyToTokenMap = {
        { "CREATE", TokenType::CREATE },
        { "TABLE", TokenType::TABLE },
        { "SELECT", TokenType::SELECT },
        { "INSERT", TokenType::INSERT },
        { "INTO", TokenType::INTO },
        { "NAME", TokenType::NAME },
        { "COLUMNS", TokenType::COLUMNS },
        { "TYPE", TokenType::TYPE },
        { "UNIQUE", TokenType::UNIQUE },
        { "PRIMARY_KEY", TokenType::PRIMARY_KEY },
        { "AUTOINCREMENT", TokenType::AUTOINCREMENT },
        { "ATTRIBUTES", TokenType::ATTRIBUTES },
        { "WHAT", TokenType::WHAT },
        { "FROM", TokenType::FROM },
        { "WHERE", TokenType::WHERE },
        { "IN", TokenType::IN },
        { "<", TokenType::IS_SMALLER },
        { ">", TokenType::IS_GREATER },
        { "=", TokenType::IS_EQUAL },
        { "ORDER", TokenType::ORDER },
        { "BY", TokenType::BY },
        { "HOW", TokenType::HOW },
        { "ASC", TokenType::ASCENDING },
        { "DESC", TokenType::DESCENDING }
    };

    auto it = keyToTokenMap.find(key);
    if (it != keyToTokenMap.end()) {
        return it->second;
    } else {
        return TokenType::LABEL;
    }
}

void iterate_through_request(const json::json& j, std::unique_ptr<Token>& t) {
    std::vector<std::unique_ptr<Token>> tokens;

    if (j.is_object()) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            std::unique_ptr<Token> token = std::make_unique<Token>();
            token->type = mapKeyToToken(it.key());
            if (token->type == TokenType::LABEL) {
                token->label = it.key();
            }

            iterate_through_request(it.value(), token);
            tokens.push_back(std::move(token));
        }
    }
    else if (j.is_array()) {
        for (const auto& item : j) {
            std::unique_ptr<Token> token = std::make_unique<Token>();
            token->type = TokenType::ARRAY_ELEMENT;
            iterate_through_request(item, token);
            tokens.push_back(std::move(token));
        }
    } 
    else {
        std::unique_ptr<Token> token = std::make_unique<Token>();

        if (j.is_string()) {
            token->type = mapKeyToToken(j.get<std::string>());
            if (token->type == TokenType::LABEL) {
                token->label = j.get<std::string>();
            }
        } 
        else if (j.is_boolean()) {
            token->type = TokenType::VALUE_BOOLEAN;
            token->value_boolean = j.get<bool>();
        } 
        else if (j.is_number()) {
            token->type = TokenType::VALUE_NUMBER;
            token->value_number = j.get<double>();
        } 
        else {
            token->type = TokenType::UNKNOWN;
        }
        tokens.push_back(std::move(token));
    }

    t->child = std::move(tokens);
}

std::unique_ptr<Token> tokenize(const json::json& j) {
    std::unique_ptr<Token> token = std::make_unique<Token>();
    token->type = TokenType::REQUEST;
    iterate_through_request(j, token);

    return token;
}

std::string tokenTypeToString(TokenType type) {
    static std::unordered_map<TokenType, std::string> tokenTypeToStringMap = {
        { TokenType::REQUEST, "REQUEST" },
        { TokenType::CREATE, "CREATE" },
        { TokenType::TABLE, "TABLE" },
        { TokenType::SELECT, "SELECT" },
        { TokenType::INSERT, "INSERT" },
        { TokenType::INTO, "INTO" },
        { TokenType::NAME, "NAME" },
        { TokenType::COLUMNS, "COLUMNS" },
        { TokenType::TYPE, "TYPE" },
        { TokenType::UNIQUE, "UNIQUE" },
        { TokenType::PRIMARY_KEY, "PRIMARY_KEY" },
        { TokenType::AUTOINCREMENT, "AUTOINCREMENT" },
        { TokenType::ATTRIBUTES, "ATTRIBUTES" },
        { TokenType::WHAT, "WHAT" },
        { TokenType::FROM, "FROM" },
        { TokenType::WHERE, "WHERE" },
        { TokenType::IN, "IN" },
        { TokenType::IS_SMALLER, "IS_SMALLER" },
        { TokenType::IS_GREATER, "IS_GREATER" },
        { TokenType::IS_EQUAL, "IS_EQUAL" },
        { TokenType::ORDER, "ORDER" },
        { TokenType::BY, "BY" },
        { TokenType::HOW, "HOW" },
        { TokenType::ASCENDING, "ASCENDING" },
        { TokenType::DESCENDING, "DESCENDING" },
        { TokenType::LABEL, "LABEL" },
        { TokenType::ARRAY_ELEMENT, "ARRAY_ELEMENT" },
        { TokenType::VALUE_NUMBER, "VALUE_NUMBER" },
        { TokenType::VALUE_BOOLEAN, "VALUE_BOOLEAN" },
        { TokenType::UNKNOWN, "UNKNOWN" }
    };

    auto it = tokenTypeToStringMap.find(type);
    if (it != tokenTypeToStringMap.end()) {
        return it->second;
    } else {
        return "UNKNOWN";
    }
}

std::string print_token(const Token& t, int depth) {
    std::ostringstream oss;
    std::string indent(depth * 2, ' ');
    oss << indent << tokenTypeToString(t.type);

    if (t.label) {
        oss << " '" << *t.label << "'";
    }
    if (t.value_number) {
        oss << " " << *t.value_number;
    }
    if (t.value_boolean) {
        oss << " " << (*t.value_boolean ? "true" : "false");
    }

    if (t.child) {
        if (auto* children = std::get_if<std::vector<std::unique_ptr<Token>>>(&*t.child)) {
            for (const auto& child : *children) {
                oss << "\n" << print_token(*child, depth + 1);
            }
        } else if (auto* singleChild = std::get_if<std::unique_ptr<Token>>(&*t.child)) {
            oss << "\n" << print_token(**singleChild, depth + 1);
        }
    }

    return oss.str();
}
