#include "CProtocol.h"
#include "CEngine.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#ifdef MAKE_ANDRIOD
#include "ConsoleShow.h"
#else
#include "../../C_lineFighter/lineFighter/ConsoleShow.h"
#endif
#include "CPlayer.h"
#include "CAction.h"
#include "CFLAI.h"

extern int SCFlag; //1 表示 服务器  2表示客户端

extern string selfName; //自身的名字

extern CProtocol * pProtocol;

//引擎循环运行
int CEngine::svc()
{
	srand(0); 

	//初始化四个人
	if (vplayers.empty())
	{
		initGame(vecUser);
	}

	while(isGameOver == -1)
	{
		RoundIndex++;
		vShowActions = vActions;
		vActions.clear();
		waitForAction(iTimeOutPerRound);
		vFormatdiscreption.clear();
		vdiscreption.clear();
		vdispPlayer.clear();
		calcARound(vplayers,vActions,vdiscreption,vFormatdiscreption,vLivedPlayers);
	}
	
	char info[200];
	for (map<int,vector<CPlayer> >::iterator it = vLivedPlayers.begin(); it != vLivedPlayers.end(); ++it)
	{
		int team = it->first;
		sprintf(info," $队伍%d$ 最终获胜 , 成员: ",team);
		string tempstr(info);
		winnerinfo += tempstr;
		for(vector<CPlayer>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV )
		{
			winnerinfo += (*itV).getName() + ";";
		}
	}

	sprintf(info,"回合数 %d",RoundIndex);
	string tempstr(info);
	winnerinfo = tempstr;
	vdiscreption[3].push_back(winnerinfo);
	RoundIndex = ROUND_FINISH;
	roomStatus = EROOM_STATUS_FINISH;
	return 0;
}

int CEngine::findIndexPLbyName(string name)
{
	for (int i = 0 ; i < vplayers.size(); ++i)
	{
		if(strcmp(vplayers[i].getName().c_str(),name.c_str())==0)
		{
			return i;
		}
	}
	return -1;
}

