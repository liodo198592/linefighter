#include "PlayerFactory.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

extern map<int,SPlace> g_mapCoordinate;

CPlayerFactory::CPlayerFactory(void):m_iPlayerNumber(0)
{
}

CPlayerFactory::~CPlayerFactory(void)
{
}

