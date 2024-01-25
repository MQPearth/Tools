#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QNetworkAccessManager>

#include "ui_printer.h"

#include "info.h"
#include "readthread.h"

class printer : public QMainWindow
{
	Q_OBJECT

public:
	printer(QWidget* parent = nullptr);
	~printer();

	void read();

	void print();

	bool checkLicense();
	
signals:
	void readSignal(const QString& value);

public slots:
	void handleReadSignal(const QString& value);

private:
	Ui::printerClass ui;
	readthread* r = nullptr;
	QNetworkAccessManager* manager = nullptr;
};