#include "TokenManager.h"
#include <random>

// Generate a random token
std::string TokenManager::generateToken() {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> dist(0, sizeof(charset) - 2);

    std::string token;
    for (int i = 0; i < 32; ++i) {
        token += charset[dist(generator)];
    }
    return token;
}

// Store token and role
void TokenManager::storeToken(const std::string& username, const std::string& token, const std::string& role) {
    tokens[username] = {token, role};
}

// Validate token and retrieve role
bool TokenManager::isTokenValid(const std::string& token, std::string& role) const {
    for (const auto& pair : tokens) {
        if (pair.second.first == token) {
            role = pair.second.second;
            return true;
        }
    }
    return false;
}
