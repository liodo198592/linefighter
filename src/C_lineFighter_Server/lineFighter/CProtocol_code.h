#ifndef _PROTOCOL_CODE_H_
#define _PROTOCOL_CODE_H_

//����	�ֽ���	˵��
//68H	1		֡��ʼ��
//NAME	30		�û����ƣ��ͻ��˷���ʱ��д������������/������������ʱ��дȫ0
//SEQ	2		�������(Ӧ��ͷ���ʹ��ͬһ����)������ʱ������ɴ˺�
//68H	1		֡��ʼ��
//C		1		������
//L		2		���ݳ��ȣ�2�ֽڣ�
//DATA	�䳤	������
//CS	1		У����
//16H	1		������
#define SUCCESS_ASW_CODE 0x01

#define ASK_LOGIN 0x01  //��½����
#define LOGIN_ASW 0x81

#define ASK_ROOMLIST 0x02  
#define ROOMLIST_ASW 0x82

#define CREATE_ROOM 0x03 
#define CREATE_ROOM_ASW 0x83

#define JION_ROOM 0x04 
#define JION_ROOM_ASW 0x84

#define ASK_PLAYER_IN_ROOM 0x05 
#define PLAYER_IN_ROOM_ASW 0x85

#define START_GAME 0x06 
#define START_GAME_ASW 0x86

#define START_GAME2 0x07 
#define START_GAME_ASW2 0x87

#define QUIT_GAME 0x08
#define QUIT_GAME_ASW 0x88

#define CHAT 0x09
#define CHAT_ASW 0x89

#define ASK_PLAYER_INFO_INGAME 0x10
#define ASK_PLAYER_INFO_INGAME_ASW 0x90

#define SEND_ACTION_INGAME 0x11
#define SEND_ACTION_INGAME_ASW 0x91

#define Get_OTHER_ACTION_INGAME 0x12
#define Get_OTHER_ACTION_INGAME_ASW 0x92

#define Get_RESULT_INGAME 0x13
#define Get_RESULT_INGAME_ASW 0x93

#define Get_FINISH_GAME 0x14
#define Get_FINISH_GAME_ASW 0x94

#define ASK_ONLINE_USERLIST 0x15
#define ASK_ONLINE_USERLIST_ASW 0x95

#define LEAVE_ROOM 0x16
#define LEAVE_ROOM_ASW 0x96

#define READY_START 0x17
#define READY_START_ASW 0x97

#define GAME_START 0x18
#define GAME_START_ASW 0x98

#define ASKSTEP_PLAYERINFO 0x19
#define ASKSTEP_PLAYERINFO_ASW 0x99

#define Get_FORMATRESULT_INGAME 0x1A
#define Get_FORMATRESULT_INGAME_ASW 0x9A

#endif
