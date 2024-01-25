#include "printer.h"
#include <QtWidgets/QApplication>

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    printer w;

    if (!w.checkLicense()) {
        QMessageBox msgBox(&w);
        msgBox.setWindowTitle("提示");
        msgBox.setText("模块加载失败");
        msgBox.exec();
        return -1;
    }

    w.show();
    return a.exec();
}
