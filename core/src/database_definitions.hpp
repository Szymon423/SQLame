#pragma once

#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>
#include "log.hpp"

namespace json = nlohmann;


/// @brief enum with allowed collumn attributes
enum class ColumnAttributes {
    UNIQUE,         // value in collumn must be unique
    PRIMARY_KEY,    // value in collumn is primary key
    AUTOINCREMENT,  // value in collumn when it's primmary key is automaticly incremented
    NOT_NULL,       // value can not be null
    NOT_FOUND       // not found data type
};


/// @brief function to convert ColumnAttributes to string
/// @param ca collumn attribute
/// @return string representation of ColumnAttributes
std::string ColumnAttributes_to_string(const ColumnAttributes& ca);


/// @brief function to convert ColumnAttributes to string
/// @param ca collumn attribute with string
/// @return ColumnAttributes
ColumnAttributes ColumnAttributes_from_string(const std::string& ca);


/// @brief enum with allowed data types in collumn
enum class DataType {
    NUMBER,         // type under which can be INT, DOUBLE, UNIX_TIME, UNIX_TIME_MS
    INT,            // signed integer 64-bit
    DOUBLE,         // double 64-bit
    TEXT,           // text/string
    BOOLEAN,        // boolean - true/false
    UNIX_TIME,      // unix time value in seconds
    UNIX_TIME_MS,   // unix time value in miliseconds
    BLOB,           // bytes array
    NOT_FOUND       // not found data type
};


/// @brief function to convert DataType to string
/// @param dt data type
/// @return string representation of DataType
std::string DataType_to_string(const DataType& dt);


/// @brief function to convert string to DataType
/// @param dt data type
/// @return found DataType
DataType DataType_from_string(const std::string& dt);


/// @brief class which describes how collumn is structured
class Column {
public:
    std::optional<std::vector<ColumnAttributes>> attributes; 
    DataType data_type;
    std::string name;

    /// @brief Function to allow creation json object from Column
    /// @return Json object
    json::json toJson() const;

    /// @brief Function to allow creation Column object from json
    /// @param j Refference to json object
    /// @return Column object
    static Column fromJson(const json::json& j);
};


/// @brief class which describes how table is structured
class Table {
public:
    std::string name;
    std::vector<Column> columns;

    /// @brief Function to allow creation json object from Table
    /// @return Json object
    json::json toJson() const;

    /// @brief Function to allow creation Table object from json
    /// @param j Refference to json object
    /// @return Table object
    static Table fromJson(const json::json& j);
};


/// @brief class which contains information about single row element
class RowElement {
public:
    /// @brief when set to true value is null
    bool is_null;

    /// @brief holded data type
    DataType dt;

    /// @brief value in one of types: [bool, double, std::string, uint64_t, int64_t]
    std::variant<bool, double, std::string, uint64_t, int64_t> value;

    /// @brief Constructor creating not null row element
    /// @tparam T type of value associated with data type
    /// @param type data type
    /// @param val value to put in row element
    template<typename T>
    RowElement(DataType type, T val) 
        : is_null(false), dt(type), value(val) {}

    /// @brief Constructor creating row with null element of data type
    /// @param type data type
    RowElement(DataType type);

    /// @brief Function converting row element to bytes
    /// @return vector of bytes
    std::vector<uint8_t> to_bytes();
};


/// @brief row definition with possible data_types
using Row = std::vector<RowElement>;


/// @brief Visitor to acces each possible element in row
class VisitInsertRowItem {
private:
    std::vector<uint8_t>& byte_vector;
public:
    VisitInsertRowItem(std::vector<uint8_t>& byte_vector);
    void operator()(bool& value);
    void operator()(double& value);
    void operator()(std::string& value);
    void operator()(uint64_t& value);
    void operator()(int64_t& value);
};