#include "DatabaseManager.h"
#include <iostream>
#include <sstream>
#include <filesystem>
// Constructor: Opens the database connection, resets it, and ensures necessary tables exist
// Constructor: Opens the database connection, resets it, and ensures necessary tables exist
DatabaseManager::DatabaseManager(const std::string &dbName) {
    // Delete the old database file (reset DB)
    /*if (std::filesystem::exists(dbName)) {
        std::filesystem::remove(dbName);
        std::cout << "⚠️  Old database deleted. A new one will be created.\n";
    }*/

    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "❌ Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
        return;
    }

    // Create the users table with three possible roles
    std::string createUsersTableSQL = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL,
            role TEXT NOT NULL CHECK (role IN ('Admin', 'Trader', 'Surveillance'))
        );
        INSERT OR IGNORE INTO users (username, password, role) VALUES ('admin', 'admin123', 'Admin');
        INSERT OR IGNORE INTO users (username, password, role) VALUES ('trader1', 'trader123', 'Trader');
        INSERT OR IGNORE INTO users (username, password, role) VALUES ('analyst1', 'analyst123', 'Surveillance');
    )";

    std::string errorMessage;
    if (!executeSQL(createUsersTableSQL, errorMessage)) {
        std::cerr << "❌ Error creating users table: " << errorMessage << std::endl;
    } else {
        std::cout << "✅ Users table created successfully.\n";
    }

    // ✅ Create the instruments table (this was missing)
    std::string createInstrumentsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS instruments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            isin TEXT NOT NULL UNIQUE,
            mic TEXT NOT NULL,
            currency TEXT NOT NULL,
            status TEXT NOT NULL CHECK (status IN ('Active', 'Inactive'))
        );
    )";

    if (!executeSQL(createInstrumentsTableSQL, errorMessage)) {
        std::cerr << "❌ Error creating instruments table: " << errorMessage << std::endl;
    } else {
        std::cout << "✅ Instruments table created successfully.\n";
    }

    // Display users in the database
    displayUsers();
}


// Destructor: Closes the database connection
DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}
// Function to display all users from the database using executeSQL
void DatabaseManager::displayUsers() {
    std::string sql = "SELECT id, username, role FROM users;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::cout << "\n🔹 Current Users in Database:" << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string role = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));

        std::cout << "🆔 ID: " << id << " | 👤 Username: " << username << " | 🎭 Role: " << role << std::endl;
    }

    sqlite3_finalize(stmt);
}

// Execute a generic SQL query
bool DatabaseManager::executeSQL(const std::string &sql, std::string &errorMessage) {
    char *errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        errorMessage = errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Validate a user and retrieve their role
bool DatabaseManager::validateUser(const std::string &username, const std::string &password, std::string &role) {
    std::string sql = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "' LIMIT 1;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        role = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
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
            std::string columnValue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, col));
            instrument[columnName] = columnValue;
        }
        instruments.push_back(instrument);
    }

    sqlite3_finalize(stmt);
    return instruments;
}

bool DatabaseManager::addUser(const std::string &username, const std::string &password, const std::string &role) {
    std::ostringstream sql;
    sql << "INSERT INTO users (username, password, role) VALUES ('"
        << username << "', '" << password << "', '" << role << "');";

    std::string errorMessage;
    return executeSQL(sql.str(), errorMessage);
}

bool DatabaseManager::deleteUser(const std::string &username) {
    std::ostringstream sql;
    sql << "DELETE FROM users WHERE username = '" << username << "';";

    std::string errorMessage;
    return executeSQL(sql.str(), errorMessage);
}
