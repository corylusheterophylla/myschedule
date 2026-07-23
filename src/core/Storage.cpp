#include "Storage.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

std::string Storage::getFilePath(const std::string& username) {
    return "data/" + username + "_tasks.json";
}

bool Storage::saveTasks(const std::string& username, const std::vector<Task>& tasks) {
    json j;
    for(const auto& t : tasks) {
        json taskJson;
        taskJson["id"] = t.id;
        taskJson["name"] = t.name;
        taskJson["startTime"] = t.startTime;
        taskJson["priority"] = t.priority;
        taskJson["category"] = t.category;
        taskJson["remindTime"] = t.remindTime;
        j.push_back(taskJson);
    }
    
    std::ofstream file(getFilePath(username));
    if(!file.is_open()) {
        std::cerr << "Error: Cannot open file for writing!" << std::endl;
        return false;
    }
    file << j.dump(4); // 缩进4空格，美观
    return true;
}

std::vector<Task> Storage::loadTasks(const std::string& username) {
    std::vector<Task> tasks;
    std::ifstream file(getFilePath(username));
    if(!file.is_open()) {
        return tasks; // 文件不存在则返回空列表
    }
    
    json j;
    file >> j;
    for(const auto& item : j) {
        Task t;
        t.id = item["id"];
        t.name = item["name"];
        t.startTime = item["startTime"];
        t.priority = static_cast<Priority>(item["priority"]);
        t.category = static_cast<Category>(item["category"]);
        t.remindTime = item["remindTime"];
        tasks.push_back(t);
    }
    return tasks;
}
