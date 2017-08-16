//
//  LoadingLayer.h
//  Casino
//
//  Created by liulihua on 15/3/25.
//
//

#ifndef __Casino__LoadingLayer__
#define __Casino__LoadingLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;


class LoadingLayer : public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
    
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LoadingLayer, create);
    
    static cocos2d::CCScene* scene();
    
    LoadingLayer();
    virtual ~LoadingLayer();
    
    virtual void onEnter();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    
    void ccb_buttonClicked1(CCObject *pSender, CCControlEvent pCCControlEvent);
    
private:
    cocos2d::CCLabelTTF * mTestTitleLabelTTF;
    
    
};



#endif /* defined(__Casino__LoadingLayer__) */
