#include "DatabaseManager.h"
#include <iostream>
#include <sstream>
#include <random>

DatabaseManager::DatabaseManager(const std::string &dbName) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }

    std::string errorMessage;

    // Create admin table
    std::string adminTableSQL =
        "CREATE TABLE IF NOT EXISTS admin ("
        "AdminName TEXT PRIMARY KEY, "
        "AdminPassword TEXT NOT NULL);";
    if (!executeSQL(adminTableSQL, errorMessage)) {
        throw std::runtime_error("Error creating admin table: " + errorMessage);
    }

    // Create instruments table
    std::string instrumentsTableSQL =
        "CREATE TABLE IF NOT EXISTS instruments ("
        "ISIN TEXT PRIMARY KEY, "
        "MIC TEXT NOT NULL, "
        "Currency TEXT NOT NULL, "
        "Status TEXT NOT NULL);";
    if (!executeSQL(instrumentsTableSQL, errorMessage)) {
        throw std::runtime_error("Error creating instruments table: " + errorMessage);
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}

bool DatabaseManager::executeSQL(const std::string &sql, std::string &errorMessage) {
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        errorMessage = errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::validateAdmin(const std::string &username, const std::string &password) {
    std::string sql = "SELECT COUNT(*) FROM admin WHERE AdminName = ? AND AdminPassword = ?";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int result = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    return result > 0;
}

std::string DatabaseManager::generateToken(const std::string &username) {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string token;
    for (int i = 0; i < 32; i++) {
        token += charset[rand() % (sizeof(charset) - 1)];
    }
    authTokens[token] = username;
    return token;
}

bool DatabaseManager::isTokenValid(const std::string &token) {
    return authTokens.find(token) != authTokens.end();
}

bool DatabaseManager::addInstrument(const std::string &isin, const std::string &mic, const std::string &currency, const std::string &status) {
    std::string sql = "INSERT INTO instruments (ISIN, MIC, Currency, Status) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, isin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, mic.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, currency.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, status.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

bool DatabaseManager::updateInstrument(const std::string &isin, const std::string &field, const std::string &value) {
    std::string sql = "UPDATE instruments SET " + field + " = ? WHERE ISIN = ?";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, isin.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

bool DatabaseManager::deleteInstrument(const std::string &isin) {
    std::string sql = "DELETE FROM instruments WHERE ISIN = ?";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, isin.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

std::vector<std::map<std::string, std::string>> DatabaseManager::getInstruments(const std::string &filter) {
    std::vector<std::map<std::string, std::string>> instruments;
    std::string sql = "SELECT * FROM instruments";
    if (!filter.empty()) {
        sql += " WHERE " + filter;
    }

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> instrument;
        instrument["ISIN"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        instrument["MIC"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        instrument["Currency"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        instrument["Status"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        instruments.push_back(instrument);
    }

    sqlite3_finalize(stmt);
    return instruments;
}
