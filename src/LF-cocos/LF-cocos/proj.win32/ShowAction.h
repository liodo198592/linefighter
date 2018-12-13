#ifndef _SHOW_ACTION_H_
#define _SHOW_ACTION_H_
#include "cocos2d.h"
#include "global_cocos.h"
USING_NS_CC;
enum ShowActionType
{
	SA_INVALID,
	SA_MOVE,
	SA_QUAN,
	SA_QIGONG,
	SA_SHUAI,
	SA_HUANXIAN,
	SA_BISHA,
	SA_SHAN,
	SA_DAMAGE
};

class CShowAction
{
public:
	CShowAction(ShowActionType showActionType):m_showActionType(showActionType),m_iActionStat(0)
	{

	}
	~CShowAction(void)
	{

	}

	int m_iActionStat;//当前播放状态，0未播放，1正在播放 2播放完毕


	string m_strActPlayer;//动作释放的玩家

	ShowActionType m_showActionType;
	
};

class CShowAction_Move : public CShowAction
{
public:
	CShowAction_Move():CShowAction(SA_MOVE),m_bUsePos(false)
	{

	}
	~CShowAction_Move()
	{

	}

	int m_iAimPos;

	bool m_bUsePos;//是否用aimPos作为目标坐标
	CCPoint aimPos;
};


class CShowAction_HuanXian : public CShowAction
{
public:
	CShowAction_HuanXian():CShowAction(SA_HUANXIAN)
	{

	}
	~CShowAction_HuanXian()
	{

	}

	int m_iAimXian;// 1上线 2下线
};

class CShowAction_Quan : public CShowAction
{
public:
	CShowAction_Quan():CShowAction(SA_QUAN)
	{

	}

	~CShowAction_Quan()
	{

	}


};

class CShowAction_QiGong : public CShowAction
{
public:
	CShowAction_QiGong():CShowAction(SA_QIGONG)
	{

	}
	~CShowAction_QiGong()
	{

	}

	int m_iAimPos;//1向左，2向右
};

class CShowAction_BiSha : public CShowAction
{
public:
	CShowAction_BiSha():CShowAction(SA_BISHA)
	{

	}

	int m_iAimPos;//1向左，2，向右
};

class CShowAction_Shuai : public CShowAction
{
public:
	CShowAction_Shuai():CShowAction(SA_SHUAI)
	{

	}
	~CShowAction_Shuai()
	{

	}
};

class CShowAction_Damage : public CShowAction
{
public:
	CShowAction_Damage():CShowAction(SA_DAMAGE)
	{

	}
	~CShowAction_Damage()
	{

	}

	int m_iDamageBlood;
};

class CShowAction_ShanBi : public CShowAction
{
public:
	CShowAction_ShanBi():CShowAction(SA_SHAN)
	{

	}
	~CShowAction_ShanBi()
	{

	}
};

#endif