#include "language_syntax.hpp"


CreateOperation::CreateOperation() {
    type = OperationType::CREATE;
}


std::string CreateOperation::resolve() {
    return "Create operation resoult";
}


SelectOperation::SelectOperation() {
    type = OperationType::SELECT;
}


std::string SelectOperation::resolve() {
    return "Select operation resoult";
}