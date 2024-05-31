#include "tokenizer.hpp"
#include <sstream>


bool Token::has_child(TokenType type) {
    if (!children.has_value()) {
        return false;
    }

    for (auto& item: children.value()) {
        if (item->type == type) {
            return true;
        }
    }

    return false;
}


std::unique_ptr<Token>* Token::get_child(TokenType type) {
    if (!children.has_value()) {
        return nullptr;
    }

    for (auto& item: children.value()) {
        if (item->type == type) {
            return &item;
        }
    }

    return nullptr;
}


int Token::get_children_number() {
    if (!children.has_value()) {
        return -1;
    }

    return children.value().size();
}


TokenType mapKeyToToken(const std::string& key) {
    static std::unordered_map<std::string, TokenType> keyToTokenMap = {
        { "NOT_SET", TokenType::NOT_SET },
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
        { "NOT_NULL", TokenType::NOT_NULL },
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
        { "DESC", TokenType::DESCENDING },
        { "TEXT", TokenType::TEXT },
        { "INT", TokenType::INT },
        { "BOOLEAN", TokenType::BOOLEAN },
        { "DOUBLE", TokenType::DOUBLE },
        { "UNIX_TIME", TokenType::UNIX_TIME },
        { "UNIX_TIME_MS", TokenType::UNIX_TIME_MS },
        { "BLOB", TokenType::BLOB },
        { "DROP", TokenType::DROP },
        { "VALUES", TokenType::VALUES }
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

        if (t->type == TokenType::ARRAY_ELEMENT) {
            if (j.is_string()) {
                t->type = mapKeyToToken(j.get<std::string>());
                if (t->type == TokenType::LABEL) {
                    t->label = j.get<std::string>();
                }
            } 
            else if (j.is_boolean()) {
                t->type = TokenType::VALUE_BOOLEAN;
                t->value_boolean = j.get<bool>();
            } 
            else if (j.is_number()) {
                t->type = TokenType::VALUE_NUMBER;
                t->value_number = j.get<double>();
            } 
            else {
                t->type = TokenType::UNKNOWN;
            }
            return;
        }

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

    t->children = std::move(tokens);
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
        { TokenType::NOT_SET, "NOT_SET" },
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
        { TokenType::NOT_NULL, "NOT_NULL" },
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
        { TokenType::UNKNOWN, "UNKNOWN" },
        { TokenType::TEXT, "TEXT" },
        { TokenType::INT, "INT" },
        { TokenType::BOOLEAN, "BOOLEAN" },
        { TokenType::DOUBLE, "DOUBLE" },
        { TokenType::UNIX_TIME, "UNIX_TIME" },
        { TokenType::UNIX_TIME_MS, "UNIX_TIME_MS" },
        { TokenType::BLOB, "BLOB" },
        { TokenType::DROP, "DROP" },
        { TokenType::VALUES, "VALUES" }
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

    if (t.children.has_value()) {
        for (const auto& child : t.children.value()) {
            oss << "\n" << print_token(*child, depth + 1);
        }
    }

    return oss.str();
}