#include "CProtocol.h"
#include "RingBuf.h"
#include "CProtocol_code.h"
#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif
#include "model_comm.h"
#ifdef MAKE_ANDRIOD
#include "status_comm.h"
#include "RingBuffer.h"
#else
#include "../../LF-cocos/LF-cocos/Classes/LFClient/status_comm.h"
#include "communication/RingBuffer.h"
#endif

//////////////////////////////////////////////////////////////////////////
CProtocol * pProtocol = NULL;
SRingBuf g_ringBuffer;
float g_iTimeOut = 0;

CProtocol* getProtocol()
{
	if (pProtocol == NULL)
	{
		pProtocol = new CProtocol();
	}
	return pProtocol;
}


unsigned char CProtocol::checkSum(const unsigned char * cBuffer, int iLen)
{
    unsigned char iResult = 0;
    int i = 0;
    for ( ; i<iLen; i++)
    {
        iResult += cBuffer[i];
    }
    return iResult;
}

int ThrowSpecialByte(SRingBuf * pRingBuf,unsigned char iMatchByte)
{
    unsigned char iTempByte;
    while(true)
    {
        if(RBReadPerFetch(&iTempByte,1,pRingBuf) < 1) //读取不到字节
        {
            return RTN_TIME_OUT;
        }
        else //如果读取到了字节,但不是68的话继续读取字节
        {
            if(iTempByte != iMatchByte)
            {
                RBRead(&iTempByte,1,pRingBuf);//丢弃本字节
                continue;
            }
            else
            {
                break;
            }
        }
    }
    return RTN_SUCCESS;
}


