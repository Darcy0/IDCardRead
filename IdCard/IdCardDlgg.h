#ifndef IDCARDDLGG_H
#define IDCARDDLGG_H

#include <QWidget>
#include "ui_IdCardDlgg.h"

class IdCardDlgg : public QWidget
{
	Q_OBJECT

public:
	IdCardDlgg(QWidget *parent = 0);
	~IdCardDlgg();

private:
	Ui::IdCardDlgg ui;
};

#endif // IDCARDDLGG_H
