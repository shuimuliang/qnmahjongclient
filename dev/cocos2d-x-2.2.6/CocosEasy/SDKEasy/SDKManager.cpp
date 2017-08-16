#include "SDKManager.h"


CSDKManager* CSDKManager::m_Instance = NULL;

CSDKManager::CSDKManager()
: m_ChannelID(SDK_CHANNEL_NONE)
, m_LoginScriptHandler(0)
, m_wchatState("")
, m_LoginListener(NULL)
, m_LogoutListener(NULL)
, m_LoginCallBack(NULL)
, m_LogoutCallBack(NULL)
,m_BuyScriptHandler(0)
,m_BuyListener(NULL)
,m_BuyCallBack(NULL)
,m_ThirdLoginScriptHandler(0)
,m_ThirdLoginListener(NULL)
,m_ThirdLoginCallBack(NULL)
,m_ShareScriptHandler(0)
,m_ShareListener(NULL)
,m_ShareCallBack(NULL)
, m_UserData(NULL)
, m_VideoListener(NULL)
, m_VideoCallBack(NULL)
,m_VideoScriptHandler(0)
,m_LogoutScriptHandler(0)
{
    // init code here
#ifdef WIN32
	m_ChannelID = SDK_CHANNEL_WP_WFK;
#endif
    
//        CCLog("hk ------------------------------  init ");
//        IGCloudVoiceEngine * en = gcloud_voice::GetVoiceEngine();
//        GCloudVoiceErrno error1 = en->SetAppInfo("1850970758","f6ef511cd4a2ed1cf41a69f29ce8d4bc","dwwfewqafdsafdwdf");
//        GCloudVoiceErrno error2 = en->Init();
//        GCloudVoiceErrno error3 = en->SetMode(gcloud_voice::IGCloudVoiceEngine::Messages);
////        GCloudVoiceErrno error4 = gcloud_voice::GetVoiceEngine()->SetNotify(this);
//        GCloudVoiceErrno error5 = en->ApplyMessageKey (6000);
}

CSDKManager::~CSDKManager()
{
	unInit();
}

CSDKManager* CSDKManager::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CSDKManager();
        m_Instance->initPlatform(0);
	}
	return m_Instance;
}

void CSDKManager::end()
{
    this->logout(false);
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}

bool CSDKManager::hasUserCenter(){
    if (m_ChannelID == SDK_CHANNEL_APP_STORE ||
        m_ChannelID == SDK_CHANNEL_GOOGLE_PLAY){
        return false;
    }
    return true;
}


void CSDKManager::loginCallBack( bool ret )
{

	// send c event
	if(m_LoginListener && m_LoginCallBack){
        (m_LoginListener->*m_LoginCallBack)(ret);
    }

	// send lua event
	if(m_LoginScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(1);
		pArrayArgs->addObject(CCBool::create(ret));
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_LoginScriptHandler,pArrayArgs);
	}
}

void CSDKManager::buyCallBack(bool succ, const char * orderSerial){
    // send c event
	if(m_BuyListener && m_BuyCallBack){
        (m_BuyListener->*m_BuyCallBack)(succ,orderSerial);
    }
    
	// send lua event
	if(m_BuyScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(2);
		pArrayArgs->addObject(CCBool::create(succ));
        pArrayArgs->addObject(CCString::create(orderSerial));
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_BuyScriptHandler,pArrayArgs);
	}
}

void CSDKManager::thirdLoginCallBack(int errorCode,const char* loginInfo){
    // send c event
    if(m_ThirdLoginListener && m_ThirdLoginCallBack){
        (m_ThirdLoginListener->*m_ThirdLoginCallBack)(errorCode,loginInfo);
    }
    
    // send lua event
    if(m_ThirdLoginScriptHandler)
    {
        CCArray* pArrayArgs = CCArray::createWithCapacity(2);
        pArrayArgs->addObject(CCInteger::create(errorCode));
        pArrayArgs->addObject(CCString::create(loginInfo));
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_ThirdLoginScriptHandler,pArrayArgs);
    }
}

void CSDKManager::logoutCallBack()
{
	// send c event
	if(m_LogoutListener && m_LogoutCallBack){
        (m_LogoutListener->*m_LogoutCallBack)();
    }
    
	// send lua event
	if(m_LogoutScriptHandler)
	{
		CCArray* pArrayArgs = CCArray::createWithCapacity(0);
		CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_LogoutScriptHandler,pArrayArgs);
	}
}

void CSDKManager::videoCallBack( int status )
{
    // send c event
    if(m_VideoListener && m_VideoCallBack){
        (m_VideoListener->*m_VideoCallBack)(status);
    }
    
    // send lua event
    if(m_VideoScriptHandler)
    {
        CCArray* pArrayArgs = CCArray::createWithCapacity(1);
        pArrayArgs->addObject(CCInteger::create(status));
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_VideoScriptHandler,pArrayArgs);
    }
}


void CSDKManager::shareCallBack( bool ret )
{
    
    // send c event
    if(m_ShareListener && m_ShareCallBack){
        (m_ShareListener->*m_ShareCallBack)(ret);
    }
    
    // send lua event
    if(m_ShareScriptHandler)
    {
        CCArray* pArrayArgs = CCArray::createWithCapacity(1);
        pArrayArgs->addObject(CCBool::create(ret));
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_ShareScriptHandler,pArrayArgs);
    }
}

