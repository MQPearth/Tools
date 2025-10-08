#include "TimerShutdown.h"
#include "configmanager.h"
#include <QLabel>
#include <QString>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QtNetwork/QHostAddress>

#include "iostream"



TimerShutdown::TimerShutdown()
{
	ui.setupUi(this);

	connect(ui.startButton, &QPushButton::clicked, this, &TimerShutdown::clickButton);

	timer = new QTimer(this);

	connect(timer, &QTimer::timeout, this, &TimerShutdown::doTimer);
	connect(ui.lanAction, &QAction::triggered, this, &TimerShutdown::doLanAction);

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


void TimerShutdown::doLanAction() {
	QDialog dialog(this);
	dialog.setWindowTitle("设置监听地址");
	dialog.setMinimumWidth(240);

	QFormLayout formLayout(&dialog);

	QLineEdit* ipEdit = new QLineEdit(&dialog);
	ipEdit->setPlaceholderText("示例: 192.168.1.42:8080");
	

	formLayout.addRow("<p>地址: </p>", ipEdit);

	if (ipAddr.isEmpty()) {
		ipEdit->setText(ConfigManager::getNotifyUrl());
	}
	else {
		ipEdit->setText(ipAddr);
	}


	QDialogButtonBox buttonBox(Qt::Horizontal, &dialog);

	QPushButton* enableButton = buttonBox.addButton("启用", QDialogButtonBox::AcceptRole);
	QPushButton* disableButton = buttonBox.addButton("禁用", QDialogButtonBox::RejectRole);
	QPushButton* cancelButton = buttonBox.addButton(QDialogButtonBox::Cancel);
	cancelButton->setText("取消");

	QString lanText = ui.lanAction->text();
	if (lanText == "局域网监听[已启用]") {
		enableButton->setDisabled(true);
		disableButton->setDisabled(false);
	}
	else {
		enableButton->setDisabled(false);
		disableButton->setDisabled(true);
	}

	formLayout.addRow(&buttonBox);

	connect(enableButton, &QPushButton::clicked, &dialog, &QDialog::accept);
	connect(disableButton, &QPushButton::clicked, this, [this, &dialog]() {
		if (server != nullptr) {
			server->stopServer();
			delete server;
			server = nullptr;
		}
		dialog.reject();
		ui.lanAction->setText("局域网监听[未启用]");
		});
	connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

	while (1 && dialog.exec() == QDialog::Accepted) {

		QString ip = ipEdit->text();
		
		if (ip.isEmpty()) {
			ipEdit->setPlaceholderText("地址是必填项");
			continue;
		}
		QStringList parts = ip.trimmed().split(":");
		qDebug() << parts.size();
		if (parts.size() != 2) {
			ipEdit->setText("");
			ipEdit->setPlaceholderText("请输入正确的地址 (ip:端口)");
			continue;
		}

		QString ipAddress = parts[0];
		quint16 port = parts[1].toUShort();

		if (server != nullptr) {
			server->stopServer();
			delete server;
		}

		server = new HttpServer();
		QHostAddress address = QHostAddress(ipAddress);

		if (!server->listen(address, port)) {
			QMessageBox msgBox(this);
			msgBox.setWindowTitle("错误");
			msgBox.setText("端口已被使用或程序未被授权访问网络");
			msgBox.exec();
			continue;
		}

		ui.lanAction->setText("局域网监听[已启用]");

		ipAddr = ip.trimmed();
		ConfigManager::setNotifyUrl(ipAddr);
		ConfigManager::saveConfig();
		break;
	}

		
}