int CProtocol::parse(int& bufferlen , CRingBuffer * pRingBuf)
{
	//检查第二个68H
	unsigned char iByte;

	if (pRingBuf->getAt(34-1,iByte) == -1 ) 
	{
		return RTN_TIME_OUT;
	}
	if(iByte != 0x68)
	{
		return RTN_ERROR;
	}

	unsigned char iLenLowByte, iLenHighByte;
	if(pRingBuf->getAt(35, iLenLowByte) == -1)
	{
		return RTN_TIME_OUT;
	}
	if(pRingBuf->getAt(36, iLenHighByte) == -1)
	{
		return RTN_TIME_OUT;
	}

	//获取报文长度,检查报文长度合法性
	int iLen = iLenLowByte + iLenHighByte * 0x100;
	if(iLen < 0 || iLen>2048)
	{
		return RTN_ERROR;
	}
	bufferlen = 37 + iLen + 2;
	//检查结束符
	if (pRingBuf->getAt(bufferlen-1,iByte) == -1 )
	{
		return RTN_TIME_OUT;
	}
	if (iByte != 0x16) 
	{
		return RTN_ERROR;
	}

	//检查校验和
	unsigned char iCheckSumByte;
	if(pRingBuf->getAt(37 + iLen,iCheckSumByte) == -1)
	{
		return RTN_TIME_OUT;
	}

	if(iCheckSumByte != pRingBuf->checkSum(0, bufferlen-2))
	{
		
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}

int CProtocol::getAreport(unsigned char * pOutBuf, int& bufferlen ,SRingBuf * pRingBuf)
{
    int iNextLenth = 0 , i = 0,iDataLenth = 0; //下面需要读取多少个字节
    unsigned char iTempByte;

    while(true) //外层循环
    {
        if (ThrowSpecialByte(pRingBuf,0x68) == RTN_TIME_OUT)//丢弃第一个Byte之前的所有数据．
        {
            return RTN_TIME_OUT;
        }

        ////////////////////////////////////////////////////////////////////
        //68 至报文头和控制域
        iNextLenth = 37;
        i = RBReadPerFetch(pOutBuf,iNextLenth,pRingBuf); 
        if(i < iNextLenth) //读取不到字节
        {
            return RTN_TIME_OUT;
        }
        else //校验第二个68和计算控制域长度
        {
            iDataLenth = pOutBuf[iNextLenth-2] + pOutBuf[iNextLenth-1] * 0x100; //数据域长度
            if(pOutBuf[iNextLenth-4] != 0x68 || iDataLenth > 4000) //645规约要求长度不能大于200
            {
                //格式错误，读取一个字节，并重新找0x68
                RBRead(&iTempByte,1,pRingBuf);
                continue;
            }
        }

        /////////////////////////////////////////////////////////////////
        //获取报文剩余数据
        iNextLenth = 37 + iDataLenth + 2;

        i = RBReadPerFetch(pOutBuf,iNextLenth,pRingBuf); 
        if(i < iNextLenth) //读取不到字节
        {
            return RTN_TIME_OUT;
        }
        else //校验尾部CHECKSUM和最后一个16
        {
            if((pOutBuf[iNextLenth-1] !=0x16) || (pOutBuf[iNextLenth-2] != checkSum(pOutBuf,iNextLenth-2)))
            {
                //格式错误，读取一个字节，并重新找0x68
                RBRead(&iTempByte,1,pRingBuf);
                continue;
            }
            else
            {
                //报文解析成功，读取报文
                if( RBRead(pOutBuf,iNextLenth,pRingBuf) != iNextLenth)
                {
                    
                    return RTN_NOT_MATCH;
                }
                break;
            }
        }
    }
    return iNextLenth;
}

int CProtocol::ptclGetGameNo(unsigned char * cBuffer, int iLen)
{
       if(cBuffer[34] != 0x03 || cBuffer[37] != 0x21 || cBuffer[38] != 0x11)
       {
            return -1;
       }
       return cBuffer[39] + cBuffer[40] * 0x100;
}

int CProtocol::ptclGetPlayerInfo(vector<SPlayerName>& info,unsigned char * cBuffer, int iLen)
{
    int playerNum = 0;
    char strNameBuf[30];
    if(cBuffer[37] == 0x21 && cBuffer[38] == 0x11)
    {
        playerNum = cBuffer[41];
        if(playerNum > 4 || playerNum < 0)
        {
            return -1;
        }

        for (int i = 0; i < playerNum; ++ i)
        {
              SPlayerName sName;
              memset(strNameBuf,0,30);
              memcpy( strNameBuf ,cBuffer + 42 + i*32, 30);
              sName.strName = (string)strNameBuf;
              sName.iID = cBuffer[72+i*32] + cBuffer[73+i*32];
              info.push_back(sName);
        }
        return playerNum;
    }
    return -1;
}

string CProtocol::ptclGetUserName(unsigned char * cBuffer, int iLen)
{
	char usName[30];
	memcpy(usName,cBuffer+1,sizeof(usName));
    return (string)usName;
}

int CProtocol::ptclGetFuncCode(unsigned char * cBuffer, int iLen)
{
	return cBuffer[34];
}

int CProtocol::ptclGetAckConfirmOk(int ControlCode, int ItemID ,unsigned char * cBuffer, int iLen)
{
    if(cBuffer[37] == ItemID % 0x100 && cBuffer[38] == ItemID / 0x100 && cBuffer[34] ==  ControlCode
        && cBuffer[35] == 5 &&  cBuffer[36] == 0)//长度高字节)
    {
        if (cBuffer[41] == 0)
        {
            return 0;
        }
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////LOGIN
int CProtocol::ptclBuildLogin(char * name ,unsigned char * pOutBuf)
{
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_LOGIN; //控制码
	pOutBuf[35] = 0x00;//长度
	pOutBuf[36] = 0x00;//长度
	pOutBuf[37] = checkSum(pOutBuf,37);
	pOutBuf[38] = 0x16;
	return 39;
}

int CProtocol::ptclBuildLoginAsw(char * name ,unsigned char * pOutBuf ,int rst)
{
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = LOGIN_ASW; //控制码
	pOutBuf[35] = 0x01;//长度
	pOutBuf[36] = 0x00;//长度
	pOutBuf[37] = rst;//c
	pOutBuf[38] = checkSum(pOutBuf,38);
	pOutBuf[39] = 0x16;
	return 40;
}

int CProtocol::ptclCheckLogin(unsigned char * pOutBuf ,int len)
{

	if (len >= 40 && pOutBuf[34] == LOGIN_ASW && pOutBuf[37] == SUCCESS_ASW_CODE)
	{
		return RTN_SUCCESS;
	}
	return RTN_ERROR;
}
//////////////////////////////////////////////////////////////////////////getNameList
int CProtocol::ptclBuildAskOnlineList(char * name ,unsigned char * pOutBuf)
{
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_ONLINE_USERLIST; //控制码
	pOutBuf[35] = 0x00;//长度
	pOutBuf[36] = 0x00;//长度
	pOutBuf[37] = checkSum(pOutBuf,37);
	pOutBuf[38] = 0x16;
	return 39;
}

int CProtocol::ptclBuildAskOnlineListAsw(char * name ,vector<CUser> onlineList ,unsigned char * pOutBuf)
{
	int dataLen = onlineList.size() * 30  + 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_ONLINE_USERLIST_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = onlineList.size();
	for (int i = 0 ; i < onlineList.size(); ++i)
	{
		memset(&pOutBuf[38 + i * 30],0,30);
		memcpy(&pOutBuf[38 + i * 30],onlineList[i].getUserName().c_str(),strlen(onlineList[i].getUserName().c_str()));
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}


int CProtocol::ptclCheckAskOnlineListAsw(map<string, CUser>& onlineList ,unsigned char * pOutBuf, int len)
{
	if (pOutBuf[34] != ASK_ONLINE_USERLIST_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	int num = pOutBuf[37];
	if (dataLen -1 != num * 30)
	{
		return RTN_ERROR;
	}
	for (int i = 0 ; i < num; ++i)
	{
		string name((char *)&pOutBuf[37 + 1 + i * 30]);
		CUser user(name);
		pair<string,CUser> pa(name,user);
		onlineList.insert(pa);
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////getRoomList
int CProtocol::ptclBuildAskRoomList(char * name ,unsigned char * pOutBuf)
{
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_ROOMLIST; //控制码
	pOutBuf[35] = 0x00;//长度
	pOutBuf[36] = 0x00;//长度
	pOutBuf[37] = checkSum(pOutBuf,37);
	pOutBuf[38] = 0x16;
	return 39;
}

int CProtocol::ptclBuildAskRoomListAsw(char * name ,vector<CRoom> onRooomList ,unsigned char * pOutBuf)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofRoom = 67;
	int dataLen = onRooomList.size() * perLenofRoom  + 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ROOMLIST_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = onRooomList.size();
	for (int i = 0 ; i < onRooomList.size(); ++i)
	{
		memset(&pOutBuf[38 + i * perLenofRoom],0,30);
		memcpy(&pOutBuf[38 + i * perLenofRoom],onRooomList[i].getRoomName().c_str(),strlen(onRooomList[i].getRoomName().c_str()));

		memset(&pOutBuf[38 + i * perLenofRoom + 30],0,30);
		memcpy(&pOutBuf[38 + i * perLenofRoom + 30],onRooomList[i].gethostUserName().c_str(),strlen(onRooomList[i].gethostUserName().c_str()));
		
		pOutBuf[38 + i * perLenofRoom + 60] = onRooomList[i].getRoomID() % 0x100;
		pOutBuf[38 + i * perLenofRoom + 61] = onRooomList[i].getRoomID() / 0x100;

		pOutBuf[38 + i * perLenofRoom + 62] = onRooomList[i].getmaxPlayer() % 0x100;
		pOutBuf[38 + i * perLenofRoom + 63] = onRooomList[i].getmaxPlayer() / 0x100;

		pOutBuf[38 + i * perLenofRoom + 64] = onRooomList[i].getnowPlayer() % 0x100;
		pOutBuf[38 + i * perLenofRoom + 65] = onRooomList[i].getnowPlayer() / 0x100;

		pOutBuf[38 + i * perLenofRoom + 66] = onRooomList[i].getRoomStatus();
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}


int CProtocol::ptclCheckAskRoomListAsw(map<int, CRoom>& onRoomList ,unsigned char * pOutBuf, int len)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofRoom = 67;

	if (pOutBuf[34] != ROOMLIST_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	int num = pOutBuf[37];
	if (dataLen -1 != num * perLenofRoom)
	{
		return RTN_ERROR;
	}
	for (int i = 0 ; i < num; ++i)
	{
		string roomName((char *)&pOutBuf[38 + i * perLenofRoom]);
		string hostName((char *)&pOutBuf[38 + i * perLenofRoom + 30]);
		int roomid = pOutBuf[38 + i * perLenofRoom + 60] + pOutBuf[38 + i * perLenofRoom + 61]*0x100;
		int maxnum = pOutBuf[38 + i * perLenofRoom + 62] + pOutBuf[38 + i * perLenofRoom + 63]*0x100;
		int nownum = pOutBuf[38 + i * perLenofRoom + 64] + pOutBuf[38 + i * perLenofRoom + 65]*0x100;
		int stat = pOutBuf[38 + i * perLenofRoom + 66];
		CRoom room(roomName,roomid,hostName);
		room.setmaxPlayer(maxnum);
		room.setnowPlayer(nownum);
		room.setRoomStatus((EROOM_STATUS)stat);
		pair<int,CRoom> pa(roomid,room);
		onRoomList.insert(pa);
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////creteroom
int CProtocol::ptclBuildCreateRoom(char * name , string roomName, unsigned char * pOutBuf)
{
	int dataLen = 30;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = CREATE_ROOM; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	memset(&pOutBuf[37],0,30);
	memcpy(&pOutBuf[37],(unsigned char *)(roomName.c_str()),strlen(roomName.c_str())); //填写名字
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckCreateRoom(string& roomName ,unsigned char * pOutBuf)
{
	int perLenofRoom = 67;

	if (pOutBuf[34] != CREATE_ROOM)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 30)
	{
		return RTN_ERROR;
	}
	string roomNameinRPT((char *)&pOutBuf[37]);
	roomName = roomNameinRPT;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildCreateRoomAsw(char * name ,CRoom rooom ,unsigned char * pOutBuf , int rst)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int dataLen = 67 + 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = CREATE_ROOM_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = rst;
	memset(&pOutBuf[38],0,30);
	memcpy(&pOutBuf[38],rooom.getRoomName().c_str(),strlen(rooom.getRoomName().c_str()));

	memset(&pOutBuf[38 + 30],0,30);
	memcpy(&pOutBuf[38 + 30],rooom.gethostUserName().c_str(),strlen(rooom.gethostUserName().c_str()));

	pOutBuf[38 + 60] = rooom.getRoomID() % 0x100;
	pOutBuf[38 + 61] = rooom.getRoomID() / 0x100;

	pOutBuf[38 + 62] = rooom.getmaxPlayer() % 0x100;
	pOutBuf[38 + 63] = rooom.getmaxPlayer() / 0x100;

	pOutBuf[38 + 64] = rooom.getnowPlayer() % 0x100;
	pOutBuf[38 + 65] = rooom.getnowPlayer() / 0x100;

	pOutBuf[38 + 66] = rooom.getRoomStatus();
	
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;

}
int CProtocol::ptclCheckCreateRoomAsw(CRoom& room ,unsigned char * pOutBuf, int len)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofRoom = 67;

	if (pOutBuf[34] != CREATE_ROOM_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	int rst = pOutBuf[37];
	if (dataLen -1 != perLenofRoom)
	{
		return RTN_ERROR;
	}
	if (rst != RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	
	string roomName((char *)&pOutBuf[38]);
	string hostName((char *)&pOutBuf[38+ 30]);
	int roomid = pOutBuf[38 + 60] + pOutBuf[38 + 61]*0x100;
	int maxnum = pOutBuf[38 + 62] + pOutBuf[38 + 63]*0x100;
	int nownum = pOutBuf[38 + 64] + pOutBuf[38 + 65]*0x100;
	int stat = pOutBuf[38 + 66];
	room.setRoomName(roomName);
	room.sethostUserName(hostName);
	room.setRoomID(roomid);
	room.setRoomStatus((EROOM_STATUS)stat);
	room.setmaxPlayer(maxnum);
	room.setnowPlayer(nownum);

	return RTN_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////jionRoom
int CProtocol::ptclBuildJionRoom(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = JION_ROOM; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckJionRoom(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != JION_ROOM)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////ask user in room
int CProtocol::ptclBuildAskUserinRoom(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_PLAYER_IN_ROOM; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskUserinRoom(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != ASK_PLAYER_IN_ROOM)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildAskUserinRoomAsw(char * name ,vector<CUser> users ,unsigned char * pOutBuf , int rst)
{
	int perLenofUser = 30 + 1;
	int dataLen = users.size() * perLenofUser   + 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = PLAYER_IN_ROOM_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = users.size();
	for (int i = 0 ; i < users.size(); ++i)
	{
		memset(&pOutBuf[38 + i * perLenofUser],0,30);
		memcpy(&pOutBuf[38 + i * perLenofUser],users[i].getUserName().c_str(),strlen(users[i].getUserName().c_str()));
		pOutBuf[38 + i * perLenofUser + 30] = users[i].getReady();
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskUserinRoomAsw(vector<CUser>& users,unsigned char * pOutBuf, int len)
{
	int perLenofUser = 30 + 1;
	if (pOutBuf[34] != PLAYER_IN_ROOM_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	int num = pOutBuf[37];
	if (dataLen -1 != num * perLenofUser || num > 4 || num < 0)
	{
		return RTN_ERROR;
	}
	for (int i = 0 ; i < num; ++i)
	{
		CUser user;
		string name((char *)&pOutBuf[38 + i * perLenofUser]);
		int ready = pOutBuf[38 + i * perLenofUser + 30];
		user.setUserName(name);
		user.setReady(ready);
		users.push_back(user);
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////发送聊天信息
int CProtocol::ptclBuildChatInfo(char * name , int roomID , CChatInfo cChatInfo, unsigned char * pOutBuf)
{
	int dataLen = 30 + 200 + 6 + 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = CHAT; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37+1] = roomID / 0x100;
	memset(&pOutBuf[37 + 2],0,30);
	memcpy(&pOutBuf[37 + 2],cChatInfo.getsenderName().c_str(),strlen(cChatInfo.getsenderName().c_str()));
	memset(&pOutBuf[37+30+2],0,200);
	memcpy(&pOutBuf[37+30+2],cChatInfo.getcontent().c_str(),strlen(cChatInfo.getcontent().c_str()));
	SDate date = cChatInfo.gettime();
	pOutBuf[37+230+2] = date.year;
	pOutBuf[37+231+2] = date.month;
	pOutBuf[37+232+2] = date.day;
	pOutBuf[37+233+2] = date.hour;
	pOutBuf[37+234+2] = date.min;
	pOutBuf[37+235+2] = date.sec;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckChatInfo(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != CHAT)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 238)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclCheckChatInfoAsw(queue<CChatInfo>& qChatqueue,unsigned char * pOutBuf, int len)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofRoom = 30 + 200 + 6 + 2;

	if (pOutBuf[34] != CHAT)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen != perLenofRoom)
	{
		return RTN_ERROR;
	}
	string sendName((char *)&pOutBuf[38+2]);
	string content((char *)&pOutBuf[38+2+30]);
	SDate date;
	date.year =   pOutBuf[37+230+2];
	date.month =  pOutBuf[37+231+2];
	date.day =    pOutBuf[37+232+2];
	date.hour =   pOutBuf[37+233+2];
	date.min =    pOutBuf[37+234+2];
	date.sec =    pOutBuf[37+235+2];
	
	CChatInfo cChatInfo;
	cChatInfo.setcontent(content);
	cChatInfo.setsenderNamee(sendName);
	cChatInfo.settime(date);
	
	if (qChatqueue.size() > 5)
	{
		qChatqueue.pop();
	}
	else
	{
		qChatqueue.push(cChatInfo);
	}

	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////leave Room
int CProtocol::ptclBuildLeaveRoom(char * name , int roomID , unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = LEAVE_ROOM; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckLeaveRoom(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != LEAVE_ROOM)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildLeaveRoomAsw(char * name ,unsigned char * pOutBuf , int rst)
{
	int dataLen = 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = LEAVE_ROOM_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = rst;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckLeaveRoomAsw(unsigned char * pOutBuf, int len)
{
	if (pOutBuf[34] != LEAVE_ROOM_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 1)
	{
		return RTN_ERROR;
	}
	int rst = pOutBuf[37];
	if (rst != RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////ready to start
int CProtocol::ptclBuildReadyStart(char * name , int roomID , unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = READY_START; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckReadyStart(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != READY_START)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}
int CProtocol::ptclBuildReadyStartAsw(char * name ,unsigned char * pOutBuf , int rst)
{
	int dataLen = 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = READY_START_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = rst;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckReadyStartAsw(unsigned char * pOutBuf, int len)
{
	if (pOutBuf[34] != READY_START_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 1)
	{
		return RTN_ERROR;
	}
	int rst = pOutBuf[37];
	if (rst != RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
int CProtocol::ptclBuildGameStartAsw(char * name , unsigned char * pOutBuf , int rst)
{
	int dataLen = 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = GAME_START; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = rst;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckGameStartAsw(unsigned char * pOutBuf, int len)
{
	if (pOutBuf[34] != GAME_START)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 1)
	{
		return RTN_ERROR;
	}
	int rst = pOutBuf[37];
	if (rst != RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////请求房间的用户信息
int CProtocol::ptclBuildAskPlayersInGame(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_PLAYER_INFO_INGAME; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskPlayersInGame(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != ASK_PLAYER_INFO_INGAME)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildAskPlayersInGameAsw(char * name ,vector<CPlayer> vPlayers,int RoundIndex ,unsigned char * pOutBuf , int rst)
{
	////m_strName 30 pX 2 pY 2 iHp 2 iMaxHp 2  iTeam 2 eLifeStat 2
	int perLenofCPlayer = 42;
	int dataLen = vPlayers.size() * perLenofCPlayer   + 3;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASK_PLAYER_INFO_INGAME_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = RoundIndex % 0x100;
	pOutBuf[38] = RoundIndex / 0x100;
	pOutBuf[39] = vPlayers.size();
	for (int i = 0 ; i < vPlayers.size(); ++i)
	{
		memset(&pOutBuf[40 + i * perLenofCPlayer],0,30);
		memcpy(&pOutBuf[40 + i * perLenofCPlayer],vPlayers[i].getName().c_str(),strlen(vPlayers[i].getName().c_str()));
		pOutBuf[40 + i * perLenofCPlayer + 30] = vPlayers[i].pX % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 1] = vPlayers[i].pX / 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 2] = vPlayers[i].pY % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 3] = vPlayers[i].pY / 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 4] = vPlayers[i].iHp % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 5] = vPlayers[i].iHp / 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 6] = vPlayers[i].iMaxHp % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 7] = vPlayers[i].iMaxHp / 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 8] = vPlayers[i].iTeam % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 9] = vPlayers[i].iTeam / 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 10] = vPlayers[i].eLifeStat % 0x100;
		pOutBuf[40 + i * perLenofCPlayer + 30 + 11] = vPlayers[i].eLifeStat / 0x100;
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskPlayersInGameAsw(vector<CPlayer>& vPlayers ,int &roundIndex ,unsigned char * pOutBuf, int len)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofCPlayer = 42;

	if (pOutBuf[34] != ASK_PLAYER_INFO_INGAME_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	int rdx = pOutBuf[37] + pOutBuf[38] * 0x100;
	if (rdx == 0)
	{
		return RTN_ERROR;
	}
	int num = pOutBuf[39];
	if (dataLen - 3 != num * perLenofCPlayer)
	{
		return RTN_ERROR;
	}

	roundIndex = pOutBuf[37] + pOutBuf[38] * 0x100;

	for (int i = 0 ; i < num; ++i)
	{
		string name((char *)&pOutBuf[40 + i * perLenofCPlayer]);
		CPlayer cp(name);
		cp.pX = pOutBuf[40 + i * perLenofCPlayer + 30] + pOutBuf[40 + i * perLenofCPlayer + 30 + 1] * 0x100;
		cp.pY = pOutBuf[40 + i * perLenofCPlayer + 30 + 2] + pOutBuf[40 + i * perLenofCPlayer + 30 + 3] * 0x100;
		cp.iHp = pOutBuf[40 + i * perLenofCPlayer + 30 + 4] + pOutBuf[40 + i * perLenofCPlayer + 30 + 5] * 0x100;
		cp.iMaxHp = pOutBuf[40 + i * perLenofCPlayer + 30 + 6] + pOutBuf[40 + i * perLenofCPlayer + 30 + 7] * 0x100;
		cp.iTeam = pOutBuf[40 + i * perLenofCPlayer + 30 + 8] + pOutBuf[40 + i * perLenofCPlayer + 30 + 9] * 0x100;
		cp.eLifeStat =(ELIFESTAT)( pOutBuf[40 + i * perLenofCPlayer + 30 + 10] + pOutBuf[40 + i * perLenofCPlayer + 30 + 11] * 0x100);
		vPlayers.push_back(cp);
	}
	return RTN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////查询上轮命令
int CProtocol::ptclBuildAskActinsInGame(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_OTHER_ACTION_INGAME; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskActinsInGame(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != Get_OTHER_ACTION_INGAME)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildAskActinsInGameAsw(char * name ,map<string ,vector<CAction> >& Actions ,unsigned char * pOutBuf , int rst)
{
	/////m_idemage;  2  m_range; 2   m_derection;  2    m_destX; 2 m_destY; 2 m_eType; 2
	/////每个人动作一定是3步组成 ，再加个30的名字
	int perLenofCPlayer = 12 * 3 + 30;
	int dataLen = Actions.size() * perLenofCPlayer   + 1;
	
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_OTHER_ACTION_INGAME_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = Actions.size();
	int i = 0;
	int j = 0;
	for (map<string ,vector<CAction> >::iterator it = Actions.begin(); it != Actions.end();  ++it)
	{
		memset(&pOutBuf[38 + i * perLenofCPlayer],0,30);
		memcpy(&pOutBuf[38 + i * perLenofCPlayer],it->first.c_str(),strlen(it->first.c_str()));
		for (j = 0 ; j < 3; j++)
		{
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 ] = (it->second)[j].m_idemage % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 1] = (it->second)[j].m_idemage / 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 2] = (it->second)[j].m_range % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 3] = (it->second)[j].m_range / 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 4] = (it->second)[j].m_derection % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 5] = (it->second)[j].m_derection / 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 6] = (it->second)[j].m_destX % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 7] = (it->second)[j].m_destX / 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 8] = (it->second)[j].m_destY % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 9] = (it->second)[j].m_destY / 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 10] = (it->second)[j].m_eType % 0x100;
			pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 11] = (it->second)[j].m_eType / 0x100;
		}
		i++;
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskActinsInGameAsw(map<string ,vector<CAction> >& Actions  ,unsigned char * pOutBuf, int len)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofCAction = 12;
	int perLenofCPlayer = perLenofCAction * 3 + 30;
	

	if (pOutBuf[34] != Get_OTHER_ACTION_INGAME_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;

	int num = pOutBuf[37];
	if (dataLen - 1 != num * perLenofCPlayer)
	{
		return RTN_ERROR;
	}

	for (int i = 0 ; i < num; ++i)
	{
		string name((char *)&pOutBuf[38 + i * perLenofCPlayer]);
		vector<CAction> tempVec;
		for (int j = 0 ; j < 3 ; ++j)
		{
			CAction cp;
			cp.m_idemage = pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 ] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 1] * 0x100;
			cp.m_range = pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 + 2] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 3] * 0x100;
			cp.m_derection = pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 + 4] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 5] * 0x100;
			cp.m_destX = pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 + 6] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 7] * 0x100;
			cp.m_destY = pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 + 8] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 9] * 0x100;
			cp.m_eType =(EACTION) (pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12 + 10] + pOutBuf[38 + i * perLenofCPlayer + 30 + j * 12  + 11] * 0x100);
			tempVec.push_back(cp);
		}
		pair<string ,vector<CAction> > tp(name,tempVec);
		Actions.insert(tp);
	}
	return RTN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////查询细节
