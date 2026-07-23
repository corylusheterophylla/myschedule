#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>

class UserManager {
public:
    // 对密码进行 SHA256 哈希，返回十六进制字符串
    static std::string hashPassword(const std::string& password);
    
    // 验证密码（比较明文和存储的哈希值）
    static bool verifyPassword(const std::string& plain, const std::string& storedHash);
};

#endif
