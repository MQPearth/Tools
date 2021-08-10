#include "timer.h"
#include "string"
#include <QDebug>


using namespace std;

TimerThread::TimerThread(int hour, int minute, int second, QLabel* lb)
{
	this->hour = hour;
	this->minute = minute;
	this->second = second;
	this->lb = lb;
}

void TimerThread::run()
{
	int totalSec = hour * 60 * 60 + minute * 60 + second;
	while (totalSec >= 0)
	{
		this->lb->setText(this->toText(totalSec));
		msleep(1000);
		totalSec--;
	}

	this->doWork();
}

QString TimerThread::toText(int sec) {

	int hour = sec / 60 / 60;

	int minute = (sec - 60 * 60 * hour) / 60;

	int second = sec % 60;

	QString label;
	string hourStr = to_string(hour);
	if (hourStr.length() < 2) {
		hourStr = "0" + hourStr;
	}
	string minuteStr = to_string(minute);
	if (minuteStr.length() < 2) {
		minuteStr = "0" + minuteStr;
	}
	string secondStr = to_string(second);
	if (secondStr.length() < 2) {
		secondStr = "0" + secondStr;
	}
	label.append(QString::fromStdString(hourStr))
		.append(":")
		.append(QString::fromStdString(minuteStr))
		.append(":")
		.append(QString::fromStdString(secondStr));

	return label;
}


void TimerThread::doWork() {
	system("shutdown -s -f -t 0");
}