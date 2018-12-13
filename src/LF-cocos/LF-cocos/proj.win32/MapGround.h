#ifndef _MAP_GROUND_H_
#define _MAP_GROUND_H_
#include "AbstractMap.h"

class CMapGround : public CAbstractMap
{
public:
	CMapGround(void);
	~CMapGround(void);

	virtual void initCoordinate();
};

#endif
