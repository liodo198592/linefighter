#include "MainWindow.h"
int g_iStatus = 0;
//状态：0：等待加入游戏，1：等待游戏开始，2：游戏中

CMainWindow::CMainWindow(QWidget* parent):QMainWindow(parent)
{


	m_pTcpDialog = new CTcpConDialog(this);
	connect(m_pTcpDialog,SIGNAL(ConSig()),this,SLOT(on_TcpDialogCon()));
	m_pTcpDialog->show();

}

CMainWindow::~CMainWindow(void)
{
}
void CMainWindow::setupUI()
{
	setGeometry(100,100,800,600);


	m_pWaitJoinWidget = new CWaitJoinWidget(this);

	m_pWaitStartWidget = new CWaitStartWidget(this);
	m_pGamingWidget = new CGamingWidget(this);

	m_pStackedWidget = new QStackedWidget();
	m_pStackedWidget->addWidget(m_pWaitJoinWidget);
	m_pStackedWidget->addWidget(m_pWaitStartWidget);
	m_pStackedWidget->addWidget(m_pGamingWidget);

	m_pStackedWidget->setCurrentWidget(m_pWaitStartWidget);

	setCentralWidget(m_pStackedWidget);
	show();


}
void CMainWindow::connects()
{
	connect(m_pWaitJoinWidget,SIGNAL(changeStatus(int)),this,SLOT(on_ChangeStatus(int)));
	connect(m_pWaitStartWidget,SIGNAL(changeStatus(int)),this,SLOT(on_ChangeStatus(int)));
	connect(m_pGamingWidget,SIGNAL(changeStatus(int)),this,SLOT(on_ChangeStatus(int)));
	
}


void CMainWindow::on_TcpDialogCon()
{
	m_pTcpDialog->close();
	setupUI();
	connects();
}
void CMainWindow::on_ChangeStatus(int iStatus)
{

	if(iStatus == 1)
	{
		m_pStackedWidget->setCurrentWidget(m_pWaitStartWidget);

	}
	else if(iStatus == 2)
	{
		m_pStackedWidget->setCurrentWidget(m_pGamingWidget);
	}
	else if(iStatus == 0)
	{
		m_pStackedWidget->setCurrentWidget(m_pWaitJoinWidget);
	}


}