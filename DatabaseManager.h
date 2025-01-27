#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

class DatabaseManager {
private:
    sqlite3 *db;

public:
    // Constructor and Destructor
    DatabaseManager(const std::string &dbName);
    ~DatabaseManager();

    // Execute SQL Query
    bool executeSQL(const std::string &sql, std::string &errorMessage);

    // Authentication
    bool validateUser(const std::string &username, const std::string &password, std::string &role);

    // Instrument Management (Admin only)
    bool addInstrument(const std::string &isin, const std::string &mic, const std::string &currency, const std::string &status);
    bool updateInstrument(const std::string &isin, const std::string &field, const std::string &value);
    bool deleteInstrument(const std::string &isin);
    std::vector<std::map<std::string, std::string>> getInstruments(const std::string &filter = "");
};

#endif // DATABASEMANAGER_H
