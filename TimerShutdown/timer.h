#pragma once
#ifndef TIMER_H
#define TIMER_H
#include <QThread>
#include <QLabel>
#include <QString>

class TimerThread : public QThread
{
public:
    TimerThread(int hour, int minute, int second, QLabel* lb);
private:
    int hour;
    int minute;
    int second;
    QLabel* lb;

    virtual void run();

    QString toText(int sec);

    void doWork();
public:

signals:

public slots:

};
#endif 