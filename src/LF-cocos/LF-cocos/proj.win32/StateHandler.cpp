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
		//�ȴ��������״̬,����Ұ���ȷ��ִ�а�ť֮���л�����һ��״̬
		if(m_pMenuControl->getInputExecStat())
		{
			//֪ͨ��������ҵĶ������л�״̬���ȴ������������״̬
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
		//�ȴ�����������������ҵ�״̬���л�״̬��չʾ��Ϸ״̬
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
					//�µ�һ�ֿ�ʼ��


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
		//չʾ�������ȴ�չʾ�������л�״̬������״̬
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
	//�л�״̬ͬʱ��ÿ��״̬�ĳ�ʼ������

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

//���ز�����ť
void CStateHandler::hidMenuButton()
{
	m_pMenuControl->hidMenuButton();
}
//��ʾ������ť
void CStateHandler::showMenuButton()
{
	m_pMenuControl->showMenuButton();
}