#include "Gateway_Acceptor.h"
#include "Gateway_Input_Handler.h"
#include "../logmonitor.h"


Gateway_Input_Handler* Gateway_Acceptor::make_handler (void) {
  Gateway_Input_Handler *ih;
 
  ACE_NEW_RETURN (ih, Gateway_Input_Handler, 0);

  return ih;
}

int Gateway_Acceptor::validate_connection(const ACE_Asynch_Accept::Result& result,
										 const ACE_INET_Addr &remote,
										 const ACE_INET_Addr &local)
{
	char addr[100];
	remote.addr_to_string(addr,100);
	ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("establish a terminal , addr&port:%s\n"), addr));
	return 0;
}