int CEngine::calcARound(vector<CPlayer>& vplayers,map<string,vector<CAction> >& vActions, 
	map<int,vector<string> >& vdiscreption , map<int,vector<string> >& vFormatdiscreption,map<int,vector<CPlayer> >& pLivedPlayers)
{
	//ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("================calcAround start index = %d ============ \n",RoundIndex)));
	EACTION eActionType = EACTION_Invalid;
	for(int iActionIndex = 0; iActionIndex < MAX_ACTNUM; ++iActionIndex)
	{
		for(int iPlayerIndex = 0 ; iPlayerIndex < MAX_PLAYER; ++iPlayerIndex)
		{
			string plname = vplayers[iPlayerIndex].getName();
			//先处理移动事件
			if(vplayers[iPlayerIndex].getLifeStat() != ELIFESTAT_ALIFE)
			{
				continue;
			}

			{
				int tempX,tempY,newX,newY;
				string strDescript = "";
				string strFormat = "";
				char startPosition[20] , endPosition[20];
				eActionType = vActions[plname][iActionIndex].getType();
				if(eActionType == EACTION_Move || eActionType == EACTION_ChangeLine)
				{
					vplayers[iPlayerIndex].getPosition(tempX,tempY);
					vActions[plname][iActionIndex].moveTo(tempX,tempY,newX,newY);
					sprintf(startPosition,"%d,%d",tempX,tempY);
					sprintf(endPosition,"%d,%d",newX,newY);
					strDescript = "" + vplayers[iPlayerIndex].getName() + 
						"从" + (string)startPosition + " 移动到 " + (string)endPosition;
					strFormat="move:"+ vplayers[iPlayerIndex].getName()+":" + (string)startPosition + ":"+(string)endPosition;
					vdiscreption[iActionIndex].push_back(strDescript);
					vFormatdiscreption[iActionIndex].push_back(strFormat);
					vplayers[iPlayerIndex].setPosition(newX,newY);
					//ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("player %s(HP%d) setPosition x=%d , y = %d \n"),plname.c_str(),vplayers[iPlayerIndex].getHp(),newX,newY));
				}
			}
		}	

		//处理攻击事件
		for(int iPlayerIndex = 0 ; iPlayerIndex < MAX_PLAYER; ++iPlayerIndex)
		{
			string plname = vplayers[iPlayerIndex].getName();
			int teamNum = vplayers[iPlayerIndex].getTeam();

			if(vplayers[iPlayerIndex].getLifeStat() != ELIFESTAT_ALIFE)
			{
				continue;
			}
			int selfX = 0,selfY = 0,yourX = 0,yourY = 0;
			vplayers[iPlayerIndex].getPosition(selfX,selfY);
			//获取攻击事件，并判断对应位置的人员
			eActionType = vActions[plname][iActionIndex].getType();
			if(eActionType == EACTION_Hand || eActionType == EACTION_Air || eActionType == EACTION_Fall || eActionType ==EACTION_Kill)
			{
				//循环找taget
				map<int,vector<string> > mIDs;
				int ret = 0;
				for(int iSubIndex = 0 ; iSubIndex < MAX_PLAYER; ++iSubIndex)
				{
					
					if(iSubIndex == iPlayerIndex || vplayers[iSubIndex].getLifeStat() != ELIFESTAT_ALIFE)//表示是自己
					{
						continue;
					}
					if (teamNum == vplayers[iSubIndex].getTeam())//如果是同一队伍的，也不演算攻击攻击动作
					{
						continue;
					}

					string subName = vplayers[iSubIndex].getName();
					vplayers[iSubIndex].getPosition(yourX,yourY);
					ret = vActions[plname][iActionIndex].rangeTo(selfX,selfY,yourX,yourY);
					//ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("player %s(HP:%d) attack player %s(HP:%d) type = %d, ret = %d ,my(%d,%d) you(%d,%d)\n")
					//	,plname.c_str(),vplayers[iPlayerIndex].getHp(),subName.c_str(),vplayers[iSubIndex].getHp(), eActionType,ret,selfX,selfY,yourX,yourY));
					if(ret >= 0)
					{
						if(mIDs.empty())
						{
							mIDs[ret].push_back(subName);

						}
						else
						{
							map<int,vector<string> > :: iterator it = mIDs.begin();
							if(ret < it->first)
							{
								mIDs.clear();
								mIDs[ret].push_back(subName);
							}
							else if(ret == it->first)
							{
								mIDs[ret].push_back(subName);
							}
						}
						
					}
				}

				int isEmpty = 1;
				for(map<int,vector<string> >::iterator itm = mIDs.begin(); itm != mIDs.end(); ++itm)
				{
					for(vector<string>::iterator itV = itm->second.begin(); itV != itm->second.end() ; ++itV)
					{
						
						int iDamege = 0;
						iDamege = vActions[plname][iActionIndex].damegeTo(vActions[*itV][iActionIndex]);
						if(iDamege == 0)
						{
							string strDescript =  vplayers[iPlayerIndex].getName() + "的" + vActions[plname][iActionIndex].getDescription() +
								 "被" + (*itV) + vActions[*itV][iActionIndex].getDescription() + "了";
							string strFormat = "attack:" +vplayers[iPlayerIndex].getName()+":"+(*itV)+":MISSED:0";
							vdiscreption[iActionIndex].push_back(strDescript);
							vFormatdiscreption[iActionIndex].push_back(strFormat);
						}
						else if(iDamege > 0)
						{
							isEmpty = 0;
							char dmg[30];
							sprintf(dmg,"%d",iDamege);
							string strDescript =  vplayers[iPlayerIndex].getName() + "的" + vActions[plname][iActionIndex].getDescription() +
								 "击中了" + (*itV) + "伤害："+ (string)dmg;
							int index = findIndexPLbyName(*itV);
							if( index >= 0)
							{
								vplayers[index].damageHp(iDamege);
							}
							string strFormat = "attack:" +vplayers[iPlayerIndex].getName()+":"+(*itV)+":ATTACKED:"+(string)dmg;
							vdiscreption[iActionIndex].push_back(strDescript);
							vFormatdiscreption[iActionIndex].push_back(strFormat);
						}	
						else //-1的情况
						{
							;
						}
					}
				}
				if(isEmpty == 1)//打空的情况
				{	
					string strDescript =  vplayers[iPlayerIndex].getName() + "的" + vActions[plname][iActionIndex].getDescription() +
								 "打空了！";
					string strFormat = "miss:"+vplayers[iPlayerIndex].getName();
					vdiscreption[iActionIndex].push_back(strDescript);
					vFormatdiscreption[iActionIndex].push_back(strFormat);
				}
			}
			
			//保存第一步以后的用户状态
			pair<int,vector<CPlayer> > tp(iActionIndex,vplayers);
			vdispPlayer.insert(tp);
		}
		
		//判断死亡状态
		pLivedPlayers.clear();
		for(int iPlayerIndex = 0 ; iPlayerIndex < MAX_PLAYER; ++iPlayerIndex)
		{
			if(vplayers[iPlayerIndex].checkLifeStat() == ELIFESTAT_DEAD)
			{
				lifeNum--;
				string strDescript = vplayers[iPlayerIndex].getName() + "被击败了";
				vdiscreption[iActionIndex].push_back(strDescript);
			}
			else if(vplayers[iPlayerIndex].checkLifeStat() == ELIFESTAT_ALIFE)
			{
				int subTeamId = vplayers[iPlayerIndex].getTeam();
				vLivedPlayers[subTeamId].push_back(vplayers[iPlayerIndex]);
			}
		}
		//判断活着的玩家所在队伍数，如果是同一队的则游戏结束
		if (vLivedPlayers.size() <= 1)
		{
			isGameOver = 1;//本局结束
		}
	}
	return 0;
}

