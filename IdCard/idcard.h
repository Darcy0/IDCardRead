#ifndef IDCARD_H
#define IDCARD_H

#include <QtWidgets/QMainWindow>
#include "ui_IDCardDlg.h"
#include"eloamDll.h"

class IdCard : public QMainWindow
{
	Q_OBJECT

public:
	IdCard(QWidget *parent = 0);
	~IdCard();
	
	static VOID ELOAMAPI IdCardReadCallback(LONG ret, LPVOID userData);
	//初始化身份证阅读窗口
	void InitIdCardDlg();

		//身份证阅读
	bool m_isSucess;                          //识别成功
	QString m_IDCardName;                    //身份证姓名
	std::vector<QString> m_IDCardNameTemp;

	QString m_IDCardSex;                    //身份证姓名
	std::vector<QString> m_IDCardSexTemp;

	QString m_IDCardAgeYear;                    //身份证姓名
	std::vector<QString> m_IDCardAgeYearTemp;

	QString m_IDCardAgeMouth;                    //身份证姓名
	std::vector<QString> m_IDCardAgeMouthTemp;

	QString m_IDCardAgeDay;                    //身份证姓名
	std::vector<QString> m_IDCardAgeDayTemp;

	QString m_IDCardNation;                    //身份证姓名
	std::vector<QString> m_IDCardNationTemp;

	QString m_IDCardAdreass;                    //身份证姓名
	std::vector<QString> m_IDCardAdreassTemp; 

	QString m_IDCardIdNum;                    //身份证姓名
	std::vector<QString> m_IDCardIdNumTemp; 

	QString m_IDCardIdIssued;                    //发证机关
	std::vector<QString> m_IDCardIdIssuedTemp;

	QString m_IDCardDate;                    //身份证姓名
	std::vector<QString> m_IDCardDateTemp;

	QString	m_CurImageSavePath;

private:
	Ui::IDCardDlg_Form IdCard_ui;

public slots:
	void OnReadCard();
	void OnExitIdCard();
	void OnIdCardSaveAs();
	void OnExeportIdImage();
	void OnClearIdCardinfo();
	void OnExitReadCard();
};

#endif // IDCARD_H
