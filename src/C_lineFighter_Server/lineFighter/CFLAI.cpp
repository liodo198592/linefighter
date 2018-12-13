#include  "CFLAI.h"
#include  "CPlayer.h"
#include  "CAction.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

void CFLAI::getAction(vector<CAction>& subVec, CPlayer player,vector<CPlayer>& vplayers)
{
    //随机选择3部的动作
	int Rate = 0;
	int frandom = 0;
	int X, Y;
	player.getPosition(X,Y);
	EACTION eType;

	for(int i = 0 ; i < 3; ++i)
	{
		if(player.getHp() < 7)
		{
			Rate = 7;
		}
		else
		{
			Rate = 6;
		}
		
		frandom = ((double)rand()/(double)RAND_MAX) *(Rate) +1;
		eType = (EACTION)frandom;
		CAction p(eType);
		if(eType == EACTION_Move)
		{
			p.m_destX = X;
			p.m_destY = ((double)rand()/(double)RAND_MAX) *(3) +1;
		}
		else if(eType == EACTION_Air || eType == EACTION_Kill)
		{
			p.m_derection = ((double)rand()/(double)RAND_MAX) *(2) +1;
		}
		else if(eType == EACTION_ChangeLine)
		{
			if(X == 1) X = 2;
			else X = 1;
		}
		subVec.push_back(p);
	}
}