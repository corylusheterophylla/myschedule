#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <vector>
#include <string>
#include <mutex>
#include "Task.h"

class TaskManager {
private:
    std::string currentUser;
    std::vector<Task> tasks;
    int nextId;
    mutable std::mutex mtx;   // 关键：mutable 使 const 成员函数可以加锁

    int getMaxIdFromTasks() const;

public:
    TaskManager();

    void loadFromFile(const std::string& username);
    void saveToFile() const;

    bool addTask(const std::string& name,
                 const std::string& startTime,
                 Priority priority = MEDIUM,
                 Category category = LIFE,
                 const std::string& remindTime = "");

    bool deleteTask(int id);

    std::vector<Task> getTasksForDate(const std::string& date) const;
    std::vector<Task> getTasksForMonth(const std::string& yearMonth) const;
    std::vector<Task> getAllTasks() const;   // 返回副本

    bool isTaskExists(const std::string& name, const std::string& startTime) const;

    void checkReminders();
};

#endif
