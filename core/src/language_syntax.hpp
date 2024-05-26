#pragma once

#include <string>

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
    DESCENDING
};

enum class OperationType {
    CREATE,
    SELECT,
    INSERT
};


class Operation {
public:
    OperationType type;

    Operation() = default;
    ~Operation() = default;
    
    virtual std::string resolve() = 0;
};


class CreateOperation : public Operation {
public:
    CreateOperation();
    ~CreateOperation() = default;

    std::string resolve() override;
};


class SelectOperation : public Operation {
public:
    SelectOperation();
    ~SelectOperation() = default;

    std::string resolve() override;
};
