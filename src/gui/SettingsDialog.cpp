#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("个性化设置");
    setMinimumSize(400, 200);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    ringtoneEdit = new QLineEdit(this);
    ringtoneEdit->setPlaceholderText("选择铃声文件 (.wav)");
    ringtoneBtn = new QPushButton("浏览...", this);
    connect(ringtoneBtn, &QPushButton::clicked, this, &SettingsDialog::selectRingtone);
    QHBoxLayout *ringtoneLayout = new QHBoxLayout();
    ringtoneLayout->addWidget(ringtoneEdit);
    ringtoneLayout->addWidget(ringtoneBtn);
    formLayout->addRow("铃声:", ringtoneLayout);
 backgroundEdit = new QLineEdit(this);
    backgroundEdit->setPlaceholderText("选择背景图片");
    backgroundBtn = new QPushButton("浏览...", this);
    connect(backgroundBtn, &QPushButton::clicked, this, &SettingsDialog::selectBackground);
    QHBoxLayout *bgLayout = new QHBoxLayout();
    bgLayout->addWidget(backgroundEdit);
    bgLayout->addWidget(backgroundBtn);
    formLayout->addRow("背景图片:", bgLayout);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("保存", this);
    QPushButton *cancelBtn = new QPushButton("取消", this);
    connect(okBtn, &QPushButton::clicked, this, &SettingsDialog::saveSettings);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    loadSettings();
}
void SettingsDialog::selectRingtone() {
    QString file = QFileDialog::getOpenFileName(this, "选择铃声文件", "", "音频文件 (*.wav *.ogg *.mp3)");
    if (!file.isEmpty()) {
        ringtoneEdit->setText(file);
    }
}

void SettingsDialog::selectBackground() {
    QString file = QFileDialog::getOpenFileName(this, "选择背景图片", "", "图片文件 (*.png *.jpg *.jpeg *.bmp)");
    if (!file.isEmpty()) {
        backgroundEdit->setText(file);
    }
}

void SettingsDialog::loadSettings() {
    QSettings settings("MySchedule", "Settings");
    ringtoneEdit->setText(settings.value("ringtone", "").toString());
    backgroundEdit->setText(settings.value("background", "").toString());
}

void SettingsDialog::saveSettings() {
    QSettings settings("MySchedule", "Settings");
    settings.setValue("ringtone", ringtoneEdit->text());
    settings.setValue("background", backgroundEdit->text());
    QMessageBox::information(this, "保存成功", "设置已保存，重启应用后生效。");
    accept();
}
