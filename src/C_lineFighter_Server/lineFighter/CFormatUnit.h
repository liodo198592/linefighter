#ifndef _CFormatUnit_H_
#define _CFormatUnit_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

#define ATTACKED 1
#define MISSED 0

enum EUnitType
{
	EUnitType_ATTACK = 0,
	EUnitType_Move,
	EUnitType_Miss,
};

class CFormatUnit
{
public:
	CFormatUnit(){}
	~CFormatUnit(){}
	virtual void setType(EUnitType type_x)
	{
		type = type_x;
	}
	virtual EUnitType getType()
	{
		return type;
	}
	virtual int decode(vector<string> str) = 0;
	virtual string encode() = 0;
protected:
	EUnitType type;
	
private:
};

class CAttackUnit : public CFormatUnit
{
public:
	CAttackUnit(){}
	~CAttackUnit(){}
	string attackerName; //攻击者名字
	string sufferName; //被攻击者名字
	int isAttacked; //是否被击中 1 被击中  0 未击中
	int damege; //本次攻击动作导致的HP伤害值
protected:
	virtual int decode(vector<string> str);
	virtual string encode();
private:
};

class CMoveUnit : public CFormatUnit
{
public:
	CMoveUnit(){}
	~CMoveUnit(){}
	string username; //移动的玩家姓名
	int srcx;
	int srcy;
	int destx;
	int desty;
protected:
	virtual int decode(vector<string> str);
	virtual string encode();
private:
};

class CMissUnit : public CFormatUnit
{
public:
	CMissUnit(){}
	~CMissUnit(){}
	string username;//攻击未命中的玩家的名字
protected:
	virtual int decode(vector<string> str);
	virtual string encode();
private:
};

class  CFormatUnitFactory
{
public:
	static CFormatUnitFactory * getInstance()
	{
		if (myFactory == NULL)
		{
			myFactory = new CFormatUnitFactory();
		}
		return myFactory;
	}

	CFormatUnit * create(string title)
	{
		CFormatUnit * item = NULL;
		if (strcmp(title.c_str(),"attack")==0 )
		{
			item = new CAttackUnit();
			item->setType(EUnitType_ATTACK);
		}
		else if (strcmp(title.c_str(),"move")==0)
		{
			item = new CMoveUnit();
			item->setType(EUnitType_Move);
		}
		else if (strcmp(title.c_str(),"miss")==0)
		{
			item = new CMissUnit();
			item->setType(EUnitType_Miss);
		}
		return item;
	}
	
protected:
	static CFormatUnitFactory * myFactory;
	CFormatUnitFactory(){}
	~CFormatUnitFactory(){}
private:

};


#endif