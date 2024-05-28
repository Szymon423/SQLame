#include "operation_generator.hpp"
#include "log.hpp"


OperationException::OperationException(const std::string msg): message(msg) {

}

const char* OperationException::what() {
    return message.c_str();
}

CreateOperation::CreateOperation() {
    operation_type = OperationType::CREATE;
}


CreateTableOperation::CreateTableOperation() {
    create_target = CreateTarget::TABLE;
}

std::string CreateTableOperation::resolve() {
    std::stringstream ss;
    ss << "Create table request\n";
    ss << "  table name: '" << table.name << "'\n";
    for (auto& col: table.columns) {
        ss << "    column: '" << col.name << "'\n";
        ss << "       attributes: \n";
        for (auto& atr: col.atributes) {
            ss << "        " << ColumnAttributes_to_string(atr) << "\n";
        }
        ss << "      data type: " << DataType_to_string(col.data_type) << "\n";
    }
    return ss.str();
}


SelectOperation::SelectOperation() {
    operation_type = OperationType::SELECT;
}

std::string SelectOperation::resolve() {
    return "Select operation resoult.";
}


std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token) {
    LOG_TRACE("Generating operation.");
    // check if first token is REQUEST token
    if (token->type == TokenType::REQUEST) {
        LOG_TRACE("Token is REQUEST token.");
        // check if REQUEST token has any children
        if (token->child.has_value()) {
            std::vector<std::unique_ptr<Token>>& children = token->child.value();
            // check if it has only one child
            if (children.size() == 1) {
                std::unique_ptr<Token>& child = children.at(0);
                if (child->type == TokenType::CREATE) {
                    try {
                        return generate_create_operation(child);
                    }
                    catch (OperationException& e) {
                        LOG_ERROR("Caught an exception while generating create operation: {}", e.what());
                        throw OperationException(e.what());
                    }
                }
                if (child->type == TokenType::SELECT) {
                    // return generate_select_operation(child);
                }
                if (child->type == TokenType::INSERT) {
                    // return generate_insert_operation(child);
                }
                // ...
            }
            // for multiple querries in single request
            for (auto& child : children) {

            }
        }
    }
    return nullptr;
}


std::unique_ptr<CreateOperation> generate_create_operation(std::unique_ptr<Token>& token) {
    LOG_TRACE("Generating CREATE operation.");
    if (token->child.has_value()) {
        auto& children = token->child.value();
        if (children.size() == 1) {
            auto& child = children.at(0);
            if (child->type == TokenType::TABLE) {
                try {
                    return generate_create_table_operation(child);
                }
                catch (OperationException& e) {
                    LOG_ERROR("Caught an exception while generating create table operation: {}", e.what());
                    throw OperationException(e.what());
                }
            }
        }
        else {
            throw OperationException("Only one element can be in create statement."); 
        }
    }
    else {
        throw OperationException("Create statement is empty."); 
    }
   
    // if (token->type == TokenType::VIEW) {
    //    return generate_create_view_operation(token);
    // }
    // if (token->type == TokenType::TRIGGER) {
    //    return generate_create_trigger_operation(token);
    // }
    return nullptr;
}

std::unique_ptr<CreateTableOperation> generate_create_table_operation(std::unique_ptr<Token>& token) {
    LOG_TRACE("Generating CREATE TABLE operation.");
    std::unique_ptr<CreateTableOperation> cto = std::make_unique<CreateTableOperation>();
    if (token->child.has_value()) {
        for (auto& child: token->child.value()) {
            if (child->type == TokenType::COLUMNS) {
                try {
                    cto->table.columns = create_columns(child);
                }
                catch (OperationException& e) {
                    LOG_ERROR("Caught an exception while creating columns: {}", e.what());
                    throw OperationException(e.what());
                }
            }
            if (child->type == TokenType::NAME) {
                try {
                    cto->table.name = get_label_string(child);
                }
                catch (OperationException& e) {
                    throw OperationException(e.what()); 
                }
            }
        }
    }
    return cto;
}


std::vector<Column> create_columns(std::unique_ptr<Token>& token) {
    LOG_TRACE("Creating collumns.");
    std::vector<Column> columns;
    if (token->child.has_value()) {
        for (auto& array_element: token->child.value()) {
            if (array_element->type == TokenType::ARRAY_ELEMENT) {
                try {
                    auto col = create_column(array_element);
                    columns.push_back(std::move(col));
                }
                catch (OperationException& e) {
                    LOG_ERROR("Caught an exception while creating column: {}", e.what());
                    throw OperationException(e.what());
                }
            }
            else {
                throw OperationException("Columns definitions must be in array."); 
            }
        }
    }
    else {
        throw OperationException("No column definitions provided."); 
    }
    return std::move(columns);
}


