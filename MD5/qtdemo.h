#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtdemo.h"

class qtdemo : public QMainWindow
{
    Q_OBJECT

public:
    qtdemo(QWidget* parent = Q_NULLPTR);
public slots:
    void clickButton();
    void clickCalButton();
    void copyValue(QListWidgetItem* item);
    void clearData();

private:
    Ui::qtdemoClass ui;
};
