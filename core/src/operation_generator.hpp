#pragma once

#include <vector>
#include <string>
#include "tokenizer.hpp"

/// @brief enum defining oprtation type
enum class OperationType {
    CREATE,     // create operation
    SELECT,     // selection operation
    INSERT      // insertion opration
};

/// @brief enum defining target of creation operation   
enum class CreateTarget {
    TABLE,      // creation of table
    TRIGGER,    // creation of trigger
    VIEW        // creation of view
};

/// @brief enum with allowed collumn attributes
enum class ColumnAttributes {
    UNIQUE,         // value in collumn must be unique
    PRIMARY_KEY,    // value in collumn is primary key
    AUTOINCREMENT,  // value in collumn when it's primmary key is automaticly incremented
    NOT_NULL        // value can not be null
};

/// @brief enum with allowed data types in collumn
enum class DataType {
    INT,            // signed integer 64-bit
    DOUBLE,         // double 64-bit
    TEXT,           // text/string
    BOLLEAN,        // boolean - true/false
    UNIX_TIME,      // unix time value in seconds
    UNIX_TIME_MS,   // unix time value in miliseconds
    BLOB            // bytes array
};

/// @brief class which describes how collumn is structured
class Column {
public:
    std::vector<ColumnAttributes> atributes; 
    DataType data_type;
    std::string name;
};

/// @brief class which describes how table is structured
class Table {
public:
    std::string name;
    std::vector<Column> columns;
};

/// @brief base class which represents SQL operation
class Operation {
public:
    OperationType operation_type;

    Operation() = default;
    ~Operation() = default;
    
    virtual std::string resolve() = 0;
};

/// @brief create operation base class
class CreateOperation : public Operation {
public:
    CreateTarget create_target;

    CreateOperation();
    ~CreateOperation() = default;
};

/// @brief create table class
class CreateTableOperation : public CreateOperation {
public:
    Table table;
    CreateTableOperation();
    ~CreateTableOperation() = default;

    std::string resolve() override;
};

/// @brief select operation class
class SelectOperation : public Operation {
public:
    SelectOperation();
    ~SelectOperation() = default;

    std::string resolve() override;
};

/// @brief function which converts tokens tree to operation classes
/// @param token root token of token tree
/// @return pointer to Operation object
std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token);

/// @brief function which converts create token to create operation class
/// @param token create token
/// @return pointer to CreateOperation object
std::unique_ptr<CreateOperation> generate_create_operation(std::unique_ptr<Token>& token);

/// @brief function which converts create table token to create table operation class
/// @param token create table token
/// @return pointer to CreateTableOperation object
std::unique_ptr<CreateTableOperation> generate_create_table_operation(std::unique_ptr<Token>& token);