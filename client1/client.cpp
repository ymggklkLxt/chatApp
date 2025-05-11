#include "client.h"
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QFontComboBox>
#include <QColorDialog>
#include <QToolButton>

ClientWidget::ClientWidget(QWidget *parent) : QWidget(parent), currentColor(Qt::black) {
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置粉蓝渐变背景色
    setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
                  "stop:0 rgb(255, 182, 193), stop:1 rgb(173, 216, 230));");

    // 标题栏
    QLabel *titleLabel = new QLabel("FaceBook 聊天", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color:rgb(202, 202, 202); background-color: rgba(0, 0, 0, 0.8); padding: 5px; border-radius: 5px;");
    QPushButton *closeButton = new QPushButton("X", this);
    closeButton->setFixedSize(20, 20);
    closeButton->setStyleSheet("background-color:rgb(255, 102, 102); color: white; border: none; border-radius: 10px;");
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeButton);

    // 群组列表
    clientListWidget = new QListWidget(this);
    clientListWidget->setFixedWidth(200);
    clientListWidget->setStyleSheet("border: 2px solid #CCCCCC; background-color: white; border-radius: 5px;");
    for (int i = 0; i < 5; ++i) {
        clientListWidget->addItem(QString("群组 %1").arg(i + 1));
    }

    // 聊天历史区域
    receivedDataTextEdit = new QTextEdit(this);
    receivedDataTextEdit->setReadOnly(true);
    receivedDataTextEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; background-color: white; border-radius: 5px;");

    // 聊天输入区域
    sendDataLineEdit = new QLineEdit(this);
    sendDataLineEdit->setPlaceholderText("输入消息");
    sendDataLineEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; border-radius: 5px; background-color: white;");
    sendButton = new QPushButton("发送", this);
    sendButton->setEnabled(false);
    sendButton->setStyleSheet("background-color:rgb(76, 155, 175); color: white; padding: 5px; border: none; border-radius: 5px;");

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(sendDataLineEdit);
    inputLayout->addWidget(sendButton);

    // 字体选择框
    fontComboBox = new QFontComboBox(this);
    connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &ClientWidget::changeFont);

    // 颜色选择按钮
    colorButton = new QPushButton("颜色", this);
    colorButton->setStyleSheet("background-color: black; color: white; border: none; padding: 5px; border-radius: 5px;");
    connect(colorButton, &QPushButton::clicked, this, &ClientWidget::changeColor);

    // 加粗按钮
    boldButton = new QToolButton(this);
    boldButton->setText("B");
    boldButton->setCheckable(true);
    boldButton->setStyleSheet("font-weight: bold; padding: 5px;");
    connect(boldButton, &QToolButton::clicked, this, &ClientWidget::toggleBold);

    // 斜体按钮
    italicButton = new QToolButton(this);
    italicButton->setText("I");
    italicButton->setCheckable(true);
    italicButton->setStyleSheet("font-style: italic; padding: 5px;");
    connect(italicButton, &QToolButton::clicked, this, &ClientWidget::toggleItalic);

    // 下划线按钮
    underlineButton = new QToolButton(this);
    underlineButton->setText("U");
    underlineButton->setCheckable(true);
    underlineButton->setStyleSheet("text-decoration: underline; padding: 5px;");
    connect(underlineButton, &QToolButton::clicked, this, &ClientWidget::toggleUnderline);

    QHBoxLayout *formatLayout = new QHBoxLayout;
    formatLayout->addWidget(fontComboBox);
    formatLayout->addWidget(colorButton);
    formatLayout->addWidget(boldButton);
    formatLayout->addWidget(italicButton);
    formatLayout->addWidget(underlineButton);

    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(receivedDataTextEdit);
    chatLayout->addLayout(formatLayout); // 添加字体和颜色选择控件
    chatLayout->addLayout(inputLayout);

    QHBoxLayout *mainContentLayout = new QHBoxLayout;
    mainContentLayout->addWidget(clientListWidget);
    mainContentLayout->addLayout(chatLayout);

    // 底部连接服务器区域
    QLineEdit *serverIPEdit = new QLineEdit(this);
    serverIPEdit->setPlaceholderText("服务器IP");
    serverIPEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; border-radius: 5px; background-color: white;");
    serverIPEdit->setFixedWidth(150);

    QLineEdit *serverPortEdit = new QLineEdit(this);
    serverPortEdit->setPlaceholderText("端口");
    serverPortEdit->setStyleSheet("padding: 5px; border: 2px solid #CCCCCC; border-radius: 5px; background-color: white;");
    serverPortEdit->setFixedWidth(80);

    QPushButton *connectButton = new QPushButton("连接", this);
    connectButton->setStyleSheet("background-color:rgb(76, 155, 175); color: white; padding: 5px; border: none; border-radius: 5px;");
    connect(connectButton, &QPushButton::clicked, [this, serverIPEdit, serverPortEdit]() {
        QString ip = serverIPEdit->text();
        quint16 port = serverPortEdit->text().toUShort();
        tcpSocket->connectToHost(ip, port);
        if (tcpSocket->waitForConnected(3000)) {
            receivedDataTextEdit->append("已连接到服务器");
        } else {
            QMessageBox::warning(this, "错误", "连接服务器失败");
        }
    });

    QHBoxLayout *connectionLayout = new QHBoxLayout;
    connectionLayout->addWidget(serverIPEdit);
    connectionLayout->addWidget(serverPortEdit);
    connectionLayout->addWidget(connectButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(mainContentLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(connectionLayout); // 添加连接服务器区域到底部
    mainLayout->addStretch();

    setLayout(mainLayout);

    // 创建TCP套接字
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientWidget::receiveData);
    connect(sendDataLineEdit, &QLineEdit::textChanged, this, &ClientWidget::onInputTextChanged);
    connect(sendButton, &QPushButton::clicked, this, &ClientWidget::sendMessage);
    connect(clientListWidget, &QListWidget::itemClicked, this, &ClientWidget::onGroupItemClicked);
}

