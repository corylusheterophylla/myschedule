#include "MainWindow.h"
#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QSound>
#include <QLineEdit>
#include <QFile>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QSettings>

MainWindow::MainWindow(const std::string& username, QWidget *parent)
    : QMainWindow(parent), currentUser(username) {
    tm.loadFromFile(username);
    setupUI();
    loadTasks();
    applySettings();

    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &MainWindow::onCheckReminders);
    reminderTimer->start(10000);
}

MainWindow::~MainWindow() {
    // 停止定时器，防止在析构时继续触发
    reminderTimer->stop();
    tm.saveToFile();
}

void MainWindow::applySettings() {
    QSettings settings("MySchedule", "Settings");
    customRingtone = settings.value("ringtone", "").toString();
    customBackground = settings.value("background", "").toString();

    if (!customBackground.isEmpty() && QFile::exists(customBackground)) {
        QString style = QString("QWidget { background-image: url(\"%1\"); background-repeat: no-repeat; background-position: center; }")
                        .arg(customBackground);
        this->setStyleSheet(style);
    } else {
        this->setStyleSheet("");
    }
}

void MainWindow::setupUI() {
    setWindowTitle(QString("日程管理系统 - %1").arg(QString::fromStdString(currentUser)));
    setMinimumSize(950, 650);

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *settingsMenu = menuBar->addMenu("设置");
    QAction *settingsAction = new QAction("个性化设置", this);
    settingsMenu->addAction(settingsAction);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    setMenuBar(menuBar);
  QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QGroupBox *addGroup = new QGroupBox("添加新任务", this);
    QFormLayout *formLayout = new QFormLayout(addGroup);

    nameEdit = new QLineEdit(this);
    startDateTimeEdit = new QDateTimeEdit(this);
    startDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    startDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    startDateTimeEdit->setCalendarPopup(true);

    remindDateTimeEdit = new QDateTimeEdit(this);
    remindDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    remindDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(60));
    remindDateTimeEdit->setCalendarPopup(true);

    priorityCombo = new QComboBox(this);
    priorityCombo->addItems({"高", "中", "低"});
    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"学习", "娱乐", "生活"});

    formLayout->addRow("任务名:", nameEdit);
    formLayout->addRow("开始时间:", startDateTimeEdit);
    formLayout->addRow("优先级:", priorityCombo);
    formLayout->addRow("分类:", categoryCombo);
    formLayout->addRow("提醒时间:", remindDateTimeEdit);

    addBtn = new QPushButton("添加任务", this);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddTask);
    formLayout->addRow("", addBtn);

 mainLayout->addWidget(addGroup);

    taskTable = new QTableWidget(this);
    taskTable->setColumnCount(7);
    taskTable->setHorizontalHeaderLabels({"ID", "任务名", "开始时间", "优先级", "分类", "提醒时间", "已提醒"});
    taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(taskTable);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    delBtn = new QPushButton("删除选中任务", this);
    refreshBtn = new QPushButton("刷新列表", this);
    btnLayout->addWidget(delBtn);
    btnLayout->addWidget(refreshBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(delBtn, &QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefresh);

    statusBar()->showMessage("就绪");
}

void MainWindow::loadTasks() {
    auto tasks = tm.getAllTasks();
    taskTable->setRowCount(static_cast<int>(tasks.size()));

    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& t = tasks[i];
        taskTable->setItem(i, 0, new QTableWidgetItem(QString::number(t.id)));
        taskTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(t.name)));
        taskTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(t.startTime)));

        QString prio = (t.priority == HIGH) ? "高" : (t.priority == MEDIUM ? "中" : "低");
        taskTable->setItem(i, 3, new QTableWidgetItem(prio));

        QString cat = (t.category == STUDY) ? "学习" : (t.category == ENTERTAINMENT ? "娱乐" : "生活");
        taskTable->setItem(i, 4, new QTableWidgetItem(cat));

        taskTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(t.remindTime)));
        taskTable->setItem(i, 6, new QTableWidgetItem(t.reminded ? "是" : "否"));
    }
}

void MainWindow::showStatusMessage(const QString& msg, int timeout) {
    statusBar()->showMessage(msg, timeout);
}

void MainWindow::playAlertSound() {
    if (!customRingtone.isEmpty() && QFile::exists(customRingtone)) {
        QSound::play(customRingtone);
        return;
    }

    bool played = false;
    QStringList soundPaths = {
        "/usr/share/sounds/ubuntu/stereo/phone-incoming-call.wav",
        "/usr/share/sounds/alsa/Rear_Left.wav",
        "/usr/share/sounds/freedesktop/stereo/complete.oga"
    };
    for (const QString& path : soundPaths) {
        if (QFile::exists(path)) {
            QSound::play(path);
            played = true;
            break;
        }
    }
 if (!played) {
        QApplication::beep();
    }
}

// ===== 槽函数 =====

void MainWindow::onAddTask() {
    QString name = nameEdit->text().trimmed();
    QString start = startDateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm");
    QString remind = remindDateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm");

    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "任务名不能为空！");
        return;
    }

    Priority prio = static_cast<Priority>(priorityCombo->currentIndex());
    Category cat = static_cast<Category>(categoryCombo->currentIndex());

    bool ok = tm.addTask(name.toStdString(), start.toStdString(), prio, cat, remind.toStdString());
    if (ok) {
        showStatusMessage("任务添加成功！");
        loadTasks();
        nameEdit->clear();
    } else {
        QMessageBox::critical(this, "添加失败", "任务名+开始时间已存在，或文件写入失败！");
    }
}
void MainWindow::onDeleteTask() {
    int row = taskTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "未选中", "请先选中要删除的任务行！");
        return;
    }
    int id = taskTable->item(row, 0)->text().toInt();
    auto reply = QMessageBox::question(this, "确认删除", 
                                       QString("确定要删除 ID 为 %1 的任务吗？").arg(id),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (tm.deleteTask(id)) {
            showStatusMessage(QString("任务 ID %1 删除成功").arg(id));
            loadTasks();
        } else {
            QMessageBox::critical(this, "删除失败", "任务不存在或文件写入失败！");
        }
    }
}

void MainWindow::onRefresh() {
    loadTasks();
    showStatusMessage("列表已刷新");
}

void MainWindow::onCheckReminders() {
    // 先调用核心检查（更新 reminded 状态并保存）
    tm.checkReminders();

    // 重新加载表格，获取最新数据
    loadTasks();

    // 检查是否有新提醒：遍历所有任务，找出 reminded == true 且 id 不在 remindedIds 中的
    bool hasNewReminder = false;
    auto tasks = tm.getAllTasks();  // 获取最新副本
    for (const Task& t : tasks) {
        if (t.reminded && !remindedIds.contains(t.id)) {
            // 新提醒！
            remindedIds.insert(t.id);
            hasNewReminder = true;
            // 可以在这里记录具体任务信息，但我们统一弹一次消息
        }
    }

    if (hasNewReminder) {
        playAlertSound();
        QMessageBox::information(this, "任务提醒", "有新的任务提醒！请查看任务列表。");
    }
}

void MainWindow::openSettings() {
    SettingsDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        applySettings();
    }
}
