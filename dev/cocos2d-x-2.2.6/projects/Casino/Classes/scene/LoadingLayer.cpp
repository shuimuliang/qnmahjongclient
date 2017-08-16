//
//  LoadingLayer.cpp
//  Casino
//
//  Created by liulihua on 15/3/25.
//
//

#include "LoadingLayer.h"

LoadingLayer::LoadingLayer(){
    
}

LoadingLayer::~LoadingLayer(){
    
}

void LoadingLayer::onEnter()
{
    CCLayer::onEnter();
    CCLOG("%s","LoadingLayer onEnter ");
}
void LoadingLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
    //    CCRotateBy * ccRotateBy = CCRotateBy::create(20.0f, 360);
    //    CCRepeatForever * ccRepeatForever = CCRepeatForever::create(ccRotateBy);
    //    this->mBurstSprite->runAction(ccRepeatForever);
    CCLog(" LoadingLayer::onNodeLoaded");
    
    //this->mTestTitleLabelTTF->setString("Hello world");
    
}

SEL_MenuHandler LoadingLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

SEL_CCControlHandler LoadingLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "ccb_buttonClicked", CCBTestScene::ccb_buttonClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpriteTestClicked", HelloCocosBuilderLayer::onSpriteTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onButtonTestClicked", HelloCocosBuilderLayer::onButtonTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAnimationsTestClicked", HelloCocosBuilderLayer::onAnimationsTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onParticleSystemTestClicked", HelloCocosBuilderLayer::onParticleSystemTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onScrollViewTestClicked", HelloCocosBuilderLayer::onScrollViewTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTimelineCallbackSoundClicked", HelloCocosBuilderLayer::onTimelineCallbackSoundClicked);
    
    return NULL;
}

bool LoadingLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBurstSprite", CCSprite *, this->mBurstSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_loadingLabel", CCLabelTTF *, this->mTestTitleLabelTTF);
    
    return false;
}

void LoadingLayer::ccb_buttonClicked1(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("button clicked");
};

