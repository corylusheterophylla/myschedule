#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>
#include "TaskManager.h"
#include "UserManager.h"

// 工具函数：字符串分割（支持引号）
std::vector<std::string> splitArgs(const std::string& str) {
    std::vector<std::string> args;
    std::istringstream iss(str);
    std::string token;
    while (iss >> std::quoted(token)) {
        args.push_back(token);
    }
    return args;
}

Priority parsePriority(const std::string& str) {
    if (str == "high" || str == "高") return HIGH;
    if (str == "low" || str == "低") return LOW;
    return MEDIUM;
}

Category parseCategory(const std::string& str) {
    if (str == "study" || str == "学习") return STUDY;
    if (str == "entertainment" || str == "娱乐") return ENTERTAINMENT;
    return LIFE;
}

void printTasks(const std::vector<Task>& tasks, const std::string& title = "") {
    if (!title.empty()) {
        std::cout << "\n=== " << title << " ===" << std::endl;
    }
    if (tasks.empty()) {
        std::cout << "(无任务)" << std::endl;
        return;
    }
    std::cout << std::left 
              << std::setw(5) << "ID" 
              << std::setw(18) << "任务名" 
              << std::setw(18) << "开始时间" 
              << std::setw(10) << "优先级" 
              << std::setw(10) << "分类" 
              << std::setw(18) << "提醒时间" 
              << std::setw(10) << "已提醒" 
              << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    for (const auto& t : tasks) {
        std::string prioStr = (t.priority == HIGH) ? "高" : (t.priority == MEDIUM ? "中" : "低");
        std::string catStr = (t.category == STUDY) ? "学习" : (t.category == ENTERTAINMENT ? "娱乐" : "生活");
        std::string remindedStr = t.reminded ? "是" : "否";
        std::cout << std::left 
                  << std::setw(5) << t.id 
                  << std::setw(18) << t.name 
                  << std::setw(18) << t.startTime 
                  << std::setw(10) << prioStr 
                  << std::setw(10) << catStr 
                  << std::setw(18) << t.remindTime 
                  << std::setw(10) << remindedStr
                  << std::endl;
    }
}

