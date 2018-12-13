#include "StateHandler.h"


CStateHandler::CStateHandler(CClientStatusGameBase* pCmdStatus,CShowScreen* pShowScreen,CMenuControler* pMenuControl,GameStat startState):
	m_pCmdStatus(pCmdStatus),m_pShowScreen(pShowScreen),m_pMenuControl(pMenuControl),m_CurrentStat(startState)
{
}


CStateHandler::~CStateHandler(void)
{
}

void CStateHandler::handleState(float tmd)
{
	if(m_CurrentStat == GAMESTAT_WAIT_INIT)
	{
		if (m_pCmdStatus->getPlayerInfo() == RTN_SUCCESS)
		{
			if( m_pCmdStatus->vplayers.size() > 0)
			{
				m_pShowScreen->initPlayers(m_pCmdStatus->vplayers);
				m_pCmdStatus->LastRoundIndex = m_pCmdStatus->RoundIndex;
				changeStat(GAMESTAT_PLAYER_INPUT);
			}
		}
	}
	else if(m_CurrentStat == GAMESTAT_PLAYER_INPUT)
	{
		//等待玩家输入状态,当玩家按下确认执行按钮之后切换到下一个状态
		if(m_pMenuControl->getInputExecStat())
		{
			//通知服务器玩家的动作并切换状态至等待其他玩家输入状态
			vector<CAction> vecAction = m_pMenuControl->getVecActions();
			if (m_pCmdStatus->getSendActions(vecAction,m_pCmdStatus->RoundIndex) == RTN_SUCCESS)
			{
				changeStat(GAMESTAT_WAITOTHERINPUT);
				m_pCmdStatus->isActed = 1;
			}
		}

	}
	else if(m_CurrentStat == GAMESTAT_WAITOTHERINPUT)
	{
		//等待服务器返回其他玩家的状态并切换状态至展示游戏状态
		if (m_pCmdStatus->getPlayerInfo() == RTN_SUCCESS)
		{
			if (m_pCmdStatus->RoundIndex == 1000)
			{
				m_pCmdStatus->isGameOver = 1;
			}
			else
			{
				if (m_pCmdStatus->LastRoundIndex != m_pCmdStatus->RoundIndex)
				{
					//新的一轮开始了


					if (m_pCmdStatus->getLastActionInfo() != RTN_SUCCESS)
					{
						return;
					}
					if (m_pCmdStatus->getDetailInfo() != RTN_SUCCESS)
					{
						return;
					}
					if (m_pCmdStatus->stepPlayerInfo() != RTN_SUCCESS)
					{
						return;
					}
					if (m_pCmdStatus->getFormatInfo() != RTN_SUCCESS)
					{
						return;
					}

					m_pCmdStatus->LastRoundIndex = m_pCmdStatus->RoundIndex;
					m_pCmdStatus->isActed = 0;

					changeStat(GAMESTAT_SHOWING);
					m_pShowScreen->showGaming(m_pCmdStatus);
				}
			}


		}

	}
	else if(m_CurrentStat == GAMESTAT_SHOWING)
	{
		//展示动作并等待展示结束并切换状态至输入状态
		if(m_pShowScreen->showingAction())
		{
			m_pShowScreen->showAction();
		}
		else
		{
			changeStat(GAMESTAT_PLAYER_INPUT);
		}

	}
}

void CStateHandler::changeStat(GameStat toStat)
{
	//切换状态同时做每个状态的初始化动作

	switch (toStat)
	{
	case GAMESTAT_WAIT_INIT:
		break;
	case GAMESTAT_PLAYER_INPUT:
		{
			m_pMenuControl->clearVecActions();
			m_pMenuControl->updateStepShow();
			showMenuButton();
			m_pMenuControl->setInputExecFalse();
		}
		break;
	case GAMESTAT_WAITOTHERINPUT:
		{
			hidMenuButton();
			break;
		}
	case GAMESTAT_SHOWING:
		break;
	case GAMESTAT_OVER:
		break;
	default:
		break;
	}

	m_CurrentStat = toStat;
}

//隐藏操作按钮
void CStateHandler::hidMenuButton()
{
	m_pMenuControl->hidMenuButton();
}
//显示操作按钮
void CStateHandler::showMenuButton()
{
	m_pMenuControl->showMenuButton();
}