#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "RingBuf.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CPlayer.h"
#include "CAction.h"

class CRingBuffer;
class CRoom;
class CUser;
class CChatInfo;
class CFormatUnit;

struct  SPlayerName
{
    string strName;
    int iID;
    int iHp;
    int iX;
    int iY;
};

struct SAction //һ��ACTION����
{
    EACTION m_type;
    string m_value; // ���1ָ��1ֵ	10	ASC�ַ������������ͱ���˵�Ƿ��� ��1�����ߡ�2���������ƶ���˵�ǰ���%d��%d�������λ�ô�����1��3����ʾ��һ�е�����
};

struct  SPlayerAction
{
    int iPlayerIndex;
    string strPlayerName;
    vector<SAction> vActions; //���������������  
};


class CProtocol
{
public:
	CProtocol(){}
	virtual ~CProtocol(){}
    int getAreport(unsigned char * pOutBuf, int& bufferlen ,SRingBuf * pRingBuf);
    unsigned char checkSum(const unsigned char * cBuffer, int iLen);
    int parse(int& bufferlen , CRingBuffer * pRingBuf);

	string ptclGetUserName(unsigned char * cBuffer, int iLen);
	int ptclGetFuncCode(unsigned char * cBuffer, int iLen);
    int ptclGetGameNo(unsigned char * cBuffer, int iLen);//��ȡ�����   ��� 0x1121
    int ptclGetPlayerInfo(vector<SPlayerName>& info,unsigned char * cBuffer, int iLen);//������Ҹ��� //��� 0x1121

    int ptclGetAckConfirmOk(int ControlCode, int ItemID ,unsigned char * cBuffer, int iLen); //��ȡָ���������������ID��ȷ��OK�������򷵻� < 0

	int recvAReportFormServer(unsigned char * pbuf , int& len);

	int ptclBuildLogin(char * name ,unsigned char * pOutBuf);
	int ptclBuildLoginAsw(char * name ,unsigned char * pOutBuf ,int rst);
	int ptclCheckLogin(unsigned char * pOutBuf ,int len);


	int ptclBuildAskOnlineList(char * name ,unsigned char * pOutBuf);
	int ptclBuildAskOnlineListAsw(char * name ,vector<CUser> onlineList ,unsigned char * pOutBuf);
	int ptclCheckAskOnlineListAsw(map<string, CUser>& onlineList ,unsigned char * pOutBuf, int len);

	int ptclBuildAskRoomList(char * name ,unsigned char * pOutBuf);
	int ptclBuildAskRoomListAsw(char * name ,vector<CRoom> onRooomList ,unsigned char * pOutBuf);
	int ptclCheckAskRoomListAsw(map<int, CRoom>& onRoomList ,unsigned char * pOutBuf, int len);

	int ptclBuildCreateRoom(char * name , string roomName, unsigned char * pOutBuf);
	int ptclCheckCreateRoom(string& roomName ,unsigned char * pOutBuf);
	int ptclBuildCreateRoomAsw(char * name ,CRoom rooom ,unsigned char * pOutBuf , int rst);
	int ptclCheckCreateRoomAsw(CRoom& room ,unsigned char * pOutBuf, int len);

	int ptclBuildJionRoom(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckJionRoom(int& ID ,unsigned char * pOutBuf);

	int ptclBuildAskUserinRoom(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskUserinRoom(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskUserinRoomAsw(char * name ,vector<CUser> users ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskUserinRoomAsw(vector<CUser>& users,unsigned char * pOutBuf, int len);

	int ptclBuildChatInfo(char * name , int roomID , CChatInfo cChatInfo, unsigned char * pOutBuf);
	int ptclCheckChatInfo(int& ID ,unsigned char * pOutBuf);
	int ptclCheckChatInfoAsw(queue<CChatInfo>& qChatqueue,unsigned char * pOutBuf, int len);

	int ptclBuildLeaveRoom(char * name , int roomID , unsigned char * pOutBuf);
	int ptclCheckLeaveRoom(int& ID ,unsigned char * pOutBuf);
	int ptclBuildLeaveRoomAsw(char * name ,unsigned char * pOutBuf , int rst);
	int ptclCheckLeaveRoomAsw(unsigned char * pOutBuf, int len);

	int ptclBuildReadyStart(char * name , int roomID , unsigned char * pOutBuf);
	int ptclCheckReadyStart(int& ID ,unsigned char * pOutBuf);
	int ptclBuildReadyStartAsw(char * name ,unsigned char * pOutBuf , int rst);
	int ptclCheckReadyStartAsw(unsigned char * pOutBuf, int len);

	int ptclBuildGameStartAsw(char * name , unsigned char * pOutBuf , int rst);
	int ptclCheckGameStartAsw(unsigned char * pOutBuf, int len);

	int ptclBuildAskPlayersInGame(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskPlayersInGame(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskPlayersInGameAsw(char * name ,vector<CPlayer> vPlayers,int RoundIndex ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskPlayersInGameAsw(vector<CPlayer>& vPlayers ,int &roundIndex ,unsigned char * pOutBuf, int len);

	int ptclBuildAskActinsInGame(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskActinsInGame(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskActinsInGameAsw(char * name ,map<string ,vector<CAction> >& Actions ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskActinsInGameAsw(map<string ,vector<CAction> >& Actions  ,unsigned char * pOutBuf, int len);

	int ptclBuildAskDetailInGame(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskDetailInGame(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskDetailInGameAsw(char * name ,map<int,vector<string> >& vdiscreption ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskDetailInGameAsw(map<int,vector<string> >& vdiscreption  ,unsigned char * pOutBuf, int len);
	
	int ptclBuildAskSendAction(char * name , int roomID,vector<CAction>& subVec, int& RoundIndex, unsigned char * pOutBuf);
	int ptclCheckAskSendAction(int& ID ,vector<CAction>& subVec, int& RoundIndex,unsigned char * pOutBuf);
	int ptclBuildAskSendActionAsw(char * name ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskSendActionAsw(unsigned char * pOutBuf, int len);

	int ptclBuildAskStepPlayerInGame(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskStepPlayerInGame(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskStepPlayerInGameAsw(char * name ,map<int,vector<CPlayer> >& vdispPlayer ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskStepPlayerInGameAsw(map<int,vector<CPlayer> >& vdispPlayer  ,unsigned char * pOutBuf, int len);

	int ptclBuildAskFormatInGame(char * name , int roomID, unsigned char * pOutBuf);
	int ptclCheckAskFormatInGame(int& ID ,unsigned char * pOutBuf);
	int ptclBuildAskFormatInGameAsw(char * name ,map<int,vector<string> >& vdiscreption ,unsigned char * pOutBuf , int rst);
	int ptclCheckAskFormatInGameAsw(map<int,vector<CFormatUnit *> >& vdiscreption  ,unsigned char * pOutBuf, int len);
};

CProtocol* getProtocol();

#endif