void ClientWidget::sendMessage() {
    QString message = sendDataLineEdit->text();
    if (!message.isEmpty()) {
        QString style = QString("color:%1; font-family:%2;").arg(currentColor.name()).arg(fontComboBox->currentFont().family());
        if (isBold) style += " font-weight:bold;";
        if (isItalic) style += " font-style:italic;";
        if (isUnderline) style += " text-decoration:underline;";

        QString formattedMessage = QString("<span style='%1'>我: %2</span>").arg(style).arg(message);
        receivedDataTextEdit->append(formattedMessage);
        tcpSocket->write(message.toUtf8());
        sendDataLineEdit->clear();
    }
}

void ClientWidget::receiveData() {
    QByteArray data = tcpSocket->readAll();
    receivedDataTextEdit->append("服务器: " + QString::fromUtf8(data));
}

void ClientWidget::onInputTextChanged() {
    sendButton->setEnabled(!sendDataLineEdit->text().isEmpty());
}

void ClientWidget::onGroupItemClicked(QListWidgetItem *item) {
    receivedDataTextEdit->clear();
    receivedDataTextEdit->append("切换到群组: " + item->text());
}

void ClientWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void ClientWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void ClientWidget::changeFont(const QFont &font) {
    sendDataLineEdit->setFont(font);
}

void ClientWidget::changeColor() {
    QColor color = QColorDialog::getColor(currentColor, this, "选择颜色");
    if (color.isValid()) {
        currentColor = color;
        colorButton->setStyleSheet(QString("background-color: %1; color: white; border: none; padding: 5px; border-radius: 5px;")
                                       .arg(color.name()));
    }
}

void ClientWidget::toggleBold() {
    isBold = boldButton->isChecked();
}

void ClientWidget::toggleItalic() {
    isItalic = italicButton->isChecked();
}

void ClientWidget::toggleUnderline() {
    isUnderline = underlineButton->isChecked();
}

void ClientWidget::connectToServer() {
    QString ip = "127.0.0.1"; // 默认服务器IP
    quint16 port = 20002;     // 默认端口号

    tcpSocket->connectToHost(ip, port);
    if (tcpSocket->waitForConnected(3000)) {
        receivedDataTextEdit->append("已连接到服务器");
    } else {
        QMessageBox::warning(this, "错误", "连接服务器失败");
    }
}
