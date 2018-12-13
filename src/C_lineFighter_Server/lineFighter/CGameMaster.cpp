#include "CProtocol.h"
#include "CProtocol_code.h"
#include "comm_com.h"
#include <map>
#include "CGameMaster.h"
#include "model_comm.h"
#include "CEngineThread.h"
#include "CEngine.h"

ACE_INT64 CGameMaster::run()
{
	return activate(THR_NEW_LWP,1);//�����߳�
}

ACE_INT32 CGameMaster::svc()//��ʱ���������ڴ��߳��м�
{
	int isUnready = 0;
	while (true)
	{
		//�����û���ĸ�����������û�������׼���У���ʼһ����Ϸ
		map<int,vector<CUser>> * pUinRMap = ClientInfo_Manager::instance()->getuserInRoomMap();
		map<int , CRoom> * pMap = ClientInfo_Manager::instance()->getRoomMap();
		map<int,CEngineThread *> * pEngineMap = ClientInfo_Manager::instance()->getEngineMap();
		//////////////////////////////////////////////////////////////////////////����Ƿ��з�����Ϸ��ʼ
		for (map<int,vector<CUser>>::iterator it = pUinRMap->begin(); it != pUinRMap->end(); ++it)
		{
			isUnready = 0;
			int i = 0;
			for (i = 0 ; i < it->second.size(); ++i)
			{
				if ((it->second)[i].getReady() != CUSER_READY)
				{
					isUnready = 1;
					break;
				}
			}
			if (isUnready == 0 && i != 0)
			{
				//���������������ó���Ϸ��
				for (int j = 0 ; j < it->second.size(); ++j)
				{
					(it->second)[j].setReady(CUSER_GAMMING);
				}
				(*pMap)[it->first].setRoomStatus(EROOM_STATUS_GAMING);

				//�����˶�׼������������engine
				CEngineThread * pEngine = new CEngineThread((it->second),it->first);
				map<int,CEngineThread *> * pEngineMap = ClientInfo_Manager::instance()->getEngineMap();
				pair<int,CEngineThread *> tp(it->first,pEngine);
				pEngineMap->insert(tp);
				pEngine->run();
			}
		}

		//�������з��䣬���������Ϸ�����ģ���Ӧ�÷���״̬Ϊ����
		for (map<int , CRoom>::iterator it = pMap->begin(); it != pMap->end(); ++it)
		{
			int RoomID = it->first;
			if (it->second.getRoomStatus() == EROOM_STATUS_GAMING)
			{
				//���Ҷ�Ӧ����������Ϸ�Ƿ����
				if (pEngineMap->find(RoomID) != pEngineMap->end())
				{
					if ((*pEngineMap)[RoomID]->m_engine.roomStatus == EROOM_STATUS_FINISH)
					{
						it->second.setRoomStatus(EROOM_STATUS_FINISH);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////����Ƿ���ڷ�����Ҫ�ͷ�
		for (map<int,vector<CUser>>::iterator it = pUinRMap->begin(); it != pUinRMap->end(); ++it)
		{
			int roomID = it->first;
			if (it->second.empty())
			{
				if (pEngineMap->find(roomID) != pEngineMap->end() && pMap->find(roomID) != pMap->end())
				{
					delete (*pEngineMap)[roomID];
					pEngineMap->erase(pEngineMap->find(roomID));
					pMap->erase(pMap->find(roomID));
					pUinRMap->erase(it);
					break;
				}
			}
		}

		ACE_OS::sleep(2);
	}
	
	return 0;//�����߳�
}


