#include "server.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ServerWidget serverWidget;
    serverWidget.setWindowTitle("TCP服务器");
    serverWidget.resize(800, 600);
    serverWidget.show();

    return a.exec();
}
