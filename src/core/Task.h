#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>

enum Priority { HIGH = 0, MEDIUM = 1, LOW = 2 };
enum Category { STUDY = 0, ENTERTAINMENT = 1, LIFE = 2 };

struct Task {
    int id;
    std::string name;
    std::string startTime;   // 格式: "YYYY-MM-DD HH:MM"
    Priority priority;
    Category category;
    std::string remindTime;  // 可为空，格式同 startTime
    bool reminded;           // 是否已提醒

    Task() : id(0), priority(MEDIUM), category(LIFE), reminded(false) {}
    
    bool operator==(const Task& other) const {
        return name == other.name && startTime == other.startTime;
    }
};

#endif
