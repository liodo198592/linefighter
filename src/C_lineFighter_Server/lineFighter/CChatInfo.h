#ifndef _CChatInfo_H_
#define _CChatInfo_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

//µ¥Àýhouse
class CChatInfo
{
protected:
	string senderName;
	string content;
    SDate date;
public:
	CChatInfo(){}
	void operator = (CChatInfo cChatInfo)
	{
		this->senderName = cChatInfo.senderName;
		this->content = cChatInfo.content;
		this->date = cChatInfo.date;
	}
	CChatInfo(const CChatInfo &cChatInfo)
	{
		this->senderName = cChatInfo.senderName;
		this->content = cChatInfo.content;
		this->date = cChatInfo.date;
	}
	virtual ~CChatInfo(){}
	virtual string getsenderName()
	{
		return senderName;
	}
	virtual void setsenderNamee(string senderName_x)
	{
		senderName = senderName_x;
	}

	virtual string getcontent()
	{
		return content;
	}
	virtual void setcontent(string content_x)
	{
		content = content_x;
	}
	
	virtual SDate gettime()
	{
		return date;
	}
	virtual void settime(SDate date_x)
	{
		date =  date_x;
	}
};

#endif