#ifndef _CLIENTINFO_MANAGER_H
#define _CLIENTINFO_MANAGER_H

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/Synch_Traits.h"
#include "ace/SString.h"
#include "Client_Handler.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "ace/Thread_Mutex.h"
#include "../CRoom.h"
#include "../CUser.h"
#include "../CEngineThread.h"

using namespace std;
class CUser;
class CRoom;
class CEngineThread;

enum EClientType 
{
	EClientType_STATION = 1,
	EClientType_TERMINAL,
};


typedef vector<void*> FrameList;
typedef vector<void*>::iterator FrameListIterator;


/**
 * 客户端信息管理器：
 * 包括用户密码，状态等在线信息。
 */
class ClientInfo_Manager{
private:
	
	map<string , Client_Handler*> * user_Map;  
	map<int,CRoom> * roomList;  
	map<int,CEngineThread *> * engineList;  
	map<int,vector<CUser>> * userInRoom;//roomID ，userName ， 最多4个目前
	int roomIndex;

	static ClientInfo_Manager* singleton;

	ClientInfo_Manager()
	{
		//this->init_authen_map();
		user_Map = new map<string , Client_Handler*>();
		roomList = new map<int,CRoom>();
		engineList = new map<int,CEngineThread *>();
		userInRoom = new map<int,vector<CUser>>();
		roomIndex = 0;
	}
	
public:
	//void init_authen_map();
	map<string , Client_Handler*> * getMap(EClientType type);
	map<int,CRoom> * getRoomMap()
	{
		return roomList;
	}

	map<int,CEngineThread *> * getEngineMap()
	{
		return engineList;
	}

	map<int,vector<CUser>> * getuserInRoomMap()
	{
		return userInRoom;
	}

	static ClientInfo_Manager* instance();

	static void remove_instance(){delete singleton;	singleton = NULL;}

	/* return: 0 if authentication failed, 1 if authentiction passed */
	 bool authen_login(string id,char* pw);

	/* 记录新的在线客户端 */
	void new_online(string id,Client_Handler* handler, EClientType type);

	/* 客户端下线 */
	void del_online(string id ,  EClientType type);

	/* 查询客户端是否在线 
	 * 如果不在线，返回-1
	 */
	int is_online(string id ,  EClientType type);

	int update_key(string oldKey,string newKey, EClientType type ); //更新Map中KEY值 ， -1代表更新失败 ,将原有的KEY改为新的KEY

	/* 更新在线客户端的最新心跳时间 */
	void update_online(string id ,  EClientType type);

	/* 获得在线客户端的处理器 */
	int get_client_handler(string id,Client_Handler*& handler ,  EClientType type);
    void show_active_map(EClientType type);

	
	//////////////////////////////////////////////////////////////////////////服务端功能
	void getOnlineList(vector<CUser>& vector);
	void getRoomList(vector<CRoom>& vector);
	int createRoom(CRoom& room, string roomName, string userName);
	int jionRoom(CRoom& room, int roomID, string userName);
	int getUserinRoom(vector<CUser>& usrVec, int roomID, string userName);
	int leaveRoom(int roomID, string userName);
	int setReady(int roomID, string userName);
	
	//////////////////////////////////////////////////////////////////////////
	int getPlayerInfoFromEngine(vector<CPlayer>& vPlayers,int& RoundIndex , int roomID, string userName);
	int getActionInfoFromEngine(map<string ,vector<CAction>>& vShowActions, int roomID, string userName);
	int getDetailInfoFromEngine(map<int,vector<string>>& vdiscreption, int roomID, string userName);
	int getFormatInfoFromEngine(map<int,vector<string>>& vdiscreption, int roomID, string userName);
	int sendActionFromEngine(vector<CAction>& subVec,int& RoundIndex, int roomID, string userName);
	int getStepPlayerFromEngine(map<int,vector<CPlayer>>& vdispPlayer, int roomID, string userName);
	

};

#endif