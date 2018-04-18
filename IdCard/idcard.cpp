#include "idcard.h"
#include<qdatetime.h>
#include<qmessagebox.h>
#include<QFileDialog>
#include <QAxObject>
#include <QAxWidget>
#include <qtextstream.h>
#include <QDesktopServices.h>
#pragma comment(lib,"eloamDll.lib")

IdCard::IdCard(QWidget *parent)
	: QMainWindow(parent)
{
	IdCard_ui.setupUi(this);
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
	connect(IdCard_ui.m_PbReadCard,SIGNAL(clicked()),this,SLOT(OnReadCard()));	
	connect(IdCard_ui.m_PbReadIdCard,SIGNAL(clicked()),this,SLOT(OnExeportIdImage()));   
	connect(IdCard_ui.m_PbReadCardExit,SIGNAL(clicked()),this,SLOT(OnExitIdCard()));
	connect(IdCard_ui.m_PbReadIdCard_SaveAs,SIGNAL(clicked()),this,SLOT(OnIdCardSaveAs()));

	QIcon icon;
	icon.addFile("://IdCard//Resources//img//eloam.png");
	this->setWindowIcon(icon);
}

IdCard::~IdCard()
{
	EloamGlobal_DeinitIdCard();
}

VOID ELOAMAPI IdCard::IdCardReadCallback(LONG ret, LPVOID userData)
{
	IdCard *pDlg = (IdCard*)userData;
	//姓名
	pDlg->m_isSucess = true;
	WCHAR * wNameg = EloamGlobal_GetIdCardData(1);	
	pDlg->m_IDCardName = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardName->setText(pDlg->m_IDCardName);
	pDlg->m_IDCardNameTemp.push_back(pDlg->m_IDCardName);
	//姓别
	wNameg = EloamGlobal_GetIdCardData(2);	
	pDlg->m_IDCardSex = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardSex->setText(pDlg->m_IDCardSex);
	pDlg->m_IDCardSexTemp.push_back(pDlg->m_IDCardSex);
	//民族
    wNameg = EloamGlobal_GetIdCardData(3);	
	pDlg->m_IDCardNation = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardNation->setText(pDlg->m_IDCardNation);
	pDlg->m_IDCardNationTemp.push_back(pDlg->m_IDCardNation);

	//出生年
	wNameg = EloamGlobal_GetIdCardData(4);	
	pDlg->m_IDCardAgeYear = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardAge_Y->setText(pDlg->m_IDCardAgeYear);
	pDlg->m_IDCardAgeYearTemp.push_back(pDlg->m_IDCardAgeYear);

	//出生月
	wNameg = EloamGlobal_GetIdCardData(5);	
	pDlg->m_IDCardAgeMouth = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardAge_M->setText(pDlg->m_IDCardAgeMouth);
	pDlg->m_IDCardAgeMouthTemp.push_back(pDlg->m_IDCardAgeMouth);

	//出生日
	wNameg = EloamGlobal_GetIdCardData(6);	
	pDlg->m_IDCardAgeDay = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardAge_D->setText(pDlg->m_IDCardAgeDay);
	pDlg->m_IDCardAgeDayTemp.push_back(pDlg->m_IDCardAgeDay);

	//QMessageBox::warning(0,pDlg->m_IDCardAgeDay,0);
	//地址
	wNameg = EloamGlobal_GetIdCardData(7);	
	pDlg->m_IDCardAdreass = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardAdreass->setText(pDlg->m_IDCardAdreass);
	pDlg->m_IDCardAdreassTemp.push_back(pDlg->m_IDCardAdreass);

	//身份证号码
	wNameg = EloamGlobal_GetIdCardData(8);	
	pDlg->m_IDCardIdNum = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_IDCardIDnum->setText(pDlg->m_IDCardIdNum);
	pDlg->m_IDCardIdNumTemp.push_back(pDlg->m_IDCardIdNum);

	//发证机关
	wNameg = EloamGlobal_GetIdCardData(9);	
	pDlg->m_IDCardIdIssued = QString::fromWCharArray(wNameg);
	pDlg->IdCard_ui.m_m_IDCardIdIssued->setText(pDlg->m_IDCardIdIssued);
	pDlg->m_IDCardIdIssuedTemp.push_back(pDlg->m_IDCardIdIssued);

	//有效期限
	wNameg = EloamGlobal_GetIdCardData(10);	
	pDlg->m_IDCardDate = QString::fromWCharArray(wNameg);
	

	//人的头像
	HELOAMIMAGE hImg = EloamGlobal_GetIdCardImage(1);
	QDateTime Datetime = QDateTime::currentDateTime();
	QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
	QString FileSaveName;
	QString temppath;
	temppath = QDir::tempPath();
	FileSaveName = temppath + QString::fromLocal8Bit("/") + Ctime + ".png";		
	if(EloamImage_Save(hImg,(BSTR)(LPCWSTR)FileSaveName.data(),0))
	{
		QString path=FileSaveName;//相对项目文件		
		QPixmap img(path);
		pDlg->IdCard_ui.label_showImage->setPixmap(img);
		pDlg->IdCard_ui.label_showImage->resize(img.width(),img.height());    
	}	

	wNameg = EloamGlobal_GetIdCardData(11);	
	QString str = QString::fromWCharArray(wNameg);
	pDlg->m_IDCardDate += "." +str; 

	wNameg = EloamGlobal_GetIdCardData(12);	
	str = QString::fromWCharArray(wNameg);
	pDlg->m_IDCardDate += "." +str; 

	wNameg = EloamGlobal_GetIdCardData(13);	
	str = QString::fromWCharArray(wNameg);
	pDlg->m_IDCardDate += " - " +str;

	wNameg = EloamGlobal_GetIdCardData(14);	
	str = QString::fromWCharArray(wNameg);
	pDlg->m_IDCardDate += "." +str;

	wNameg = EloamGlobal_GetIdCardData(15);	
	str = QString::fromWCharArray(wNameg);
	pDlg->m_IDCardDate += "." +str;

	pDlg->m_IDCardDateTemp.push_back(pDlg->m_IDCardDate);

	pDlg->IdCard_ui.m_IDCardData->setText(pDlg->m_IDCardDate);
	EloamGlobal_DestroyString(wNameg);

	pDlg->IdCard_ui.label_ShowCardState->setText(QString::fromLocal8Bit("读卡成功，请换另一张卡。。。"));
		

}
void IdCard::OnReadCard()
{
	EloamGlobal_DiscernIdCard(ELOAM_IDCARDCALLBACK(IdCardReadCallback), this);
	IdCard_ui.label_ShowCardState->setText(QString::fromLocal8Bit("请放卡。。。"));	
}

