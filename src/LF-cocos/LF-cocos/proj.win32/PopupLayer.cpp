#include "PopupLayer.h"

//void PopupLayer::popupLayer(){
//	// ����һ�������㣬����һ�ű���ͼ
//	PopupLayer* pl = PopupLayer::create("popuplayer/BackGround.png");
//	// ContentSize �ǿ�ѡ�����ã����Բ����ã�������ð������� 9 ͼ����
//	pl->setContentSize(CCSizeMake(400, 350));
//	pl->setTitle("����һҶ");
//	pl->setContentText("������÷�����ͣ�ȴ���ķҰ���ء������ٻ������ޣ�������ӣһ֦�㡣", 20, 60, 250);
//	// ���ûص��������ص�����һ�� CCNode �Ի�ȡ tag �жϵ���İ�ť
//	// ��ֻ����Ϊһ�ַ�װʵ�֣����ʹ�� delegate �Ǿ��ܹ������Ŀ��Ʋ�����
//	pl->setCallbackFunc(this, callfuncN_selector(Popup::buttonCallback));
//	// ��Ӱ�ť������ͼƬ�����֣�tag ��Ϣ
//	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "ȷ��", 0);
//	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "ȡ��", 1);
//	// ��ӵ���ǰ��
//	this->addChild(pl);
//}
//
//void PopupLayer::buttonCallback(cocos2d::CCNode *pNode){
//	// ��ӡ tag 0�� ȷ����1 ��ȡ��
//	CCLog("button call back. tag: %d", pNode->getTag());
//}
//
//// ����Ч�� �ؼ����룬��ǰ��ֱִ���������
//CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
//										  CCScaleTo::create(0.06, 1.05),
//										  CCScaleTo::create(0.08, 0.95),
//										  CCScaleTo::create(0.08, 1.0), NULL);