#include "TaskManager.h"
#include "Storage.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

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
    std::lock_guard<std::mutex> lock(mtx);
    currentUser = username;
    tasks = Storage::loadTasks(username);
    nextId = getMaxIdFromTasks() + 1;
}

bool TaskManager::saveToFile() const {
    if (currentUser.empty()) {
        std::cerr << "Error: No user logged in!" << std::endl;
        return false;
    }
    std::lock_guard<std::mutex> lock(mtx);
    return Storage::saveTasks(currentUser, tasks);
}

bool TaskManager::isTaskExists(const std::string& name, const std::string& startTime) const {
    std::lock_guard<std::mutex> lock(mtx);
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
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& t : tasks) {
        if (t.name == name && t.startTime == startTime) {
            std::cerr << "Error: Task with same name and start time already exists!" << std::endl;
            return false;
        }
    }

    Task newTask;
    newTask.id = nextId++;
    newTask.name = name;
    newTask.startTime = startTime;
    newTask.priority = priority;
    newTask.category = category;
    newTask.remindTime = remindTime;
    newTask.reminded = false;



   auto backup = tasks;
    tasks.push_back(newTask);
    if (!Storage::saveTasks(currentUser, tasks)) {
        tasks = backup;
        std::cerr << "Error: Failed to save task to file, task not added." << std::endl;
        return false;
    }
    std::cout << "Task added successfully! ID: " << newTask.id << std::endl;
    return true;
}

bool TaskManager::deleteTask(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& t) {
        return t.id == id;
    });

    if (it == tasks.end()) {
        std::cerr << "Error: Task with ID " << id << " not found!" << std::endl;
        return false;
    }

    auto backup = tasks;
    tasks.erase(it);
    if (!Storage::saveTasks(currentUser, tasks)) {
        tasks = backup;
        std::cerr << "Error: Failed to save deletion to file, task not deleted." << std::endl;
        return false;
    }
    std::cout << "Task ID " << id << " deleted successfully!" << std::endl;
    return true;
}


std::vector<Task> TaskManager::getTasksForDate(const std::string& date) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Task> result;
    for (const auto& t : tasks) {
        if (t.startTime.substr(0, 10) == date) {
            result.push_back(t);
        }
    }
    std::sort(result.begin(), result.end(), [](const Task& a, const Task& b) {
        return a.startTime < b.startTime;
    });
    return result;
}

std::vector<Task> TaskManager::getTasksForMonth(const std::string& yearMonth) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<Task> result;
    for (const auto& t : tasks) {
        if (t.startTime.substr(0, 7) == yearMonth) {
            result.push_back(t);
        }
    }
    std::sort(result.begin(), result.end(), [](const Task& a, const Task& b) {
        return a.startTime < b.startTime;
    });
    return result;
}

std::vector<Task> TaskManager::getAllTasks() const {
    std::lock_guard<std::mutex> lock(mtx);
    return tasks;
}


bool TaskManager::checkReminders() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M");
    std::string currentTime = oss.str();

    std::lock_guard<std::mutex> lock(mtx);
    bool newReminder = false;
    for (auto& t : tasks) {
        if (!t.reminded && !t.remindTime.empty() && t.remindTime <= currentTime) {
            t.reminded = true;
            newReminder = true;
            // 打印到终端（保留CLI兼容）
            std::cout << "\n\033[1;31m[Reminder]\033[0m Task \"" << t.name 
                      << "\" (ID: " << t.id << ") reminder time " << t.remindTime << " has come!" << std::endl;
        }
    }
    if (newReminder) {
        Storage::saveTasks(currentUser, tasks);
    }
    return newReminder;
}
