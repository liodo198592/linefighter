#include "../CProtocol.h"
#include "ClientInfo_Manager.h"
#include <map>
#include <string>
#include "../model_comm.h"

ACE_Thread_Mutex mapMutex;

ClientInfo_Manager* ClientInfo_Manager::singleton = NULL;


map<string , Client_Handler*> * ClientInfo_Manager::getMap(EClientType type)
{
	return user_Map;
}

ClientInfo_Manager* ClientInfo_Manager::instance()
{
	if(singleton != NULL) return singleton;
	else
		return singleton = new ClientInfo_Manager;
}

int ClientInfo_Manager::is_online(string id ,  EClientType type)
{
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return - 1;
	}
	if (pMap->find(id) == pMap->end())
	{
		return -1;
	}
	return 1;
}

int ClientInfo_Manager::get_client_handler(string id,Client_Handler*& handler,  EClientType type)
{	
	//ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Get Client Handler type = [%d]: [%x]\n")	,type,id));
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return RTN_ERROR;
	}
	map<string , Client_Handler*>::iterator it = pMap->find(id);
	if (it != pMap->end())
	{
		handler = it->second;
		return RTN_SUCCESS;
	}
	return RTN_ERROR;
}

void ClientInfo_Manager::new_online(string id,Client_Handler* handler, EClientType type)
{		
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return;
	}

	if (type == EClientType_STATION)
	{
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("new station , handler: %d \n"),handler->get_ihandler()));
	}
	else if (type == EClientType_TERMINAL)
	{
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("new terminal login, handler: %d \n"),handler->get_ihandler()));
	}
	//ClientInfo_Manager::instance()->show_active_map();


	Client_Handler * phandler = NULL;	
	map<string , Client_Handler*>::iterator it = pMap->find(id);
	if (it != pMap->end())
	{
		phandler = it->second;
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("ACTIVEMAP内已有相同终端的信息\n")));
	}
	
	if (pMap->find(id) != pMap->end())
	{
		pMap->erase(id);
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("delete existing old handler:0x%x\n"), phandler->get_ihandler()));
	}
	(*pMap)[id] = handler;

}

int ClientInfo_Manager::update_key(string oldKey,string newKey, EClientType type)
{
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("begin Update Key! \n")));
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return -1;
	}

	Client_Handler * phandler = NULL;
	map<string , Client_Handler*>::iterator it = pMap->find(newKey);
	if (it != pMap->end())
	{
		phandler = it->second;
	}
	if (phandler != NULL)
	{
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("map already has the newKey! \n")));
		return -1;
	}

	it = pMap->find(oldKey);
	if (it != pMap->end())
	{
		phandler = it->second;
	}
	if (phandler == NULL) 
	{
		ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("can't find oldKey! \n")));
		return -1;
	}
	pMap->erase(oldKey);
	(*pMap)[newKey] = phandler;
	return 0;
}


/* 客户端下线 */
void ClientInfo_Manager::del_online(string id, EClientType type)
{
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("begin Update Key! \n")));
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return ;
	}

	Client_Handler* c_handler = NULL;
	map<string , Client_Handler*>::iterator it = pMap->find(id);
	if (it != pMap->end())
	{
		c_handler = it->second;
	}
	pMap->erase(id);

	if( c_handler != NULL) delete c_handler; // Descontructor also delete its registered Gateway_Input_Handler
	
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Delete Online ")));
}


/* 更新在线客户端的最新心跳时间 */
void ClientInfo_Manager::update_online(string id, EClientType type)
{
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return ;
	}

	Client_Handler* handler = NULL;
	map<string , Client_Handler*>::iterator it = pMap->find(id);
	if (it != pMap->end())
	{
		handler = it->second;
	}
	if(handler != NULL)
	{
		handler->update_heart_beat();
	}
}

void ClientInfo_Manager::show_active_map(EClientType type)
{
	map<string , Client_Handler*> * pMap = getMap(type);
	if (pMap == NULL)
	{
		return ;
	}

	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Showing Active Map of Terminal, Total Entries: %d\n"),pMap->size()));
	for (map<string , Client_Handler*>::iterator iter = pMap->begin ();  iter != pMap->end (); iter++)
    { 
	   ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Terminal ID: [%s], 0x%x\n"),(*iter).first,(*iter).second));
    }
}


void ClientInfo_Manager::getOnlineList(vector<CUser>& vector)
{
	map<string , Client_Handler*> * pMap = getMap(EClientType_TERMINAL);
	if (pMap == NULL)
	{
		return ;
	}

	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("getOnlineList \n")));
	for (map<string , Client_Handler*>::iterator iter = pMap->begin ();  iter != pMap->end (); iter++)
	{ 
		CUser usr((*iter).first);
		vector.push_back(usr);
	}
}

void ClientInfo_Manager::getRoomList(vector<CRoom>& vector)
{
	map<int , CRoom> * pMap = getRoomMap();
	if (pMap == NULL)
	{
		return ;
	}

	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("getRoomMap \n")));
	for (map<int ,CRoom>::iterator iter = pMap->begin ();  iter != pMap->end (); iter++)
	{ 
		CRoom room((*iter).second);
		vector.push_back(room);
	}
}

int ClientInfo_Manager::createRoom(CRoom& room ,string roomName, string userName)
{
	room.setRoomID(++roomIndex);
	room.setRoomName(roomName);
	room.setRoomStatus(EROOM_STATUS_READY);
	room.setmaxPlayer(4);
	room.setnowPlayer(1);
	room.sethostUserName(userName);
	pair<int,CRoom> tp(room.getRoomID(),room);
	map<int , CRoom> * pMap = getRoomMap();
	pMap->insert(tp);
	
	map<int,vector<CUser>> * pUinRMap = getuserInRoomMap();
	if (pUinRMap->find(room.getRoomID()) == pUinRMap->end())
	{
		vector<CUser> vec;
		CUser user;
		user.setUserName(room.gethostUserName());
		user.setReady(CUSER_UNREADY);
		vec.push_back(user);
		pair<int,vector<CUser>> tp(room.getRoomID(),vec);
		pUinRMap->insert(tp);
	}
	return RTN_SUCCESS;
}

