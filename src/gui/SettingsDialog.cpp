#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Preferences");
    setMinimumSize(400, 200);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    ringtoneEdit = new QLineEdit(this);
    ringtoneEdit->setPlaceholderText("Select ringtone (.wav/.ogg/.mp3)");
    ringtoneBtn = new QPushButton("Browse...", this);
    connect(ringtoneBtn, &QPushButton::clicked, this, &SettingsDialog::selectRingtone);
    QHBoxLayout *ringtoneLayout = new QHBoxLayout();
    ringtoneLayout->addWidget(ringtoneEdit);
    ringtoneLayout->addWidget(ringtoneBtn);
    formLayout->addRow("Ringtone:", ringtoneLayout);

  backgroundEdit = new QLineEdit(this);
    backgroundEdit->setPlaceholderText("Select background image");
    backgroundBtn = new QPushButton("Browse...", this);
    connect(backgroundBtn, &QPushButton::clicked, this, &SettingsDialog::selectBackground);
    QHBoxLayout *bgLayout = new QHBoxLayout();
    bgLayout->addWidget(backgroundEdit);
    bgLayout->addWidget(backgroundBtn);
    formLayout->addRow("Background:", bgLayout);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("Save", this);
    QPushButton *cancelBtn = new QPushButton("Cancel", this);
    connect(okBtn, &QPushButton::clicked, this, &SettingsDialog::saveSettings);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    loadSettings();
}

void SettingsDialog::selectRingtone() {
    QString file = QFileDialog::getOpenFileName(this, "Select Ringtone", "", "Audio files (*.wav *.ogg *.mp3)");
    if (!file.isEmpty()) {
        ringtoneEdit->setText(file);
    }
}

void SettingsDialog::selectBackground() {
    QString file = QFileDialog::getOpenFileName(this, "Select Background", "", "Image files (*.png *.jpg *.jpeg *.bmp)");
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
    QMessageBox::information(this, "Saved", "Settings saved. Restart to apply background.");
    accept();
}
