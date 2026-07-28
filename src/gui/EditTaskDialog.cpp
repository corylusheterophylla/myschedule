#include "EditTaskDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>

EditTaskDialog::EditTaskDialog(const Task& task, QWidget *parent)
    : QDialog(parent), original(task) {
    setWindowTitle("Edit Task");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *form = new QFormLayout();

    nameEdit = new QLineEdit(QString::fromStdString(task.name), this);
    startEdit = new QDateTimeEdit(QDateTime::fromString(QString::fromStdString(task.startTime), "yyyy-MM-dd HH:mm"), this);
    startEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    startEdit->setCalendarPopup(true);

    remindEdit = new QDateTimeEdit(QDateTime::fromString(QString::fromStdString(task.remindTime), "yyyy-MM-dd HH:mm"), this);
    remindEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    remindEdit->setCalendarPopup(true);


    priorityCombo = new QComboBox(this);
    priorityCombo->addItems({"High", "Medium", "Low"});
    priorityCombo->setCurrentIndex(static_cast<int>(task.priority));

    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"Study", "Entertainment", "Life"});
    categoryCombo->setCurrentIndex(static_cast<int>(task.category));

    form->addRow("Name:", nameEdit);
    form->addRow("Start Time:", startEdit);
    form->addRow("Priority:", priorityCombo);
    form->addRow("Category:", categoryCombo);
    form->addRow("Remind Time:", remindEdit);

    mainLayout->addLayout(form);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);
}

Task EditTaskDialog::getTask() const {
    Task t = original;
    t.name = nameEdit->text().toStdString();
    t.startTime = startEdit->dateTime().toString("yyyy-MM-dd HH:mm").toStdString();
    t.remindTime = remindEdit->dateTime().toString("yyyy-MM-dd HH:mm").toStdString();
    t.priority = static_cast<Priority>(priorityCombo->currentIndex());
    t.category = static_cast<Category>(categoryCombo->currentIndex());
    return t;
}
