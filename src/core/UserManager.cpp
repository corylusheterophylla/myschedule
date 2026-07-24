#include "UserManager.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

std::string UserManager::usersFilePath = "data/users.json";

std::unordered_map<std::string, std::string> UserManager::loadUsers() {
    std::unordered_map<std::string, std::string> users;
    std::ifstream file(usersFilePath);
    if (!file.is_open()) return users;
    
    json j;
    file >> j;
    for (auto& [key, value] : j.items()) {
        users[key] = value;
    }
    return users;
}

void UserManager::saveUsers(const std::unordered_map<std::string, std::string>& users) {
    json j(users);
    std::ofstream file(usersFilePath);
    if (file.is_open()) {
        file << j.dump(4);
    } else {
        std::cerr << "Warning: Cannot save users file!" << std::endl;
    }
}

std::string UserManager::hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool UserManager::verifyPassword(const std::string& plain, const std::string& storedHash) {
    return hashPassword(plain) == storedHash;
}

bool UserManager::registerUser(const std::string& username, const std::string& password) {
    auto users = loadUsers();
    if (users.find(username) != users.end()) {
        return false; // 用户已存在
    }
    users[username] = hashPassword(password);
    saveUsers(users);
    return true;
}

bool UserManager::authenticate(const std::string& username, const std::string& password) {
    auto users = loadUsers();
    auto it = users.find(username);
    if (it == users.end()) {
        return false; // 用户不存在
    }
    return verifyPassword(password, it->second);
}
