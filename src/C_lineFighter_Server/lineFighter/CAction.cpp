#include "CPlayer.h"
#include "CAction.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

CAction::CAction(EACTION eType)
{
	m_eType = eType;
	m_idemage = 0;
	m_range = -1;
	m_derection = -1;
	m_destX = 0;
	m_destY = 0;
	switch(m_eType)
	{
		case EACTION_Hand:
			{
				init(2,1);
				break;
			}
		case EACTION_Air:
			{
				init(1,2);

				break;
			}
		case EACTION_Fall:
			{
				init(2,1);
				break;
			}
		case EACTION_Move:
			{
				init(0,0);
				break;
			}
		case EACTION_ChangeLine:
		case EACTION_Avoid:
			{
				break;
			}
		case EACTION_Kill:
			{
				init(5,2);
				break;
			}
		case EACTION_Invalid:
			{
				init(1,2);
				break;
			}
		default:
			break;
	}
}

int CAction::rangeTo(int selfX , int selfY , int youX, int youY)
{
	if(this->m_eType == EACTION_Move ||
		this->m_eType == EACTION_ChangeLine ||
	    this->m_eType == EACTION_Avoid || this->m_eType == EACTION_Invalid)
	{
		return -1;
	}

	if (m_range == 1)
	{
		if(selfX == youX && selfY == youY) return 0;
		else return -1;
	}
	else if(m_range == 2)
	{
		if(selfX != youX) return -1;
		else if(m_derection == 1)
		{
			if(selfY >= youY) return (abs(youY-selfY));
			else return -1;
		}
		else if(m_derection == 2)
		{
			if(selfY <= youY) return (abs(selfY - youY));
			else return -1;
		}
	}
	return -1;
}

int CAction::damegeTo(CAction act) //返回伤对方多少血
{
	if(this->m_eType == EACTION_Hand || this->m_eType == EACTION_Air || this->m_eType == EACTION_Kill)
	{
		if(act.m_eType == EACTION_Avoid)
		{
			return 0;
		}
		else return m_idemage;
	}
	if(this->m_eType == EACTION_Fall)
	{
		if(act.m_eType == EACTION_Avoid)
		{
			return m_idemage * 2;
		}
		else return -1;
	}
		
	return 0;
}

void CAction::moveTo(int oldX , int oldY, int& newX , int& newY)
{
	if(this->m_eType == EACTION_Move)
	{
		newY = m_destY;
		newX = oldX;
	}
	else if(this->m_eType == EACTION_ChangeLine)
	{
		newY = oldY;
		if(oldX == 1) newX = 2;
		else newX = 1;

	}
	else
	{
		newX = oldX;
		newY = oldY;
	}
}

string CAction::getDescription()
{
	switch(m_eType)
	{
		case EACTION_Hand:
			{
				return "拳";
				break;
			}
		case EACTION_Air:
			{
				if(m_derection == 1)
				{
					return "向左气功";
				}
				else
				{
					return "向右气功";
				}
				break;
			}
		case EACTION_Fall:
			{
				return "摔";
				break;
			}
		case EACTION_Move:
			{
				char temp[20];
				sprintf(temp," (%d,%d) ",m_destX,m_destY);
				return "移动到 " + string(temp);
				break;
			}
		case EACTION_ChangeLine:
			{
				return "换线";
			}
		case EACTION_Avoid:
			{
				return "回避";
				break;
			}
		case EACTION_Kill:
			{
				if(m_derection == 1)
				{
					return "向左必杀";
				}
				else
				{
					return "向右必杀";
				}
				break;
			}
		case EACTION_Invalid:
			{
				return "待机";
				break;
			}
		default:
			break;
	}
	return "";
}