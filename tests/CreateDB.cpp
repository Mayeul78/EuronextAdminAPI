#include <sqlite3.h>
#include <iostream>
#include <string>

int main() {
    std::cout << "hello"<< std::endl;
    sqlite3 *db;  // SQLite database handle
    char *errorMessage = nullptr;

    // Database file name
    const std::string db_name = "users.db";

    // Open or create the SQLite database
    int rc = sqlite3_open(db_name.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Database opened/created successfully!" << std::endl;
    }

    // SQL statement to create a table
    const char *create_table_sql =
            "CREATE TABLE IF NOT EXISTS admin ("
            "AdminName TEXT PRIMARY KEY, "
            "AdminPassword TEXT NOT NULL);";

    // Execute the SQL statement
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "Table created successfully!" << std::endl;
    }

    // Insert an example user into the table
    const char *insert_user_sql =
            "INSERT INTO admin (AdminName, AdminPassword) "
            "VALUES ('admin', 'admin123');";

    rc = sqlite3_exec(db, insert_user_sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "Example user inserted successfully!" << std::endl;
    }

    // Close the database
    sqlite3_close(db);
    std::cout << "Database connection closed." << std::endl;

    return 0;
}