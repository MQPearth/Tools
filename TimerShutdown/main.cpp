#include "TimerShutdown.h"
#include "configmanager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerShutdown w;
    ConfigManager::loadConfig();
    w.show();
    return a.exec();
}
