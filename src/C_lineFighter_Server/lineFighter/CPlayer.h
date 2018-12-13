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
		this->pX = room.pX; //  X轴 1 ， 2 表示 A ， B 
		this->pY = room.pY; //  1 ， 2， 3   
		this->iHp = room.iHp;// 血量
		this->iMaxHp = room.iMaxHp;// 血量
		this->iTeam = room.iTeam; //纯数字，数字一样的为同一个组
		this->eLifeStat = room.eLifeStat; //1 表示存活 2 表示死亡
		this->pAI = room.pAI; //智能AI
	}
	CPlayer(const CPlayer &room)
	{
		this->m_strName = room.m_strName;
		this->pX = room.pX; //  X轴 1 ， 2 表示 A ， B 
		this->pY = room.pY; //  1 ， 2， 3   
		this->iHp = room.iHp;// 血量
		this->iMaxHp = room.iMaxHp;// 血量
		this->iTeam = room.iTeam; //纯数字，数字一样的为同一个组
		this->eLifeStat = room.eLifeStat; //1 表示存活 2 表示死亡
		this->pAI = room.pAI; //智能AI
	}

	virtual int setPosition(int X , int Y){pX = X; pY = Y; return 0;}; //设定位置
	virtual int getPosition(int& X , int& Y){X = pX ; Y = pY; return 0;}; //获取位置信息
	virtual int damageHp(int idmgHp)
	{
		iHp -= idmgHp;
		if (iHp < 0)
		{
			iHp = 0;
		}
		return 0;
	}//修改HP
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
	int pX; //  X轴 1 ， 2 表示 A ， B 
	int pY; //  1 ， 2， 3   
	int iHp;// 血量
	int iMaxHp;// 血量
	int iTeam; //纯数字，数字一样的为同一个组
	ELIFESTAT eLifeStat; //1 表示存活 2 表示死亡
	CFLAI * pAI; //智能AI

private:
	
	
};

#endif