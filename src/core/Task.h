#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

enum Priority { HIGH = 0, MEDIUM = 1, LOW = 2 };
enum Category { STUDY = 0, ENTERTAINMENT = 1, LIFE = 2 };

struct Task {
    int id;                 // 唯一ID（自动递增）
    std::string name;       // 任务名称
    std::string startTime;  // 格式: "2026-07-23 14:30"
    Priority priority;      // 默认 MEDIUM
    Category category;      // 默认 LIFE
    std::string remindTime; // 可为空

    // 构造函数：设置默认值
    Task() : id(0), priority(MEDIUM), category(LIFE) {}
    
    // 用于唯一性校验（任务名+开始时间）
    bool operator==(const Task& other) const {
        return name == other.name && startTime == other.startTime;
    }
};

#endif
