#ifndef _CLOG_DIALOG_H_
#define _CLOG_DIALOG_H_
#include "global.h"

class CLogDialog : public QDialog
{
public:
	CLogDialog(QWidget *parent=0);
	~CLogDialog(void);
private:
	void setupUi();
	void connects();

private:
	QLabel *m_lblName;
	QLabel *m_lblIP;
public:	
	QLineEdit *m_LineIP;
	QLineEdit *m_LineName;
	QPushButton *m_BtnServer;
	QPushButton *m_BtnClient;


};

#endif
