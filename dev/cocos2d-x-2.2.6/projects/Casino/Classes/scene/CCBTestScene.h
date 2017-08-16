//
//  CCBTestScene.h
//  Casino
//
//  Created by liulihua on 15/3/25.
//
//

#ifndef __Casino__CCBTestScene__
#define __Casino__CCBTestScene__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;


class CCBTestScene : public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CCBTestScene, create); //如果此处提示GameScene有错误，要么是下面的虚函数没有重写完，要么是重写的方法中参数有误（例如：Allocating an object of abstract class type 'GameScene'；)，当然，这个方法也是可以去掉的，直接用CREATE_FUNC(GameScene)能达到相同的效果。
    
    static cocos2d::CCScene* scene();
    
    CCBTestScene();
    virtual ~CCBTestScene();

    virtual void onEnter();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    
    void ccb_buttonClicked1(CCObject *pSender, CCControlEvent pCCControlEvent);
    
//    // HTML events
//    void onHTMLClicked(
//                       IRichNode* root, IRichElement* ele, int _id);
//    void onHTMLMoved(
//                     IRichNode* root, IRichElement* ele, int _id,
//                     const CCPoint& location, const CCPoint& delta);
    
private:
    cocos2d::CCLabelTTF * mTestTitleLabelTTF;

};

class CCBTestSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBTestSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCBTestScene);
};




#endif /* defined(__Casino__CCBTestScene__) */
