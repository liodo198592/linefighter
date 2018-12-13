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
	void minusHp(int hp);//减血
	void setSize(int width,int height);
	int moveTo(int place);//移动到某位置
	void setCoordinate(int x,int y);//设置frame的真实位置
	void setFrameGeometry(int x,int y,int w,int h,int FontSize);//设置frame属性，实际位置，实际大小，字体大小
	void changeLine();//换线
	void changeToMyColor();
	int getPlace();
	
private:
	QVBoxLayout *m_vBoxLayout;
	QLabel *m_pNameLabel;
	QLabel *m_pHPLabel;
	QLabel *m_pHPShow;
	QString m_PlayerName;
	int m_iPlayerHP;
	int m_iWidth;//frame当前宽度
	int m_iHeight;//frame当前高度
	int m_iPlace;//frame所在的位置格子代码
	int m_iX;//frame真实坐标x
	int m_iY;//frame真实坐标Y

};

#endif
