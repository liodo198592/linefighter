#ifndef _CClientStatusHouse_H_
#define _CClientStatusHouse_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CRoom.h"
#include "CUser.h"

class CCmdStatus;
class CRoom;
class CUser;

class CClientStatusHouse : public CCmdStatus
{
public:
	CClientStatusHouse(string name_x):name(name_x){}
	CClientStatusHouse(){}
	virtual ~CClientStatusHouse(){}
	virtual void runStatus(int sigal){return;}
	
	virtual void showInfo();
	virtual void showMenu();
	virtual void showOnlineList();
	virtual void showRoomList();

	
	virtual int getOnlneList();
	virtual int getRoomList();
	virtual int createRoom(CRoom& room, string roomName);
	virtual int jionRoom(CRoom& room, int roomID);
	
	map<int,CRoom> m_roomList;//房间ID， 房间名称
	map<string,CUser> m_UserList;//在线用户名
	string lastCommandResult;
	string name;


	//////////////////////////////////////////////////////////////////////////
	virtual string getTitleInfo();
	virtual string getUserInfoStr();
	virtual string getRoomTotalInfoStr();
	virtual string getRoomInfoStr(int index, int& roomID);
	virtual string getOneUserStr(int index);
	//////////////////////////////////////////////////////////////////////////
protected:
	//CClientStatusHouse * m_pCmdStatus;
};


#endif