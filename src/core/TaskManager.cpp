#include "TaskManager.h"
#include "Storage.h"
#include <algorithm>
#include <iostream>

TaskManager::TaskManager() : currentUser(""), nextId(1) {}

int TaskManager::getMaxIdFromTasks() const {
    if (tasks.empty()) return 0;
    int maxId = 0;
    for (const auto& t : tasks) {
        if (t.id > maxId) maxId = t.id;
    }
    return maxId;
}

void TaskManager::loadFromFile(const std::string& username) {
    currentUser = username;
    tasks = Storage::loadTasks(username);
    nextId = getMaxIdFromTasks() + 1;
}

void TaskManager::saveToFile() const {
    if (currentUser.empty()) {
        std::cerr << "Error: No user logged in!" << std::endl;
        return;
    }
    Storage::saveTasks(currentUser, tasks);
}

bool TaskManager::isTaskExists(const std::string& name, const std::string& startTime) const {
    for (const auto& t : tasks) {
        if (t.name == name && t.startTime == startTime) {
            return true;
        }
    }
    return false;
}

bool TaskManager::addTask(const std::string& name, 
                          const std::string& startTime, 
                          Priority priority, 
                          Category category, 
                          const std::string& remindTime) {
    // 1. 校验唯一性（任务名 + 开始时间）
    if (isTaskExists(name, startTime)) {
        std::cerr << "Error: Task with same name and start time already exists!" << std::endl;
        return false;
    }

    // 2. 构造任务
    Task newTask;
    newTask.id = nextId++;
    newTask.name = name;
    newTask.startTime = startTime;
    newTask.priority = priority;
    newTask.category = category;
    newTask.remindTime = remindTime;

    // 3. 添加到内存
    tasks.push_back(newTask);

    // 4. 自动保存到文件
    saveToFile();
    
    std::cout << "Task added successfully! ID: " << newTask.id << std::endl;
    return true;
}

bool TaskManager::deleteTask(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
        return t.id == id;
    });

    if (it == tasks.end()) {
        std::cerr << "Error: Task with ID " << id << " not found!" << std::endl;
        return false;
    }

    tasks.erase(it);
    saveToFile();  // 自动保存
    std::cout << "Task ID " << id << " deleted successfully!" << std::endl;
    return true;
}

std::vector<Task> TaskManager::getTasksForDate(const std::string& date) const {
    std::vector<Task> result;
    for (const auto& t : tasks) {
        // startTime 格式是 "YYYY-MM-DD HH:MM"，取前10位比较
        if (t.startTime.substr(0, 10) == date) {
            result.push_back(t);
        }
    }
    // 按开始时间排序（字典序对时间格式有效）
    std::sort(result.begin(), result.end(), [](const Task& a, const Task& b) {
        return a.startTime < b.startTime;
    });
    return result;
}

std::vector<Task> TaskManager::getTasksForMonth(const std::string& yearMonth) const {
    std::vector<Task> result;
    for (const auto& t : tasks) {
        // startTime 格式是 "YYYY-MM-DD HH:MM"，取前7位比较
        if (t.startTime.substr(0, 7) == yearMonth) {
            result.push_back(t);
        }
    }
    std::sort(result.begin(), result.end(), [](const Task& a, const Task& b) {
        return a.startTime < b.startTime;
    });
    return result;
}
