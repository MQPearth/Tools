#pragma once

#include <QThread>
#include <QLabel>
#include <QPushButton>

#include "info.h"

class readthread : public QThread
{
public:
    readthread(QLabel* lb, QString path, QPushButton* btn, QPushButton* readBtn);
    static std::vector<info> list;
private:
    QLabel* lb;
    QString path;
    QPushButton* btn;
    QPushButton* readBtn;
    virtual void run();
};

