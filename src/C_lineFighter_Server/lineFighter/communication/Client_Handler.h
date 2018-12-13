#ifndef _CLIENT_HANDLER_H
#define _CLIENT_HANDLER_H

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Proactor.h"
#include "ace/Asynch_IO.h"
#include "Config.h"

class Gateway_Input_Handler;

class Client_Handler: public ACE_Service_Handler{
	
public:
	Client_Handler(){}
	Client_Handler(Gateway_Input_Handler* handler):ihandler(handler),m_iIsValid(true){
		max_timeout_value = ACE_Time_Value(HEART_BEAT_INTERVAL);
	};
	//Client_Handler(ACE_TString client_id_):client_id(client_id_){}
	virtual ~Client_Handler();

	virtual void handle_time_out(const ACE_Time_Value &now, const void* act);

	void update_heart_beat();

	ACE_Time_Value& get_max_timeout_value();
	bool getValid(){return m_iIsValid;}
	void setValid(bool bvalid){m_iIsValid = bvalid;}

	Gateway_Input_Handler* get_ihandler();
private:
	ACE_Time_Value time_of_login;			// 本次登录时间
	ACE_Time_Value time_of_last_heartbeat;	// 最后一次心跳时刻
	ACE_Time_Value max_timeout_value;		// 最大超时时间
	Gateway_Input_Handler* ihandler;
	
	bool  m_iIsValid;
};


#endif