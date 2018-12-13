#include "Client_Handler.h"
#include "Gateway_Input_Handler.h"


Client_Handler::~Client_Handler(){

	if(this->ihandler != NULL)
	{
		this->ihandler->close();
		this->ihandler = NULL;
	}
}

void Client_Handler::handle_time_out(const ACE_Time_Value &now, const void *act)
{//不需要超十注销信息

}

void Client_Handler::update_heart_beat()
{
	this->time_of_last_heartbeat = ACE_OS::gettimeofday();
}

Gateway_Input_Handler* Client_Handler::get_ihandler()
{
	return this->ihandler;
}

ACE_Time_Value& Client_Handler::get_max_timeout_value()
{
	return this->max_timeout_value;
}