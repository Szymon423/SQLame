#pragma once

#include <vector>
#include <string>

enum class OperationType {
    CREATE,
    SELECT,
    INSERT
};

enum class CreateTarget {
    TABLE,
    TRIGGER,
    VIEW
};

enum class ColumnAttributes {
    UNIQUE,
    PRIMARY_KEY,
    AUTOINCREMENT
};


enum class DataType {
    INT,
    DOUBLE,
    TEXT,
    UNIX_TIME,
    UNIX_TIME_MS,
    BLOB
};


class Column {
public:
    std::vector<ColumnAttributes> atributes; 
    DataType data_type;
    std::string name;
};


class Table {
public:
    std::string name;
    std::vector<Column> columns;
};


class Operation {
public:
    OperationType operation_type;

    Operation() = default;
    ~Operation() = default;
    
    virtual std::string resolve() = 0;
};


class CreateOperation : public Operation {
public:
    CreateTarget create_target;

    CreateOperation();
    ~CreateOperation() = default;
};


class CreateTableOperation : public CreateOperation {
public:
    Table table;
    CreateTableOperation();
    ~CreateTableOperation() = default;

    std::string resolve() override;
};


class SelectOperation : public Operation {
public:
    SelectOperation();
    ~SelectOperation() = default;

    std::string resolve() override;
};
