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
		this->m_range = room.m_range; // 1 ��ʾ �� ��2 ��ʾ �߹���
		this->m_derection = room.m_derection; //1 ��ʾ�� �� С�ڵķ���  2��ʾ�ң� ���ڵķ��� , -1 ��ʾ��Ч
		this->m_destX = room.m_destX;
		this->m_destY = room.m_destY;
		this->m_eType = room.m_eType;
	}
	CAction(const CAction &room)
	{
		this->m_idemage = room.m_idemage;         
		this->m_range = room.m_range; // 1 ��ʾ �� ��2 ��ʾ �߹���
		this->m_derection = room.m_derection; //1 ��ʾ�� �� С�ڵķ���  2��ʾ�ң� ���ڵķ��� , -1 ��ʾ��Ч
		this->m_destX = room.m_destX;
		this->m_destY = room.m_destY;
		this->m_eType = room.m_eType;
	}

	void init(int idemage, int range)
	{
		m_idemage = idemage;
		m_range = range;
	}

	virtual int damegeTo(CAction act); //�����˶Է�����Ѫ

	//�Ƿ��ܹ��������Է�, > 0 ��ʾ �ܹ�������-1 ��ʾ���ܹ�����, ����ԽС���еĿ�����Խ��
	virtual int rangeTo(int selfX , int selfY , int youX, int youY); 

	virtual void moveTo(int oldX , int oldY, int& newX , int& newY);

	virtual EACTION getType(){return m_eType;}

	virtual string getDescription();
public:
	int m_idemage;         
	int m_range; // 1 ��ʾ �� ��2 ��ʾ �߹���
	int m_derection; //1 ��ʾ�� �� С�ڵķ���  2��ʾ�ң� ���ڵķ��� , -1 ��ʾ��Ч
	int m_destX;
	int m_destY;
	EACTION m_eType;
};

#endif