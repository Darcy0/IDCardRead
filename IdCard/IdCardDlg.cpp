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
		MessageBox(0,L"初始化失败EloamGlobal_InitDevs",0,0);
	}
	if(!EloamGlobal_InitIdCard(NULL,NULL))
	{
		MessageBox(0,L"初始化失败EloamGlobal_InitIdCard",0,0);
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

	this->setWindowTitle(QString::fromLocal8Bit("身份证信息读取"));
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
	ui.label_tipMsg->setText(QString::fromLocal8Bit("。。。"));
}

void IdCardDlg::onReadCard()
{
	EloamGlobal_DiscernIdCard(ELOAM_IDCARDCALLBACK(IdCardReadCallback), this);
	ui.label_tipMsg->setText(QString::fromLocal8Bit("请放卡。。。"));	
}

void IdCardDlg::onSaveImg()
{
	if(!m_isSucess)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("未识别到身份证信息。"));
		return;
	}
	QString fileSavePath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择保存路径"),
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
	QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("图片保存成功"));
}

void IdCardDlg::onExport()
{
	if(!m_isSucess)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("未识别到身份证信息。"));
		return;
	}
	QString fileSaveName = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("选择保存路径"),
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
		QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("打开文件未成功。"));
		return;
	}
	QTextStream write(&file);
	for (int i=0;i<m_idCardInfos.size();i++)
	{
		IdCardInfo idCardInfo=m_idCardInfos[i]; 
		QString birthDay=QString("%1%2%3%4%5%6")
			.arg(idCardInfo.birthday.year())
			.arg(QString::fromLocal8Bit("年"))
			.arg(idCardInfo.birthday.month())
			.arg(QString::fromLocal8Bit("月"))
			.arg(idCardInfo.birthday.day())
			.arg(QString::fromLocal8Bit("日"));

		write<<QString::fromLocal8Bit("姓名：")<<idCardInfo.name<<"\r\n"
			<<QString::fromLocal8Bit("性别：")<<idCardInfo.gender<<"\r\n" 
			<<QString::fromLocal8Bit("民族：")<<idCardInfo.nationality<<"\r\n"
			<<QString::fromLocal8Bit("出生年月日：")<<birthDay<<"\r\n"
			<<QString::fromLocal8Bit("住址：")<<idCardInfo.address<<"\r\n" 
			<<QString::fromLocal8Bit("身份证号码：")<<idCardInfo.IdNumber<<"\r\n" 
			<<QString::fromLocal8Bit("签发机关：")<<idCardInfo.visaOffice<<"\r\n"
			<<QString::fromLocal8Bit("有效期：")<<idCardInfo.validPeriod<<"\r\n"
			<<"\r\n";
	}
	file.close();
	
	QMessageBox msgBox(QMessageBox::Question,QString(QString::fromLocal8Bit("提示")),QString(QString::fromLocal8Bit(" \n 导出成功，是否查看？\n")));
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
	
	//身份证号码
	WCHAR * wIdNumber = EloamGlobal_GetIdCardData(8);	
	idCardInfo.IdNumber = QString::fromWCharArray(wIdNumber);
	EloamGlobal_DestroyString(wIdNumber);
	int index=-1;
	for (int i=0;i<pDlg->m_idCardInfos.size();i++)
	{//测试在队列里面当前身份证是否存在
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
		//姓名
		WCHAR * wName = EloamGlobal_GetIdCardData(1);
		idCardInfo.name=QString::fromWCharArray(wName);
		EloamGlobal_DestroyString(wName);
		//姓别
		WCHAR * wGender = EloamGlobal_GetIdCardData(2);	
		idCardInfo.gender=QString::fromWCharArray(wGender);
		EloamGlobal_DestroyString(wGender);
		//民族
		WCHAR * wNationality = EloamGlobal_GetIdCardData(3);	
		idCardInfo.nationality = QString::fromWCharArray(wNationality);
		EloamGlobal_DestroyString(wNationality);
		//出生年
		WCHAR * wBirthYear = EloamGlobal_GetIdCardData(4);	
		QString birthYear = QString::fromWCharArray(wBirthYear);
		EloamGlobal_DestroyString(wBirthYear);
		//出生月
		WCHAR * wBirthMonth = EloamGlobal_GetIdCardData(5);
		QString birthMonth = QString::fromWCharArray(wBirthMonth);
		EloamGlobal_DestroyString(wBirthMonth);
		//出生日
		WCHAR * wBirthDay = EloamGlobal_GetIdCardData(6);	
		QString birthDay = QString::fromWCharArray(wBirthDay);
		EloamGlobal_DestroyString(wBirthDay);

		idCardInfo.birthday.setDate(birthYear.toInt(),birthMonth.toInt(),birthDay.toInt());

		//地址
		WCHAR * wAddress = EloamGlobal_GetIdCardData(7);
		idCardInfo.address = QString::fromWCharArray(wAddress);
		EloamGlobal_DestroyString(wAddress);
		//发证机关
		WCHAR * wVisaOffice = EloamGlobal_GetIdCardData(9);
		idCardInfo.visaOffice = QString::fromWCharArray(wVisaOffice);
		EloamGlobal_DestroyString(wVisaOffice);
		//有效期限年(起)
		WCHAR * wValidYearStart = EloamGlobal_GetIdCardData(10);	
		QString validYearStart = QString::fromWCharArray(wValidYearStart);
		EloamGlobal_DestroyString(wValidYearStart);
		//有效期限月(起)
		WCHAR * wValidMonthStart = EloamGlobal_GetIdCardData(11);	
		QString validMonthStart = QString::fromWCharArray(wValidMonthStart);
		EloamGlobal_DestroyString(wValidMonthStart);
		//有效期限日(起)
		WCHAR * wValidDayStart = EloamGlobal_GetIdCardData(12);	
		QString validDayStart = QString::fromWCharArray(wValidDayStart);
		EloamGlobal_DestroyString(wValidDayStart);

		idCardInfo.startDate.setDate(validYearStart.toInt(),validMonthStart.toInt(),validDayStart.toInt());

		//有效期限年(止)
		WCHAR * wValidYearEnd = EloamGlobal_GetIdCardData(13);	
		QString validYearEnd = QString::fromWCharArray(wValidYearEnd);
		EloamGlobal_DestroyString(wValidYearEnd);
		//有效期限月(止)
		WCHAR * wValidMonthEnd = EloamGlobal_GetIdCardData(14);	
		QString validMonthEnd = QString::fromWCharArray(wValidMonthEnd);
		EloamGlobal_DestroyString(wValidMonthEnd);
		//有效期限日(止)
		WCHAR * wValidDayEnd = EloamGlobal_GetIdCardData(15);	
		QString validDayEnd = QString::fromWCharArray(wValidDayEnd);
		EloamGlobal_DestroyString(wValidDayEnd);

		idCardInfo.endDate.setDate(validYearEnd.toInt(),validMonthEnd.toInt(),validDayEnd.toInt());
		idCardInfo.validPeriod=QString("%1-%2").arg(idCardInfo.startDate.toString("yyyy.MM.dd")).arg(idCardInfo.endDate.toString("yyyy.MM.dd"));

		//芯片序列号
		WCHAR * wSerialNumber = EloamGlobal_GetIdCardData(16);	
		idCardInfo.serialNumber = QString::fromWCharArray(wSerialNumber);
		EloamGlobal_DestroyString(wSerialNumber);

		//人的头像
		idCardInfo.photoImage = EloamGlobal_GetIdCardImage(1);
		//正反垂直合成照片
		idCardInfo.photoCombineImage = EloamGlobal_GetIdCardImage(4);

		pDlg->m_idCardInfos.push_back(idCardInfo);
	}
	//界面显示
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

	pDlg->ui.label_tipMsg->setText(QString::fromLocal8Bit("读卡成功，请换另一张卡。。。"));
}