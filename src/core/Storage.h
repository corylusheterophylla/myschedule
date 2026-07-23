#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "Task.h"

class Storage {
public:
    // 保存任务列表到文件（路径: data/用户名_tasks.json）
    static bool saveTasks(const std::string& username, const std::vector<Task>& tasks);
    
    // 从文件加载任务列表
    static std::vector<Task> loadTasks(const std::string& username);
    
private:
    static std::string getFilePath(const std::string& username);
};

#endif
