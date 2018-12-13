#include "ace/OS.h"
#include "Config.h"
#include "Gateway_Acceptor.h"
#include "ClientInfo_Manager.h"
#include <iostream>
#include <string>
#include "ErrorCode.h"
#include "RingBuffer.h"
#include "Gateway_Input_Handler.h"
#include "ace/os_include/os_signal.h"
#include "ace/Reactor.h"
#include "ace/Signal.h"
#include "ace/Sig_Handler.h"
#include "ace/POSIX_Proactor.h"
#include "ace/POSIX_Asynch_IO.h"
#include "../CProtocol.h"
#include "../CProtocol_code.h"
#include "../model_comm.h"
#include <map>
extern ACE_Thread_Mutex mapMutex;

Gateway_Input_Handler::Gateway_Input_Handler() 
{
	mblk_ = NULL;
	logined = false;
    m_usrName = "";
	CRingBuffer * pCRingBuffer = new CRingBuffer(1024*2);
	m_ringBuffer = pCRingBuffer;
}

void Gateway_Input_Handler::addresses (const ACE_INET_Addr &remote_address,const ACE_INET_Addr &local_address)
{
	remote = remote_address;
	local = local_address;
}

void Gateway_Input_Handler::printRemoteAddr()
{
	char addr[100];
	remote.addr_to_string(addr,100);
	ACE_DEBUG ((LM_DEBUG,  ACE_TEXT("remote ip,port:%s\n"), addr));
}

Gateway_Input_Handler::~Gateway_Input_Handler () 
{
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("handle %d Disconnected from Gateway\n"),this));
	//ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("close socket\n")));
	reader_.cancel();
	writer_.cancel();
	ACE_OS::closesocket (handle ());
	
	delete m_ringBuffer;
	
	if (mblk_) mblk_->release ();
	mblk_ = NULL;
}


void Gateway_Input_Handler::close () 
{
	ACE_OS::closesocket (handle ());
}

//当远程连接到来时，被Acceptor调用的hook method
void Gateway_Input_Handler::open(ACE_HANDLE new_handle, ACE_Message_Block &) 
{
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("terminal Connected ! handle = %d !\n"), this));
	this->handle(new_handle);

	reader_.open (*this, new_handle, 0, proactor ());
	writer_.open(*this, new_handle, 0, proactor());

	ACE_NEW_NORETURN
		(mblk_, ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE));
 
	reader_.read (*mblk_, IBUFFER_SIZE);

}

//当异步读操作完成时，被调用的hook method
void Gateway_Input_Handler::handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result)
{		
	  //ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Gateway_Input_Handler ::handle_read_stream %d !\n"),this));
	  if (!result.success () || result.bytes_transferred () == 0)
	  {//连接断开时
		 mapMutex.acquire();
		 Client_Handler* c_handler = NULL ;
		 int res = ClientInfo_Manager::instance()->get_client_handler(this->m_usrName,c_handler,EClientType_TERMINAL);
		 if(c_handler != NULL && res != -1)
		 {	
			if (c_handler->get_ihandler() == this)//如果绑定的连接是自己的话则置无效，否则不置无效
			{
				c_handler->setValid(false);	
			}
		 }
		 else if(c_handler == NULL )
		 {
			 //未在active map中登记的连接应直接关闭并析构handler		
			 mapMutex.release();
			 delete this;
		 }	
		 mapMutex.release();
		 //ClientInfo_Manager::instance()->del_online(this->client_id_);	// 从在线信息表中删除,同时释放资源
	  }
	  else 
	  {
	  //负载读取完成，并将包发送到消息队列中，等待被处理
	  //同时读取下一个完整的包
		int bytes_read = result.bytes_transferred ();
		bool connected = true;

		//////////////////////////////////////////////////////////////////////////加入ringbuffer
		if (m_ringBuffer->writeBuffer(this->mblk_->rd_ptr(),bytes_read) == -1) 
		{
 			ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("failed to write into ringbuffer, RingBuffer is full!:")));
		}
		else
		{
// 			ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("写入RINGBUFFER!:")));
// 			ACE_HEX_DUMP((LM_DEBUG,(const char*)(this->mblk_->rd_ptr()),(ACE_INT32)bytes_read));
		}
		this->mblk_->release(); //放入缓冲以后释放消息
		//////////////////////////////////////////////////////////////////////////获取完整报文
		
		
		ACE_INT32 iBuffLen = 0 ;
		while (true)
		{
			ACE_Message_Block * pBlock = NULL;
			ACE_INT32 iErrorFalg = RTN_UNKOW;
			if ( (pBlock = getOneReport(iBuffLen, m_ringBuffer,iErrorFalg)) != NULL)
			{
				pBlock->size(iBuffLen);
				ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("Get A whole Report! from Terminal")));
				printRemoteAddr();
				ACE_HEX_DUMP((LM_DEBUG,(const char*)(pBlock->rd_ptr()),(ACE_INT32)iBuffLen));

				connected = parse(pBlock);		// 对通信帧做协议解析、处理

			}
			else if (iErrorFalg == RTN_TIME_OUT || iErrorFalg == RTN_UNKOW)
			{
				break;
			}
		}

		if(connected)
		{	
			//ACE_NEW_NORETURN(this->mblk_, ACE_Message_Block (bytes_read));
			ACE_NEW_NORETURN(this->mblk_, ACE_Message_Block (IBUFFER_SIZE));
			reader_.read (*mblk_, IBUFFER_SIZE);
		}
	  }
}

