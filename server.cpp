#include <crow.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <csignal>

// SQLite database handle
sqlite3 *db = nullptr;

// Token storage (in memory for simplicity)
std::unordered_map<std::string, std::string> tokens;

// Signal handler to close the database
void handle_signal(int signal) {
    if (db) {
        sqlite3_close(db);
        std::cout << "\nDatabase connection closed due to signal (" << signal << ")." << std::endl;
    }
    exit(signal); // Exit gracefully
}

// Function to generate a random token (UUID-like)
std::string generate_token() {
    static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string token;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    for (int i = 0; i < 32; ++i) {
        token += alphanum[dis(gen)];
    }

    return token;
}

int main() {
    // Register signal handlers for graceful termination
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    // Open SQLite database
    const std::string db_name = "users.db";
    if (sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Create "admin" table if it doesn't exist
    const char *create_table_sql ="";
    /* "CREATE TABLE IF NOT EXISTS admin ("
     "AdminName TEXT PRIMARY KEY, "
        "INSERT INTO admin (AdminName, AdminPassword) "
        "VALUES ('admin', 'admin123');";*/
       
    char *errorMessage = nullptr;
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return 1;
    }

    crow::SimpleApp app;

    // Login route
    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body["username"] || !body["password"]) {
            return crow::response(400, "Invalid JSON or missing fields");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        // Check credentials in the database
        std::string query = "SELECT AdminPassword FROM admin WHERE AdminName = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return crow::response(500, "Database error");
        }

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            std::string stored_password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sqlite3_finalize(stmt);

            if (stored_password == password) {
                // Generate and return token
                std::string token = generate_token();
                tokens[username] = token; // Store token for the user
                crow::json::wvalue response;
                response["token"] = token;
                return crow::response(200, response);
            } else {
                return crow::response(401, "Invalid password");
            }
        }

        sqlite3_finalize(stmt);
        return crow::response(404, "User not found");
    });

    // Secure route requiring token authentication
    CROW_ROUTE(app, "/secure").methods("GET"_method)([](const crow::request& req) {
        auto token = req.get_header_value("Authorization");
        if (token.empty()) {
            return crow::response(401, "Authorization token missing");
        }

        // Check if the token exists
        for (const auto& pair : tokens) {
            if (pair.second == token) {
                return crow::response(200, "Secure content accessed");
            }
        }

        return crow::response(403, "Invalid or expired token");
    });

    // Run the app
    app.port(18080).multithreaded().run();

    return 0;
}
