#include "idcard.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IdCard w;
	w.show();
	return a.exec();
}
