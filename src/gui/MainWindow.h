#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QStatusBar>
#include <QCalendarWidget>
#include <QDockWidget>
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
    void onCalendarClicked(const QDate &date);
    void onTableDoubleClicked(int row, int column);  // 编辑任务

private:
    TaskManager tm;
    std::string currentUser;

    // UI components
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
    QCalendarWidget *calendar;
    QDockWidget *calendarDock;

    QString customRingtone;
    QString customBackground;

    void setupUI();
    void loadTasks(const QString& dateFilter = QString());
    void showStatusMessage(const QString& msg, int timeout = 2000);
    void playAlertSound();
    void applySettings();
    void editTask(int taskId);
};

#endif
