#ifndef _WAITJION_WIDGET_H_
#define _WAITJION_WIDGET_H_
#include "global.h"

class CWaitJoinWidget : public QWidget
{
	Q_OBJECT
public:
	CWaitJoinWidget(QWidget* parent = 0);

	~CWaitJoinWidget(void)
	{

	}


private:
	void connects();

	void testAddRoom(unsigned char* buf,int ilen);

protected:
	void timerEvent(QTimerEvent * evt);
public:signals:
	void changeStatus(int);

private slots:
	void on_BtnCreateRoom();
	void on_BtnJoinRoom();



private:
	QTreeWidget* m_pTreeWidgetRoom;
	QGroupBox* m_pGroupBoxCreateRoom;
	QGroupBox* m_pGroupBoxJionRoom;
	QLabel* m_pLabelRoomName;
	QLabel* m_pLabelMaxNum;
	QLineEdit* m_pLineEditRoomName;
	QLineEdit* m_pLineEditMaxNum;
	QPushButton* m_pBtnCreateRoom;
	QPushButton* m_pBtnJoinRoom;


};

#endif
