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
 * �ͻ�����Ϣ��������
 * �����û����룬״̬��������Ϣ��
 */
class ClientInfo_Manager{
private:
	
	map<string , Client_Handler*> * user_Map;  
	map<int,CRoom> * roomList;  
	map<int,CEngineThread *> * engineList;  
	map<int,vector<CUser>> * userInRoom;//roomID ��userName �� ���4��Ŀǰ
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

	/* ��¼�µ����߿ͻ��� */
	void new_online(string id,Client_Handler* handler, EClientType type);

	/* �ͻ������� */
	void del_online(string id ,  EClientType type);

	/* ��ѯ�ͻ����Ƿ����� 
	 * ��������ߣ�����-1
	 */
	int is_online(string id ,  EClientType type);

	int update_key(string oldKey,string newKey, EClientType type ); //����Map��KEYֵ �� -1�������ʧ�� ,��ԭ�е�KEY��Ϊ�µ�KEY

	/* �������߿ͻ��˵���������ʱ�� */
	void update_online(string id ,  EClientType type);

	/* ������߿ͻ��˵Ĵ����� */
	int get_client_handler(string id,Client_Handler*& handler ,  EClientType type);
    void show_active_map(EClientType type);

	
	//////////////////////////////////////////////////////////////////////////����˹���
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