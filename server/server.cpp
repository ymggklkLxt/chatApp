#include "server.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

ServerWidget::ServerWidget(QWidget *parent) : QWidget(parent) {
    // 界面初始化
    portLineEdit = new QLineEdit(this);
    portLineEdit->setPlaceholderText("服务器端口");
    portLineEdit->setText("20002");

    startButton = new QPushButton("启动服务器", this);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(portLineEdit);
    topLayout->addWidget(startButton);

    clientListWidget = new QListWidget(this);
    clientListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    clientListWidget->setToolTip("已连接的客户端");

    dataInfoTextEdit = new QTextEdit(this);
    dataInfoTextEdit->setReadOnly(true);
    dataInfoTextEdit->setPlaceholderText("客户端发送的数据");

    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget(clientListWidget, 1);
    middleLayout->addWidget(dataInfoTextEdit, 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);

    // 信号槽连接
    connect(startButton, &QPushButton::clicked, this, &ServerWidget::startServer);

    // 网络初始化
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &ServerWidget::handleNewConnection);
}

void ServerWidget::startServer() {
    bool ok;
    quint16 port = portLineEdit->text().toUShort(&ok);

    if (!ok) {
        dataInfoTextEdit->append("无效的端口号");
        return;
    }

    if (tcpServer->isListening()) {
        tcpServer->close();
        startButton->setText("启动服务器");
        dataInfoTextEdit->append("服务器已停止监听");
    } else {
        if (tcpServer->listen(QHostAddress::Any, port)) {
            startButton->setText("停止服务器");
            dataInfoTextEdit->append(QString("服务器已启动，监听端口 %1").arg(port));
        } else {
            dataInfoTextEdit->append(QString("服务器启动失败: %1").arg(tcpServer->errorString()));
        }
    }
}

void ServerWidget::handleNewConnection() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    QString clientAddress = clientSocket->peerAddress().toString();
    quint16 clientPort = clientSocket->peerPort();
    QString clientInfo = QString("%1:%2").arg(clientAddress).arg(clientPort);

    dataInfoTextEdit->append(QString("新客户端连接: %1").arg(clientInfo));
    clientListWidget->addItem(clientInfo);

    // 连接客户端信号槽
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerWidget::handleReceivedData);
    connect(clientSocket, &QTcpSocket::disconnected, [this, clientSocket, clientInfo]() {
        dataInfoTextEdit->append(QString("客户端断开连接: %1").arg(clientInfo));

        // 从列表中移除客户端
        for (int i = 0; i < clientListWidget->count(); ++i) {
            if (clientListWidget->item(i)->text() == clientInfo) {
                delete clientListWidget->takeItem(i);
                break;
            }
        }

        clientSocket->deleteLater();
    });
}

void ServerWidget::handleReceivedData() {
    // 使用qobject_cast确保类型安全
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    QByteArray data = clientSocket->readAll();
    QString clientInfo = QString("%1:%2").arg(clientSocket->peerAddress().toString())
                             .arg(clientSocket->peerPort());

    dataInfoTextEdit->append(QString("[%1] 发送: %2").arg(clientInfo).arg(QString::fromUtf8(data)));
}
