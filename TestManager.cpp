#include "DatabaseManager.h"
#include <iostream>

void testAdminAuthentication(DatabaseManager &dbManager) {
    std::cout << "Testing Admin Authentication..." << std::endl;

    // Add an admin user to test with (if not already present)
    std::string addAdminSQL = "INSERT OR IGNORE INTO admin (AdminName, AdminPassword) VALUES ('admin', 'admin123');";
    std::string errorMessage;
    dbManager.executeSQL(addAdminSQL, errorMessage);

    // Test login
    if (dbManager.validateAdmin("admin", "admin123")) {
        std::cout << "Admin login successful!" << std::endl;
        std::string token = dbManager.generateToken("admin");
        std::cout << "Generated Token: " << token << std::endl;
        std::cout << "Token validation: " << (dbManager.isTokenValid(token) ? "Valid" : "Invalid") << std::endl;
    } else {
        std::cout << "Admin login failed!" << std::endl;
    }
}

void testInstrumentOperations(DatabaseManager &dbManager) {
    std::cout << "\nTesting Instrument Operations..." << std::endl;

    // Add instruments
    if (dbManager.addInstrument("ISIN12345", "MIC123", "USD", "Active")) {
        std::cout << "Instrument added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add instrument!" << std::endl;
    }

    // Retrieve all instruments
    std::cout << "Retrieving all instruments:" << std::endl;
    auto instruments = dbManager.getInstruments();
    for (const auto &instrument : instruments) {
        std::cout << "ISIN: " << instrument.at("ISIN")
                  << ", MIC: " << instrument.at("MIC")
                  << ", Currency: " << instrument.at("Currency")
                  << ", Status: " << instrument.at("Status") << std::endl;
    }

    // Update an instrument
    if (dbManager.updateInstrument("ISIN12345", "Status", "Inactive")) {
        std::cout << "Instrument updated successfully!" << std::endl;
    } else {
        std::cout << "Failed to update instrument!" << std::endl;
    }

    // Retrieve instruments after update
    std::cout << "Retrieving all instruments after update:" << std::endl;
    instruments = dbManager.getInstruments();
    for (const auto &instrument : instruments) {
        std::cout << "ISIN: " << instrument.at("ISIN")
                  << ", MIC: " << instrument.at("MIC")
                  << ", Currency: " << instrument.at("Currency")
                  << ", Status: " << instrument.at("Status") << std::endl;
    }

    // Delete an instrument
    if (dbManager.deleteInstrument("ISIN12345")) {
        std::cout << "Instrument deleted successfully!" << std::endl;
    } else {
        std::cout << "Failed to delete instrument!" << std::endl;
    }

    // Retrieve instruments after deletion
    std::cout << "Retrieving all instruments after deletion:" << std::endl;
    instruments = dbManager.getInstruments();
    if (instruments.empty()) {
        std::cout << "No instruments found." << std::endl;
    } else {
        for (const auto &instrument : instruments) {
            std::cout << "ISIN: " << instrument.at("ISIN")
                      << ", MIC: " << instrument.at("MIC")
                      << ", Currency: " << instrument.at("Currency")
                      << ", Status: " << instrument.at("Status") << std::endl;
        }
    }
}

int main() {
    try {
        DatabaseManager dbManager("test_users.db");

        // Test Admin Authentication
        testAdminAuthentication(dbManager);

        // Test Instrument Operations
        testInstrumentOperations(dbManager);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
