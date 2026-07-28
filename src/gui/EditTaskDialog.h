#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include "Task.h"

class EditTaskDialog : public QDialog {
    Q_OBJECT
public:
    explicit EditTaskDialog(const Task& task, QWidget *parent = nullptr);
    Task getTask() const;

private:
    QLineEdit *nameEdit;
    QDateTimeEdit *startEdit;
    QDateTimeEdit *remindEdit;
    QComboBox *priorityCombo;
    QComboBox *categoryCombo;
    Task original;
};

#endif
