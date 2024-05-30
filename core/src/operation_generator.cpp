#include "operation_generator.hpp"


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
    if (check_table_meta_exists(table.name)) {
        LOG_TRACE("Table '{}' allready exists.", table.name);
        return "Table allready exists.";
    }
    
    if (create_table_meta(table)) {
        LOG_TRACE("Table '{}' created succesfully.", table.name);
        return "Table " + table.name + " created succesfully";
    }
    
    LOG_TRACE("Could not create table {}.", table.name);
    return "Could not create table " + table.name;
}


SelectOperation::SelectOperation() {
    operation_type = OperationType::SELECT;
}

std::string SelectOperation::resolve() {
    return "Select operation resoult.";
}


std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token) {
    if (token->type != TokenType::REQUEST) {
        throw OperationException("Request is fucked up."); 
    }

    if (!token->child.has_value()) {
        throw OperationException("Request is missing body."); 
    }

    auto& children = token->child.value();
    if (children.size() != 1) {
        // TODO extend in fiture to handle multiple querries in single request
        throw OperationException("Request must contain only one element. To be extended in future :)");
    }

    auto& child = children.at(0);
    if (child->type == TokenType::CREATE) {
        try {
            return generate_create_operation(child);
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while generating create operation: {}", e.what());
            throw OperationException(e.what());
        }
    }
    else if (child->type == TokenType::SELECT) {
        // TODO create generate_select_operation
        // return generate_select_operation(child);
    }
    else if (child->type == TokenType::INSERT) {
        // TODO create generate_insert_operation
        // return generate_insert_operation(child);
    }
    else {
        throw OperationException("Unsupported request operation.");
    }
    return nullptr;
}


std::unique_ptr<CreateOperation> generate_create_operation(std::unique_ptr<Token>& token) {
    if (!token->child.has_value()) {
        throw OperationException("Create statement is empty."); 
    }

    auto& children = token->child.value();
    if (children.size() != 1) {
        throw OperationException("Only one element must be in create statement."); 
    }

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
    
    // if (child->type == TokenType::VIEW) {
    //    return generate_create_view_operation(child);
    // }
    // if (child->type == TokenType::TRIGGER) {
    //    return generate_create_trigger_operation(child);
    // }
    return nullptr;
}

std::unique_ptr<CreateTableOperation> generate_create_table_operation(std::unique_ptr<Token>& token) {
    if (!token->child.has_value()) {
        throw OperationException("Create table body is missing."); 
    }
    
    std::unique_ptr<CreateTableOperation> cto = std::make_unique<CreateTableOperation>();
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
        else if (child->type == TokenType::NAME) {
            try {
                cto->table.name = get_label_string(child);
            }
            catch (OperationException& e) {
                LOG_ERROR("Caught an exception while creating table name: {}", e.what());
                throw OperationException(e.what()); 
            }
        }
    }
    return cto;
}


std::vector<Column> create_columns(std::unique_ptr<Token>& token) {
    if (!token->child.has_value()) {
        throw OperationException("No column definitions provided."); 
    }

    std::vector<Column> columns;
    for (auto& array_element: token->child.value()) {
        if (array_element->type != TokenType::ARRAY_ELEMENT) {
            throw OperationException("Columns definitions must be in array."); 
        }
        try {
            auto col = create_column(array_element);
            columns.push_back(std::move(col));
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while creating column: {}", e.what());
            throw OperationException(e.what());
        }
    }
    return std::move(columns);
}


Column create_column(std::unique_ptr<Token>& token) {
    if (!token->child.has_value()) {
        throw OperationException("Collumn definition with empty body."); 
    }

    Column column;
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
            if (!item->child.has_value()) {
                throw OperationException("Column definition is missing data type."); 
            }

            auto& c = item->child.value();
            if (c.size() != 1) {
                throw OperationException("Column definition contains multiple data type declarations."); 
            }

            column.data_type = get_data_type(c.at(0));
            if (column.data_type == DataType::NOT_FOUND) {
                throw OperationException("Data type in column definition is wrong."); 
            }
        }
        else if (item->type == TokenType::ATTRIBUTES) {
            try {
                column.attributes = get_column_attributes(item);
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
    if (!token->child.has_value()) {
        throw OperationException("Column attributes definition with empty body."); 
    }

    std::vector<ColumnAttributes> attributes;
    for (auto& item: token->child.value()) {
        auto atr = get_column_attribute(item);
        if (atr == ColumnAttributes::NOT_FOUND) {
            throw OperationException("Unknonw collumn attribute."); 
        }
        attributes.push_back(std::move(atr));
    }
    return std::move(attributes);
}

ColumnAttributes get_column_attribute(std::unique_ptr<Token>& token) {
    if (token->type == TokenType::UNIQUE) return ColumnAttributes::UNIQUE;
    if (token->type == TokenType::PRIMARY_KEY) return ColumnAttributes::PRIMARY_KEY;
    if (token->type == TokenType::AUTOINCREMENT) return ColumnAttributes::AUTOINCREMENT;
    if (token->type == TokenType::NOT_NULL) return ColumnAttributes::NOT_NULL;
    return ColumnAttributes::NOT_FOUND;
}

std::string get_label_string(std::unique_ptr<Token>& token) {
    if (!token->child.has_value()) {
        throw OperationException("Element does not have label.");
    }

    auto& children = token->child.value();
    if (children.size() != 1) {
        throw OperationException("Label must have only one element.");
    }

    auto& child = children.at(0);
    if (!child->label.has_value()) {
        throw OperationException("No label provided.");
    }
    return child->label.value();
}