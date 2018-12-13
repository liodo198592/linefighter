#include "WaitStartWidget.h"

CWaitStartWidget::CWaitStartWidget(QWidget* parent):QWidget(parent)
{

	resize(800,600);
	m_pTableWidget = new QTableWidget(this);
	m_pTableWidget->setGeometry(20,20,310,530);
	m_pTableWidget->setColumnCount(3);
	m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	QStringList strList;
	strList.push_back(tr("�����"));
	strList.push_back(tr("�ȼ�"));
	strList.push_back(tr("����"));

	m_pTableWidget->setHorizontalHeaderLabels(strList);


	m_pBtnStart = new QPushButton(tr("��ʼ��Ϸ"),this);
	m_pBtnStart->setGeometry(460,120,230,80);
	m_pBtnExit = new QPushButton(tr("����"),this);
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