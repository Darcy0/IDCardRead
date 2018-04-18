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
	//��ʼ�����֤�Ķ�����
	void InitIdCardDlg();

		//���֤�Ķ�
	bool m_isSucess;                          //ʶ��ɹ�
	QString m_IDCardName;                    //���֤����
	std::vector<QString> m_IDCardNameTemp;

	QString m_IDCardSex;                    //���֤����
	std::vector<QString> m_IDCardSexTemp;

	QString m_IDCardAgeYear;                    //���֤����
	std::vector<QString> m_IDCardAgeYearTemp;

	QString m_IDCardAgeMouth;                    //���֤����
	std::vector<QString> m_IDCardAgeMouthTemp;

	QString m_IDCardAgeDay;                    //���֤����
	std::vector<QString> m_IDCardAgeDayTemp;

	QString m_IDCardNation;                    //���֤����
	std::vector<QString> m_IDCardNationTemp;

	QString m_IDCardAdreass;                    //���֤����
	std::vector<QString> m_IDCardAdreassTemp; 

	QString m_IDCardIdNum;                    //���֤����
	std::vector<QString> m_IDCardIdNumTemp; 

	QString m_IDCardIdIssued;                    //��֤����
	std::vector<QString> m_IDCardIdIssuedTemp;

	QString m_IDCardDate;                    //���֤����
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
