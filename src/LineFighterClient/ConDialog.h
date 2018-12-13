#ifndef _CON_DIALOG_H_
#define _CON_DIALOG_H_
#include "global.h"
#include "channel.h"
//extern QSettings m_gSetting;
class CTcpConDialog : public QDialog
{
	Q_OBJECT
public:
	CTcpConDialog(QWidget* parent):QDialog(parent)
	{
		resize(310,190);
		setMaximumSize(310,190);
		setMinimumSize(310,190);


		m_pLabelName = new QLabel(tr("昵称"),this);
		m_pLabelName->setGeometry(35,15,50,30);

		m_pLabelIP = new QLabel(tr("IP地址"),this);
		m_pLabelIP->setGeometry(35,50,50,30);


		m_pLabelPort = new QLabel(tr("端口号"),this);
		m_pLabelPort->setGeometry(40,95,50,30);


		m_pNameEdit = new QLineEdit(this);
		m_pNameEdit->setGeometry(110,15,160,20);

		m_pEditIP = new QLineEdit(this);
		m_pEditIP->setGeometry(110,55,160,20);
		//string strIP = (string)m_gSetting.value("IP").toString().toLocal8Bit().data();
		//m_pEditIP->setText(strIP.c_str());

		m_pEditPort = new QLineEdit(this);
		m_pEditPort->setGeometry(110,95,160,20);


		//if(m_gSetting.contains("Port"))
		//{
		//	int iPort = m_gSetting.value("Port").toInt();
		//	m_pEditPort->setText(QString("%1").arg(iPort));
		//}


		QValidator *validator = new QRegExpValidator(QRegExp("[0-9.]{,15}"), m_pEditIP);
		m_pEditIP->setValidator(validator);

		validator = new QRegExpValidator(QRegExp("[0-9]{0,10}"),m_pEditPort);
		m_pEditPort->setValidator(validator);

		m_pBtnCon = new QPushButton(tr("连接"),this);
		m_pBtnCon->setGeometry(100,140,110,25);

		connect(m_pBtnCon,SIGNAL(clicked()),this,SLOT(connectToTcp()));

	}
	~CTcpConDialog(){}

	
private slots:
	void connectToTcp();

public:signals:
	void ConSig();

private:
	string m_strIp;
	int m_iPort;

	QLabel* m_pLabelIP;
	QLabel* m_pLabelPort;
	QLabel* m_pLabelName;
	QLineEdit* m_pEditIP;
	QLineEdit* m_pEditPort;
	QLineEdit* m_pNameEdit;
	QPushButton* m_pBtnCon;


	//CTCPClient* m_pTcpClient;

};


#endif
