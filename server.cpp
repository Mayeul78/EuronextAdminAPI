#include "crow.h"
#include "DatabaseManager.h"
#include <unordered_map>
#include <string>
#include <random>

// Global token map: Key is username, Value is token
std::unordered_map<std::string, std::string> tokens;

// Generate a random token for authentication
std::string generateToken() {
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static const size_t length = 32;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> dist(0, sizeof(charset) - 2);

    std::string token;
    for (size_t i = 0; i < length; ++i) {
        token += charset[dist(generator)];
    }
    return token;
}

// Check if a token is valid
bool isTokenValid(const std::string &token) {
    for (const auto &pair : tokens) {
        if (pair.second == token) {
            return true;
        }
    }
    return false;
}

int main() {
    crow::SimpleApp app;
    DatabaseManager dbManager("my_database.db");

    // PING
    CROW_ROUTE(app, "/ping").methods("GET"_method)([](const crow::request &req) {
        return crow::response(200, "pong");
       
    });

    // POST /login - Authenticate admin and provide a token
    CROW_ROUTE(app, "/login").methods("POST"_method)([&dbManager](const crow::request &req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("username") || !body.has("password")) {
            return crow::response(400, "Missing username or password");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (dbManager.validateAdmin(username, password)) {
            // Generate and store token
            std::string token = generateToken();
            tokens[username] = token;

            crow::json::wvalue response;
            response["token"] = token;
            return crow::response(200, response);
        } else {
            return crow::response(401, "Invalid username or password");
        }
    });

    // GET /secure - Validate token
    CROW_ROUTE(app, "/secure").methods("GET"_method)([](const crow::request &req) {
        auto token = req.get_header_value("Authorization");

        if (token.empty()) {
            return crow::response(400, "Missing Authorization header");
        }

        if (isTokenValid(token)) {
            return crow::response(200, "Valid token");
        } else {
            return crow::response(401, "Invalid token");
        }
    });

    // POST /instruments - Add a new instrument
    CROW_ROUTE(app, "/instruments").methods("POST"_method)([&dbManager](const crow::request &req) {
        auto token = req.get_header_value("Authorization");
        if (!isTokenValid(token)) {
            return crow::response(401, "Invalid token");
        }

        auto body = crow::json::load(req.body);
        if (!body || !body.has("isin") || !body.has("mic") || !body.has("currency") || !body.has("status")) {
            return crow::response(400, "Missing instrument details");
        }

        std::string isin = body["isin"].s();
        std::string mic = body["mic"].s();
        std::string currency = body["currency"].s();
        std::string status = body["status"].s();

        if (dbManager.addInstrument(isin, mic, currency, status)) {
            return crow::response(200, "Instrument added successfully");
        } else {
            return crow::response(500, "Failed to add instrument");
        }
    });
    // Retrieve all instruments: GET /instruments
    CROW_ROUTE(app, "/instruments").methods("GET"_method)([&dbManager](const crow::request &req) {
        auto token = req.get_header_value("Authorization");
        if (!isTokenValid(token)) {
            return crow::response(401, "Invalid token");
        }

        auto instruments = dbManager.getInstruments();
        crow::json::wvalue response;
        crow::json::wvalue::list instrumentList;

        for (const auto &instrument : instruments) {
            crow::json::wvalue instrumentJson;
            for (const auto &[key, value] : instrument) {
                instrumentJson[key] = value;
            }
            instrumentList.emplace_back(std::move(instrumentJson));
        }

        response["instruments"] = std::move(instrumentList);
        return crow::response(200, response);
    });

    // PUT /instruments/<isin> - Update an instrument
    CROW_ROUTE(app, "/instruments/<string>").methods("PUT"_method)([&dbManager](const crow::request &req, const std::string &isin) {
        auto token = req.get_header_value("Authorization");
        if (!isTokenValid(token)) {
            return crow::response(401, "Invalid token");
        }

        auto body = crow::json::load(req.body);
        if (!body || !body.has("field") || !body.has("value")) {
            return crow::response(400, "Missing field or value");
        }

        std::string field = body["field"].s();
        std::string value = body["value"].s();

        if (dbManager.updateInstrument(isin, field, value)) {
            return crow::response(200, "Instrument updated successfully");
        } else {
            return crow::response(500, "Failed to update instrument");
        }
    });

    // DELETE /instruments/<isin> - Delete an instrument
    CROW_ROUTE(app, "/instruments/<string>").methods("DELETE"_method)([&dbManager](const crow::request &req, const std::string &isin) {
        auto token = req.get_header_value("Authorization");
        if (!isTokenValid(token)) {
            return crow::response(401, "Invalid token");
        }

        if (dbManager.deleteInstrument(isin)) {
            return crow::response(200, "Instrument deleted successfully");
        } else {
            return crow::response(500, "Failed to delete instrument");
        }
    });


    std::cout << "Server running on http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();

    return 0;
}