// 释放消息块
void Gateway_Input_Handler::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result){
	result.message_block().release();
}

//sending reply message 
void Gateway_Input_Handler::send_message( ACE_Message_Block* block)
{
	ACE_INT32 iLen = block->length();
	block->rd_ptr(block->rd_ptr());
	writer_.write(*block, iLen);
}

bool Gateway_Input_Handler::parse(ACE_Message_Block* block)
{
	string userName = getProtocol()->ptclGetUserName((unsigned char*)block->rd_ptr(),(int)(unsigned char*)block->size());
	int code = getProtocol()->ptclGetFuncCode((unsigned char*)block->rd_ptr(),(int)(unsigned char*)block->size());

	if (userName != "" && strcmp(userName.c_str(),m_usrName.c_str()) != 0)
	{
		m_usrName = userName;
	}	

	if (strcmp(userName.c_str(),this->m_usrName.c_str()) != 0 && this->m_usrName != "" && userName != "")  //判断区县码是否改变过 , 第一次连接建立 ，在client_id_ == ""的时候不进行判断
	{
		//用新id 代替该HANDLE 所在的map中的旧id;
		if (ClientInfo_Manager::instance()->update_key(this->m_usrName,userName,EClientType_TERMINAL) != -1)
		{
			this->m_usrName = userName; //更新该通道的ID
		}
	}

	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("%s has login!"),userName.c_str()));
	if (!this->logined)
	{
		mapMutex.acquire();
		Client_Handler* c_handler = new Client_Handler(this);
		this->m_usrName = userName;
		ClientInfo_Manager::instance()->new_online(userName,c_handler,EClientType_TERMINAL);
		this->logined = 1;
		mapMutex.release();
	}

	if (code == ASK_LOGIN)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		int len = getProtocol()->ptclBuildLoginAsw((char *)userName.c_str(),(unsigned char *)replyBlk->wr_ptr(),SUCCESS_ASW_CODE);
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == ASK_ONLINE_USERLIST)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CUser> tempVec;
		ClientInfo_Manager::instance()->getOnlineList(tempVec);
		int len = getProtocol()->ptclBuildAskOnlineListAsw((char *)userName.c_str(),tempVec,(unsigned char *)replyBlk->wr_ptr());
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == ASK_ROOMLIST)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		ClientInfo_Manager::instance()->getRoomList(tempVec);
		int len = getProtocol()->ptclBuildAskRoomListAsw((char *)userName.c_str(),tempVec,(unsigned char *)replyBlk->wr_ptr());
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == CREATE_ROOM)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		CRoom room;
		int len = 0;
		string roomName;
		if (getProtocol()->ptclCheckCreateRoom( roomName,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->createRoom(room, roomName,userName) == RTN_SUCCESS)
			{
				len = getProtocol()->ptclBuildCreateRoomAsw((char *)userName.c_str(),room,(unsigned char *)replyBlk->wr_ptr(),RTN_SUCCESS);	
			}
			else
			{
				len = getProtocol()->ptclBuildCreateRoomAsw((char *)userName.c_str(),room,(unsigned char *)replyBlk->wr_ptr(),RTN_ERROR);	
			}
		}
		else
		{
			len = getProtocol()->ptclBuildCreateRoomAsw((char *)userName.c_str(),room,(unsigned char *)replyBlk->wr_ptr(),RTN_ERROR);	
		}
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == JION_ROOM)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		if (getProtocol()->ptclCheckJionRoom( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->jionRoom(room, roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildCreateRoomAsw((char *)userName.c_str(),room,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == ASK_PLAYER_IN_ROOM)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CUser> tempVec;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		if (getProtocol()->ptclCheckAskUserinRoom( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getUserinRoom(tempVec, roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskUserinRoomAsw((char *)userName.c_str(),tempVec,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == CHAT)
	{
		int roomID;
		int ret = RTN_ERROR;
		vector<CUser> tempVec;
		if (getProtocol()->ptclCheckChatInfo( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getUserinRoom(tempVec, roomID,userName) == RTN_SUCCESS)
			{
				for (int idx = 0; idx < tempVec.size() ; ++idx)
				{
					ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
					memcpy(replyBlk->wr_ptr(), (unsigned char*)block->rd_ptr(), (int)(unsigned char*)block->size());
					reply_toOther(tempVec[idx].getUserName(),replyBlk,(int)(unsigned char*)block->size());
				}
			}
		}
	}

	if (code == LEAVE_ROOM)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		if (getProtocol()->ptclCheckLeaveRoom( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->leaveRoom(roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildLeaveRoomAsw((char *)userName.c_str(),(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == READY_START)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		if (getProtocol()->ptclCheckReadyStart( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->setReady(roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildReadyStartAsw((char *)userName.c_str(),(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == ASK_PLAYER_INFO_INGAME)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CRoom> tempVec;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		vector<CPlayer> vplayers;
		int RoundIndex = 0;
		if (getProtocol()->ptclCheckAskPlayersInGame( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getPlayerInfoFromEngine(vplayers,RoundIndex,roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskPlayersInGameAsw((char *)userName.c_str(),vplayers,RoundIndex,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == Get_OTHER_ACTION_INGAME)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		map<string ,vector<CAction>> vShowActions;
		if (getProtocol()->ptclCheckAskActinsInGame( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getActionInfoFromEngine(vShowActions,roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskActinsInGameAsw((char *)userName.c_str(),vShowActions,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == Get_RESULT_INGAME)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		map<int,vector<string>> vdiscreption;
		if (getProtocol()->ptclCheckAskDetailInGame( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getDetailInfoFromEngine(vdiscreption,roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskDetailInGameAsw((char *)userName.c_str(),vdiscreption,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == SEND_ACTION_INGAME)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		vector<CAction> subVec;
		int RoundIndex = 0;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		map<int,vector<string>> vdiscreption;
		if (getProtocol()->ptclCheckAskSendAction( roomID,subVec,RoundIndex,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->sendActionFromEngine(subVec,RoundIndex,roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskSendActionAsw((char *)userName.c_str(),(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}


	if (code == ASKSTEP_PLAYERINFO)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		map<int,vector<CPlayer>> vdispPlayer;
		CRoom room;
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		if (getProtocol()->ptclCheckAskStepPlayerInGame( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getStepPlayerFromEngine(vdispPlayer, roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskStepPlayerInGameAsw((char *)userName.c_str(),vdispPlayer,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	if (code == Get_FORMATRESULT_INGAME)
	{
		ACE_Message_Block * replyBlk =  new ACE_Message_Block(2048);//申请的报数内存
		ACE_INT64 iSize = block->size();
		int len = 0;
		int roomID;
		int ret = RTN_ERROR;
		map<int,vector<string>> vdiscreption;
		if (getProtocol()->ptclCheckAskFormatInGame( roomID,(unsigned char*)block->rd_ptr()) == RTN_SUCCESS)
		{
			if (ClientInfo_Manager::instance()->getFormatInfoFromEngine(vdiscreption,roomID,userName) == RTN_SUCCESS)
			{
				ret = RTN_SUCCESS;
			}
		}
		len = getProtocol()->ptclBuildAskFormatInGameAsw((char *)userName.c_str(),vdiscreption,(unsigned char *)replyBlk->wr_ptr(),ret);	
		reply_msg(replyBlk,len);	// 回复心跳
	}

	//TODO
	return true;  // always connected
}

void Gateway_Input_Handler::reply_toOther(string usrName, ACE_Message_Block* block, int iLen)
{
	Client_Handler * pHandle = NULL;
	ClientInfo_Manager::instance()->get_client_handler(usrName,pHandle,EClientType_TERMINAL);
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("转发报文给 %s \n"), usrName));
	ACE_HEX_DUMP((LM_DEBUG,(const char*)(block->rd_ptr()), iLen));	
	block->wr_ptr(block->rd_ptr() + iLen);
    pHandle->get_ihandler()->send_message(block);
}

void Gateway_Input_Handler::reply_msg(ACE_Message_Block* block, int iLen)
{
	ACE_HEX_DUMP((LM_DEBUG,(const char*)(block->rd_ptr()), iLen));	
	block->wr_ptr(block->rd_ptr() + iLen);
	this->send_message(block);
}

//每次来获取一个报文，如果不构成一个报文则不进行任何操作
ACE_Message_Block * Gateway_Input_Handler::getOneReport(ACE_INT32& bufferlen,CRingBuffer * pRingBuffer,ACE_INT32& iErrorFalg)
{
	unsigned char iByte;
	ACE_INT32 iResult = RTN_UNKOW , iReportLen = 0; // flag为1表示数据错误
	iErrorFalg = RTN_UNKOW;

	if (m_ringBuffer->length() == 0)
	{
		return NULL;
	}

	while ((iResult = m_ringBuffer->readNextByte(iByte)) != -1 
		&& (iByte != 0x68 && iByte != 0x10 && iByte != 0xE5));

	pRingBuffer->m_readPtr--;

	iErrorFalg = m_vProtocol.parse(iReportLen,pRingBuffer); 
	
	if (iErrorFalg == RTN_SUCCESS)
	{
		ACE_Message_Block * pmb =  new ACE_Message_Block(iReportLen);
		pRingBuffer->readBuffer(pmb->wr_ptr(),iReportLen);
		pmb->wr_ptr( iReportLen);
		bufferlen = iReportLen;
		return pmb;
	}
	else if (iErrorFalg == RTN_ERROR) //只要有一个返回状态为RTN_TIME_OUT则置状态为timeout，除非成功
	{
		pRingBuffer->m_readPtr++;
	}

	return NULL;
}
