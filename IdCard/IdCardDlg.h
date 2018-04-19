#ifndef IDCARDDLGG_H
#define IDCARDDLGG_H

#include <QWidget>
#include <QDate>
#include"eloamDll.h"
#include "ui_IdCardDlg.h"

struct IdCardInfo{
	QString name;					//����	
	QString gender;					//�Ա�
	QString nationality;			//����
	QDate birthday;					//��������
	QString address;				//��ַ
	QString IdNumber;				//���֤����
	QString visaOffice;				//ǩ֤����
	QDate startDate;				//��Ч����ʼ
	QDate endDate;					//��Ч�ڽ���
	QString validPeriod;			//��Ч��
	QString serialNumber;			//оƬ���к�
	HELOAMIMAGE photoImage;			//ͷ��
	HELOAMIMAGE photoCombineImage;	//������ֱ�ϳ���Ƭ
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
	bool m_isSucess;                          //ʶ��ɹ�
	QVector<IdCardInfo> m_idCardInfos;
	void clearIdCardInfo();

public slots:
	void onReadCard();
	void onSaveImg();
	void onExport();
	void onExit();
};

#endif // IDCARDDLGG_H
