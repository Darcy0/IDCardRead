#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardPaths>
#include "IdCardDlg.h"


#pragma comment(lib,"eloamDll.lib")

IdCardDlg::IdCardDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_isSucess=false;
	if(!EloamGlobal_InitDevs(0,this))
	{
		MessageBox(0,L"��ʼ��ʧ��EloamGlobal_InitDevs",0,0);
	}
	if(!EloamGlobal_InitIdCard(NULL,NULL))
	{
		MessageBox(0,L"��ʼ��ʧ��EloamGlobal_InitIdCard",0,0);
	}
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowFlags(Qt::Window|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint); //|Qt::Dialog|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);	
	
	connect(ui.pushButton_readCard,SIGNAL(clicked()),this,SLOT(onReadCard()));
	connect(ui.pushButton_saveImg,SIGNAL(clicked()),this,SLOT(onSaveImg()));
	connect(ui.pushButton_export,SIGNAL(clicked()),this,SLOT(onExport()));
	connect(ui.pushButton_exit,SIGNAL(clicked()),this,SLOT(onExit()));

	QIcon icon;
	icon.addFile("://IdCard//Resources//img//eloam.png");
	this->setWindowIcon(icon);

	this->setWindowTitle(QString::fromLocal8Bit("���֤��Ϣ��ȡ"));
}

IdCardDlg::~IdCardDlg()
{
	EloamGlobal_DeinitIdCard();
}

void IdCardDlg::clearIdCardInfo()
{
	ui.lineEdit_name->clear();
	ui.lineEdit_gender->clear();
	ui.lineEdit_nationality->clear();
	ui.lineEdit_year->clear();
	ui.lineEdit_month->clear();
	ui.lineEdit_day->clear();
	ui.lineEdit_address->clear();
	ui.lineEdit_idNumber->clear();
	ui.lineEdit_visaOffice->clear();
	ui.lineEdit_validPeriod->clear();
	ui.label_photo->clear();
	ui.label_tipMsg->setText(QString::fromLocal8Bit("������"));
}

void IdCardDlg::onReadCard()
{
	EloamGlobal_DiscernIdCard(ELOAM_IDCARDCALLBACK(IdCardReadCallback), this);
	ui.label_tipMsg->setText(QString::fromLocal8Bit("��ſ�������"));	
}

void IdCardDlg::onSaveImg()
{
	if(!m_isSucess)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("δʶ�����֤��Ϣ��"));
		return;
	}
	QString fileSavePath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ�񱣴�·��"),
		QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if(fileSavePath.isEmpty())
	{		
		return;
	}
	QDateTime currentDatetime = QDateTime::currentDateTime();
	QString timeStr=currentDatetime.toString("yyyyMMddhhmmss");
	for (int i=0;i<m_idCardInfos.size();i++)
	{
		IdCardInfo idCardInfo=m_idCardInfos[i];
		QString saveFileName=fileSavePath+QString::fromLocal8Bit("/")+timeStr+"_"+QString::number(i+1)+".jpg";
		EloamImage_Save(idCardInfo.photoCombineImage,(BSTR)(LPCWSTR)saveFileName.data(),0);
	}
	QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("ͼƬ����ɹ�"));
}

