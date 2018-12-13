#ifndef _PROTOCOL_CODE_H_
#define _PROTOCOL_CODE_H_

//代码	字节数	说明
//68H	1		帧起始符
//NAME	30		用户名称，客户端发送时填写，服务端网络层/引擎主动发送时填写全0
//SEQ	2		命令序号(应答和发送使用同一个号)，发送时随机生成此号
//68H	1		帧起始符
//C		1		控制码
//L		2		数据长度（2字节）
//DATA	变长	数据域
//CS	1		校验码
//16H	1		结束码
#define SUCCESS_ASW_CODE 0x01

#define ASK_LOGIN 0x01  //登陆报文
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
