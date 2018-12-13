#include "WaitStartWidget.h"

CWaitStartWidget::CWaitStartWidget(QWidget* parent):QWidget(parent)
{

	resize(800,600);
	m_pTableWidget = new QTableWidget(this);
	m_pTableWidget->setGeometry(20,20,310,530);
	m_pTableWidget->setColumnCount(3);
	m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	QStringList strList;
	strList.push_back(tr("玩家名"));
	strList.push_back(tr("等级"));
	strList.push_back(tr("荣誉"));

	m_pTableWidget->setHorizontalHeaderLabels(strList);


	m_pBtnStart = new QPushButton(tr("开始游戏"),this);
	m_pBtnStart->setGeometry(460,120,230,80);
	m_pBtnExit = new QPushButton(tr("返回"),this);
	m_pBtnExit->setGeometry(460,310,230,90);


	startTimer(500);
	connects();


}

void CWaitStartWidget::connects()
{
	connect(m_pBtnExit,SIGNAL(clicked()),this,SLOT(on_BtnExit()));
	
	
}

void CWaitStartWidget::on_BtnExit()
{

	emit(changeStatus(0));

}