#ifndef CGameMaster_H
#define CGameMaster_H

#include "global.h"
#include "RingBuf.h"
#include "CProtocol.h"

class CGameMaster : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CGameMaster(){}//Ĭ�ϲ�������ʱ����
	virtual ~CGameMaster(){}
	ACE_INT64 run();
protected:
	ACE_INT32 svc();
private:

};

#endif