#include "operation_generator.hpp"


OperationException::OperationException(const std::string msg): message(msg) {

}


const std::string OperationException::what() {
    return message;
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
        return "Table '" + table.name + "' does not exist.";
    }

    // create vector with bytes to save
    std::vector<uint8_t> byte_vector;
    for (auto& row: rows) {
        LOG_TRACE("Data in row: ");
        for (auto& element: row) {
            auto bytes = element.to_bytes();
            byte_vector.insert(byte_vector.end(), bytes.begin(), bytes.end());
        }
    }
    try {
        save_to_file(byte_vector, Configuration::base_path() / fs::path{ "data/tables/" + table.name + ".db" });
    }
    catch(UtilitiesException& e) {
        LOG_ERROR("Insert values into table '{}' failed becouse of '{}'.", table.name, e.what());
        return "Insert values into table '" + table.name + "' failed becouse of '" + e.what() + "'.";
    }
    
    LOG_TRACE("Inserted values into table '{}'.", table.name);
    return "Inserted values into table '" + table.name + "'.";
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
        auto child = token->get_child(TokenType::NAME);
        cto->table.name = get_label_string(*child);
    }
    catch (OperationException& e) {
        LOG_ERROR("Caught an exception while creating table name: {}", e.what());
        throw OperationException(e.what()); 
    }

    try {
        auto child = token->get_child(TokenType::COLUMNS);
        cto->table.columns = create_columns(*child);
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
        Column col;
        try {
            col = create_column(array_element);

        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while creating column: {}", e.what());
            throw OperationException(e.what());
        }
        for (auto& c: columns) {
            if (col.name == c.name) {
                throw OperationException("Each column must have unique name."); 
            }
        }
        columns.push_back(std::move(col));
    }
    return columns;
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
        auto child = token->get_child(TokenType::NAME);
        column.name = get_label_string(*child);
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    try {
        auto child = token->get_child(TokenType::TYPE);
        column.data_type = get_data_type(*child);
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    if (token->has_child(TokenType::ATTRIBUTES)) {
        try {
            auto child = token->get_child(TokenType::ATTRIBUTES);
            column.attributes = get_column_attributes(*child);
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

    if (child->type == TokenType::NUMBER) return DataType::NUMBER;
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


TokenType token_type_from_data_type(DataType dt) {
    switch (dt) {
        case DataType::NUMBER: return TokenType::NUMBER;
        case DataType::TEXT: return TokenType::TEXT;
        case DataType::INT: return TokenType::INT;
        case DataType::DOUBLE: return TokenType::DOUBLE;
        case DataType::BOOLEAN: return TokenType::BOOLEAN;
        case DataType::UNIX_TIME: return TokenType::UNIX_TIME;
        case DataType::UNIX_TIME_MS: return TokenType::UNIX_TIME_MS;
        case DataType::BLOB: return TokenType::BLOB;
        case DataType::NOT_FOUND: return TokenType::UNKNOWN;
    }
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
    return attributes;
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

    auto child = token->get_child(TokenType::LABEL);

    if (!(*child)->label.has_value()) {
        throw OperationException("No label provided.");
    }
    return (*child)->label.value();
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
            auto child = token->get_child(TokenType::TABLE);
            return generate_drop_table_operation(*child);
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
        auto child = token->get_child(TokenType::NAME);
        dto->table_name = get_label_string(*child);
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

    std::string table_name;
    try {
        auto child = token->get_child(TokenType::INTO);
        table_name = get_label_string(*child);
    }
    catch (OperationException& e) {
        throw OperationException(e.what()); 
    }

    if (!check_table_meta_exists(table_name)) {
        throw OperationException("Can not insert becouse table '" + table_name + "' does not exist.");
    }

    std::unique_ptr<InsertOperation> io = std::make_unique<InsertOperation>();
    try {
        io->table = load_table_from_meta(table_name);
    }
    catch (MetadataException& e) {
        throw OperationException(e.what()); 
    }

    try {
        auto child = token->get_child(TokenType::VALUES);
        io->rows = get_rows(*child, io->table.columns);
    }
    catch (MetadataException& e) {
        throw OperationException(e.what()); 
    }

    return io;
}


bool check_column_can_be_null(const std::optional<std::vector<ColumnAttributes>>& atributes) {
    if (!atributes.has_value()) {
        return true;
    }

    for (auto& atr: atributes.value()) {
        if (atr == ColumnAttributes::NOT_NULL) {
            return false;
        }
    }

    return true;
}


std::vector<Row> get_rows(std::unique_ptr<Token>& token, const std::vector<Column>& columns) {
    if (!token->children.has_value()) {
        throw OperationException("No row definitions provided."); 
    }

    std::vector<Row> rows;
    for (auto& array_element: token->children.value()) {
        if (array_element->type != TokenType::ARRAY_ELEMENT) {
            throw OperationException("Row Values definitions must be in array."); 
        }
        try {
            auto row = get_row(array_element, columns);
            rows.push_back(std::move(row));
        }
        catch (OperationException& e) {
            LOG_ERROR("Caught an exception while creating row: {}", e.what());
            throw OperationException(e.what());
        }
    }
    return rows;
}


Row get_row(std::unique_ptr<Token>& token, const std::vector<Column>& columns) {
    if (!token->children.has_value()) {
        throw OperationException("Row value definition with empty body."); 
    }

    Row row;
    auto& row_elements = token->children.value();
    for (auto& col: columns) {
        bool found_column_val = false;
        for (auto& element: row_elements) {
            if (element->type != TokenType::LABEL) {
                throw OperationException("Not allowed element in row.");
            }

            if (!element->label.has_value()) {
                throw OperationException("Column label is missing.");
            }

            if (!element->children.has_value()) {
                throw OperationException("No value associated with column name '" + col.name + "'.");
            }

            if (element->get_children_number() != 1) {
                throw OperationException("Only one value can be associated with column name '" + col.name + "'.");
            }

            std::string column_name = element->label.value();

            if (column_name != col.name) {
                continue;
            }
            found_column_val = true;

            // when value was not found check if collumn can be null
            if (!found_column_val) {
                if (!check_column_can_be_null(col.attributes)) {
                    throw OperationException("Column '" + col.name + "' is can not be null.");
                }
                row.emplace_back(col.data_type);
                continue;
            }

            auto& child = element->children.value().at(0);
            switch (col.data_type) {
                case DataType::BOOLEAN: {
                    if (!child->value_boolean.has_value()) {
                        throw OperationException("Data type (BOOLEAN) of column '" + col.name + "' does not match witch provided data type.");
                    }
                    row.emplace_back(DataType::BOOLEAN, child->value_boolean.value());
                    break;
                }
                case DataType::DOUBLE: {
                    if (!child->value_double.has_value()) {
                        throw OperationException("Data type (DOUBLE) of column '" + col.name + "' does not match witch provided data type.");
                    }
                    row.emplace_back(DataType::DOUBLE, child->value_double.value());
                    break;
                }
                case DataType::TEXT: {
                    if (!child->label.has_value()) {
                        throw OperationException("Data type (TEXT) of column '" + col.name + "' does not match witch provided data type.");
                    }
                    row.emplace_back(DataType::TEXT, child->label.value());
                    break;
                }
                case DataType::INT: {
                    if (!child->value_int.has_value()) {
                        throw OperationException("Data type (INT) of column '" + col.name + "' does not match witch provided data type.");
                    }
                    row.emplace_back(DataType::INT, child->value_int.value());
                    break;
                }
                case DataType::UNIX_TIME: {
                    if (!child->value_unix_time.has_value()) {
                        throw OperationException("Data type (INT) of column '" + col.name + "' does not match witch provided data type.");
                    }
                    row.emplace_back(DataType::UNIX_TIME, child->value_unix_time.value());
                    break;
                }
                // TODO add not listed above datatypes
            }
        }
    }
    
    return row;
}