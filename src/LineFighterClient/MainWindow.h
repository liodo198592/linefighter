#ifndef _CMAIN_WINDOW_H_
#define _CMAIN_WINDOW_H_
#include "global.h"
#include "ConDialog.h"
#include "WaitJoinWidget.h"
#include "WaitStartWidget.h"
#include "GamingWidget.h"

class CMainWindow : public QMainWindow
{ 
	Q_OBJECT
public:
	CMainWindow(QWidget* parent = 0);
	~CMainWindow(void);

private:
	void setupUI();
	void connects();


private slots:
	void on_TcpDialogCon();
	void on_ChangeStatus(int);

private:
	CTcpConDialog* m_pTcpDialog;
	CWaitJoinWidget* m_pWaitJoinWidget;
	CWaitStartWidget* m_pWaitStartWidget;
	CGamingWidget* m_pGamingWidget;

	QStackedWidget* m_pStackedWidget;

};

#endif