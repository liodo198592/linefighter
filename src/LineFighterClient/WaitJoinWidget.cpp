#include "WaitJoinWidget.h"
extern int g_iStatus;//状态：0：等待加入游戏，1：等待游戏开始，2：游戏中
CWaitJoinWidget::CWaitJoinWidget(QWidget* parent):QWidget(parent)
{

	resize(800,600);
	m_pTreeWidgetRoom = new QTreeWidget(this);
	m_pTreeWidgetRoom->setGeometry(30,30,340,500);

	m_pTreeWidgetRoom->headerItem()->setText(0,tr("房间列表"));


	m_pGroupBoxCreateRoom = new QGroupBox(tr("新建主机"),this);
	m_pGroupBoxCreateRoom->setGeometry(400,30,370,220);
	m_pLabelRoomName = new QLabel(tr("房间名称"),m_pGroupBoxCreateRoom);
	m_pLabelRoomName->setGeometry(70,30,70,40);
	m_pLineEditRoomName = new QLineEdit(m_pGroupBoxCreateRoom);
	m_pLineEditRoomName->setGeometry(170,40,110,20);
	m_pLabelMaxNum = new QLabel(tr("最大人数"),m_pGroupBoxCreateRoom);
	m_pLabelMaxNum->setGeometry(70,90,70,40);
	m_pLineEditMaxNum = new QLineEdit(m_pGroupBoxCreateRoom);
	m_pLineEditMaxNum->setGeometry(170,96,110,20);
	m_pBtnCreateRoom = new QPushButton(tr("建立房间"),m_pGroupBoxCreateRoom);
	m_pBtnCreateRoom->setGeometry(130,170,120,30);


	m_pGroupBoxJionRoom = new QGroupBox(tr("加入房间"),this);
	m_pGroupBoxJionRoom->setGeometry(400,290,370,90);
	m_pBtnJoinRoom = new QPushButton(tr("加入房间"),m_pGroupBoxJionRoom);
	m_pBtnJoinRoom->setGeometry(130,30,120,30);

	connects();

	startTimer(500);

}
void CWaitJoinWidget::connects()
{
	connect(m_pBtnJoinRoom,SIGNAL(clicked()),this,SLOT(on_BtnJoinRoom()));

}

void CWaitJoinWidget::on_BtnCreateRoom()
{

}
void CWaitJoinWidget::on_BtnJoinRoom()
{

	QTreeWidgetItem* pCurrentItem = m_pTreeWidgetRoom->currentItem();

	if(pCurrentItem != NULL)
	{



		
	}
	else
	{
		QMessageBox::warning(this,tr("无法加入"),tr("请选择一个房间"));
	}

}

void CWaitJoinWidget::timerEvent(QTimerEvent * evt)
{
	
	if(g_iStatus == 0)
	{
		unsigned char buf[1024] = {};
		int iResult = g_recvMessage(buf,1024);

		if(iResult > 0)
		{
			//判断报文类型，查询房间返回报文则显示房间
			//创建房间和加入房间成功报文则发送改变房间消息
			
			
			//testAddRoom(buf,iResult);//测试代码
		}
	}
	
}

void CWaitJoinWidget::testAddRoom(unsigned char* buf,int ilen)
{
	//测试代码

	static int iNum = 1;

	QTreeWidgetItem* item1 = new QTreeWidgetItem(m_pTreeWidgetRoom);
	char strBuf[32] = {};
	sprintf(strBuf,"房间%d",iNum);
	item1->setText(0,tr(strBuf));

	iNum++;

}