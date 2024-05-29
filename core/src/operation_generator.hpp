#pragma once

#include <vector>
#include <string>
#include "tokenizer.hpp"

class OperationException : public std::exception {
private:
    std::string message;

public:
    OperationException(const std::string msg);
    const char* what ();
};

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
    NOT_NULL,       // value can not be null
    NOT_FOUND       // not found data type
};

/// @brief enum with allowed data types in collumn
enum class DataType {
    INT,            // signed integer 64-bit
    DOUBLE,         // double 64-bit
    TEXT,           // text/string
    BOOLEAN,        // boolean - true/false
    UNIX_TIME,      // unix time value in seconds
    UNIX_TIME_MS,   // unix time value in miliseconds
    BLOB,           // bytes array
    NOT_FOUND       // not found data type
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
/// @throws OperationException: Request is fucked up.
/// @throws OperationException: Request is missing body.
/// @throws OperationException: Request must contain only one element. To be extended in future :)
/// @throws OperationException: Unsupported request operation.
std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token);

/// @brief function which converts create token to create operation class
/// @param token create token
/// @return pointer to CreateOperation object
/// @throws OperationException: Create statement is empty.
/// @throws OperationException: Only one element must be in create statement.
std::unique_ptr<CreateOperation> generate_create_operation(std::unique_ptr<Token>& token);

/// @brief function which converts create table token to create table operation class
/// @param token create table token
/// @return pointer to CreateTableOperation object
/// @throws OperationException: Create table body is missing.
std::unique_ptr<CreateTableOperation> generate_create_table_operation(std::unique_ptr<Token>& token);

/// @brief function which converts columns token to vector of column objects
/// @param token columns token
/// @return vector of columns
/// @throws OperationException: No column definitions provided.
/// @throws OperationException: Columns definitions must be in array.
std::vector<Column> create_columns(std::unique_ptr<Token>& token);

/// @brief function which converts column token to vector of column objects
/// @param token array element - column token
/// @return column
/// @throws OperationException: Collumn definition with empty body.
/// @throws OperationException: Column definition is missing data type.
/// @throws OperationException: Column definition contains multiple data type declarations.
/// @throws OperationException: Data type in column definition is wrong.
/// @throws OperationException: Unknown element in column definition.
Column create_column(std::unique_ptr<Token>& token);

/// @brief function which retrieves data type from token
/// @param token with data type
/// @return selected data type
DataType get_data_type(std::unique_ptr<Token>& token);

/// @brief function which converts atributes token to vector of column atributes
/// @param token atributes token
/// @return vector of ColumnAttributes
std::vector<ColumnAttributes> get_column_attributes(std::unique_ptr<Token>& token);

/// @brief function which retrieves ColumnAttributes from token
/// @param token with ColumnAttributes
/// @return selected ColumnAttributes
/// @throws OperationException: Column attributes definition with empty body.
/// @throws OperationException: Unknonw collumn attribute.
ColumnAttributes get_column_attribute(std::unique_ptr<Token>& token);

/// @brief function which retireives label from token which should have label child
/// @param token token which should have label child
/// @return string from label
/// @throws OperationException: Element does not have label.
/// @throws OperationException: Label must have only one element.
/// @throws OperationException: No label provided.
std::string get_label_string(std::unique_ptr<Token>& token);

/// @brief function to convert DataType to string
/// @param dt data type
/// @return string representation of DataType
std::string DataType_to_string(const DataType& dt);

/// @brief function to convert ColumnAttributes to string
/// @param ca collumn attribute
/// @return string representation of ColumnAttributes
std::string ColumnAttributes_to_string(const ColumnAttributes& ca);