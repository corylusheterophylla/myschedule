#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include "UserManager.h"
#include "MainWindow.h"
#include <iostream>

class LoginDialog : public QDialog {
public:
    LoginDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Schedule Manager - Login");
        setModal(true);
        setFixedSize(300, 150);

     QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userEdit = new QLineEdit(this);
        passEdit = new QLineEdit(this);
        passEdit->setEchoMode(QLineEdit::Password);

        formLayout->addRow("Username:", userEdit);
        formLayout->addRow("Password:", passEdit);
        mainLayout->addLayout(formLayout);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        loginBtn = new QPushButton("Login / Register", this);
        cancelBtn = new QPushButton("Cancel", this);
        btnLayout->addWidget(loginBtn);
        btnLayout->addWidget(cancelBtn);
        mainLayout->addLayout(btnLayout);

        connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    }

    QString getUsername() const { return userEdit->text(); }
    QString getPassword() const { return passEdit->text(); }

private slots:
    void onLogin() {
        QString username = userEdit->text().trimmed();
        QString password = passEdit->text().trimmed();
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Username and password cannot be empty!");
            return;
        }

        std::string u = username.toStdString();
        std::string p = password.toStdString();

        if (!UserManager::authenticate(u, p)) {
            auto reply = QMessageBox::question(this, "User Not Found",
                                               "User doesn't exist or wrong password. Register new user?",
                                               QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                if (UserManager::registerUser(u, p)) {
                    QMessageBox::information(this, "Success", "Registration successful! Welcome!");
                    accept();
                } else {
                    QMessageBox::critical(this, "Registration Failed", "Registration failed, please retry.");
                }
            } else {
                QMessageBox::warning(this, "Cancelled", "Login cancelled.");
                reject();
            }
        } else {
            accept();
        }
    }

private:
    QLineEdit *userEdit, *passEdit;
    QPushButton *loginBtn, *cancelBtn;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted) {
        return 0;
    }

    std::string username = loginDlg.getUsername().toStdString();
    MainWindow mainWin(username);
    mainWin.show();

    return app.exec();
}
