#include "PlayerFrame.h"
//extern map<int,vector<CPlayerFrame*>> g_mapFramePlace;
//extern map<int,SPlace> g_mapCoordinate;
CPlayerFrame::CPlayerFrame(QString playerName,int place,QWidget *parent):QFrame(parent),m_PlayerName(playerName),m_iPlace(place),m_iPlayerHP(MAX_HP),m_iWidth(FRAME_WIDTH),m_iHeight(FRAME_HEIGHT)
{
	setSize(m_iWidth,m_iHeight);
	setFrameShape(QFrame::Box);
	setFrameShadow(QFrame::Raised);

	setAutoFillBackground(true);
	QPalette palette=this->palette();
	palette.setColor(QPalette::Background,QColor(Qt::GlobalColor::lightGray));
	setPalette(palette);

	QFont font;
	font.setPointSize(FONT_SIZE_MAX);
	font.setBold(true);
	font.setWeight(45);
	m_vBoxLayout=new QVBoxLayout;
	m_pNameLabel=new QLabel(m_PlayerName);
//	m_pNameLabel->setGeometry(0,20,85,20);
	m_pNameLabel->setAlignment(Qt::AlignCenter);
	m_pNameLabel->setFont(font);
	m_pNameLabel->adjustSize();

	m_pHPLabel=new QLabel(QString("HP:%1").arg(m_iPlayerHP));
//	m_pHPLabel->setGeometry(0,50,85,20);
	m_pHPLabel->setAlignment(Qt::AlignCenter);
//	QPalette palette;
	palette.setColor(QPalette::WindowText,Qt::blue);
	m_pHPLabel->setPalette(palette);
	m_pHPLabel->setFont(font);
	m_pHPLabel->adjustSize();

	m_pHPShow=new QLabel(QString(""));
	m_pHPShow->setAlignment(Qt::AlignCenter);
	m_pHPShow->setFont(font);
	m_pHPShow->setPalette(palette);
	m_pHPShow->adjustSize();
	m_vBoxLayout->addWidget(m_pNameLabel);
	m_vBoxLayout->addWidget(m_pHPLabel);
//	m_vBoxLayout->addWidget(m_pHPShow);
	setLayout(m_vBoxLayout);
}

CPlayerFrame::~CPlayerFrame(void)
{
	delete m_pNameLabel;
	delete m_pHPLabel;
}
QString CPlayerFrame::getPlayerName()
{
	return m_PlayerName;
}
int CPlayerFrame::getPlayerHP()
{
	return m_iPlayerHP;
}
void CPlayerFrame::setPlayerName(QString name)
{
	m_PlayerName=name;
}
void CPlayerFrame::setPlayerHP(int hp)
{
	m_iPlayerHP=hp;
}
void CPlayerFrame::changeToMyColor()
{
	QPalette palette=this->palette();
	palette.setColor(QPalette::Background,QColor(Qt::GlobalColor::cyan));
	setPalette(palette);
}
void CPlayerFrame::minusHp(int hp)
{
//	setAutoFillBackground(true);
	m_iPlayerHP-=hp;
	QPalette OldPalette=this->palette();
	QPalette palette=this->palette();
	palette.setColor(QPalette::Background,QColor(Qt::GlobalColor::red));
	setPalette(palette);
	repaint();
	Sleep(400);
	setPalette(OldPalette);
//	setAutoFillBackground(FALSE);
	repaint();
	Sleep(100);
	m_pHPLabel->setText(QString("- %1").arg(hp));
	repaint();
	Sleep(700);
	m_pHPLabel->setText(QString("HP:%1").arg(m_iPlayerHP));
	if(m_iPlayerHP<=5)
	{
		QPalette paletteFont;
		paletteFont.setColor(QPalette::WindowText,Qt::red);
		m_pHPLabel->setPalette(paletteFont);

	}
	repaint();
	

}
void CPlayerFrame::setSize(int width,int height)
{
	resize(width,height);
	m_iWidth=width;
	m_iHeight=height;
}
int CPlayerFrame::getPlace()
{
	return m_iPlace;
}
void CPlayerFrame::changeLine()
{

}
int CPlayerFrame::moveTo(int place)
{
	
	return 0;
}
void CPlayerFrame::setCoordinate(int x,int y)
{
	m_iX=x;
	m_iY=y;
}

void CPlayerFrame::setFrameGeometry(int x,int y,int w,int h,int FontSizes)
{
	setGeometry(x,y,w,h);
	setSize(w,h);
	setCoordinate(x,y);
	QFont font;
	font.setPointSize(FontSizes);
	font.setBold(true);
	font.setWeight(45);
	m_pNameLabel->setFont(font);
	m_pHPLabel->setFont(font);
	m_pHPShow->setFont(font);

}
