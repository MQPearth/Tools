#include "TimerShutdown.h"

#include <QLabel>
#include "timer.h"

TimerShutdown::TimerShutdown(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(clickButton()));
}

void TimerShutdown::clickButton() {
	int hour = ui.spinBoxHour->value();
	if (hour < 0) {
		hour = 0;
	}
	int minute = ui.spinBoxMinute->value();
	if (minute < 0) {
		minute = 0;
	}
	int second = ui.spinBoxSecond->value();
	if (second < 0) {
		second = 0;
	}

	//移除原有控件
	ui.spinBoxHour->deleteLater();
	ui.spinBoxMinute->deleteLater();
	ui.spinBoxSecond->deleteLater();
	ui.startButton->deleteLater();
	//添加新控件
	QLabel* timeLabel = new QLabel;
	QFont ft;
	ft.setPointSize(56);
	ft.setFamily(QString::fromUtf8("JetBrains Mono"));
	timeLabel->setFont(ft);
	ui.horizontalLayout->addWidget(timeLabel);

	QPushButton* stopButton = new QPushButton("停止");
	
	ui.verticalLayout->addWidget(stopButton);
	//开启新线程
	this->timerThread = new TimerThread(hour, minute, second, timeLabel);
	
	this->timerThread->start();

	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButton()));
}

void TimerShutdown::stopButton() {
	this->timerThread->terminate();
}