int CProtocol::ptclBuildAskDetailInGame(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_RESULT_INGAME; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskDetailInGame(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != Get_RESULT_INGAME)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildAskDetailInGameAsw(char * name ,map<int,vector<string> >& vdiscreption ,unsigned char * pOutBuf , int rst)
{
	int dataLen = 0;
	int mapIndex = 0;
	int vecIndex = 0;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_RESULT_INGAME_ASW; //控制码
	//长度后面补算
	pOutBuf[37] =  vdiscreption.size();
	dataLen += 1;

	mapIndex = 0;
	for (map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end() ; ++it)
	{
		pOutBuf[37 + dataLen] = it->second.size();
		dataLen += 1;

		for (int i = 0 ; i < it->second.size(); ++i)
		{
			string desc = it->second[i];
			int strLen =  strlen(desc.c_str()) + 1;
			pOutBuf[37 + dataLen] = strLen % 0x100;
			pOutBuf[37 + dataLen + 1] = strLen / 0x100;
			dataLen += 2;
			memset(&(pOutBuf[37 + dataLen]),0,strLen);
			memcpy(&(pOutBuf[37 + dataLen]),desc.c_str(),strlen(desc.c_str()));
			dataLen += strLen;
		}
	}
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度

	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskDetailInGameAsw(map<int,vector<string> >& vdiscreption  ,unsigned char * pOutBuf, int len)
{
	int lenIndex = 0;
	if (pOutBuf[34] != Get_RESULT_INGAME_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;

	int num = pOutBuf[37];
	lenIndex += 1;

	for (int i = 0 ; i < num; ++i)
	{
		int vecSize = pOutBuf[37+lenIndex];
		lenIndex += 1;
		vector<string> tempVec;
		for (int j = 0 ; j < vecSize; ++j)
		{
			int strLen = pOutBuf[37+lenIndex] + pOutBuf[37+lenIndex + 1] * 0x100;
			lenIndex += 2;
			string strContent((char *)(&pOutBuf[37+lenIndex]));
			tempVec.push_back(strContent);
			lenIndex +=  strLen;
		}
		pair<int,vector<string> > tp(i,tempVec);
		vdiscreption.insert(tp);
	}
	return RTN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////format
int CProtocol::ptclBuildAskFormatInGame(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_FORMATRESULT_INGAME; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskFormatInGame(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != Get_FORMATRESULT_INGAME)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}
int CProtocol::ptclBuildAskFormatInGameAsw(char * name ,map<int,vector<string> >& vdiscreption ,unsigned char * pOutBuf , int rst)
{
	int dataLen = 0;
	int mapIndex = 0;
	int vecIndex = 0;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = Get_FORMATRESULT_INGAME_ASW; //控制码
	//长度后面补算
	pOutBuf[37] =  vdiscreption.size();
	dataLen += 1;

	mapIndex = 0;
	for (map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end() ; ++it)
	{
		pOutBuf[37 + dataLen] = it->second.size();
		dataLen += 1;

		for (int i = 0 ; i < it->second.size(); ++i)
		{
			string desc = it->second[i];
			int strLen =  strlen(desc.c_str()) + 1;
			pOutBuf[37 + dataLen] = strLen % 0x100;
			pOutBuf[37 + dataLen + 1] = strLen / 0x100;
			dataLen += 2;
			memset(&(pOutBuf[37 + dataLen]),0,strLen);
			memcpy(&(pOutBuf[37 + dataLen]),desc.c_str(),strlen(desc.c_str()));
			dataLen += strLen;
		}
	}
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度

	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskFormatInGameAsw(map<int,vector<CFormatUnit *> >& vdiscreption  ,unsigned char * pOutBuf, int len)
{
	int lenIndex = 0;
	if (pOutBuf[34] != Get_FORMATRESULT_INGAME_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;

	int num = pOutBuf[37];
	lenIndex += 1;

	for (int i = 0 ; i < num; ++i)
	{
		int vecSize = pOutBuf[37+lenIndex];
		lenIndex += 1;
		vector<CFormatUnit *> subVec;
		for (int j = 0 ; j < vecSize; ++j)
		{
			int strLen = pOutBuf[37+lenIndex] + pOutBuf[37+lenIndex + 1] * 0x100;
			lenIndex += 2;
			string strContent((char *)(&pOutBuf[37+lenIndex]));
			vector<string> tempVec = stringSplit(strContent,":");
			if ( tempVec.size() > 0)
			{
				string title = tempVec[0];
				CFormatUnit * pUnit = CFormatUnitFactory::getInstance()->create(title);
				if (pUnit->decode(tempVec) == RTN_SUCCESS)
				{
					subVec.push_back(pUnit);
				}
			}
			lenIndex +=  strLen;
		}
		pair<int,vector<CFormatUnit *> > tp(i,subVec);
		vdiscreption.insert(tp);
	}
	return RTN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////发送动作
int CProtocol::ptclBuildAskSendAction(char * name , int roomID,vector<CAction>& subVec, int& RoundIndex, unsigned char * pOutBuf)
{
	/////m_idemage;  2  m_range; 2   m_derection;  2    m_destX; 2 m_destY; 2 m_eType; 2
	/////每个人动作一定是3步组成 ，再加个30的名字
	int dataLen = 12 * 3 + 5;

	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = SEND_ACTION_INGAME; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[38] = roomID / 0x100;
	pOutBuf[39] = RoundIndex % 0x100;
	pOutBuf[40] = RoundIndex / 0x100;
	pOutBuf[41] = subVec.size();
	for (int i = 0 ; i < subVec.size(); ++i)
	{
		pOutBuf[42 + i *  12 ] = subVec[i].m_idemage % 0x100;
		pOutBuf[42 + i *  12  + 1] = subVec[i].m_idemage / 0x100;
		pOutBuf[42 + i *  12  + 2] = subVec[i].m_range % 0x100;
		pOutBuf[42 + i *  12  + 3] = subVec[i].m_range / 0x100;
		pOutBuf[42 + i *  12  + 4] = subVec[i].m_derection % 0x100;
		pOutBuf[42 + i *  12  + 5] = subVec[i].m_derection / 0x100;
		pOutBuf[42 + i *  12  + 6] = subVec[i].m_destX % 0x100;
		pOutBuf[42 + i *  12  + 7] = subVec[i].m_destX / 0x100;
		pOutBuf[42 + i *  12  + 8] = subVec[i].m_destY % 0x100;
		pOutBuf[42 + i *  12  + 9] = subVec[i].m_destY / 0x100;
		pOutBuf[42 + i *  12  + 10] = subVec[i].m_eType % 0x100;
		pOutBuf[42 + i *  12  + 11] = subVec[i].m_eType / 0x100;
	}
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskSendAction(int& ID ,vector<CAction >& subVec, int& RoundIndex,unsigned char * pOutBuf)
{
	//房间名字30 ，创建者名字30，房间ID 2 ， 最大人数 2  ，当前人数 2， 房间状态 1 
	int perLenofCAction = 12;

	if (pOutBuf[34] != SEND_ACTION_INGAME)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	RoundIndex = pOutBuf[39] + pOutBuf[40] * 0x100;
	int num = pOutBuf[41];
	if (dataLen - 5 != num * perLenofCAction)
	{
		return RTN_ERROR;
	}

	for (int i = 0 ; i < num; ++i)
	{
		CAction  cp;
		cp.m_idemage = pOutBuf[42 + i  * 12 ] + pOutBuf[42 + i *  12  + 1] * 0x100;
		cp.m_range = pOutBuf[42 + i *  12 + 2] + pOutBuf[42 + i *  12  + 3] * 0x100;
		cp.m_derection = pOutBuf[42 + i *  12 + 4] + pOutBuf[42 + i *  12  + 5] * 0x100;
		cp.m_destX = pOutBuf[42 + i *  12 + 6] + pOutBuf[42 + i * 12  + 7] * 0x100;
		cp.m_destY = pOutBuf[42 + i *  12 + 8] + pOutBuf[42 + i * 12  + 9] * 0x100;
		cp.m_eType =(EACTION) (pOutBuf[42 + i *  12 + 10] + pOutBuf[42 + i * 12  + 11] * 0x100);
		subVec.push_back(cp);
	}
	return RTN_SUCCESS;
}
int CProtocol::ptclBuildAskSendActionAsw(char * name ,unsigned char * pOutBuf , int rst)
{
	int dataLen = 1;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = SEND_ACTION_INGAME_ASW; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = rst;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}
int CProtocol::ptclCheckAskSendActionAsw(unsigned char * pOutBuf, int len)
{
	if (pOutBuf[34] != SEND_ACTION_INGAME_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 1)
	{
		return RTN_ERROR;
	}
	int rst = pOutBuf[37];
	if (rst != RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////获取每一步的玩家信息
int CProtocol::ptclBuildAskStepPlayerInGame(char * name , int roomID, unsigned char * pOutBuf)
{
	int dataLen = 2;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASKSTEP_PLAYERINFO; //控制码
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度
	pOutBuf[37] = roomID % 0x100;
	pOutBuf[37 + 1] = roomID / 0x100;
	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskStepPlayerInGame(int& ID ,unsigned char * pOutBuf)
{
	if (pOutBuf[34] != ASKSTEP_PLAYERINFO)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;
	if (dataLen  != 2)
	{
		return RTN_ERROR;
	}
	ID = pOutBuf[37] + pOutBuf[38] * 0x100;
	return RTN_SUCCESS;
}

int CProtocol::ptclBuildAskStepPlayerInGameAsw(char * name ,map<int,vector<CPlayer> >& vdispPlayer ,unsigned char * pOutBuf , int rst)
{
	int perLenofCPlayer = 42;
	int dataLen = 0;
	int mapIndex = 0;
	int vecIndex = 0;
	pOutBuf[0] = 0x68;
	memset(pOutBuf+1,0,30);
	memcpy(pOutBuf+1,name,strlen(name)); //填写名字
	pOutBuf[31] = 0x01;//任务号低字节
	pOutBuf[32] = 0x00;//任务号高字节
	pOutBuf[33] = 0x68;
	pOutBuf[34] = ASKSTEP_PLAYERINFO_ASW; //控制码
	//长度后面补算
	pOutBuf[37] =  vdispPlayer.size();
	dataLen += 1;

	mapIndex = 0;
	for (map<int,vector<CPlayer> >::iterator it = vdispPlayer.begin(); it != vdispPlayer.end() ; ++it)
	{
		pOutBuf[37 + dataLen] = it->second.size();
		dataLen += 1;

		for (int i = 0 ; i < it->second.size(); ++i)
		{
			memset(&pOutBuf[37 + dataLen],0,30);
			memcpy(&pOutBuf[37 + dataLen],it->second[i].getName().c_str(),strlen(it->second[i].getName().c_str()));
			pOutBuf[37 + dataLen + 30] = it->second[i].pX % 0x100;
			pOutBuf[37 + dataLen + 30 + 1] = it->second[i].pX / 0x100;
			pOutBuf[37 + dataLen + 30 + 2] = it->second[i].pY % 0x100;
			pOutBuf[37 + dataLen + 30 + 3] = it->second[i].pY / 0x100;
			pOutBuf[37 + dataLen + 30 + 4] = it->second[i].iHp % 0x100;
			pOutBuf[37 + dataLen + 30 + 5] = it->second[i].iHp / 0x100;
			pOutBuf[37 + dataLen + 30 + 6] = it->second[i].iMaxHp % 0x100;
			pOutBuf[37 + dataLen + 30 + 7] = it->second[i].iMaxHp / 0x100;
			pOutBuf[37 + dataLen + 30 + 8] = it->second[i].iTeam % 0x100;
			pOutBuf[37 + dataLen + 30 + 9] = it->second[i].iTeam / 0x100;
			pOutBuf[37 + dataLen + 30 + 10] = it->second[i].eLifeStat % 0x100;
			pOutBuf[37 + dataLen + 30 + 11] = it->second[i].eLifeStat / 0x100;
			dataLen += perLenofCPlayer;
		}
	}
	pOutBuf[35] = dataLen % 0x100;//长度
	pOutBuf[36] = dataLen / 0x100;//长度

	pOutBuf[37+dataLen] = checkSum(pOutBuf,37+dataLen);
	pOutBuf[38+dataLen] = 0x16;
	return 39+dataLen;
}

int CProtocol::ptclCheckAskStepPlayerInGameAsw(map<int,vector<CPlayer> >& vdispPlayer  ,unsigned char * pOutBuf, int len)
{
	int lenIndex = 0;
	int perLenofCPlayer = 42;
	if (pOutBuf[34] != ASKSTEP_PLAYERINFO_ASW)
	{
		return RTN_ERROR;
	}
	int dataLen = pOutBuf[35] + pOutBuf[36] * 0x100;

	int num = pOutBuf[37];
	lenIndex += 1;

	for (int i = 0 ; i < num; ++i)
	{
		int vecSize = pOutBuf[37+lenIndex];
		lenIndex += 1;
		vector<CPlayer> tempVec;
		for (int j = 0 ; j < vecSize; ++j)
		{
			string name((char *)&pOutBuf[37 + lenIndex + i * perLenofCPlayer]);
			CPlayer cp(name);
			cp.pX = pOutBuf[37 + lenIndex + 30] + pOutBuf[37 + lenIndex + 30 + 1] * 0x100;
			cp.pY = pOutBuf[37 + lenIndex + 30 + 2] + pOutBuf[37 + lenIndex + 30 + 3] * 0x100;
			cp.iHp = pOutBuf[37 + lenIndex + 30 + 4] + pOutBuf[37 + lenIndex + 30 + 5] * 0x100;
			cp.iMaxHp = pOutBuf[37 + lenIndex + 30 + 6] + pOutBuf[37 + lenIndex + 30 + 7] * 0x100;
			cp.iTeam = pOutBuf[37 + lenIndex + 30 + 8] + pOutBuf[37 + lenIndex + 30 + 9] * 0x100;
			cp.eLifeStat =(ELIFESTAT)( pOutBuf[37 + lenIndex + 30 + 10] + pOutBuf[37 + lenIndex + 30 + 11] * 0x100);
			lenIndex +=  perLenofCPlayer;
			tempVec.push_back(cp);
		}
		pair<int,vector<CPlayer> > tp(i,tempVec);
		vdispPlayer.insert(tp);
	}
	return RTN_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////

int CProtocol::recvAReportFormServer(unsigned char * pbuf , int& len)
{
	size_t recvLen = 0;
	int LastTime = OSgetNowSeconds();
	int iNow = 0;
	while (true)
	{
		getConn()->CtcpReceive(pbuf,2000,g_iTimeOut,&recvLen);
		if (recvLen > 0)
		{
			if (RBWrite(pbuf,recvLen,&g_ringBuffer) == -1) 
			{
				
			}
			int iBuffLen = 0 ;
			while (true)
			{
				int retVal = pProtocol->getAreport(pbuf,len,&g_ringBuffer);
				if (retVal > 0)
				{
					len = retVal;
					return RTN_SUCCESS;
				}
				else if (retVal == RTN_TIME_OUT)
				{
					break ;
				}
			}
		}
		iNow = OSgetNowSeconds();
		if (iNow - LastTime > 2)
		{
			return RTN_TIME_OUT;
		}
	}
	len = 0;
	return RTN_ERROR;
}
