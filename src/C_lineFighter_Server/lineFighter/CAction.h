#ifndef _CACTION_H_
#define _CACTION_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CProtocol.h"

class CAction
{
public:
	CAction(){}
	CAction(EACTION eType);

	virtual ~CAction(){}

	void operator = (CAction room)
	{
		this->m_idemage = room.m_idemage;         
		this->m_range = room.m_range; // 1 表示 点 ，2 表示 线攻击
		this->m_derection = room.m_derection; //1 表示左 ： 小于的方向  2表示右： 大于的方向 , -1 表示无效
		this->m_destX = room.m_destX;
		this->m_destY = room.m_destY;
		this->m_eType = room.m_eType;
	}
	CAction(const CAction &room)
	{
		this->m_idemage = room.m_idemage;         
		this->m_range = room.m_range; // 1 表示 点 ，2 表示 线攻击
		this->m_derection = room.m_derection; //1 表示左 ： 小于的方向  2表示右： 大于的方向 , -1 表示无效
		this->m_destX = room.m_destX;
		this->m_destY = room.m_destY;
		this->m_eType = room.m_eType;
	}

	void init(int idemage, int range)
	{
		m_idemage = idemage;
		m_range = range;
	}

	virtual int damegeTo(CAction act); //返回伤对方多少血

	//是否能够攻击到对方, > 0 表示 能攻击到，-1 表示不能攻击到, 数字越小击中的可能性越大
	virtual int rangeTo(int selfX , int selfY , int youX, int youY); 

	virtual void moveTo(int oldX , int oldY, int& newX , int& newY);

	virtual EACTION getType(){return m_eType;}

	virtual string getDescription();
public:
	int m_idemage;         
	int m_range; // 1 表示 点 ，2 表示 线攻击
	int m_derection; //1 表示左 ： 小于的方向  2表示右： 大于的方向 , -1 表示无效
	int m_destX;
	int m_destY;
	EACTION m_eType;
};

#endif