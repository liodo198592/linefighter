#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CFLAI.h"

class CFLAI;
class CAction;


enum ELIFESTAT
{
	ELIFESTAT_INVALID = -1,
	ELIFESTAT_ALIFE,
	ELIFESTAT_DEAD,
};

class CPlayer
{
public:
	CPlayer(){pAI = NULL;}
	CPlayer(string strName);
	virtual ~CPlayer(){}
	void operator = (CPlayer room)
	{
		this->m_strName = room.m_strName;
		this->pX = room.pX; //  X�� 1 �� 2 ��ʾ A �� B 
		this->pY = room.pY; //  1 �� 2�� 3   
		this->iHp = room.iHp;// Ѫ��
		this->iMaxHp = room.iMaxHp;// Ѫ��
		this->iTeam = room.iTeam; //�����֣�����һ����Ϊͬһ����
		this->eLifeStat = room.eLifeStat; //1 ��ʾ��� 2 ��ʾ����
		this->pAI = room.pAI; //����AI
	}
	CPlayer(const CPlayer &room)
	{
		this->m_strName = room.m_strName;
		this->pX = room.pX; //  X�� 1 �� 2 ��ʾ A �� B 
		this->pY = room.pY; //  1 �� 2�� 3   
		this->iHp = room.iHp;// Ѫ��
		this->iMaxHp = room.iMaxHp;// Ѫ��
		this->iTeam = room.iTeam; //�����֣�����һ����Ϊͬһ����
		this->eLifeStat = room.eLifeStat; //1 ��ʾ��� 2 ��ʾ����
		this->pAI = room.pAI; //����AI
	}

	virtual int setPosition(int X , int Y){pX = X; pY = Y; return 0;}; //�趨λ��
	virtual int getPosition(int& X , int& Y){X = pX ; Y = pY; return 0;}; //��ȡλ����Ϣ
	virtual int damageHp(int idmgHp)
	{
		iHp -= idmgHp;
		if (iHp < 0)
		{
			iHp = 0;
		}
		return 0;
	}//�޸�HP
	virtual ELIFESTAT checkLifeStat()
	{
		if(eLifeStat == ELIFESTAT_DEAD)
		{
			return ELIFESTAT_INVALID;
		}
		if(iHp <= 0)
		{
			eLifeStat = ELIFESTAT_DEAD;
			return eLifeStat;
		}
		return eLifeStat;
	}
	virtual void init()
	{
		iMaxHp = MAX_HP;
		iHp = 	iMaxHp;
	}

	virtual void setHp(int iHp_x)
	{
		iHp = iHp_x;
	}

	virtual int getTeam()
	{
		return iTeam;
	}
	virtual void setTeam(int team_x)
	{
		iTeam = team_x;
	}

	virtual string getName(){return m_strName;}
	virtual int getHp(){return iHp;}
	virtual ELIFESTAT getLifeStat(){return eLifeStat;}
	virtual void getAction(vector<CAction>& subVec , vector<CPlayer>& vplayers);
	
	virtual void setAI(CFLAI ai);

	string m_strName;
	int pX; //  X�� 1 �� 2 ��ʾ A �� B 
	int pY; //  1 �� 2�� 3   
	int iHp;// Ѫ��
	int iMaxHp;// Ѫ��
	int iTeam; //�����֣�����һ����Ϊͬһ����
	ELIFESTAT eLifeStat; //1 ��ʾ��� 2 ��ʾ����
	CFLAI * pAI; //����AI

private:
	
	
};

#endif