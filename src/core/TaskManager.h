#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <string>
#include "Task.h"

class TaskManager {
private:
    std::string currentUser;        // 当前登录用户名
    std::vector<Task> tasks;        // 内存中的任务列表
    int nextId;                     // 下一个可用的ID

    // 内部方法：从向量中查找最大ID（用于初始化nextId）
    int getMaxIdFromTasks() const;

public:
    TaskManager();

    // 加载/保存（与Storage联动）
    void loadFromFile(const std::string& username);
    void saveToFile() const;        // 自动保存到 currentUser

    // 核心业务操作（返回bool表示是否成功）
    bool addTask(const std::string& name, 
                 const std::string& startTime, 
                 Priority priority = MEDIUM, 
                 Category category = LIFE, 
                 const std::string& remindTime = "");
    
    bool deleteTask(int id);
    
    // 查询接口
    std::vector<Task> getTasksForDate(const std::string& date) const;   // 格式: "2026-07-23"
    std::vector<Task> getTasksForMonth(const std::string& yearMonth) const; // 格式: "2026-07"
    const std::vector<Task>& getAllTasks() const { return tasks; }

    // 工具方法
    bool isTaskExists(const std::string& name, const std::string& startTime) const;
};

#endif
