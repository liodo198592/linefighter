#ifndef _CONFIG_H
#define _CONFIG_H

/**
* Define all the parameters that're used to 
* properly setup the gateway.
*
* #<Config_File_Parser> may be utilized to setup the gateway configuration
*/
/* 网关配置参数 */
const int TERMINAL_PORT = 10000;
const int STATION_PORT  = 10001;

const char AUTHEN_CLIENT_FILE[] = "passwd.db";


#define  TO_STATION_CHANNEL  'S'
#define  TO_TERMINAL_CHANNEL 'T'

/* 终端在线信息参数 */
const int MAX_MAP_ENTRY_COUNT = 20000;	// 哈希表大小限制
const int HEART_BEAT_INTERVAL = 600;  // 600秒(待定)，考虑网络传输的延时。

/* 线程池设定参数 */
const short MINNUM_OF_THREAD = 3;
const short MAXNUM_OF_THREAD = 100;
const short WAIT_SIZE = 5000;
const short PARA_SIZE = 1024;

/* 通信协议配置 */
const short PORT_A_PROTOCOL = 0;
const short PORT_B_PROTOCOL = 1;

const short IBUFFER_SIZE = 512;
const short HEART_BEAT_PKT_SIZE = 6;
const char STATION_HEARTBEAT_CONTKEY = 0x40;
const char CLIENT_HEARTBEAT_CONTKEY = 0x24;


#endif