void IdCard::OnExitReadCard()
{
	OnClearIdCardinfo();
	EloamGlobal_StopIdCardDiscern();

	m_isSucess = false;
}

void IdCard::OnClearIdCardinfo()
{
	 m_IDCardName = "";                    //身份证姓名
	 m_IDCardSex= "";                    //身份证姓名
	 m_IDCardAgeYear= "";                    //身份证姓名
	 m_IDCardAgeMouth= "";                    //身份证姓名
	 m_IDCardAgeDay= "";                    //身份证姓名
	 m_IDCardNation= "";                    //身份证姓名
	 m_IDCardAdreass= "";                    //身份证姓名
	 m_IDCardIdNum= "";                    //身份证姓名
	 m_IDCardIdIssued= "";                    //发证机关
	 m_IDCardDate= ""; 

	
	IdCard_ui.m_IDCardName->setText(m_IDCardName);
	//姓别
	
	IdCard_ui.m_IDCardSex->setText(m_IDCardSex);
	//民族
   
	IdCard_ui.m_IDCardNation->setText(m_IDCardNation);
	//出生年
	
	IdCard_ui.m_IDCardAge_Y->setText(m_IDCardAgeYear);
	//出生月
	
	IdCard_ui.m_IDCardAge_M->setText(m_IDCardAgeMouth);
	//出生日
	
	IdCard_ui.m_IDCardAge_D->setText(m_IDCardAgeDay);
	//QMessageBox::warning(0,pDlg->m_IDCardAgeDay,0);
	//地址
	
	IdCard_ui.m_IDCardAdreass->setText(m_IDCardAdreass);
	//身份证号码
	
	IdCard_ui.m_IDCardIDnum->setText(m_IDCardIdNum);
	//发证机关
	
	IdCard_ui.m_m_IDCardIdIssued->setText(m_IDCardIdIssued);
	//有效期限
	

	//人的头像
	
	IdCard_ui.label_showImage->setPixmap(QPixmap(""));		
	
	IdCard_ui.m_IDCardData->setText(m_IDCardDate);
	IdCard_ui.label_ShowCardState->setText(QString::fromLocal8Bit("。。。"));
}
void IdCard::OnExeportIdImage()
{
	QString strName = QString::fromLocal8Bit("选择保存路径");
	QString fileSaveName = QFileDialog::getSaveFileName(this,
        strName,
        "",
        tr("jpg (*.jpg);;bmp (*.bmp)"));
	if(fileSaveName.isEmpty())
	{		
		return;
	}
	HELOAMIMAGE hImg = EloamGlobal_GetIdCardImage(4);
	QDateTime Datetime = QDateTime::currentDateTime();
	QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
	QString FileSaveName;	
	//FileSaveName = m_CurImageSavePath + QString::fromLocal8Bit("/") + Ctime + ".jpg";
	if(EloamImage_Save(hImg,(BSTR)(LPCWSTR)fileSaveName.data(),0))
	{
		//EloamThumbnail_Add(m_hThumb,(BSTR)(LPCWSTR)FileSaveName.data());
		//SendMsgToParent(FileSaveName);
	}
}
void IdCard::OnIdCardSaveAs()
{
	if(!m_isSucess)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("身份证识别未成功。"));
		return;
	}
	QString strLid,strName;
	strLid = "STRING_QINGXUANZE_LUXIANG_BAOCUN_LUJING";
	strName = QString::fromLocal8Bit("选择保存路径");
	QString fileSaveName = QFileDialog::getSaveFileName(this,
        strName,
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
	 QString age = m_IDCardAgeYearTemp[0] + QString::fromLocal8Bit("年")+ m_IDCardAgeMouthTemp[0] + QString::fromLocal8Bit("月") + m_IDCardAgeDayTemp[0] + QString::fromLocal8Bit("日");

	 QTextStream write(&file);
	 write<<QString::fromLocal8Bit("姓名：")<<m_IDCardNameTemp[0]<<"\r\n"
	  <<QString::fromLocal8Bit("性别：")<< m_IDCardSexTemp[0]<<"\r\n" 
	  <<QString::fromLocal8Bit("民族：")<<m_IDCardNationTemp[0]<<"\r\n"
	    <<QString::fromLocal8Bit("出生年月：")<<age<<"\r\n"
		 <<QString::fromLocal8Bit("住址：")<<m_IDCardAdreassTemp[0]<<"\r\n" 
		  <<QString::fromLocal8Bit("身份证号码：")<<m_IDCardIdNumTemp[0]<<"\r\n" 
		   <<QString::fromLocal8Bit("签发机关：")<<m_IDCardIdIssuedTemp[0]<<"\r\n"
			<<QString::fromLocal8Bit("有效期：")<<m_IDCardDateTemp[0]<<endl;
	 file.close();

	QMessageBox msgBox(QMessageBox::Question,QString(QString::fromLocal8Bit("提示")),QString(QString::fromLocal8Bit(" \n 导出成功，是否查看？\n")));
	msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret=msgBox.exec();
	if(ret==QMessageBox::Yes)
	{
		ShellExecute(NULL, L"open",(BSTR)(LPCWSTR)fileSaveName.data(), NULL, NULL, SW_NORMAL);
	
	}
	else if(ret==QMessageBox::No)
	{
	   
	}
	m_IDCardNameTemp.clear();
	m_IDCardSexTemp.clear();
	m_IDCardAgeYearTemp.clear();
	m_IDCardAgeMouthTemp.clear();
	m_IDCardAgeDayTemp.clear();
	m_IDCardNationTemp.clear();
	m_IDCardAdreassTemp.clear();
	m_IDCardIdNumTemp.clear();
	m_IDCardIdIssuedTemp.clear();
	m_IDCardDateTemp.clear();

}
void IdCard::OnExitIdCard()
{
	OnClearIdCardinfo();
	EloamGlobal_StopIdCardDiscern();
	close();
}