#include "Storage.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>   // C++17

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string Storage::getFilePath(const std::string& username) {
    return "data/" + username + "_tasks.json";
}

bool Storage::saveTasks(const std::string& username, const std::vector<Task>& tasks) {
    // 确保 data/ 目录存在
    fs::path dir = "data";
    if (!fs::exists(dir)) {
        if (!fs::create_directory(dir)) {
            std::cerr << "Error: Cannot create data directory!" << std::endl;
            return false;
        }
    }

    json j;
    for(const auto& t : tasks) {
        json taskJson;
        taskJson["id"] = t.id;
        taskJson["name"] = t.name;
        taskJson["startTime"] = t.startTime;
        taskJson["priority"] = t.priority;
        taskJson["category"] = t.category;
        taskJson["remindTime"] = t.remindTime;
        taskJson["reminded"] = t.reminded;
        j.push_back(taskJson);
    }
    
    std::ofstream file(getFilePath(username));
    if(!file.is_open()) {
        std::cerr << "Error: Cannot open file for writing!" << std::endl;
        return false;
    }
    file << j.dump(4);
    return true;
}

std::vector<Task> Storage::loadTasks(const std::string& username) {
    std::vector<Task> tasks;
    std::ifstream file(getFilePath(username));
    if(!file.is_open()) {
        return tasks;
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
        t.remindTime = item.value("remindTime", "");
        t.reminded = item.value("reminded", false);
        tasks.push_back(t);
    }
    return tasks;
}