// 交互式 Shell（带后台提醒线程）
void runInteractiveShell(const std::string& username) {
    TaskManager tm;
    tm.loadFromFile(username);
    std::cout << "\n欢迎, " << username << "! 输入 help 查看命令列表。" << std::endl;

    std::atomic<bool> stopReminder(false);
    std::thread reminderThread([&tm, &stopReminder]() {
        while (!stopReminder.load()) {
            tm.checkReminders();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    });

    std::string line;
    while (true) {
        std::cout << "\n[myschedule]> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        auto args = splitArgs(line);
        if (args.empty()) continue;

        std::string cmd = args[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (cmd == "quit" || cmd == "exit" || cmd == "q") {
            std::cout << "正在退出，停止提醒线程..." << std::endl;
            stopReminder.store(true);
            if (reminderThread.joinable()) reminderThread.join();
            std::cout << "再见！" << std::endl;
            break;
        }
        else if (cmd == "help") {
            std::cout << "可用命令:\n"
                      << "  add <任务名> <开始时间> [优先级] [分类] [提醒时间]\n"
                      << "     例: add \"写大作业\" \"2026-07-24 14:00\" high study \"2026-07-24 13:50\"\n"
                      << "  del <任务ID>\n"
                      << "  show day <日期>       (例: show day 2026-07-24)\n"
                      << "  show month <年月>     (例: show month 2026-07)\n"
                      << "  list                  显示全部任务\n"
                      << "  quit / exit           退出程序\n";
        }
        else if (cmd == "add") {
            if (args.size() < 3) {
                std::cout << "用法: add <任务名> <开始时间> [优先级] [分类] [提醒时间]" << std::endl;
                continue;
            }
            std::string name = args[1];
            std::string startTime = args[2];
            Priority prio = (args.size() > 3) ? parsePriority(args[3]) : MEDIUM;
            Category cat = (args.size() > 4) ? parseCategory(args[4]) : LIFE;
            std::string remind = (args.size() > 5) ? args[5] : "";

            tm.addTask(name, startTime, prio, cat, remind);
        }
        else if (cmd == "del") {
            if (args.size() < 2) {
                std::cout << "用法: del <任务ID>" << std::endl;
                continue;
            }
            int id = std::stoi(args[1]);
            tm.deleteTask(id);
        }
        else if (cmd == "show") {
            if (args.size() < 3) {
                std::cout << "用法: show day <日期> 或 show month <年月>" << std::endl;
                continue;
            }
            std::string type = args[1];
            std::string time = args[2];
            if (type == "day") {
                auto tasks = tm.getTasksForDate(time);
                printTasks(tasks, time + " 的任务");
            } else if (type == "month") {
                auto tasks = tm.getTasksForMonth(time);
                printTasks(tasks, time + " 的任务");
            } else {
                std::cout << "未知的 show 类型，使用 day 或 month" << std::endl;
            }
        }
        else if (cmd == "list") {
            auto tasks = tm.getAllTasks();
            // 由于 getAllTasks 返回 const 引用，但内部有锁，返回的引用在锁释放后可能被修改？实际上 get 函数返回引用，但调用后锁已释放，我们复制一份更安全。
            std::vector<Task> copy = tasks;
            printTasks(copy, "全部任务");
        }
        else {
            std::cout << "未知命令: " << cmd << "，输入 help 查看帮助。" << std::endl;
        }
    }
}

// ============ 主函数入口 ============
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "MySchedule 日程管理系统 v1.0" << std::endl;
        std::cout << "用法:\n"
                  << "  交互式模式: " << argv[0] << " run\n"
                  << "  一次性命令: " << argv[0] << " <用户名> <密码> addtask|deltask|showtask ...\n"
                  << "  输入 " << argv[0] << " run 然后根据提示操作。" << std::endl;
        return 0;
    }

    std::string arg1 = argv[1];

    // ----- 模式 A：交互式 Shell -----
    if (arg1 == "run") {
        std::string username, password;
        std::cout << "用户名: ";
        std::cin >> username;
        std::cout << "密码: ";
        std::cin >> password;

        // 尝试认证
        if (!UserManager::authenticate(username, password)) {
            std::cout << "用户不存在或密码错误。是否注册新用户？(y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                if (UserManager::registerUser(username, password)) {
                    std::cout << "注册成功！欢迎, " << username << "!" << std::endl;
                } else {
                    std::cout << "注册失败，请重试。" << std::endl;
                    return 1;
                }
            } else {
                std::cout << "认证失败，退出。" << std::endl;
                return 1;
            }
        } else {
            std::cout << "登录成功！" << std::endl;
        }

        runInteractiveShell(username);
        return 0;
    }

    // ----- 模式 B：一次性命令（需要至少 4 个参数：用户名 密码 命令 ...）-----
    if (argc < 4) {
        std::cerr << "一次性命令需要提供: 用户名 密码 命令" << std::endl;
        return 1;
    }

    std::string username = argv[1];
    std::string password = argv[2];
    std::string command = argv[3];

    if (!UserManager::authenticate(username, password)) {
        std::cout << "用户未注册，正在自动注册..." << std::endl;
        if (!UserManager::registerUser(username, password)) {
            std::cerr << "注册失败！" << std::endl;
            return 1;
        }
        std::cout << "新用户注册成功！" << std::endl;
    }

    TaskManager tm;
    tm.loadFromFile(username);

    if (command == "addtask" || command == "add") {
        if (argc < 6) {
            std::cerr << "用法: addtask <任务名> <开始时间> [优先级] [分类] [提醒时间]" << std::endl;
            return 1;
        }
        std::string name = argv[4];
        std::string startTime = argv[5];
        Priority prio = (argc > 6) ? parsePriority(argv[6]) : MEDIUM;
        Category cat = (argc > 7) ? parseCategory(argv[7]) : LIFE;
        std::string remind = (argc > 8) ? argv[8] : "";
        tm.addTask(name, startTime, prio, cat, remind);
    }
    else if (command == "deltask" || command == "del") {
        if (argc < 5) {
            std::cerr << "用法: deltask <任务ID>" << std::endl;
            return 1;
        }
        int id = std::stoi(argv[4]);
        tm.deleteTask(id);
    }
    else if (command == "showtask" || command == "show") {
        if (argc < 6) {
            std::cerr << "用法: showtask day <日期> 或 showtask month <年月>" << std::endl;
            return 1;
        }
        std::string type = argv[4];
        std::string time = argv[5];
        if (type == "day") {
            auto tasks = tm.getTasksForDate(time);
            printTasks(tasks, time + " 的任务");
        } else if (type == "month") {
            auto tasks = tm.getTasksForMonth(time);
            printTasks(tasks, time + " 的任务");
        } else {
            std::cerr << "无效的 show 类型，使用 day 或 month" << std::endl;
        }
    }
    else {
        std::cerr << "未知命令: " << command << std::endl;
        return 1;
    }

    return 0;
}
