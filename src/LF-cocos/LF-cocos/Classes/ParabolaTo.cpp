#include "ParabolaTo.h"

CCEaseInOut* ParabolaTo::create(float t, 
								CCPoint startPoint, 
								CCPoint endPoint, 
								float height /* = 0 */, 
								float angle /* = 60 */)
{  
	// �ѽǶ�ת��Ϊ���� 
	float radian = angle*3.14159/180.0; 
	// ��һ�����Ƶ�Ϊ��������뻡���е� 
	float q1x = startPoint.x+(endPoint.x - startPoint.x)/4.0;   
	CCPoint q1 = ccp(q1x, height + startPoint.y+cos(radian)*q1x);           
	// �ڶ������Ƶ�Ϊ���������ߵ��е�  
	float q2x = startPoint.x + (endPoint.x - startPoint.x)/2.0; 
	CCPoint q2 = ccp(q2x, height + startPoint.y+cos(radian)*q2x);       
	//��������  
	ccBezierConfig cfg; cfg.controlPoint_1 = q1;    
	cfg.controlPoint_2 = q2;    
	cfg.endPosition = endPoint; 
	//ʹ��CCEaseInOut�������˶���һ����������ı仯���Եø���Ȼ    
	return CCEaseInOut::create(CCBezierTo::create(t,cfg),0.5); 
}  





