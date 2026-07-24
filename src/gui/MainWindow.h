#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QStatusBar>
#include <QSound>
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
    QDateTimeEdit *startDateTimeEdit;
    QDateTimeEdit *remindDateTimeEdit;
    QComboBox *priorityCombo;
    QComboBox *categoryCombo;
    QPushButton *addBtn;
    QPushButton *delBtn;
    QPushButton *refreshBtn;
    QTimer *reminderTimer;

    void setupUI();
    void loadTasks();
    void showStatusMessage(const QString& msg, int timeout = 2000);
    void playAlertSound();   // 播放提示音
};

#endif
