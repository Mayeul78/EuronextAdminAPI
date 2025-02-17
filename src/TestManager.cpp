#include "DatabaseManager.h"
#include <iostream>

void testUserAuthentication(DatabaseManager &dbManager) {
    std::cout << "Testing User Authentication..." << std::endl;

    std::string role;

    // Test Admin login
    if (dbManager.validateUser("admin", "admin123", role) && role == "Admin") {
        std::cout << "Admin login successful!" << std::endl;
    } else {
        std::cout << "Admin login failed!" << std::endl;
    }

    // Test Regular User login
    if (dbManager.validateUser("user1", "user123", role) && role == "User") {
        std::cout << "User login successful!" << std::endl;
    } else {
        std::cout << "User login failed!" << std::endl;
    }
}

void testInstrumentOperations(DatabaseManager &dbManager) {
    std::cout << "\nTesting Instrument Operations..." << std::endl;

    // Add instruments
    if (dbManager.addInstrument("ISIN12345", "MIC123", "USD", "Active")) {
        std::cout << "First Instrument added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add First instrument!" << std::endl;
    }

    if (dbManager.addInstrument("ISIN2222", "MIC124", "USD", "Active")) {
        std::cout << "Second Instrument added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add Second instrument!" << std::endl;
    }

    if (dbManager.addInstrument("ISIN2222", "MIC124", "USD", "Active")) {
        std::cout << "Duplicate Instrument added successfully! --> ERROR" << std::endl;
    } else {
        std::cout << "Failed to add Duplicate instrument! --> Good" << std::endl;
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

        // Test User Authentication
        testUserAuthentication(dbManager);

        // Test Instrument Operations
        testInstrumentOperations(dbManager);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
