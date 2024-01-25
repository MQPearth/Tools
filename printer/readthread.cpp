#include "readthread.h"

#include <QAxObject>

#include "info.h"

std::vector<info> readthread::list;

readthread::readthread(QLabel* lb, QString path, QPushButton* btn, QPushButton* readBtn) {
	this->lb = lb;
	this->path = path;
	this->btn = btn;
	this->readBtn = readBtn;
}

void readthread::run() {

	QAxObject excel("Excel.Application", 0);
	excel.setProperty("Visible", false);

	QAxObject* workbooks = excel.querySubObject("Workbooks");
	QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", path);

	QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1);

	int total = 0;
	int effCnt = 0;
	int row = 2;

	int no = 1;

	while (true) {

		QAxObject* cellName = worksheet->querySubObject("Cells(int, int)", row, 1);
		QAxObject* cellType = worksheet->querySubObject("Cells(int, int)", row, 2);
		QAxObject* cellCnt = worksheet->querySubObject("Cells(int, int)", row, 3);
		QAxObject* cellDate = worksheet->querySubObject("Cells(int, int)", row, 4);

		QString type = cellType->dynamicCall("Value()").toString();
		QString cnt = cellCnt->dynamicCall("Value()").toString();
		QString name = cellName->dynamicCall("Value()").toString();
		QString date = cellDate->dynamicCall("Value()").toString();

		if (type.isEmpty() && cnt.isEmpty() && name.isEmpty() && date.isEmpty()) {
			break;
		}
		row++;
		total++;

		delete cellType;
		delete cellCnt;
		delete cellName;
		delete cellDate;

		if (type.isEmpty() || cnt.isEmpty() || name.isEmpty() || date.isEmpty()) {
			continue;
		}
		effCnt++;

		auto typeList = type.split("/");
		for (const QString item : typeList) {
			info in;

			auto noStr = QString("24%1").arg(no, 4, 10, QLatin1Char('0'));

			in.no = noStr;
			in.type = item;
			in.cnt = cnt;
			in.name = name;
			in.date = date;

			this->list.push_back(in);
			no++;

			lb->setText(QString("读取到%1行数据, 其中%2行有效, 预计产生票据%3张").arg(total).arg(effCnt).arg(this->list.size()));
		}
	}

	delete worksheet;
	workbook->dynamicCall("Close()");
	delete workbook;
	delete workbooks;

	excel.dynamicCall("Quit()");

	QMetaObject::invokeMethod(btn, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
	QMetaObject::invokeMethod(readBtn, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
}