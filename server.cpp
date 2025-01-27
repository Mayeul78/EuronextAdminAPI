#include "crow.h"
#include "DatabaseManager.h"
#include "TokenManager.h"
#include <string>

TokenManager tokenManager;

int main() {
    crow::SimpleApp app;
    DatabaseManager dbManager("project_database.db");

    CROW_ROUTE(app, "/login").methods("POST"_method)([&](const crow::request &req) {
    auto body = crow::json::load(req.body);
    if (!body || !body.has("username") || !body.has("password")) {
        return crow::response(400, "Missing username or password");
    }

    std::string username = body["username"].s();
    std::string password = body["password"].s();
    std::string role;

    // Validate the user and get their role
    if (dbManager.validateUser(username, password, role)) {
        std::string token = tokenManager.generateToken();
        tokenManager.storeToken(username, token, role);

        crow::json::wvalue response;
        response["token"] = token;
        response["role"] = role;
        return crow::response(200, response);
    } else {
        return crow::response(401, "Invalid username or password");
    }
});


    // Add a new instrument (Admin only)
    CROW_ROUTE(app, "/instruments").methods("POST"_method)([&](const crow::request &req) {
        std::string role;
        auto token = req.get_header_value("Authorization");

        // Check if token is valid and the user is an admin
        if (!tokenManager.isTokenValid(token, role) || role != "Admin") {
            return crow::response(403, "Forbidden: Admin access only");
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

    // Retrieve all instruments (Admin only)
    CROW_ROUTE(app, "/instruments").methods("GET"_method)([&](const crow::request &req) {
        std::string role;
        auto token = req.get_header_value("Authorization");

        if (!tokenManager.isTokenValid(token, role) || role != "Admin") {
            return crow::response(403, "Forbidden: Admin access only");
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

    // Update an instrument (Admin only)
    CROW_ROUTE(app, "/instruments/<string>").methods("PUT"_method)([&](const crow::request &req, const std::string &isin) {
        std::string role;
        auto token = req.get_header_value("Authorization");

        if (!tokenManager.isTokenValid(token, role) || role != "Admin") {
            return crow::response(403, "Forbidden: Admin access only");
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

    // Delete an instrument (Admin only)
    CROW_ROUTE(app, "/instruments/<string>").methods("DELETE"_method)([&](const crow::request &req, const std::string &isin) {
        std::string role;
        auto token = req.get_header_value("Authorization");

        if (!tokenManager.isTokenValid(token, role) || role != "Admin") {
            return crow::response(403, "Forbidden: Admin access only");
        }

        if (dbManager.deleteInstrument(isin)) {
            return crow::response(200, "Instrument deleted successfully");
        } else {
            return crow::response(500, "Failed to delete instrument");
        }
    });

    // General endpoint to check server status
    CROW_ROUTE(app, "/ping").methods("GET"_method)([] {
        return crow::response(200, "pong");
    });

    // Start the server
    std::cout << "Server running on http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().run();

    return 0;
}
