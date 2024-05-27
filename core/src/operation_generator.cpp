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
                // TODO continue from here
            }
        }
    }
    return nullptr;
}