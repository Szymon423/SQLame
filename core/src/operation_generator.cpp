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


DropOperation::DropOperation() {
    operation_type = OperationType::DROP;
}


DropTableOperation::DropTableOperation() {
    drop_target = DropTarget::TABLE;
}


std::string DropTableOperation::resolve() {
    if (!check_table_meta_exists(table_name)) {
        LOG_TRACE("Table '{}' does not exist.", table_name);
        return "Table " + table_name + " does not exist.";
    }
    
    // TODO delete table data, not only metadata

    if (delete_table_meta(table_name)) {
        LOG_TRACE("Table '{}' dropped succesfully.", table_name);
        return "Table " + table_name + " dropped succesfully.";
    }
    
    LOG_TRACE("Could not drop table {}.", table_name);
    return "Could not drop table " + table_name + ".";
}


InsertOperation::InsertOperation() {
    operation_type = OperationType::INSERT;
}


std::string InsertOperation::resolve() {
    if (!check_table_meta_exists(table.name)) {
        LOG_TRACE("Table '{}' does not exist.", table.name);
        return "Table " + table.name + " does not exist.";
    }

    // TODO here it must insert values into table_name.db
    for (auto& row: rows) {
        LOG_TRACE("Data in row: ");
        std::visit(VisitInsertRowItem(), row);
    }
    
    LOG_TRACE("Could not insert values into table {}.", table.name);
    return "Could not insert values into table " + table.name + ".";
}


void VisitInsertRowItem::operator()(bool& value) {
    LOG_TRACE(" > bool value: {}", value);
}


void VisitInsertRowItem::operator()(double& value) {
    LOG_TRACE(" > double value: {}", value);
}


void VisitInsertRowItem::operator()(std::string& value) {
    LOG_TRACE(" > std::string value: {}", value);
}


void VisitInsertRowItem::operator()(uint64_t& value) {
    LOG_TRACE(" > uint64_t value: {}", value);
}


std::unique_ptr<Operation> generate_operation(std::unique_ptr<Token>& token) {
    if (token->type != TokenType::REQUEST) {
        throw OperationException("Request is fucked up."); 
    }

    if (!token->children.has_value()) {
        throw OperationException("Request is missing body."); 
    }

    auto& children = token->children.value();
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
    else if (child->type == TokenType::DROP) {
        try {
            return generate_drop_operation(child);
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while generating drop operation: {}", e.what());
            throw OperationException(e.what());
        }
    }
    else if (child->type == TokenType::INSERT) {
        try {
            return generate_insert_operation(child);
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while generating insert operation: {}", e.what());
            throw OperationException(e.what());
        }
    }
    else if (child->type == TokenType::SELECT) {
        // TODO create generate_select_operation
        // return generate_select_operation(child);
    }
    else {
        throw OperationException("Unsupported request operation.");
    }
    return nullptr;
}


std::unique_ptr<CreateOperation> generate_create_operation(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Create statement is empty."); 
    }

    auto& children = token->children.value();
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
    if (!token->children.has_value()) {
        throw OperationException("Create table body is missing."); 
    }

    if (!token->has_child(TokenType::NAME)) {
        throw OperationException("Table definition must contain name."); 
    }

    if (!token->has_child(TokenType::COLUMNS)) {
        throw OperationException("Table definition must contain columns definitions."); 
    }
    
    std::unique_ptr<CreateTableOperation> cto = std::make_unique<CreateTableOperation>();

    try {
        cto->table.name = get_label_string(token->get_child(TokenType::NAME));
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception while creating table name: {}", e.what());
        throw OperationException(e.what()); 
    }

    try {
        cto->table.columns = create_columns(token->get_child(TokenType::COLUMNS));
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception while creating columns: {}", e.what());
        throw OperationException(e.what());
    }

    return cto;
}


