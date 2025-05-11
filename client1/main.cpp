#include "login.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LoginWidget loginWidget;
    loginWidget.setWindowTitle("登录");
    loginWidget.resize(400, 300);
    loginWidget.show();

    return a.exec();
}