Column create_column(std::unique_ptr<Token>& token) {
    LOG_TRACE("Creating collumn.");
    Column column;
    if (token->child.has_value()) {
        for (auto& item: token->child.value()) {
            if (item->type == TokenType::NAME) {
                try {
                    column.name = get_label_string(item);
                }
                catch (OperationException& e) {
                    throw OperationException(e.what()); 
                }
            }
            else if (item->type == TokenType::TYPE) {
                if (item->child.has_value()) {
                    auto& c = item->child.value();
                    if (c.size() == 1) {
                        column.data_type = get_data_type(c.at(0));
                        if (column.data_type == DataType::NOT_FOUND) {
                            throw OperationException("Data type in column definition is wrong."); 
                        }
                    }
                    else {
                        throw OperationException("Column definition contains multiple data type declarations."); 
                    }
                }
                else {
                    throw OperationException("Column definition is missing data type."); 
                }
            }
            else if (item->type == TokenType::ATTRIBUTES) {
                try {
                    column.atributes = get_column_attributes(item);
                }
                catch (OperationException& e) {
                    LOG_ERROR("Caught an exception while getting column attributes: {}", e.what());
                    throw OperationException(e.what());
                }
            }
            else {
                throw OperationException("Unknown element in column definition."); 
            }
        }
    }
    else {
        throw OperationException("Collumn definition with empty body."); 
    }
    return std::move(column);
}

DataType get_data_type(std::unique_ptr<Token>& token) {
    if (token->type == TokenType::TEXT) return DataType::TEXT;
    if (token->type == TokenType::INT) return DataType::INT;
    if (token->type == TokenType::DOUBLE) return DataType::DOUBLE;
    if (token->type == TokenType::TEXT) return DataType::TEXT;
    if (token->type == TokenType::BOOLEAN) return DataType::BOOLEAN;
    if (token->type == TokenType::UNIX_TIME) return DataType::UNIX_TIME;
    if (token->type == TokenType::UNIX_TIME_MS) return DataType::UNIX_TIME_MS;
    if (token->type == TokenType::BLOB) return DataType::BLOB;
    return DataType::NOT_FOUND;
}

std::vector<ColumnAttributes> get_column_attributes(std::unique_ptr<Token>& token) {
    LOG_TRACE("Getting collumn attributes.");
    std::vector<ColumnAttributes> atributes;
    if (token->child.has_value()) {
        for (auto& item: token->child.value()) {
            auto atr = get_column_attribute(item);
            if (atr == ColumnAttributes::NOT_FOUND) {
                throw OperationException("Unknonw collumn attribute."); 
            }
            else {
                atributes.push_back(std::move(atr));
            }
        }
    }
    else {
        throw OperationException("Column attributes definition with empty body."); 
    }
    return std::move(atributes);
}

ColumnAttributes get_column_attribute(std::unique_ptr<Token>& token) {
    if (token->type == TokenType::UNIQUE) return ColumnAttributes::UNIQUE;
    if (token->type == TokenType::PRIMARY_KEY) return ColumnAttributes::PRIMARY_KEY;
    if (token->type == TokenType::AUTOINCREMENT) return ColumnAttributes::AUTOINCREMENT;
    if (token->type == TokenType::NOT_NULL) return ColumnAttributes::NOT_NULL;
    return ColumnAttributes::NOT_FOUND;
}

std::string get_label_string(std::unique_ptr<Token>& token) {
    if (token->child.has_value()) {
        auto& children = token->child.value();
        if (children.size() == 1) {
            auto& child = children.at(0);
            if (child->label.has_value()) {
                return child->label.value();
            }
            else {
                throw OperationException("No label provided.");
            }
        }
        else {
            throw OperationException("Label must have only one element.");
        }
    }
    else {
        throw OperationException("Element does not have label.");
    }
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
        default: "Something went wrong";
    }
}

std::string ColumnAttributes_to_string(const ColumnAttributes& ca) {
    switch (ca) {
        case ColumnAttributes::UNIQUE: return "UNIQUE";
        case ColumnAttributes::PRIMARY_KEY: return "PRIMARY_KEY";
        case ColumnAttributes::AUTOINCREMENT: return "AUTOINCREMENT";
        case ColumnAttributes::NOT_NULL: return "NOT_NULL";
        case ColumnAttributes::NOT_FOUND: return "NOT_FOUND";
        default: "Something went wrong";
    }
}