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

// 自定义登录对话框
class LoginDialog : public QDialog {
public:
    LoginDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("日程管理 - 登录");
        setModal(true);
        setFixedSize(300, 150);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QFormLayout *formLayout = new QFormLayout();

        userEdit = new QLineEdit(this);
        passEdit = new QLineEdit(this);
        passEdit->setEchoMode(QLineEdit::Password);

        formLayout->addRow("用户名:", userEdit);
        formLayout->addRow("密码:", passEdit);
        mainLayout->addLayout(formLayout);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        loginBtn = new QPushButton("登录/注册", this);
        cancelBtn = new QPushButton("取消", this);
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
            QMessageBox::warning(this, "输入错误", "用户名和密码不能为空！");
            return;
        }

        std::string u = username.toStdString();
        std::string p = password.toStdString();

        if (!UserManager::authenticate(u, p)) {
            // 未注册，询问是否注册
            auto reply = QMessageBox::question(this, "用户不存在", 
                                               "用户不存在或密码错误。是否注册新用户？",
                                               QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                if (UserManager::registerUser(u, p)) {
                    QMessageBox::information(this, "注册成功", "注册成功！欢迎使用！");
                    accept();
                } else {
                    QMessageBox::critical(this, "注册失败", "注册失败，请重试。");
                }
            } else {
                QMessageBox::warning(this, "取消", "已取消登录");
                reject();
            }
        } else {
            accept();  // 认证成功
        }
    }

private:
    QLineEdit *userEdit, *passEdit;
    QPushButton *loginBtn, *cancelBtn;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 显示登录对话框
    LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted) {
        return 0;  // 用户取消
    }

    std::string username = loginDlg.getUsername().toStdString();
    MainWindow mainWin(username);
    mainWin.show();

    return app.exec();
}