void IdCardDlg::onExport()
{
	if(!m_isSucess)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("δʶ�����֤��Ϣ��"));
		return;
	}
	QString fileSaveName = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("ѡ�񱣴�·��"),
		"",
		tr("txt (*.txt);;TXT (*.TXT)"));
	if(fileSaveName.isEmpty())
	{		 
		return;
	} 
	fileSaveName.replace("/","\\");
	QFile file(fileSaveName);
	if(!file.open(QIODevice::Append|QIODevice::Text))
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���ļ�δ�ɹ���"));
		return;
	}
	QTextStream write(&file);
	for (int i=0;i<m_idCardInfos.size();i++)
	{
		IdCardInfo idCardInfo=m_idCardInfos[i]; 
		QString birthDay=QString("%1%2%3%4%5%6")
			.arg(idCardInfo.birthday.year())
			.arg(QString::fromLocal8Bit("��"))
			.arg(idCardInfo.birthday.month())
			.arg(QString::fromLocal8Bit("��"))
			.arg(idCardInfo.birthday.day())
			.arg(QString::fromLocal8Bit("��"));

		write<<QString::fromLocal8Bit("������")<<idCardInfo.name<<"\r\n"
			<<QString::fromLocal8Bit("�Ա�")<<idCardInfo.gender<<"\r\n" 
			<<QString::fromLocal8Bit("���壺")<<idCardInfo.nationality<<"\r\n"
			<<QString::fromLocal8Bit("���������գ�")<<birthDay<<"\r\n"
			<<QString::fromLocal8Bit("סַ��")<<idCardInfo.address<<"\r\n" 
			<<QString::fromLocal8Bit("���֤���룺")<<idCardInfo.IdNumber<<"\r\n" 
			<<QString::fromLocal8Bit("ǩ�����أ�")<<idCardInfo.visaOffice<<"\r\n"
			<<QString::fromLocal8Bit("��Ч�ڣ�")<<idCardInfo.validPeriod<<"\r\n"
			<<"\r\n";
	}
	file.close();
	
	QMessageBox msgBox(QMessageBox::Question,QString(QString::fromLocal8Bit("��ʾ")),QString(QString::fromLocal8Bit(" \n �����ɹ����Ƿ�鿴��\n")));
	msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret=msgBox.exec();
	if(ret==QMessageBox::Yes)
	{
		ShellExecute(NULL, L"open",(BSTR)(LPCWSTR)fileSaveName.data(), NULL, NULL, SW_NORMAL);

	}
	m_idCardInfos.clear();
	m_isSucess=false;
}

void IdCardDlg::onExit()
{
	clearIdCardInfo();
	EloamGlobal_StopIdCardDiscern();
	this->close();
}

