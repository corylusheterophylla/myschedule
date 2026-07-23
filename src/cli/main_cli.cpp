#include <iostream>
#include <iomanip>
#include "TaskManager.h"
#include "UserManager.h"

void printTasks(const std::vector<Task>& tasks, const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
    if (tasks.empty()) {
        std::cout << "(无任务)" << std::endl;
        return;
    }
    std::cout << std::left 
              << std::setw(4) << "ID" 
              << std::setw(20) << "任务名" 
              << std::setw(20) << "开始时间" 
              << std::setw(10) << "优先级" 
              << std::setw(10) << "分类" 
              << std::setw(20) << "提醒时间" 
              << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for (const auto& t : tasks) {
        std::string prioStr = (t.priority == HIGH) ? "高" : (t.priority == MEDIUM ? "中" : "低");
        std::string catStr = (t.category == STUDY) ? "学习" : (t.category == ENTERTAINMENT ? "娱乐" : "生活");
        std::cout << std::left 
                  << std::setw(4) << t.id 
                  << std::setw(20) << t.name 
                  << std::setw(20) << t.startTime 
                  << std::setw(10) << prioStr 
                  << std::setw(10) << catStr 
                  << std::setw(20) << t.remindTime 
                  << std::endl;
    }
}

int main() {
    std::cout << "====== MySchedule 阶段2 测试 ======" << std::endl;
    std::cout << "测试用户: testuser" << std::endl;

    // 1. 创建管理器并加载数据（如果文件不存在则创建空列表）
    TaskManager tm;
    tm.loadFromFile("testuser");

    // 2. 添加任务（演示默认值和指定值）
    tm.addTask("完成大作业", "2026-07-23 22:00", HIGH, STUDY, "2026-07-23 21:50");
    tm.addTask("健身锻炼", "2026-07-24 07:00", MEDIUM, LIFE, "2026-07-24 06:50");
    tm.addTask("看书学习", "2026-07-23 15:00", LOW, STUDY, ""); // 使用默认分类LIFE？等一下，这里我传了STUDY
    // 让第三个任务更明显一点
    tm.addTask("买菜", "2026-07-24 18:00", LOW, LIFE, "");

    // 3. 测试唯一性约束（故意添加重复的，应该报错）
    std::cout << "\n[测试唯一性] 尝试添加重复任务 (完成大作业 + 2026-07-23 22:00):" << std::endl;
    tm.addTask("完成大作业", "2026-07-23 22:00");

    // 4. 按天查询
    auto tasks23 = tm.getTasksForDate("2026-07-23");
    printTasks(tasks23, "2026-07-23 的任务");

    // 5. 按月份查询
    auto tasksMonth = tm.getTasksForMonth("2026-07");
    printTasks(tasksMonth, "2026-07 所有任务");

    // 6. 删除任务（删除ID为1的任务）
    std::cout << "\n[删除] 删除任务 ID=1 ..." << std::endl;
    tm.deleteTask(1);

    // 7. 再次查看剩余任务
    auto remaining = tm.getAllTasks();
    printTasks(remaining, "删除后的全部任务");

    // 8. 测试密码加密（复用之前的）
    std::cout << "\n=== 密码验证测试 ===" << std::endl;
    std::string pwd = "123456";
    std::string hash = UserManager::hashPassword(pwd);
    std::cout << "密码明文: " << pwd << std::endl;
    std::cout << "SHA256: " << hash << std::endl;
    std::cout << "验证结果: " << (UserManager::verifyPassword(pwd, hash) ? "成功" : "失败") << std::endl;

    std::cout << "\n所有阶段2功能测试完成！数据已保存到 data/testuser_tasks.json" << std::endl;
    return 0;
}
