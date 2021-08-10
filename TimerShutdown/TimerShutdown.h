#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TimerShutdown.h"
#include "timer.h"

class TimerShutdown : public QMainWindow
{
    Q_OBJECT

public:
    TimerShutdown(QWidget *parent = Q_NULLPTR);
public slots:
    void clickButton();

    void stopButton();

private:
    TimerThread* timerThread;

    Ui::TimerShutdownClass ui;
};
