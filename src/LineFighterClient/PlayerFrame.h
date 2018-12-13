#ifndef _PLAYER_LABEL_H_
#define _PLAYER_LABEL_H_
#include "global.h"
#include <QtGui/QtGui>
#include <QtCore/Qtcore>

class CPlayerFrame : public QFrame
{
public:
	CPlayerFrame(QString playerName,int place,QWidget *parent=0);
	~CPlayerFrame(void);
public:
	QString getPlayerName();
	int getPlayerHP();
	void setPlayerName(QString name);
	void setPlayerHP(int HP);
	void minusHp(int hp);//��Ѫ
	void setSize(int width,int height);
	int moveTo(int place);//�ƶ���ĳλ��
	void setCoordinate(int x,int y);//����frame����ʵλ��
	void setFrameGeometry(int x,int y,int w,int h,int FontSize);//����frame���ԣ�ʵ��λ�ã�ʵ�ʴ�С�������С
	void changeLine();//����
	void changeToMyColor();
	int getPlace();
	
private:
	QVBoxLayout *m_vBoxLayout;
	QLabel *m_pNameLabel;
	QLabel *m_pHPLabel;
	QLabel *m_pHPShow;
	QString m_PlayerName;
	int m_iPlayerHP;
	int m_iWidth;//frame��ǰ���
	int m_iHeight;//frame��ǰ�߶�
	int m_iPlace;//frame���ڵ�λ�ø��Ӵ���
	int m_iX;//frame��ʵ����x
	int m_iY;//frame��ʵ����Y

};

#endif
