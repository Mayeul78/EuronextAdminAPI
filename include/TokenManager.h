#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H

#include <string>
#include <unordered_map>

class TokenManager {
public:
    std::string generateToken();
    bool isTokenValid(const std::string& token, std::string& role) const;
    void storeToken(const std::string& username, const std::string& token, const std::string& role);

private:
    std::unordered_map<std::string, std::pair<std::string, std::string>> tokens; // username -> (token, role)
};

#endif
