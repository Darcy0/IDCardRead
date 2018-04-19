#ifndef IDCARDDLGG_H
#define IDCARDDLGG_H

#include <QWidget>
#include <QDate>
#include"eloamDll.h"
#include "ui_IdCardDlg.h"

struct IdCardInfo{
	QString name;					//姓名	
	QString gender;					//性别
	QString nationality;			//民族
	QDate birthday;					//出生日期
	QString address;				//地址
	QString IdNumber;				//身份证号码
	QString visaOffice;				//签证机关
	QDate startDate;				//有效期起始
	QDate endDate;					//有效期结束
	QString validPeriod;			//有效期
	QString serialNumber;			//芯片序列号
	HELOAMIMAGE photoImage;			//头像
	HELOAMIMAGE photoCombineImage;	//正反垂直合成照片
};

class IdCardDlg : public QWidget
{
	Q_OBJECT

public:
	static VOID ELOAMAPI IdCardReadCallback(LONG ret, LPVOID userData);

	IdCardDlg(QWidget *parent = 0);
	~IdCardDlg();

private:
	Ui::IdCardDlg ui;
	bool m_isSucess;                          //识别成功
	QVector<IdCardInfo> m_idCardInfos;
	void clearIdCardInfo();

public slots:
	void onReadCard();
	void onSaveImg();
	void onExport();
	void onExit();
};

#endif // IDCARDDLGG_H
