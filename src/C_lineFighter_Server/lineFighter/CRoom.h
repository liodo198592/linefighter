#ifndef _CRoom_H_
#define _CRoom_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

enum EROOM_STATUS
{
	EROOM_STATUS_READY,
	EROOM_STATUS_GAMING,
	EROOM_STATUS_FINISH,
};

//µ¥Àýhouse
class CRoom
{
protected:
	string roomName;
    int roomID;
	EROOM_STATUS roomStatus;
	int maxPlayer;
	int nowPlayer;
	string hostUserName;
public:
	CRoom(){}
	CRoom(string roomName_x, int roomID_x ,string hostUserName_x):roomName(roomName_x),roomID(roomID_x),hostUserName(hostUserName_x)
	{
		roomStatus = EROOM_STATUS_READY;
		int maxPlayer = 4;
		int nowPlayer = 1;
	}
	void operator = (CRoom room)
	{
		this->roomName = room.roomName;
		this->roomID = room.roomID;
		this->roomStatus = room.roomStatus;
		this->maxPlayer = room.maxPlayer;
		this->nowPlayer = room.nowPlayer;
		this->hostUserName = room.hostUserName;
	}
	CRoom(const CRoom &room)
	{
		this->roomName = room.roomName;
		this->roomID = room.roomID;
		this->roomStatus = room.roomStatus;
		this->maxPlayer = room.maxPlayer;
		this->nowPlayer = room.nowPlayer;
		this->hostUserName = room.hostUserName;
	}

	virtual ~CRoom(){}

	virtual string gethostUserName()
	{
		return hostUserName;
	}
	virtual void sethostUserName(string hostUserName_x)
	{
		hostUserName = hostUserName_x;
	}

	virtual string getRoomName()
	{
		return roomName;
	}
	virtual void setRoomName(string roomName_x)
	{
		roomName = roomName_x;
	}
	
	virtual int getRoomID()
	{
		return roomID;
	}
	virtual void setRoomID(int roomID_x)
	{
		roomID =  roomID_x;
	}

	virtual int getmaxPlayer()
	{
		return maxPlayer;
	}
	virtual void setmaxPlayer(int maxPlayer_x)
	{
		maxPlayer =  maxPlayer_x;
	}

	virtual int getnowPlayer()
	{
		return nowPlayer;
	}
	virtual void setnowPlayer(int nowPlayer_x)
	{
		nowPlayer =  nowPlayer_x;
	}

	virtual EROOM_STATUS getRoomStatus()
	{
		return roomStatus;
	}
	
	virtual void setRoomStatus(EROOM_STATUS roomStatus_x)
	{
		roomStatus = roomStatus_x; 
	}
};

#endif