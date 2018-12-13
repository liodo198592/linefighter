#ifndef _WAITSTART_WIDGET_H_
#define _WAITSTART_WIDGET_H_
#include "global.h"

class CWaitStartWidget : public QWidget
{
	Q_OBJECT
public:
	CWaitStartWidget(QWidget* parent = 0);
	~CWaitStartWidget(void){}

private:
	void connects();

public:signals:
	void changeStatus(int);

private slots:
	void on_BtnExit();

private:
	QTableWidget* m_pTableWidget;
	QPushButton* m_pBtnStart;
	QPushButton* m_pBtnExit;
};


#endif