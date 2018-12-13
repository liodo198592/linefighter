#ifndef _GAMING_WIDGET_H_
#define _GAMING_WIDGET_H_
#include "global.h"

class CGamingWidget : public QWidget
{
	Q_OBJECT
public:
	CGamingWidget(QWidget* parent = 0):QWidget(parent)
	{

	}
	~CGamingWidget(void){}


public:signals:
	void changeStatus(int);
};

#endif