std::vector<Column> create_columns(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("No column definitions provided."); 
    }

    std::vector<Column> columns;
    for (auto& array_element: token->children.value()) {
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
    if (!token->children.has_value()) {
        throw OperationException("Collumn definition with empty body."); 
    }

    if (!token->has_child(TokenType::NAME)) {
        throw OperationException("Column definition is missing name."); 
    }

    if (!token->has_child(TokenType::TYPE)) {
        throw OperationException("Column definition is missing data type."); 
    }

    Column column;
    try {
        column.name = get_label_string(token->get_child(TokenType::NAME));
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    try {
        column.data_type = get_data_type(token->get_child(TokenType::TYPE));
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    if (token->has_child(TokenType::ATTRIBUTES)) {
        try {
            column.attributes = get_column_attributes(token->get_child(TokenType::ATTRIBUTES));
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while getting column attributes: {}", e.what());
            throw OperationException(e.what());
        }
    }

    return column;
}


DataType get_data_type(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Element does not have body with data type.");
    }
    
    if (token->get_children_number() != 1) {
        throw OperationException("Element with data type must have data type only inside.");
    }

    auto& child = token->children.value().at(0);

    if (child->type == TokenType::TEXT) return DataType::TEXT;
    if (child->type == TokenType::INT) return DataType::INT;
    if (child->type == TokenType::DOUBLE) return DataType::DOUBLE;
    if (child->type == TokenType::TEXT) return DataType::TEXT;
    if (child->type == TokenType::BOOLEAN) return DataType::BOOLEAN;
    if (child->type == TokenType::UNIX_TIME) return DataType::UNIX_TIME;
    if (child->type == TokenType::UNIX_TIME_MS) return DataType::UNIX_TIME_MS;
    if (child->type == TokenType::BLOB) return DataType::BLOB;
    return DataType::NOT_FOUND;
}


std::vector<ColumnAttributes> get_column_attributes(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Column attributes definition with empty body."); 
    }

    std::vector<ColumnAttributes> attributes;
    for (auto& item: token->children.value()) {
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
    if (!token->children.has_value()) {
        throw OperationException("Element does not have body with label.");
    }
    
    if (token->get_children_number() != 1) {
        throw OperationException("Element with label must have label only inside.");
    }

    if (!token->has_child(TokenType::LABEL)) {
        throw OperationException("Element with label is missing label.");
    }

    auto& child = token->get_child(TokenType::LABEL);

    if (!child->label.has_value()) {
        throw OperationException("No label provided.");
    }
    return child->label.value();
}


std::unique_ptr<DropOperation> generate_drop_operation(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Drop statement is empty."); 
    }

    if (token->get_children_number() > 1) {
        throw OperationException("Only one element must be in drop statement.");
    }

    if (token->has_child(TokenType::TABLE)) {
        try {
            return generate_drop_table_operation(token->get_child(TokenType::TABLE));
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while generating drop table operation: {}", e.what());
            throw OperationException(e.what());
        }
    }

    // if (token->has_child(TokenType::VIEW)) {
    //     try {
    //         return generate_drop_view_operation(token->get_child(TokenType::VIEW));
    //     }
    //     catch (OperationException& e) {
    //         LOG_ERROR("Caught an exception while generating drop view operation: {}", e.what());
    //         throw OperationException(e.what());
    //     }
    // }

    // if (token->has_child(TokenType::TRIGGER)) {
    //     try {
    //         return generate_drop_trigger_operation(token->get_child(TokenType::TRIGGER));
    //     }
    //     catch (OperationException& e) {
    //         LOG_ERROR("Caught an exception while generating drop trigger operation: {}", e.what());
    //         throw OperationException(e.what());
    //     }
    // }

    return nullptr;
}


std::unique_ptr<DropTableOperation> generate_drop_table_operation(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Drop table body is missing."); 
    }

    if (!token->has_child(TokenType::NAME)) {
        throw OperationException("Drop table operation must contain table name.");
    }

    if (token->get_children_number() > 1) {
        throw OperationException("Drop table operation must contain only one element with table name.");
    }
    
    std::unique_ptr<DropTableOperation> dto = std::make_unique<DropTableOperation>();
    try {
        dto->table.name = get_label_string(token->get_child(TokenType::NAME));
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    return dto;
}


std::unique_ptr<InsertOperation> generate_insert_operation(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Insert body is missing."); 
    }

    if (!token->has_child(TokenType::INTO)) {
        throw OperationException("Insert querry is missing table name.");
    }

    if (!token->has_child(TokenType::VALUES)) {
        throw OperationException("Insert querry is missing values to insert.");
    }

    if (token->get_children_number() > 2) {
        throw OperationException("Insert querry has unwanted objects.");
    }

    std::unique_ptr<InsertOperation> io = std::make_unique<InsertOperation>();
    try {
        io->table.name = get_label_string(token->get_child(TokenType::INTO));
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }
        
    return io;
}


Row get_row(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("Row value definition with empty body."); 
    }

    Row row;
    
    return row;
}


std::vector<Row> get_rows(std::unique_ptr<Token>& token) {
    if (!token->children.has_value()) {
        throw OperationException("No row definitions provided."); 
    }

    std::vector<Row> rows;
    for (auto& array_element: token->children.value()) {
        if (array_element->type != TokenType::ARRAY_ELEMENT) {
            throw OperationException("Row Values definitions must be in array."); 
        }
        try {
            auto row = get_row(array_element);
            columns.push_back(std::move(col));
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while creating row: {}", e.what());
            throw OperationException(e.what());
        }
    }
    return rows;
}