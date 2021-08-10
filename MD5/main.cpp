#include "qtdemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	qtdemo w;
	w.show();
	return a.exec();
}
