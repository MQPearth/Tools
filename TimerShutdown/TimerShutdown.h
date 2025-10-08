#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TimerShutdown.h"
#include <QTimer>
#include <QLabel>
#include "HttpServer.h"

class TimerShutdown : public QMainWindow
{
    Q_OBJECT

public:
    TimerShutdown();

    void updateLabel();

public slots:
    void clickButton();

    void handleStopButton();

    void doTimer();

    void doLanAction();

private:
    int hour = 0;
    int minute = 0;
    int second = 0;
    bool state = false;

    QTimer* timer = nullptr;

    int totalSec = 0;

    QString ipAddr;
    HttpServer* server = nullptr;

    Ui::TimerShutdownClass ui;
    QPushButton* stopButton = nullptr;
    QLabel* timeLabel = nullptr;
};
