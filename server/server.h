#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

// 添加缺失的Qt Widgets头文件
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>

class ServerWidget : public QWidget {
    Q_OBJECT
public:
    ServerWidget(QWidget *parent = nullptr);
private slots:
    void startServer();
    void handleNewConnection();
    void handleReceivedData();
private:
    QLineEdit *portLineEdit;
    QPushButton *startButton;
    QListWidget *clientListWidget;
    QTextEdit *dataInfoTextEdit;
    QTcpServer *tcpServer;
};

#endif
