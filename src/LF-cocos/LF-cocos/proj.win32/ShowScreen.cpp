#include "ShowScreen.h"
#ifdef MAKE_ANDRIOD
#include "CClientStatusGame.h"
#else
#include "LFClient\CClientStatusGame.h"
#endif

#include "CFormatUnit.h"
#include "tool.h"

extern string g_userName;
CShowScreen::CShowScreen(CCLayer* pLayer,CAbstractMap* pMap):m_pLayer(pLayer),m_pMap(pMap)
{

}


CShowScreen::~CShowScreen(void)
{
}

void CShowScreen::initPlayers(vector<CPlayer> vecPlayer)
{

	int iPosIndex = 1;
	for(vector<CPlayer>::iterator it = vecPlayer.begin();it != vecPlayer.end();++it)
	{
		string name = it->getName();
		int iPos = (it->pX - 1)*3 + it->pY;
		PerMapPos mapPos = getMapPosByIndex(iPosIndex);
		CCPDirection direction = getDirectionByPosIndex(iPos);
		m_pBattlePlayer[name] = new CCharBattleGirl(m_pLayer,name,direction,it->getHp());

		CCPoint girlPos = m_pMap->getPosCoordinate(iPos,mapPos);
		m_pBattlePlayer[name]->setPosition(girlPos.x,girlPos.y);

		CCPoint showStatPos = m_pMap->getPlayerStatCoordinate(iPosIndex);
		initPlayerStat(name,showStatPos,it->iHp);
		

		m_pPlayerPos[name].bigPos = iPos;
		m_pPlayerPos[name].posDirect = mapPos;
		m_PlayerTempPos[name] = iPos;
		iPosIndex++;
	}
}
int CShowScreen::getMyPos()
{
	return m_pPlayerPos[g_userName].bigPos;
}
void CShowScreen::initPlayerStat(string PlayerName,CCPoint showPos,int iHp)
{
	string showName = PlayerName;
	GBKToUTF8(showName,"gbk","utf-8");    
	char strBuf[32]={};
	sprintf(strBuf,"%s HP:%d",showName.c_str(),iHp);

	CCLabelTTF* pTtf = CCLabelTTF::create(strBuf,"Marker Felt",18);

	pTtf->setColor(ccc3(255,255,255));

	pTtf->setPosition(showPos);
	m_pLayer->addChild(pTtf);

	m_mapPlayerStatShow[PlayerName] = pTtf;
}
void CShowScreen::updatePlayerHp(string strPlayer)
{
	string showName = strPlayer;
	GBKToUTF8(showName,"gbk","utf-8");   
	if(m_mapPlayerStatShow.find(strPlayer) != m_mapPlayerStatShow.end())
	{
		int iHp = m_pBattlePlayer[strPlayer]->getCurrentHp();
		char strBuf[32]={};
		sprintf(strBuf,"%s HP:%d",showName.c_str(),iHp);
		m_mapPlayerStatShow[strPlayer]->setString(strBuf);
	}
}
CCPDirection CShowScreen::getDirectionByPosIndex(int iIndex)
{
	switch (iIndex)
	{
	case 1:
	case 2:
	case 4:
	case 5:
		return DIRECTION_RIGHT;
	case 3:
	case 6:
		return DIRECTION_LEFT;
	default:
		return DIRECTION_RIGHT;
	}

}
PerMapPos CShowScreen::getMapPosByIndex(int iIndex)
{
	switch (iIndex)
	{
	case 1:
		return LEFT_UP;
	case 2:
		return RIGHT_UP;
	case 3:
		return LEFT_DOWN;
	case 4:
		return RIGHT_DOWN;
	default:
		return CENTER;
		break;
	}
}
ShowActionType CShowScreen::getShowTypeByServerType(EACTION serverType)
{

	switch (serverType)
	{
	case EACTION_Hand:
		return SA_QUAN;
	case EACTION_Air:
		return SA_QIGONG;
	case EACTION_Fall:
		return SA_SHUAI;
	case EACTION_Move:
		return SA_MOVE;
	case EACTION_ChangeLine:
		return SA_HUANXIAN;
	case EACTION_Avoid:
		return SA_SHAN;
	case EACTION_Kill:
		return SA_BISHA;
	case EACTION_Invalid:
	default:
		return SA_INVALID;
	}

}
void CShowScreen::showGaming(CClientStatusGameBase* pCmdStatus)
{
	generateShowAction(pCmdStatus);

	m_iShowStat = 1;
}
bool CShowScreen::showingAction()
{
	return m_iShowStat == 1;
}
void CShowScreen::generateShowAction(CClientStatusGameBase* pCmdStatus)
{

	m_mapShowAction.clear();

	for(int i=0;i<3;i++)
	{
		set<string> setPlayerHasAction;
		//3步循环
		for(vector<CFormatUnit *>::iterator it = pCmdStatus->vformatdescription[i].begin();
			it != pCmdStatus->vformatdescription[i].end();++it)
		{
			EUnitType unitType = (*it)->getType();
			string strName;
			
			switch (unitType)
			{
			case EUnitType_Move:
				{
					CShowAction_Move * pAction = new CShowAction_Move();
					CMoveUnit* pMoveUnit = (CMoveUnit*)*it;
					strName = pMoveUnit->username;
					pAction->m_strActPlayer = pMoveUnit->username;
					pAction->m_iAimPos = (pMoveUnit->destx - 1)*3 + pMoveUnit->desty;
					
					m_mapShowAction[i].push_back(pAction);
					//addShowAction(m_mapShowAction[i],pAction);
					m_PlayerTempPos[pAction->m_strActPlayer] = pAction->m_iAimPos;
					break;
				}
			case EUnitType_ATTACK:
				{
					CAttackUnit* pAttackUnit = (CAttackUnit*)*it;
					strName = pAttackUnit->attackerName;
					CAction playerAction = pCmdStatus->vShowActions[pAttackUnit->attackerName][i];
					ShowActionType showActionType = getShowTypeByServerType(playerAction.getType());
					CShowAction* pShowAction = NULL;

					if(showActionType == SA_INVALID)
					{
						continue;
					}

					if(setPlayerHasAction.find(pAttackUnit->attackerName) == setPlayerHasAction.end())
					{
						if(showActionType == SA_QUAN)
						{
							pShowAction = new CShowAction_Quan();

						}
						else if(showActionType == SA_QIGONG)
						{
							pShowAction = new CShowAction_QiGong();
							((CShowAction_QiGong*)pShowAction)->m_iAimPos = playerAction.m_derection;

						}
						else if(showActionType == SA_BISHA)
						{
							pShowAction = new CShowAction_BiSha();
							((CShowAction_BiSha*)pShowAction)->m_iAimPos = playerAction.m_derection;
						}
						else if(showActionType == SA_SHUAI)
						{
							pShowAction = new CShowAction_Shuai();
						}
						else
						{
							continue;
						}
						pShowAction->m_strActPlayer = pAttackUnit->attackerName;

						//m_mapShowAction[i].push_back(pShowAction);
						addShowAction(m_mapShowAction[i],pShowAction);
					}

					if(setPlayerHasAction.find(pAttackUnit->sufferName) == setPlayerHasAction.end())
					{
						CPlayer* pPlayer = pCmdStatus->getPlayerByName(pAttackUnit->sufferName);
						if(pPlayer == NULL)
						{
							continue;
						}
						if(pAttackUnit->isAttacked)
						{
							pShowAction = new CShowAction_Damage();
							((CShowAction_Damage*)pShowAction)->m_iDamageBlood = pAttackUnit->damege;
						}
						else
						{
							pShowAction = new CShowAction_ShanBi();
						}
						pShowAction->m_strActPlayer = pAttackUnit->sufferName;

						m_mapShowAction[i].push_back(pShowAction);
						//addShowAction(m_mapShowAction[i],pShowAction);
					}

					break;
				}
			case EUnitType_Miss:
				{
					CMissUnit* pMisUnit = (CMissUnit*)*it;
					strName = pMisUnit->username;
					CAction playerAction = pCmdStatus->vShowActions[pMisUnit->username][i];

					ShowActionType showActionType = getShowTypeByServerType(playerAction.getType());
					CShowAction* pShowAction = NULL;

					if(showActionType == SA_INVALID)
					{
						continue;
					}

					if(setPlayerHasAction.find(pMisUnit->username) == setPlayerHasAction.end())
					{
						if(showActionType == SA_QUAN)
						{
							pShowAction = new CShowAction_Quan();

						}
						else if(showActionType == SA_QIGONG)
						{
							pShowAction = new CShowAction_QiGong();
							((CShowAction_QiGong*)pShowAction)->m_iAimPos = playerAction.m_derection;

						}
						else if(showActionType == SA_BISHA)
						{
							pShowAction = new CShowAction_BiSha();
							((CShowAction_BiSha*)pShowAction)->m_iAimPos = playerAction.m_derection;
						}
						else if(showActionType == SA_SHUAI)
						{
							pShowAction = new CShowAction_Shuai();
						}
						else
						{
							continue;
						}
						pShowAction->m_strActPlayer = pMisUnit->username;

						//m_mapShowAction[i].push_back(pShowAction);
						addShowAction(m_mapShowAction[i],pShowAction);
					}
					break;
				}

			}

		}
	}

	m_iShowStat = 1;
	m_iCueentShowRound = 0;
	m_iCurrentShowIndex = 0;
}
void CShowScreen::addShowAction(vector<CShowAction*>& vecShowAction,CShowAction* pShowAction)
{
	CCPoint currentPoint = m_pMap->getPosCoordinate(m_PlayerTempPos[pShowAction->m_strActPlayer],
		m_pPlayerPos[pShowAction->m_strActPlayer].posDirect);

	CCPoint toPoint = m_pMap->getPosCoordinate(m_PlayerTempPos[pShowAction->m_strActPlayer],CENTER);

	CShowAction_Move* pMoveAction = new CShowAction_Move();
	pMoveAction->m_strActPlayer = pShowAction->m_strActPlayer;
	pMoveAction->m_bUsePos = true;
	pMoveAction->aimPos = toPoint;

	CShowAction_Move* pMoveActionBack = new CShowAction_Move();
	pMoveActionBack->m_strActPlayer = pShowAction->m_strActPlayer;
	pMoveActionBack->m_bUsePos = true;
	pMoveActionBack->aimPos = currentPoint;

	vecShowAction.push_back(pMoveAction);
	vecShowAction.push_back(pShowAction);
	vecShowAction.push_back(pMoveActionBack);

}
void CShowScreen::showAction()
{
	
	if(m_iCueentShowRound >= m_mapShowAction.size())
	{
		m_iShowStat = 0;
		return;
	}

	if(m_iCurrentShowIndex >= m_mapShowAction[m_iCueentShowRound].size())
	{
		m_iCueentShowRound++;
		m_iCurrentShowIndex = 0;
		return;
	}

	CShowAction* pShowAction = m_mapShowAction[m_iCueentShowRound][m_iCurrentShowIndex];

	if(pShowAction->m_iActionStat == 0)
	{
		if(m_pBattlePlayer.find(pShowAction->m_strActPlayer) != m_pBattlePlayer.end())
		{
			CCharBattleGirl* pGirl = m_pBattlePlayer[pShowAction->m_strActPlayer];

			switch (pShowAction->m_showActionType)
			{
			case SA_MOVE:
			case SA_HUANXIAN:
				{
					CShowAction_Move* pMoveAction = (CShowAction_Move*)pShowAction;
					//pGirl->showmove();
					CCPoint aimPos;
					if(!pMoveAction->m_bUsePos)
					{
						aimPos = m_pMap->getPosCoordinate(pMoveAction->m_iAimPos,m_pPlayerPos[pMoveAction->m_strActPlayer].posDirect);
						m_pPlayerPos[pMoveAction->m_strActPlayer].bigPos = pMoveAction->m_iAimPos;
					}
					else
					{
						aimPos = pMoveAction->aimPos;
					}
					
					pGirl->moveToPos(aimPos.x,aimPos.y);
					break;
				}
			case SA_QUAN:
				{
					//CShowAction_Quan* pQunAction = (CShowAction_Quan*)pShowAction;
					pGirl->showfist();
					break;
				}
			case SA_QIGONG:
				{
					CShowAction_QiGong* pQigongAction = (CShowAction_QiGong*)pShowAction;
					pGirl->showqigong(pQigongAction->m_iAimPos);

					break;
				}
			case SA_SHUAI:
				{
					//先用拳代替摔
					pGirl->showfist();
					break;
				}
			case SA_BISHA:
				{
					pGirl->showbisha();
					break;
				}
			case SA_SHAN:
				{
					pGirl->showaviod();
					break;
				}
			case SA_DAMAGE:
				{
					CShowAction_Damage* pDamageAction = (CShowAction_Damage*)pShowAction;
					//pGirl->setBlood(pDamageAction->m_iAimBlood);
					pGirl->damageBlood(pDamageAction->m_iDamageBlood);
					updatePlayerHp(pDamageAction->m_strActPlayer);
					pGirl->showbeattack();
					break;
				}
			case SA_INVALID:
			default:
				break;
			}
		}
		pShowAction->m_iActionStat = 1;
	}
	else if(pShowAction->m_iActionStat == 1)
	{
		if(!m_pBattlePlayer[pShowAction->m_strActPlayer]->isShowingAction())
		{
			pShowAction->m_iActionStat = 2;
		}
	}
	else if(pShowAction->m_iActionStat == 2)
	{
		m_iCurrentShowIndex++;
	}

}