void CEngine::setPlayers(vector<CPlayer> vplayers_x)
{
	this->vplayers = vplayers_x;
	lifeNum = vplayers_x.size();
}

int CEngine::initGame(vector<CUser> vecUser)
{
	int userNum = vecUser.size();
	char aiName[40];
	int aiindex = 1;
	vplayers.clear();
	for (int i = 0 ; i < 4; ++i)
	{
		if (i < vecUser.size())
		{
			CPlayer p(vecUser[i].getUserName());
			p.setPosition(i/2 + 1,i % 2 + 1 );
			vplayers.push_back(p);
			p.setTeam(i);
		}
		else
		{
			sprintf(aiName,"$bot-%d$",aiindex++);
			string aiNamestr(aiName);
			CPlayer p(aiNamestr);
			CFLAI ai;
			p.setAI(ai);
			p.setPosition(i/2 + 1,i % 2 + 1);
			vplayers.push_back(p);
			p.setTeam(i);
		}
	}
	lifeNum = MAX_PLAYER;
	return 0;
}

void CEngine::waitForAction(int timeout)
{
	int iLastTime = OSgetNowSeconds();
	int iNow;
	//先生成AI的动作
	for ( int i =0 ; i < vplayers.size(); ++i) 
	{
		vector<CAction> subVec;
		string name = vplayers[i].getName();
		vplayers[i].getAction(subVec,vplayers);
		if (!subVec.empty())
		{
			pair<string ,vector<CAction> > tp(name,subVec);
			vActions.insert(tp);
		}
	}
	
	while(true)
	{
		int allCommandfinish = 1;
		for ( int i =0 ; i < vplayers.size(); ++i) 
		{
			if (vplayers[i].getLifeStat() == ELIFESTAT_ALIFE)
			{
				if (vActions.find(vplayers[i].getName()) == vActions.end())
				{
					allCommandfinish = 0;
					break;
				}
			}
		}
		if (allCommandfinish == 1)
		{
			break;
		}

		OSTimeDly(1);
		iNow = OSgetNowSeconds();
		if (iNow - iLastTime > iTimeOutPerRound)
		{
			break;
		}
	}
	
	//对于没有及时发送的命令，使用待机指令填充
	for ( int i =0 ; i < vplayers.size(); ++i) 
	{
		string name = vplayers[i].getName();
		if (vActions.find(name) == vActions.end())
		{
			vector<CAction> subVec;
			for (int s = 0; s< 3; s++)
			{
				CAction ca(EACTION_Invalid);
				subVec.push_back(ca);
			}
			pair<string ,vector<CAction> > tp(name,subVec);
			vActions.insert(tp);
		}
	}
	
	return;
}

int CEngine::commitAction(vector<CAction>& subVec,int& RoundIndex_x, string userName)
{
	int roomIndex = RoundIndex;
	if (RoundIndex_x != roomIndex)
	{
		return RTN_ERROR;
	}
	pair<string ,vector<CAction> > tp(userName,subVec);
	vActions.insert(tp);
	return RTN_SUCCESS;
}

