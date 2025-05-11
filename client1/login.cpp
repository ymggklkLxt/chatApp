#include "login.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include <QCoreApplication> // 添加 QCoreApplication 头文件
#include <QMouseEvent>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置粉蓝渐变背景色
    setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
                  "stop:0 rgb(255, 182, 193), stop:1 rgb(173, 216, 230));");

    // 标题栏
    QLabel *titleLabel = new QLabel("FaceBook", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color:rgb(202, 202, 202); background-color: rgba(0, 0, 0, 0.8); padding: 5px; border-radius: 5px;");
    QPushButton *closeButton = new QPushButton("X", this);
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet("background-color:rgb(255, 102, 102); color: white; border: none; border-radius: 10px;");
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);

    // 用户名输入框
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");
    usernameEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; border-radius: 5px; background-color: white;");
    usernameEdit->setFixedHeight(50);

    // 密码输入框
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password); // 隐藏密码输入
    passwordEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; border-radius: 5px; background-color: white;");
    passwordEdit->setFixedHeight(50);

    // 登录按钮
    loginButton = new QPushButton("登录", this);
    loginButton->setStyleSheet("background-color:rgb(76, 155, 175); color: white; padding: 5px; border: none; border-radius: 5px;");
    loginButton->setFixedSize(66, 39);
    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::handleLogin);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(titleLayout);
    layout->addSpacing(60); // 增加标题与文本框之间的间距
    layout->addWidget(usernameEdit);
    layout->addSpacing(10);
    layout->addWidget(passwordEdit); // 添加密码输入框到布局
    layout->addSpacing(10);
    layout->addLayout(buttonLayout);
    layout->addStretch();

    setLayout(layout);
}

void LoginWidget::handleLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名不能为空！");
        return;
    }

    if (password != "123456") { // 验证密码
        QMessageBox::warning(this, "错误", "密码错误！");
        return;
    }

    // 登录成功，进入聊天界面
    ClientWidget *clientWidget = new ClientWidget();
    clientWidget->setWindowTitle(QString("聊天 - %1").arg(username));
    clientWidget->resize(600, 400);
    clientWidget->show();
    this->close();
}

void LoginWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}
