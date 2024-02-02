#include "TimerShutdown.h"
#include <QLabel>
#include <QString>
#include "iostream"


TimerShutdown::TimerShutdown()
{
	ui.setupUi(this);

	connect(ui.startButton, &QPushButton::clicked, this, &TimerShutdown::clickButton);

	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &TimerShutdown::doTimer);

	timer->setInterval(1000);
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

	ui.spinBoxHour->deleteLater();
	ui.spinBoxMinute->deleteLater();
	ui.spinBoxSecond->deleteLater();
	ui.startButton->deleteLater();

	this->timeLabel = new QLabel;

	this->timeLabel->setAlignment(Qt::AlignCenter);

	QFont ft;
	ft.setPointSize(56);
	ft.setFamily(QString::fromUtf8("JetBrains Mono"));
	timeLabel->setFont(ft);
	ui.horizontalLayout->addWidget(timeLabel);

	stopButton = new QPushButton("停止");
	
	ui.verticalLayout->addWidget(stopButton);

	connect(stopButton, &QPushButton::clicked, this, &TimerShutdown::handleStopButton);

	totalSec = hour * 60 * 60 + minute * 60 + second;

	updateLabel();

	this->timer->start();
}

void TimerShutdown::updateLabel() {
	int hour = totalSec / 60 / 60;

	int minute = (totalSec - 60 * 60 * hour) / 60;

	int second = totalSec % 60;

	QString hourStr = QString::number(hour);
	if (hourStr.length() < 2) {
		hourStr = "0" + hourStr;
	}
	QString minuteStr = QString::number(minute);
	if (minuteStr.length() < 2) {
		minuteStr = "0" + minuteStr;
	}
	QString secondStr = QString::number(second);
	if (secondStr.length() < 2) {
		secondStr = "0" + secondStr;
	}

	QString label = hourStr + ":" + minuteStr + ":" + secondStr;
	this->timeLabel->setText(label);

}

void TimerShutdown::doTimer() {
	totalSec--;

	updateLabel();

	if (!totalSec) {
		std::cout << "te" << std::endl;
		system("shutdown -s -f -t 0");
	}
}

void TimerShutdown::handleStopButton() {
	state = !state;
	if (state) {
		timer->stop();
		stopButton->setText("开启");
	}
	else {
		timer->start();
		stopButton->setText("停止");
	}
}
