#include "CPlayer.h"
#include "CFLAI.h"
#include "CAction.h"
#ifdef MAKE_ANDRIOD
#include "ConsoleShow.h"
#else
#include "../../C_lineFighter/lineFighter/ConsoleShow.h"
#endif
#include "CFLAI.h"

extern char g_Control1Name[30];;

CPlayer::CPlayer(string strName)
{
	m_strName = strName;
	iMaxHp = MAX_HP;
	iHp = 	iMaxHp;
	eLifeStat = ELIFESTAT_ALIFE;
	pAI = NULL;
}

void CPlayer::getAction(vector<CAction>& subVec , vector<CPlayer>& vplayers)
{
	if(pAI != NULL)
	{
		pAI->getAction(subVec,*this,vplayers); 
	}
	return;
}

void CPlayer::setAI(CFLAI ai)
{
	pAI = new CFLAI(ai);
}
