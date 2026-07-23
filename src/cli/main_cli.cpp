#include <iostream>
#include "UserManager.h"
#include "Storage.h"

int main() {
    std::cout << "====== MySchedule 阶段1 测试 ======" << std::endl;
    
    // 测试密码加密
    std::string pwd = "123456";
    std::string hash = UserManager::hashPassword(pwd);
    std::cout << "明文: " << pwd << std::endl;
    std::cout << "SHA256: " << hash << std::endl;
    std::cout << "验证结果: " << (UserManager::verifyPassword(pwd, hash) ? "成功" : "失败") << std::endl;
    
    // 测试存储
    std::vector<Task> tasks;
    Task t1; t1.id = 1; t1.name = "写大作业"; t1.startTime = "2026-07-23 20:00";
    tasks.push_back(t1);
    
    if(Storage::saveTasks("testuser", tasks)) {
        std::cout << "任务保存成功！" << std::endl;
    }
    
    auto loaded = Storage::loadTasks("testuser");
    std::cout << "加载任务数量: " << loaded.size() << std::endl;
    if(!loaded.empty()) {
        std::cout << "任务名: " << loaded[0].name << std::endl;
    }
    
    return 0;
}
