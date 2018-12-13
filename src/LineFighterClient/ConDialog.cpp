#include "ConDialog.h"

CTCPClient* g_pTcpClient;
void CTcpConDialog::connectToTcp()
{
	if(g_pTcpClient != NULL)
	{
		g_pTcpClient->close();
		delete g_pTcpClient;
	}

	m_strIp = (string)(m_pEditIP->text().toLocal8Bit().data());
	m_iPort = atoi(m_pEditPort->text().toLocal8Bit().data());

	g_pTcpClient = new CTCPClient(m_strIp,m_iPort);
	g_pTcpClient->open();
	if(g_pTcpClient->isOpen())
	{
		emit ConSig();
	}
	else
	{
		QMessageBox::warning(this,tr("´íÎó"),tr("Á¬½Ó´íÎó"),QMessageBox::Yes);
	}

	//m_gSetting.setValue("IP",QString(m_strIp.c_str()));
	//m_gSetting.setValue("Port",m_iPort);
	
}
