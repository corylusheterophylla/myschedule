#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QStatusBar>
#include "TaskManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const std::string& username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTask();
    void onDeleteTask();
    void onRefresh();
    void onCheckReminders();  // QTimer 回调

private:
    TaskManager tm;
    std::string currentUser;

    // UI 组件
    QTableWidget *taskTable;
    QLineEdit *nameEdit;
    QLineEdit *startEdit;
    QLineEdit *remindEdit;
    QComboBox *priorityCombo;
    QComboBox *categoryCombo;
    QPushButton *addBtn;
    QPushButton *delBtn;
    QPushButton *refreshBtn;
    QTimer *reminderTimer;

    void setupUI();
    void loadTasks();            // 刷新表格
    void showStatusMessage(const QString& msg, int timeout = 2000);
};

#endif
