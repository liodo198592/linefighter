#ifndef _GATEWAY_ACCEPTOR_H
#define _GATEWAY_ACCEPTOR_H

#include "ace/Asynch_Acceptor.h"
#include "ace/Unbounded_Set.h"


class Event_Channel;

class Gateway_Input_Handler;

class Gateway_Acceptor
  : public ACE_Asynch_Acceptor<Gateway_Input_Handler> {
public:
  Gateway_Acceptor(){}
  virtual int validate_connection
    (const ACE_Asynch_Accept::Result& result,
     const ACE_INET_Addr &remote,
     const ACE_INET_Addr &local);
protected:
  // Service handler factory method.
  virtual Gateway_Input_Handler *make_handler (void);

};

#endif
