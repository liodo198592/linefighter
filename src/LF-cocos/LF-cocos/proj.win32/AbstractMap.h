#ifndef _ABSTRACT_MAP_H_
#define _ABSTRACT_MAP_H_
#include "cocos2d.h"
USING_NS_CC;
#include "global_cocos.h"

enum PerMapPos
{
	LEFT_UP,
	RIGHT_UP,
	CENTER,
	LEFT_DOWN,
	RIGHT_DOWN,
};

class CAbstractMap
{
public:
	CAbstractMap(string MapFilePath);
	~CAbstractMap(void);

	string getMapFilePath();

	CCPoint getPosCoordinate(int posIndex,PerMapPos posDirect);

	//��ȡ����״̬�������꣬����Ϊ�ڼ���
	CCPoint getPlayerStatCoordinate(int iIndex);

	virtual void initCoordinate() = 0;


protected:
	//ÿ��Сλ�õ�����
	//<��λ�ã�<Сλ�ã�����> >	��λ�ô��Ͽ�ʼ����Ϊ1,2,3,4,5,6
	map<int,map<PerMapPos,CCPoint> > m_mapPosCoordinate;

	map<int,CCPoint> m_mapPlayerStatShowPos;

	string m_MapFilePath;

	CCMenu *m_pActionMenu;

};

#endif