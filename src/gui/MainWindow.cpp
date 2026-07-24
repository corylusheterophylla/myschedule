#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(const std::string& username, QWidget *parent)
    : QMainWindow(parent), currentUser(username) {
    // 加载用户任务
    tm.loadFromFile(username);
    setupUI();
    loadTasks();

    // 启动定时器，每 10 秒检查提醒
    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &MainWindow::onCheckReminders);
    reminderTimer->start(10000);  // 10 秒
}

MainWindow::~MainWindow() {
    // 停止定时器，自动保存（析构函数中 tm 会自动调用析构，但最好显式保存一次）
    tm.saveToFile();
}

void MainWindow::setupUI() {
    setWindowTitle(QString("日程管理系统 - %1").arg(QString::fromStdString(currentUser)));
    setMinimumSize(900, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // ===== 1. 添加任务区域 =====
    QGroupBox *addGroup = new QGroupBox("添加新任务", this);
    QFormLayout *formLayout = new QFormLayout(addGroup);

    nameEdit = new QLineEdit(this);
    startEdit = new QLineEdit(this);
    startEdit->setPlaceholderText("2026-07-24 15:00");
    remindEdit = new QLineEdit(this);
    remindEdit->setPlaceholderText("2026-07-24 14:50 (可选)");

    priorityCombo = new QComboBox(this);
    priorityCombo->addItems({"高", "中", "低"});
    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"学习", "娱乐", "生活"});

    formLayout->addRow("任务名:", nameEdit);
    formLayout->addRow("开始时间:", startEdit);
    formLayout->addRow("优先级:", priorityCombo);
    formLayout->addRow("分类:", categoryCombo);
    formLayout->addRow("提醒时间:", remindEdit);

    addBtn = new QPushButton("添加任务", this);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddTask);
    formLayout->addRow("", addBtn);

    mainLayout->addWidget(addGroup);

    // ===== 2. 任务列表表格 =====
    taskTable = new QTableWidget(this);
    taskTable->setColumnCount(7);
    taskTable->setHorizontalHeaderLabels({"ID", "任务名", "开始时间", "优先级", "分类", "提醒时间", "已提醒"});
    taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(taskTable);

    // ===== 3. 操作按钮栏 =====
    QHBoxLayout *btnLayout = new QHBoxLayout();
    delBtn = new QPushButton("删除选中任务", this);
    refreshBtn = new QPushButton("刷新列表", this);
    btnLayout->addWidget(delBtn);
    btnLayout->addWidget(refreshBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(delBtn, &QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefresh);

    // 状态栏
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

// ===== 槽函数 =====

void MainWindow::onAddTask() {
    QString name = nameEdit->text().trimmed();
    QString start = startEdit->text().trimmed();
    QString remind = remindEdit->text().trimmed();

    if (name.isEmpty() || start.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "任务名和开始时间不能为空！");
        return;
    }

    Priority prio = static_cast<Priority>(priorityCombo->currentIndex());
    Category cat = static_cast<Category>(categoryCombo->currentIndex());

    bool ok = tm.addTask(name.toStdString(), start.toStdString(), prio, cat, remind.toStdString());
    if (ok) {
        showStatusMessage("任务添加成功！");
        loadTasks();
        nameEdit->clear();
        startEdit->clear();
        remindEdit->clear();
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
    // 调用核心提醒检查（它会打印到终端，并更新内存状态）
    tm.checkReminders();

    // 重新加载表格以更新 "已提醒" 状态
    loadTasks();

    // 额外：如果有提醒，弹出消息框（增强体验）
    // 我们可以检查是否有新提醒，但由于 checkReminders 不返回列表，我们简单检查当前是否有 reminded 任务
    // 更可靠：修改 checkReminders 返回 bool，为了不改核心代码，我们用个小技巧：在调用 checkReminders 后重新加载，检查是否有已提醒的，但无法区分新旧。
    // 所以这里我们简单调用 loadTasks，不弹框，因为终端已经有打印了。
    // 如果想弹框，可以修改 TaskManager 添加 getLastReminded 接口，但为了保持纯洁性，我们现在不做。
    // 这样至少表格状态会更新。
}
