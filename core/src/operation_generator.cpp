#include "operation_generator.hpp"

CreateOperation::CreateOperation() {
    operation_type = OperationType::CREATE;
}


CreateTableOperation::CreateTableOperation() {
    create_target = CreateTarget::TABLE;
}

std::string CreateTableOperation::resolve() {
    return "Create table operation resoult.";
}


SelectOperation::SelectOperation() {
    operation_type = OperationType::SELECT;
}

std::string SelectOperation::resolve() {
    return "Select operation resoult.";
}


std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token) {
    // check if first token is REQUEST token
    if (token->type == TokenType::REQUEST) {
        // check if REQUEST token has any children
        if (token->child.has_value()) {
            std::vector<std::unique_ptr<Token>>& children = token->child.value();
            // check if it has only one child
            if (children.size() == 1) {
                std::unique_ptr<Token>& child = children.at(0);
                if (child->type == TokenType::CREATE) {
                    return generate_create_operation(child);
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
    if (token->type == TokenType::TABLE) {
        return generate_create_table_operation(token);
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
    CreateTableOperation cto;
    if (token->child.has_value()) {
        for (auto& child: token->child.value()) {
            if (child->type == TokenType::COLUMNS) {
                cto.table.columns = create_columns(child);
            }
            if (child->type == TokenType::LABEL) {
                if (child->label.has_value()) {
                    cto.table.name = child->label.value();
                }
                else {
                    // throw exception: missing table name
                }
            }
        }
    }
    return nullptr;
}


std::vector<Column> create_columns(std::unique_ptr<Token>& token) {
    // toke->type == TokenType::COLUMNS
    std::vector<Column> columns;
    if (token->child.has_value()) {
        for (auto& array_element: token->child.value()) {
            if (array_element->type == TokenType::ARRAY_ELEMENT) {
                auto col = create_column(array_element);
                columns.push_back(std::move(col));
            }
            else {
                // throw exception: columns definitions must be in array
            }
        }
    }
    else {
        // throw exception: no column definitions provided
    }
    return std::move(columns);
}


Column create_column(std::unique_ptr<Token>& token) {
    // toke->type == TokenType::ARRAY_ELEMENT
    Column column;
    if (token->child.has_value()) {
        for (auto& item: token->child.value()) {
            if (item->type == TokenType::NAME) {
                if (item->label.has_value()) {
                    column.name = item->label.value();
                }
                else {
                    // throw exception: column definition is missing name
                }
            }
            else if (item->type == TokenType::TYPE) {
                if (item->child.has_value()) {
                    auto& c = item->child.value();
                    if (c.size() == 1) {
                        column.data_type = get_data_type(c.at(0));
                        if (column.data_type == DataType::NOT_FOUND) {
                            // throw exception: not correct column data type 
                        }
                    }
                    else {
                        // throw exception: column definition contains multiple type declarations
                    }
                }
                else {
                    // throw exception: column definition is missing type
                }
            }
            else if (item->type == TokenType::ATTRIBUTES) {
                column.atributes = get_column_attributes(item);
            }
            else {
                // throw exception: unknown element in column definition
            }
        }

    }
    else {
        // throw exception: collumn definition with empty body
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
    std::vector<ColumnAttributes> atributes;
    if (token->child.has_value()) {
        for (auto& item: token->child.value()) {
            auto atr = get_column_attribute(item);
            if (atr == ColumnAttributes::NOT_FOUND) {
                // throw exception: unknonw collumn attribute
            }
            else {
                atributes.push_back(std::move(atr));
            }
        }
    }
    else {
        // throw exception: collumn definition with empty body
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