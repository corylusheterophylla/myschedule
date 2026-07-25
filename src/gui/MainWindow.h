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
#include <QSet>
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
    void onCheckReminders();
    void openSettings();

private:
    TaskManager tm;
    std::string currentUser;

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

    QString customRingtone;
    QString customBackground;

    QSet<int> remindedIds;   // 记录本次会话已弹出提醒的任务ID
    void setupUI();
    void loadTasks();
    void showStatusMessage(const QString& msg, int timeout = 2000);
    void playAlertSound();
    void applySettings();
};

#endif
