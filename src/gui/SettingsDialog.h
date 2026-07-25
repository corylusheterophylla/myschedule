#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void selectRingtone();
    void selectBackground();

private:
    QLineEdit *ringtoneEdit;
    QLineEdit *backgroundEdit;
    QPushButton *ringtoneBtn;
    QPushButton *backgroundBtn;
    void loadSettings();
    void saveSettings();
};

#endif
