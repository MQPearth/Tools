#include "printer.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTimer>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>

#include <QDebug>

#include "info.h"
#include "readthread.h"

#include <QString>
#include <QMap>

QString intToChineseTraditional(QString number) {
	static const QMap<QString, QString> chineseTraditionalNumbers = {
		{"0", "零"}, {"1", "壹"}, {"2", "贰"}, {"3", "叁"}, {"4", "肆"}, {"5", "伍"},
		{"6", "陆"}, {"7", "柒"}, {"8", "捌"}, {"9", "玖"}
	};

	QString result;

	if (number.length() > 2) {
		result = "超出范围";
	}
	else if (number.length() == 2) {


		QString tens = number[0];
		QString ones = number[1];

		if (tens == "1" && ones == "0") {
			return "拾";
		}

		result += chineseTraditionalNumbers.value(tens) + "拾";

		if (ones != "0") {
			result += chineseTraditionalNumbers.value(ones);
		}
	}
	else {
		QString ones = number[0];
		result += chineseTraditionalNumbers.value(ones);
	}

	return result;
}


printer::printer(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.printButton->setEnabled(false);

	this->manager = new QNetworkAccessManager(this);

	connect(ui.readButton, &QPushButton::clicked, this, &printer::read);

	connect(ui.printButton, &QPushButton::clicked, this, &printer::print);

	connect(this, &printer::readSignal, this, &printer::handleReadSignal);
}

void printer::handleReadSignal(const QString& value) {
	ui.readButton->setEnabled(false);
	readthread::list.clear();
	if (r) {
		delete r;
	}

	r = new readthread(ui.label, value, ui.printButton, ui.readButton);
	r->start();
}

void printer::read() {

	ui.label->setText("读取中...请稍等");

	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("选择文件"));

	QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

	fileDialog->setDirectory(desktopPath);
	fileDialog->setNameFilter(tr("Excel Files (*.xls *.xlsx)"));

	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
	}

	if (fileNames.size() == 0) {
		ui.label->setText("请选择xls/xlsx文件");
		return;
	}

	emit readSignal(fileNames[0]);
}

void printer::print() {

	if (readthread::list.size() == 0) {
		QMessageBox::information(this, "提示", "请选择有效数据");
		return;
	}

	QPrinter printer;
	printer.setPageSize(QPrinter::A4);
	printer.setFullPage(true);

	QPrintDialog printDialog(&printer);

	if (printDialog.exec() == QDialog::Rejected) {
		return;
	}

	QPainter painter;
	painter.begin(&printer);

	int maxWidth = 760;

	int x = 12;
	int y = 40;
	int width = 250;
	int height = 158;
	for (const info& item : readthread::list) {
		QFont largeFontSmall;
		largeFontSmall.setPointSize(11);
		painter.setFont(largeFontSmall);

		painter.drawRect(x, y, width, height);
		painter.drawText(x + 110, y + 20, "编号: " + item.no);
		painter.drawText(x + 20, y + 50, "南村2024年春节福利分配小票");
		QString cntStr = intToChineseTraditional(item.cnt);
		painter.drawText(x + 100, y + 100, "户主: " + item.name + " " + cntStr + "人");
		painter.drawText(x + 100, y + 150, "日期" + item.date);

		QFont largeFontBig;
		largeFontBig.setPointSize(29);
		painter.setFont(largeFontBig);

		painter.drawText(x + 15, y + 110, item.type);

		x += (width + 10);
		if (x >= maxWidth) {
			x = 12;
			y += (height + 20);
		}
		if (y > 1100) {
			printer.newPage();
			x = 12;
			y = 40;
		}
	}

	painter.end();

	QMessageBox::information(this, "完成", "打印完成");
}

bool printer::checkLicense() {
	try
	{
		QNetworkRequest request;

		request.setUrl(QUrl("https://localhost/api/license/check"));

		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
		request.setRawHeader("token", "40b86e853e154d16babe42a61ad21b77");
		QJsonObject req;
		req["name"] = "ExcelPrinter";
		req["key"] = "88389f87-d671-40c5-8d4e-bd1cdacdb76a";
		QJsonDocument jsonDocument(req);
		QByteArray jsonData = jsonDocument.toJson();

		QNetworkReply* reply = this->manager->post(request, jsonData);

		QTimer timer;
		timer.setSingleShot(true);
		timer.setInterval(1000);
		QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
		QObject::connect(reply, &QNetworkReply::finished, &timer, &QTimer::stop);
		timer.start();

		QEventLoop loop;
		QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		loop.exec();

		qDebug() << reply->error();
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();

			QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

			if (jsonDoc.isNull()) {
				reply->deleteLater();
				return false;
			}

			auto result = jsonDoc.object();
			auto code = result["code"].toInt();
			if (code != 0) {
				return false;
			}

			auto data = result["data"].toBool();
			if (!data) {
				return false;
			}

			return true;
		}
		reply->deleteLater();
		return false;
	}
	catch (...) {
		return false;
	}
}

printer::~printer()
{}
