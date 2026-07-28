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
    mutable std::mutex mtx;

    int getMaxIdFromTasks() const;

public:
    TaskManager();

    void loadFromFile(const std::string& username);
    bool saveToFile() const;   // 关键修改：bool

    bool addTask(const std::string& name, 
                 const std::string& startTime, 
                 Priority priority = MEDIUM, 
                 Category category = LIFE, 
                 const std::string& remindTime = "");
    bool deleteTask(int id);
    
    std::vector<Task> getTasksForDate(const std::string& date) const;
    std::vector<Task> getTasksForMonth(const std::string& yearMonth) const;
    std::vector<Task> getAllTasks() const;

    bool isTaskExists(const std::string& name, const std::string& startTime) const;

    bool checkReminders();
};
#endif