int ClientInfo_Manager::jionRoom(CRoom& room, int roomID, string userName)
{
	map<int , CRoom> * pMap = getRoomMap();
	map<int,vector<CUser>> * pUinRMap = getuserInRoomMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	if (pUinRMap->find(roomID) == pUinRMap->end())
	{
		return RTN_ERROR;
	}
	if ((*pUinRMap)[roomID].size() >= 4)
	{
		room = (*pMap)[roomID];
		return RTN_ERROR;
	}
	if ((*pMap)[roomID].getRoomStatus()  !=  EROOM_STATUS_READY)
	{
		room = (*pMap)[roomID];
		return RTN_ERROR;
	}
	(*pMap)[roomID].setnowPlayer((*pMap)[roomID].getnowPlayer() + 1);
	CUser user;
	user.setUserName(userName);
	user.setReady(CUSER_UNREADY);
	(*pUinRMap)[roomID].push_back(user);
	room = (*pMap)[roomID];
	return RTN_SUCCESS;
}

int ClientInfo_Manager::getUserinRoom(vector<CUser>& usrVec, int roomID, string userName)
{
	map<int , CRoom> * pMap = getRoomMap();
	map<int,vector<CUser>> * pUinRMap = getuserInRoomMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	if (pUinRMap->find(roomID) == pUinRMap->end())
	{
		return RTN_ERROR;
	}
	usrVec  = (*pUinRMap)[roomID];
	return RTN_SUCCESS;
}

int ClientInfo_Manager::leaveRoom(int roomID, string userName)
{
	map<int , CRoom> * pMap = getRoomMap();
	map<int,vector<CUser>> * pUinRMap = getuserInRoomMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	if (pUinRMap->find(roomID) == pUinRMap->end())
	{
		return RTN_ERROR;
	}
	for (vector<CUser>::iterator it = ((*pUinRMap)[roomID]).begin(); it != ((*pUinRMap)[roomID]).end() ; ++it)
	{
		string strName = (*it).getUserName();
		if (strcmp(strName.c_str(),userName.c_str()) == 0)
		{
			((*pUinRMap)[roomID]).erase(it);
			(*pMap)[roomID].setnowPlayer((*pMap)[roomID].getnowPlayer() - 1);
			return RTN_SUCCESS;
		}
	}
	return RTN_ERROR;
}

int ClientInfo_Manager::setReady(int roomID, string userName)
{
	map<int , CRoom> * pMap = getRoomMap();
	map<int,vector<CUser>> * pUinRMap = getuserInRoomMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	if (pUinRMap->find(roomID) == pUinRMap->end())
	{
		return RTN_ERROR;
	}
	for (vector<CUser>::iterator it = ((*pUinRMap)[roomID]).begin(); it != ((*pUinRMap)[roomID]).end() ; ++it)
	{
		string strName = (*it).getUserName();
		if (strcmp(strName.c_str(),userName.c_str()) == 0)
		{
			(*it).setReady(CUSER_READY);
			return RTN_SUCCESS;
		}
	}
	return RTN_ERROR;
}

int ClientInfo_Manager::getPlayerInfoFromEngine(vector<CPlayer>& vPlayers,int& RoundIndex , int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		RoundIndex = 0;
		return RTN_ERROR;
	}
	
	vPlayers = (*pMap)[roomID]->m_engine.vplayers;
	RoundIndex = (*pMap)[roomID]->m_engine.RoundIndex;
	return RTN_SUCCESS;
	
}

int ClientInfo_Manager::getActionInfoFromEngine(map<string ,vector<CAction>>& vShowActions , int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	vShowActions = (*pMap)[roomID]->m_engine.vShowActions;
	return RTN_SUCCESS;
}

int ClientInfo_Manager::getDetailInfoFromEngine(map<int,vector<string>>& vdiscreption, int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	vdiscreption = (*pMap)[roomID]->m_engine.vdiscreption;
	return RTN_SUCCESS;
}

int ClientInfo_Manager::getFormatInfoFromEngine(map<int,vector<string>>& vdiscreption, int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	vdiscreption = (*pMap)[roomID]->m_engine.vFormatdiscreption;
	return RTN_SUCCESS;
}

int ClientInfo_Manager::sendActionFromEngine(vector<CAction>& subVec,int& RoundIndex, int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	int roomIndex = (*pMap)[roomID]->m_engine.RoundIndex;
	if (RoundIndex != roomIndex)
	{
		return RTN_ERROR;
	}
	if ((*pMap)[roomID]->m_engine.vActions.find(userName) != (*pMap)[roomID]->m_engine.vActions.end())
	{
		return RTN_ERROR;
	}
	pair<string ,vector<CAction>> tp(userName,subVec);
	(*pMap)[roomID]->m_engine.vActions.insert(tp);
	return RTN_SUCCESS;
}

int ClientInfo_Manager::getStepPlayerFromEngine(map<int,vector<CPlayer>>& vdispPlayer, int roomID, string userName)
{
	map<int,CEngineThread *> * pMap = getEngineMap();
	if (pMap->find(roomID) == pMap->end())
	{
		return RTN_ERROR;
	}
	vdispPlayer = (*pMap)[roomID]->m_engine.vdispPlayer;
	return RTN_SUCCESS;
}