CShowScreenSingle::CShowScreenSingle(map<string,CCharBattleGirl *> vChars,CCLayer* pLayer,CAbstractMap* pMap):
	CShowScreen(pLayer,pMap)
{
	m_pBattlePlayer = vChars;
}
CShowScreenSingle::~CShowScreenSingle()
{

}
//初始化为移动已经存在的人物至指定位置
void CShowScreenSingle::initPlayers(vector<CPlayer> vecPlayer)
{

	int iPosIndex = 1;
	for(vector<CPlayer>::iterator it = vecPlayer.begin();it != vecPlayer.end();++it)
	{
		string name = it->getName();
		int iPos = (it->pX - 1)*3 + it->pY;
		PerMapPos mapPos = getMapPosByIndex(iPosIndex);
		CCPDirection direction = getDirectionByPosIndex(iPos);
		//m_pBattlePlayer[name] = new CCharBattleGirl(m_pLayer,name,direction,it->getHp());
		m_pBattlePlayer[name]->setDirection(direction);

		CCPoint girlPos = m_pMap->getPosCoordinate(iPos,mapPos);
		//m_pBattlePlayer[name]->setPosition(girlPos.x,girlPos.y);
		m_pBattlePlayer[name]->moveToPos(2,girlPos.x,girlPos.y);

		CCPoint showStatPos = m_pMap->getPlayerStatCoordinate(iPosIndex);
		initPlayerStat(name,showStatPos,it->iHp);
		

		m_pPlayerPos[name].bigPos = iPos;
		m_pPlayerPos[name].posDirect = mapPos;
		m_PlayerTempPos[name] = iPos;
		iPosIndex++;
	}

}