VOID ELOAMAPI IdCardDlg::IdCardReadCallback(LONG ret, LPVOID userData)
{
	if (1!=ret)
	{
		return;
	}
	IdCardDlg *pDlg = (IdCardDlg*)userData;
	pDlg->m_isSucess = true;

	IdCardInfo idCardInfo;
	
	//���֤����
	WCHAR * wIdNumber = EloamGlobal_GetIdCardData(8);	
	idCardInfo.IdNumber = QString::fromWCharArray(wIdNumber);
	EloamGlobal_DestroyString(wIdNumber);
	int index=-1;
	for (int i=0;i<pDlg->m_idCardInfos.size();i++)
	{//�����ڶ������浱ǰ���֤�Ƿ����
		IdCardInfo tempIdCardInfo=(pDlg->m_idCardInfos)[i];
		if (idCardInfo.IdNumber==tempIdCardInfo.IdNumber)
		{
			index=i;
			break;
		}
	}
	if (-1!=index)
	{
		idCardInfo=(pDlg->m_idCardInfos)[index];
	}
	else
	{
		//����
		WCHAR * wName = EloamGlobal_GetIdCardData(1);
		idCardInfo.name=QString::fromWCharArray(wName);
		EloamGlobal_DestroyString(wName);
		//�ձ�
		WCHAR * wGender = EloamGlobal_GetIdCardData(2);	
		idCardInfo.gender=QString::fromWCharArray(wGender);
		EloamGlobal_DestroyString(wGender);
		//����
		WCHAR * wNationality = EloamGlobal_GetIdCardData(3);	
		idCardInfo.nationality = QString::fromWCharArray(wNationality);
		EloamGlobal_DestroyString(wNationality);
		//������
		WCHAR * wBirthYear = EloamGlobal_GetIdCardData(4);	
		QString birthYear = QString::fromWCharArray(wBirthYear);
		EloamGlobal_DestroyString(wBirthYear);
		//������
		WCHAR * wBirthMonth = EloamGlobal_GetIdCardData(5);
		QString birthMonth = QString::fromWCharArray(wBirthMonth);
		EloamGlobal_DestroyString(wBirthMonth);
		//������
		WCHAR * wBirthDay = EloamGlobal_GetIdCardData(6);	
		QString birthDay = QString::fromWCharArray(wBirthDay);
		EloamGlobal_DestroyString(wBirthDay);

		idCardInfo.birthday.setDate(birthYear.toInt(),birthMonth.toInt(),birthDay.toInt());

		//��ַ
		WCHAR * wAddress = EloamGlobal_GetIdCardData(7);
		idCardInfo.address = QString::fromWCharArray(wAddress);
		EloamGlobal_DestroyString(wAddress);
		//��֤����
		WCHAR * wVisaOffice = EloamGlobal_GetIdCardData(9);
		idCardInfo.visaOffice = QString::fromWCharArray(wVisaOffice);
		EloamGlobal_DestroyString(wVisaOffice);
		//��Ч������(��)
		WCHAR * wValidYearStart = EloamGlobal_GetIdCardData(10);	
		QString validYearStart = QString::fromWCharArray(wValidYearStart);
		EloamGlobal_DestroyString(wValidYearStart);
		//��Ч������(��)
		WCHAR * wValidMonthStart = EloamGlobal_GetIdCardData(11);	
		QString validMonthStart = QString::fromWCharArray(wValidMonthStart);
		EloamGlobal_DestroyString(wValidMonthStart);
		//��Ч������(��)
		WCHAR * wValidDayStart = EloamGlobal_GetIdCardData(12);	
		QString validDayStart = QString::fromWCharArray(wValidDayStart);
		EloamGlobal_DestroyString(wValidDayStart);

		idCardInfo.startDate.setDate(validYearStart.toInt(),validMonthStart.toInt(),validDayStart.toInt());

		//��Ч������(ֹ)
		WCHAR * wValidYearEnd = EloamGlobal_GetIdCardData(13);	
		QString validYearEnd = QString::fromWCharArray(wValidYearEnd);
		EloamGlobal_DestroyString(wValidYearEnd);
		//��Ч������(ֹ)
		WCHAR * wValidMonthEnd = EloamGlobal_GetIdCardData(14);	
		QString validMonthEnd = QString::fromWCharArray(wValidMonthEnd);
		EloamGlobal_DestroyString(wValidMonthEnd);
		//��Ч������(ֹ)
		WCHAR * wValidDayEnd = EloamGlobal_GetIdCardData(15);	
		QString validDayEnd = QString::fromWCharArray(wValidDayEnd);
		EloamGlobal_DestroyString(wValidDayEnd);

		idCardInfo.endDate.setDate(validYearEnd.toInt(),validMonthEnd.toInt(),validDayEnd.toInt());
		idCardInfo.validPeriod=QString("%1-%2").arg(idCardInfo.startDate.toString("yyyy.MM.dd")).arg(idCardInfo.endDate.toString("yyyy.MM.dd"));

		//оƬ���к�
		WCHAR * wSerialNumber = EloamGlobal_GetIdCardData(16);	
		idCardInfo.serialNumber = QString::fromWCharArray(wSerialNumber);
		EloamGlobal_DestroyString(wSerialNumber);

		//�˵�ͷ��
		idCardInfo.photoImage = EloamGlobal_GetIdCardImage(1);
		//������ֱ�ϳ���Ƭ
		idCardInfo.photoCombineImage = EloamGlobal_GetIdCardImage(4);

		pDlg->m_idCardInfos.push_back(idCardInfo);
	}
	//������ʾ
	pDlg->ui.lineEdit_name->setText(idCardInfo.name);
	pDlg->ui.lineEdit_gender->setText(idCardInfo.gender);	
	pDlg->ui.lineEdit_nationality->setText(idCardInfo.nationality);
	pDlg->ui.lineEdit_year->setText(QString::number(idCardInfo.birthday.year()));
	pDlg->ui.lineEdit_month->setText(QString::number(idCardInfo.birthday.month()));
	pDlg->ui.lineEdit_day->setText(QString::number(idCardInfo.birthday.day()));	
	pDlg->ui.lineEdit_address->setText(idCardInfo.address);
	pDlg->ui.lineEdit_idNumber->setText(idCardInfo.IdNumber);
	pDlg->ui.lineEdit_visaOffice->setText(idCardInfo.visaOffice);
	pDlg->ui.lineEdit_validPeriod->setText(idCardInfo.validPeriod);
	QString tempImgName = QDir::tempPath() + QString::fromLocal8Bit("/tempImg.png"); 
	if(EloamImage_Save(idCardInfo.photoImage,(BSTR)(LPCWSTR)tempImgName.data(),0))
	{	
		QPixmap img(tempImgName);
		pDlg->ui.label_photo->setPixmap(img);
		pDlg->ui.label_photo->resize(img.width(),img.height());    
	}

	pDlg->ui.label_tipMsg->setText(QString::fromLocal8Bit("�����ɹ����뻻��һ�ſ�������"));
}