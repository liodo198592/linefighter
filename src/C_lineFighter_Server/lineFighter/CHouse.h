#ifndef _CHouse_H_
#define _CHouse_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

//µ¥Àýhouse
class CHouse
{
protected:
	CHouse();
	virtual ~CHouse(){}
	
	
public:
	static CHouse * getInstance()
	{
		if (m_phourse == NULL)
		{
			m_phourse = new  CHouse();
		}
		return m_phourse;
	}
	static CHouse * m_phourse;
};








#endif