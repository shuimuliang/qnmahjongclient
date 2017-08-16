//
//  CCBTestScene.cpp
//  Casino
//
//  Created by liulihua on 15/3/25.
//
//

#include "CCBTestScene.h"

//static CCHTMLLabel* s_htmlLabel = NULL;
std::string tt;

CCScene* CCBTestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    CCPoint origin = pEGLView->getVisibleOrigin();
    CCSize size = pEGLView->getVisibleSize();
    
    
    
    
    // 'layer' is an autorelease object
    CCBTestScene *layer = CCBTestScene::create();
    
    
//    /* Create an autorelease CCNodeLoaderLibrary. */
//    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
//    ccNodeLoaderLibrary->registerCCNodeLoader("CCBTestScene", CCBTestSceneLoader::loader());
//    
//    /* Create an autorelease CCBReader. */
//    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
//    
//    /* Read a ccbi file. */
//    // Load the scene from the ccbi-file, setting this class as
//    // the owner will cause lblTestTitle to be set by the CCBReader.
//    // lblTestTitle is in the TestHeader.ccbi, which is referenced
//    // from each of the test scenes.
//    CCNode * node = ccbReader->readNodeGraphFromFile("logoScene.ccbi");
//    ccbReader->autorelease();
//    
//    
//    
//    if(node != NULL) {
//       scene-> addChild(node);
//    }

    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


CCBTestScene::CCBTestScene(){

}

CCBTestScene::~CCBTestScene(){

}

void CCBTestScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("%s","CCBTestScene onEnter ");

//    using namespace dfont;
// 
//    // font1
//    FontCatalog* font_catalog = NULL;
//    font_catalog = FontFactory::instance()->create_font(
//                                                        "font1", "fonts/MicrosoftYaHeiGB.ttf", 0xffffffff, 32, e_plain, 0.0f, 0xffffffff, 0);
//    // font2
//    font_catalog = FontFactory::instance()->create_font(
//                                                        "font2", "fonts/MicrosoftYaHeiGB.ttf", 0xffffffff, 24, e_shadow, 1.0f, 0xff000000, 0);
//    font_catalog->add_hackfont("fonts/MicrosoftYaHeiGB.ttf", latin_charset(), 3);
//    // font3
//    font_catalog = FontFactory::instance()->create_font(
//                                                        "font3", "fonts/MicrosoftYaHeiGB.ttf", 0xffffffff, 20, e_border, 1.0f, 0xff000000, 0);
//    font_catalog->add_hackfont("fonts/MicrosoftYaHeiGB.ttf", latin_charset(), 5);
//
//
//    
//    //////////////////////////////////////////////////////////////////////////
//    
//    
//    CCSize vsize = CCDirector::sharedDirector()->getVisibleSize();
//    CCString* str_utf8 = CCString::createWithContentsOfFile("html.htm");
//    
//    CCHTMLLabel* htmllabel = CCHTMLLabel::createWithString(str_utf8->getCString(),
//                                                           CCSize(vsize.width*0.8f, vsize.height), "font2");
//    htmllabel->setAnchorPoint(ccp(0.5f,0.5f));
//    htmllabel->setPosition(ccp(vsize.width*0.5f, vsize.height*0.5f));
//    
//    addChild(htmllabel);
//    
//    s_htmlLabel = htmllabel;
//    
//    htmllabel->registerListener(this, &CCBTestScene::onHTMLClicked, &CCBTestScene::onHTMLMoved );
    
//    FontFactory::instance()->dump_textures();
    
    
}

//void CCBTestScene::onHTMLClicked(
//                             IRichNode* root, IRichElement* ele, int _id)
//{
//    CCLog("[On Clicked] id=%d", _id);
//    
//    if ( !s_htmlLabel )
//    {
//        return;
//    }
//    else if ( _id == 1002 ) // close
//    {
//        s_htmlLabel->setVisible(false);
//    }
//    else if ( _id == 2000 ) //reload
//    {
//        CCString* str_utf8 = CCString::createWithContentsOfFile("html.htm");
//        s_htmlLabel->setString(str_utf8->getCString());
//    }
//}
//
//void CCBTestScene::onHTMLMoved(
//                           IRichNode* root, IRichElement* ele, int _id,
//                           const CCPoint& location, const CCPoint& delta)
//{
//    CCLog("[On Moved] id=%d", _id);
//    
//    if ( !s_htmlLabel )
//    {
//        return;
//    }
//    else if ( _id == 1001 )
//    {
//        s_htmlLabel->setPosition(ccpAdd(delta, s_htmlLabel->getPosition()));
//    }
//}


void CCBTestScene::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
    CCLOG("here4");

    //    CCRotateBy * ccRotateBy = CCRotateBy::create(20.0f, 360);
    //    CCRepeatForever * ccRepeatForever = CCRepeatForever::create(ccRotateBy);
    //    this->mBurstSprite->runAction(ccRepeatForever);
     this->mTestTitleLabelTTF->setString("Hello world");
    
}

SEL_MenuHandler CCBTestScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    CCLOG("here3");

    return NULL;
}

SEL_CCControlHandler CCBTestScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    CCLOG("here1");

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "ccb_buttonClicked", CCBTestScene::ccb_buttonClicked1);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpriteTestClicked", HelloCocosBuilderLayer::onSpriteTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onButtonTestClicked", HelloCocosBuilderLayer::onButtonTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAnimationsTestClicked", HelloCocosBuilderLayer::onAnimationsTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onParticleSystemTestClicked", HelloCocosBuilderLayer::onParticleSystemTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onScrollViewTestClicked", HelloCocosBuilderLayer::onScrollViewTestClicked);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTimelineCallbackSoundClicked", HelloCocosBuilderLayer::onTimelineCallbackSoundClicked);
    
    return NULL;
}

bool CCBTestScene::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBurstSprite", CCSprite *, this->mBurstSprite);
    
    CCLOG("here2");
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_loadingLabel", CCLabelTTF *, this->mTestTitleLabelTTF);
    
    return false;
}

void CCBTestScene::ccb_buttonClicked1(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("button clicked");
};


