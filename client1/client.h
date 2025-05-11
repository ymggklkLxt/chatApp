#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFontComboBox>
#include <QColorDialog>
#include <QToolButton> // 添加 QToolButton

class ClientWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override; // 添加鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;  // 添加鼠标移动事件

private slots:
    void connectToServer();
    void sendMessage();
    void receiveData();
    void onInputTextChanged();
    void onGroupItemClicked(QListWidgetItem *item);
    void changeFont(const QFont &font); // 改变字体
    void changeColor();                // 改变颜色
    void toggleBold();       // 切换加粗
    void toggleItalic();     // 切换斜体
    void toggleUnderline();  // 切换下划线

private:
    QLineEdit *sendDataLineEdit;
    QPushButton *sendButton;
    QTextEdit *receivedDataTextEdit;
    QListWidget *clientListWidget;
    QTcpSocket *tcpSocket;

    QFontComboBox *fontComboBox; // 字体选择框
    QPushButton *colorButton;    // 颜色选择按钮
    QToolButton *boldButton;       // 加粗按钮
    QToolButton *italicButton;     // 斜体按钮
    QToolButton *underlineButton;  // 下划线按钮
    QColor currentColor;         // 当前选择的颜色

    bool isBold = false;       // 是否加粗
    bool isItalic = false;     // 是否斜体
    bool isUnderline = false;  // 是否下划线

    QPoint dragPosition; // 用于记录鼠标拖动的位置
};

#endif // CLIENT_H
