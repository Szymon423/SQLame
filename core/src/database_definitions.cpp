#include "database_definitions.hpp"


std::string ColumnAttributes_to_string(const ColumnAttributes& ca) {
    switch (ca) {
        case ColumnAttributes::UNIQUE: return "UNIQUE";
        case ColumnAttributes::PRIMARY_KEY: return "PRIMARY_KEY";
        case ColumnAttributes::AUTOINCREMENT: return "AUTOINCREMENT";
        case ColumnAttributes::NOT_NULL: return "NOT_NULL";
        case ColumnAttributes::NOT_FOUND: return "NOT_FOUND";
        default: return "Something went wrong";
    }
}


ColumnAttributes ColumnAttributes_from_string(const std::string& ca) {
    if (ca == "UNIQUE") return ColumnAttributes::UNIQUE;
    if (ca == "PRIMARY_KEY") return ColumnAttributes::PRIMARY_KEY;
    if (ca == "AUTOINCREMENT") return ColumnAttributes::AUTOINCREMENT;
    if (ca == "NOT_NULL") return ColumnAttributes::NOT_NULL;
    return ColumnAttributes::NOT_FOUND;
}


std::string DataType_to_string(const DataType& dt) {
    switch (dt) {
        case DataType::INT: return "INT";
        case DataType::DOUBLE: return "DOUBLE";
        case DataType::TEXT: return "TEXT";
        case DataType::BOOLEAN: return "BOOLEAN";
        case DataType::UNIX_TIME: return "UNIX_TIME";
        case DataType::UNIX_TIME_MS: return "UNIX_TIME_MS";
        case DataType::BLOB: return "BLOB";
        case DataType::NOT_FOUND: return "NOT_FOUND";
        default: return "Something went wrong";
    }
}


DataType DataType_from_string(const std::string& dt) {
    if (dt == "INT") return DataType::INT;
    if (dt == "DOUBLE") return DataType::DOUBLE;
    if (dt == "TEXT") return DataType::TEXT;
    if (dt == "BOOLEAN") return DataType::BOOLEAN;
    if (dt == "UNIX_TIME") return DataType::UNIX_TIME;
    if (dt == "UNIX_TIME_MS") return DataType::UNIX_TIME_MS;
    if (dt == "BLOB") return DataType::BLOB;
    return DataType::NOT_FOUND;
}


json::json Column::toJson() const {
    json::json j;
    j["name"] = name;
    j["data_type"] = DataType_to_string(data_type);
    if (attributes.has_value()) {
        std::vector<std::string> attrs;
        for (const auto& attr : attributes.value()) {
            attrs.push_back(ColumnAttributes_to_string(attr));
        }
        j["attributes"] = attrs;
    }
    return j;
}


Column Column::fromJson(const json::json& j) {
    Column c;
    try {
        c.name = j.at("name").get<std::string>();
        c.data_type = DataType_from_string(j.at("data_type").get<std::string>());
        
        if (j.contains("attributes")) {
            std::vector<ColumnAttributes> attrs;
            for (const auto& item : j.at("attributes")) {
                attrs.push_back(ColumnAttributes_from_string(item.get<std::string>()));
            }
            c.attributes = attrs;
        }
    } 
    catch (const json::json::exception& e) {
        LOG_ERROR("Error parsing JSON to Column: {}", e.what());
    }
    return c;
}


json::json Table::toJson() const {
    json::json j;
    j["name"] = name;
    j["columns"] = json::json::array();
    for (const auto& column : columns) {
        j["columns"].push_back(column.toJson());
    }
    return j;
}


Table Table::fromJson(const json::json& j) {
    Table t;
    try {
        t.name = j.at("name").get<std::string>();
        
        if (j.contains("columns")) {
            for (const auto& item : j.at("columns")) {
                t.columns.push_back(Column::fromJson(item));
            }
        }
    } 
    catch (const json::json::exception& e) {
        LOG_ERROR("Error parsing JSON to Table: {}", e.what());
    }
    return t;
}


RowElement::RowElement(DataType type) : is_null(true), dt(type) {}


std::vector<uint8_t> RowElement::to_bytes() {
    // bytes for any type are created as shown below
    // > when value is null: bytes = [0x01]
    // > when value is not null: bytes = [0x00, <rest of bytes from actuall value>]
    std::vector<uint8_t> bytes;
    if (is_null) {
        bytes.push_back(0x01);
    }
    else {
        bytes.push_back(0x00);
        std::visit(VisitInsertRowItem(bytes), value);
    }
    return bytes;
}


VisitInsertRowItem::VisitInsertRowItem(std::vector<uint8_t>& byte_vector) : byte_vector(byte_vector) {}


void VisitInsertRowItem::operator()(bool& value) {
    byte_vector.push_back(static_cast<uint8_t>(value));
    // TODO remove log
    LOG_TRACE(" > bool value: {}", value);
}


void VisitInsertRowItem::operator()(double& value) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    byte_vector.insert(byte_vector.end(), ptr, ptr + sizeof(double));
    // TODO remove log
    LOG_TRACE(" > double value: {}", value);
}


void VisitInsertRowItem::operator()(std::string& value) {
    byte_vector.insert(byte_vector.end(), value.begin(), value.end());
    byte_vector.push_back('\0');
    // TODO remove log
    LOG_TRACE(" > std::string value: {}", value);
}


void VisitInsertRowItem::operator()(uint64_t& value) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    byte_vector.insert(byte_vector.end(), ptr, ptr + sizeof(uint64_t));
    // TODO remove log
    LOG_TRACE(" > uint64_t value: {}", value);
}


void VisitInsertRowItem::operator()(int64_t& value) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    byte_vector.insert(byte_vector.end(), ptr, ptr + sizeof(int64_t));
    // TODO remove log
    LOG_TRACE(" > int64_t value: {}", value);
}