#include "qtdemo.h"
#include <QMessageBox>
#include <QFileDialog>
#include "qclipboard.h"
#include "md5file.h"
#include "string"


using namespace std;

qtdemo::qtdemo(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.selectBtn, SIGNAL(clicked()), this, SLOT(clickButton()));

	connect(ui.calButton, SIGNAL(clicked()), this, SLOT(clickCalButton()));

	connect(ui.rightListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
		this, SLOT(copyValue(QListWidgetItem*)));


	connect(ui.clearDataAction, SIGNAL(triggered()), this, SLOT(clearData()));

}

void qtdemo::clickButton() {
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("选择文件"));
	fileDialog->setDirectory("C:/");
	fileDialog->setNameFilter(tr("File(*.*)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
	}
	QStringListIterator strIterator(fileNames);
	while (strIterator.hasNext()) {
		QString str = strIterator.next();
		this->ui.leftListWidget->addItem(str);
	}
}


void qtdemo::clickCalButton() {


	this->ui.rightListWidget->clear();
	int row = 0;
	int count = this->ui.leftListWidget->count();
	if (count == 1) {
		this->ui.progressBar->setValue(50);
	}
	else {
		this->ui.progressBar->setValue(0);
	}
	QString line;
	while (row < count)
	{

		line = this->ui.leftListWidget->item(row)->text();
		string str = getFileMD5(line.toLocal8Bit().data());

		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(str));

		this->ui.rightListWidget->addItem(item);

		row++;

		this->ui.progressBar->setValue(row / count);
	}
	this->ui.progressBar->setValue(100);
}


void qtdemo::copyValue(QListWidgetItem* item) {
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(item->text());

	QMessageBox::about(this, "提示", "已复制到剪切板");

}

void qtdemo::clearData() {
	this->ui.rightListWidget->clear();
	this->ui.leftListWidget->clear();
	this->ui.progressBar->setValue(0);
}



