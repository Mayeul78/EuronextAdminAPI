#include "DatabaseManager.h"
#include <iostream>
#include <sstream>

// Constructor: Opens the database connection and ensures necessary tables exist
DatabaseManager::DatabaseManager(const std::string &dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }

    // Create the admins table if it doesn't exist
    std::string createAdminsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS admins (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
        INSERT OR IGNORE INTO admins (username, password) VALUES ('admin', 'admin123');
        INSERT OR IGNORE INTO admins (username, password) VALUES ('admin2', 'admin123');

    )";

    std::string errorMessage;
    if (!executeSQL(createAdminsTableSQL, errorMessage)) {
        std::cerr << "Error creating admins table: " << errorMessage << std::endl;
    }

    // Create the instruments table if it doesn't exist
    std::string createInstrumentsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS instruments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            isin TEXT NOT NULL UNIQUE,
            mic TEXT NOT NULL,
            currency TEXT NOT NULL,
            status TEXT NOT NULL
        );
    )";

    if (!executeSQL(createInstrumentsTableSQL, errorMessage)) {
        std::cerr << "Error creating instruments table: " << errorMessage << std::endl;
    }
}

// Destructor: Closes the database connection
DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

// Execute a SQL query
bool DatabaseManager::executeSQL(const std::string &sql, std::string &errorMessage) {
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        errorMessage = errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
// Validate admin credentials
bool DatabaseManager::validateAdmin(const std::string &username, const std::string &password) {
    // SQL query to check for a matching username and password
    std::string sql = "SELECT 1 FROM admins WHERE username = '" + username + "' AND password = '" + password + "' LIMIT 1;";
    sqlite3_stmt *stmt;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Execute the query
    int result = sqlite3_step(stmt);
    bool isValid = (result == SQLITE_ROW); // If we get a row, the credentials are valid

    // Finalize the statement to clean up
    sqlite3_finalize(stmt);

    return isValid;
}



// Add an instrument to the database
bool DatabaseManager::addInstrument(const std::string &isin, const std::string &mic, const std::string &currency, const std::string &status) {
    std::ostringstream sql;
    sql << "INSERT INTO instruments (isin, mic, currency, status) VALUES ('"
        << isin << "', '" << mic << "', '" << currency << "', '" << status << "');";

    std::string errorMessage;
    return executeSQL(sql.str(), errorMessage);
}

// Update an instrument in the database
bool DatabaseManager::updateInstrument(const std::string &isin, const std::string &field, const std::string &value) {
    std::ostringstream sql;
    sql << "UPDATE instruments SET " << field << " = '" << value << "' WHERE isin = '" << isin << "';";

    std::string errorMessage;
    return executeSQL(sql.str(), errorMessage);
}

// Delete an instrument from the database
bool DatabaseManager::deleteInstrument(const std::string &isin) {
    std::ostringstream sql;
    sql << "DELETE FROM instruments WHERE isin = '" << isin << "';";

    std::string errorMessage;
    return executeSQL(sql.str(), errorMessage);
}

// Retrieve instruments from the database with an optional filter
std::vector<std::map<std::string, std::string>> DatabaseManager::getInstruments(const std::string &filter) {
    std::ostringstream sql;
    sql << "SELECT * FROM instruments";
    if (!filter.empty()) {
        sql << " WHERE " << filter;
    }
    sql << ";";

    sqlite3_stmt *stmt;
    std::vector<std::map<std::string, std::string>> instruments;

    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return instruments;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> instrument;
        for (int col = 0; col < sqlite3_column_count(stmt); ++col) {
            std::string columnName = sqlite3_column_name(stmt, col);
            std::string columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
            instrument[columnName] = columnValue;
        }
        instruments.push_back(instrument);
    }

    sqlite3_finalize(stmt);
    return instruments;
}
