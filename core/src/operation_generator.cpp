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
                if (array_element->child.has_value()) {
                    columns.push_back(create_column(array_element->child.value()))
                }
                else {
                    // throw exception: collumn definition missing
                }
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
                        
                        
                    }
                    else {
                        // throw exception: column definition contains multiple type declarations
                    }
                }
                else {
                    // throw exception: column definition is missing type
                }
            }
        }

    }
    else {
        // throw exception: collumn definition with empty body
    }
    return std::move(column);
}

DataType get_data_type(std::unique_ptr<Token>& token) {
    swich (token->type) {
        case TokenType::TEXT: return DataType::TEXT;
        case TokenType::INT: return DataType::INT;
        case TokenType::DOUBLE: return DataType::DOUBLE;
        case TokenType::TEXT: return DataType::TEXT;
        case TokenType::BOLLEAN: return DataType::BOLLEAN;
        case TokenType::UNIX_TIME: return DataType::UNIX_TIME;
        case TokenType::UNIX_TIME_MS: return DataType::UNIX_TIME_MS;
        case TokenType::BLOB: return DataType::BLOB;
        default: return DataType::NOT_FOUND;
    }
}