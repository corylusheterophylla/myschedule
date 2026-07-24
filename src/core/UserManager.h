#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <unordered_map>

class UserManager {
private:
    static std::string usersFilePath;
    static std::unordered_map<std::string, std::string> loadUsers();
    static void saveUsers(const std::unordered_map<std::string, std::string>& users);

public:
    // 对密码进行 SHA256 哈希
    static std::string hashPassword(const std::string& password);
    
    // 验证密码
    static bool verifyPassword(const std::string& plain, const std::string& storedHash);
    
    // 注册新用户（若已存在则返回 false）
    static bool registerUser(const std::string& username, const std::string& password);
    
    // 用户认证（返回 true 表示用户名存在且密码正确）
    static bool authenticate(const std::string& username, const std::string& password);
};
#endif
