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