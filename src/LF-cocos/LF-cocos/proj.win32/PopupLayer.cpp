#include "PopupLayer.h"

//void PopupLayer::popupLayer(){
//	// 定义一个弹出层，传入一张背景图
//	PopupLayer* pl = PopupLayer::create("popuplayer/BackGround.png");
//	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
//	pl->setContentSize(CCSizeMake(400, 350));
//	pl->setTitle("吾名一叶");
//	pl->setContentText("娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香。", 20, 60, 250);
//	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
//	// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
//	pl->setCallbackFunc(this, callfuncN_selector(Popup::buttonCallback));
//	// 添加按钮，设置图片，文字，tag 信息
//	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "确定", 0);
//	pl->addButton("popuplayer/pop_button.png", "popuplayer/pop_button.png", "取消", 1);
//	// 添加到当前层
//	this->addChild(pl);
//}
//
//void PopupLayer::buttonCallback(cocos2d::CCNode *pNode){
//	// 打印 tag 0， 确定，1 ，取消
//	CCLog("button call back. tag: %d", pNode->getTag());
//}
//
//// 弹出效果 关键代码，当前层直执行这个动作
//CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
//										  CCScaleTo::create(0.06, 1.05),
//										  CCScaleTo::create(0.08, 0.95),
//										  CCScaleTo::create(0.08, 1.0), NULL);