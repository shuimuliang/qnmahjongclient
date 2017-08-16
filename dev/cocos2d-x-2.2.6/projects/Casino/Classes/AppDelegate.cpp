#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "Lua_extensions_CCB.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Lua_web_socket.h"
#endif

// libs
#include "pbc/pbc-lua.h"
#include "CocosEasy.h"
#include "Utility/Utils.h"
#include "lualoadexts.h"
// game
#include "Config.h"
//#include "network/Network.h"
//#include "scene/CheckUpdateScene.h"

//luaProxy
#include "tolua/luaopen_LuaProxy.h"
#include "Key.h"

#include <support/zip_support/ZipUtils.h>

//bugly
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "bugly/CrashReport.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "bugly/CrashReport.h"
#include "bugly/lua/BuglyLuaAgent.h"
#endif

//#include "CCBTestScene.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{

    
    int n = sizeof(void*);
    CCLog("%d",n);
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *pEglView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEglView);
    
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    pDirector->setProjection(kCCDirectorProjection2D);

	// set resolution
    CCSize designSize = CCSizeMake(1136, 640);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
//set search path
    std::vector<std::string> searchPaths;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    searchPaths.push_back("/storage/emulated/0");
    searchPaths.push_back("/storage/emulated/0/fuiou_wmp/temp/");
#endif
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    //texture2D related
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    //encryption key : 864f8e9b 4f470c7a 36a4893e ef373bb4
    ZipUtils::ccSetPvrEncryptionKeyPart(0, 0x864f8e9b);
    ZipUtils::ccSetPvrEncryptionKeyPart(1, 0x4f470c7a);
    ZipUtils::ccSetPvrEncryptionKeyPart(2, 0x36a4893e);
    ZipUtils::ccSetPvrEncryptionKeyPart(3, 0xef373bb4);
    //zjh key : 95d447ec df0e1da2 535a884e 6ed3e204
    // load shaders
    loadCocosEasyShaders();
    
    // read config
    readConfig();
    
	// start socket for wp and win32
	i_socket_open();
    
    startGame();
	// start download scene
//
 //   CCScene* pScene = CheckUpdateScene::scene();
//	CSceneTransitMgr::getInstance()->runWithScene(pScene);
    
//    CCScene* pScene = CCBTestScene::scene();
//    CSceneTransitMgr::getInstance()->runWithScene(pScene);
        
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

//    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CEventManager::getInstance()->sendEvent(CE_EVENT_APP_HOME);
#if CASINO_VOICE
    CIMManager::getInstance()->OnPause();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    CEventManager::getInstance()->sendEvent(CE_EVENT_APP_BACK);
#if CASINO_VOICE
    CIMManager::getInstance()->OnResume();
#endif

//    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}


void AppDelegate::startGame()
{
	// close c++ net manager
    
	// register lua engine
	CCELuaEngine* pEngine = CCELuaEngine::defaultEngine();
	CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
	CCLuaStack *pStack = pEngine->getLuaStack();
	lua_State *tolua_s = pStack->getLuaState();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
	CCFileUtils::sharedFileUtils()->addSearchPath("lua");
#endif
//	 init lua libs
    luax_loadexts(tolua_s);
    luaopen_LuaProxy(tolua_s);
	tolua_extensions_ccb_open(tolua_s);
	luaopen_protobuf_c(tolua_s);
	tolua_CocosEasy_open(tolua_s);
    tolua_web_socket_open(tolua_s);
	register_all_CocosEasy_manual(tolua_s);
    pStack->setXXTEAKeyAndSign(KEY_LUA, strlen(KEY_LUA), ENCRYPT_FILE_HEADER, strlen(ENCRYPT_FILE_HEADER));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CrashReport::initCrashReport(zjhBuglyAndroidID, false);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CrashReport::initCrashReport("f184ab45cf", false);
    BuglyLuaAgent::registerLuaExceptionHandler(pEngine);
#endif

	// enter lua
    pEngine->executeString("require \"lua/AppDelegate.lua\"");
#if CASINO_VOICE
    CIMManager::getInstance()->init();
#endif
    CCLog("latitude:%lf; longitude:%lf", CCEPlugin::getLatitude(), CCEPlugin::getLongitude());
}

void AppDelegate::restart()
{
	doBeforeDestruct();
	applicationDidFinishLaunching();
}

void AppDelegate::doBeforeDestruct()
{
    CCHttpClient::getInstance()->destroyInstance();
	CCocosEasyManager::getInstance()->end();
	SimpleAudioEngine::end();
}

void AppDelegate::checkVersion()
{
    
}
