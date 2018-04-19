#include <QtWidgets/QApplication>
#include <QTranslator>
#include "IdCardDlg.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator *translator=new QTranslator;
	bool isSuccess=translator->load("qt_zh_CN.qm");
	a.installTranslator(translator);
	IdCardDlg w;
	w.show();
	return a.exec();
}
