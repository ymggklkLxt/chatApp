#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "client.h"

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override; // 添加鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;  // 添加鼠标移动事件

private slots:
    void handleLogin();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;

    QPoint dragPosition; // 用于记录鼠标拖动的位置
};

#endif // LOGIN_H
