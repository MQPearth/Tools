#include "TimerShutdown.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerShutdown w;
    w.show();
    return a.exec();
}
