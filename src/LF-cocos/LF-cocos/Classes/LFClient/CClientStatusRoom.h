#ifndef _CClientStatusRoom_H_
#define _CClientStatusRoom_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CChatInfo.h"

class CCmdStatus;
class CRoom;
class CChatInfo;

class CClientStatusRoom : public CCmdStatus
{
public:
	CClientStatusRoom(){}
	CClientStatusRoom(CRoom room_x , string name_x)
	{
		room = room_x;
		userName = name_x;
	}
	virtual ~CClientStatusRoom(){}
	virtual void runStatus(int sigal){return;}

	virtual int getMyStatus();

	virtual void showInfo();
	virtual void showMenu();

	virtual int getRoomUser(int roomID);
	virtual int showRoomUser();
	virtual int showChatQueue();
	virtual int sendChartInfo(int roomID, CChatInfo cChatInfo);
	virtual int quitRoom(int roomID); 
	virtual int readytoStart(int roomID);

	//////////////////////////////////////////////////////////////////////////
	virtual string getTitleInfo(string username);
	virtual string getChatInfo();
	virtual string getRoomUserInfo(int index);
	//////////////////////////////////////////////////////////////////////////

	string userName;
	CRoom room;
	vector<CUser> vecUserInRoom;
	string lastCommandResult;
protected:
	//CClientStatusRoom * m_pCmdStatus;
};


#endif