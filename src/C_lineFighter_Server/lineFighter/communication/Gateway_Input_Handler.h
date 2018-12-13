#ifndef _GATEWAY_INPUT_HANDLER_H
#define _GATEWAY_INPUT_HANDLER_H

//-----------------------------------------------------------------
//File: Gateway_Input_Handler.h
//Responsibility: When a new client connect the gateway, a Gateway_Input_Handler is created
//                to handle all messages from the client.There are main three types of messages:
//                (1)LogIn message
//                (2)Data message
//                (3)Heart beat message
//                
// 
//-----------------------------------------------------------------
#include "Config.h"
#include "ace/Asynch_IO.h"
#include "ace/Message_Block.h"
#include "ace/SString.h"
#include <string>
#include <map>
#include "ace/INET_Addr.h"
#include "ace/RW_Thread_Mutex.h"
#include "../CProtocol.h"

using namespace std;
class CRingBuffer;
class CProtocol;
class Event_Channel;

class Gateway_Input_Handler : public ACE_Service_Handler 
{
public:
  Gateway_Input_Handler();
  
  virtual ~Gateway_Input_Handler ();

  // Called by ACE_Asynch_Acceptor when a client connects.
  virtual void open (ACE_HANDLE new_handle,ACE_Message_Block &message_block);

  virtual void addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address);
  
  void send_message( ACE_Message_Block* block);
  
  string get_client_id(){return this->m_usrName;}

  virtual ACE_Message_Block * getOneReport(ACE_INT32& bufferlen , CRingBuffer * pRingBuffer,ACE_INT32& iErrorFalg);//每次来获取一个报文，如果不构成一个报文则不进行任何操作

  virtual void close ();

  ACE_INT32 getLastHeartBeat()
  {
	  return iLastHeartBeat;
  }

  CProtocol m_vProtocol;//终端所有可能的规约都需要被加入到此容器中
 
protected: 
  ACE_Message_Block *mblk_;       
  ACE_Asynch_Read_Stream reader_; 
  ACE_Asynch_Write_Stream writer_;

  CRingBuffer * m_ringBuffer;
  ACE_INT32 m_iTerminalID; //记录解析出来的终端地址

  virtual void reply_msg(ACE_Message_Block* block, int iLen);

  virtual bool parse(ACE_Message_Block* block);

  // Handle input from logging clients.
  virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
  virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
  virtual void printRemoteAddr();
  void reply_toOther(string usrName, ACE_Message_Block* block, int iLen);


protected:
	bool logined;
	string m_usrName;
	ACE_INET_Addr remote;
	ACE_INET_Addr local;
	ACE_INT32 iLastHeartBeat; 
	
};

#endif