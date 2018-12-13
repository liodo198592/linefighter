#ifndef __PARABOLATO_H__
#define __PARABOLATO_H__
#include "cocos2d.h"

USING_NS_CC;
// �������˶�
class ParabolaTo
{
	public:    /*����һ�������߶��� 
	������     t                  
	ʱ��      startPoint      
	��ʼ��     endPoint        
	������     height          
	�߶ȣ�Ӱ�������ߵĸ߶ȣ�        
	angle           �Ƕȣ������������������Ƶ���y��ļнǣ�ֱ��Ӱ�쾫����׳��Ƕȣ�    */  
	static CCEaseInOut* create(float t, CCPoint startPoint, CCPoint endPoint, float height = 0, float angle = 60);
};
#endif // !__PARABOLATO_H__  