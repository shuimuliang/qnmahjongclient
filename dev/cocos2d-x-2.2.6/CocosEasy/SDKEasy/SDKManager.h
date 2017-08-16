#ifndef __CE_SDK_MANAGER_H__
#define __CE_SDK_MANAGER_H__

#include "Common/CocosEasyDef.h"
#include "SDKConstants.h"



typedef void (CCObject::*SEL_LoginCallBack)(bool);
#define LoginCallBack_selector(_SELECTOR) (SEL_LoginCallBack)(&_SELECTOR)
typedef void (CCObject::*SEL_ThirdLoginCallBack)(int,const char*);
#define ThirdLoginCallBack_selector(_SELECTOR) (SEL_ThirdLoginCallBack)(&_SELECTOR)
typedef void (CCObject::*SEL_BuyCallBack)(bool,const char*);
#define BuyCallBack_selector(_SELECTOR) (SEL_BuyCallBack)(&_SELECTOR)
typedef void (CCObject::*SEL_LogoutCallBack)();
#define LogoutCallBack_selector(_SELECTOR) (SEL_LogoutCallBack)(&_SELECTOR)
typedef void (CCObject::*SEL_ShareCallBack)(bool);
#define ShareCallBack_selector(_SELECTOR) (SEL_ShareCallBack)(&_SELECTOR)
//ad related
typedef void (CCObject::*SEL_VideoCallBack)(int);
#define VideoCallBack_selector(_SELECTOR) (SEL_VideoCallBack)(&_SELECTOR)

class CSDKManager : public CCObject
{
public:
	CSDKManager();
	~CSDKManager();
	static CSDKManager* getInstance();
	void end();

// implement interface for different platforms
public:
	void initPlatform(int code);
    void preLoadGoods(const char* goodsList);
    LanguageType getDesignLanguage(); //获取设计语言

    AppThirdLoginType getLoginType(); //获取登陆方式
    void login();
    void logout(bool iscallback = true);
    bool isLogined();
    bool isInited();
    bool hasUserCenter();
	void userCenter();
    
    //ad related
    void playVideo(const char* parmas = "");
    void createRoleAnayasis(unsigned int serverID);
    void enterMainSceneAnayasis(unsigned int serverID);

    void pay(const char* parmas);
    void share(const char* parmas);
    
    bool isWXAppInstalled();

    const std::string getMissOrderList();
    const std::string getOrderInfoAndRemove(const char* orderID);
    
    const std::string getAccountName();
    const std::string getAccountID();
    const std::string getSessionID();
    const std::string getMoneyName();
    const std::string getSimCountryISO();
    
    void showWebPayView(std::string url,
                        std::string jsonData);
    
    void payWithPayPal(const char* parmas);

    void thirdLogin(int loginType); // 第三方登陆
private:
	void unInit();
// needn't  multiple platform
public:
	

public:
	void registerLoginScriptHandler(int handler){m_LoginScriptHandler = handler;}
    void registerThirdLoginScriptHandler(int handler){m_ThirdLoginScriptHandler = handler;}
    void registerLogoutScriptHandler(int handler){m_LogoutScriptHandler = handler;}
	void registerBuyScriptHandler(int handler){m_BuyScriptHandler = handler;}
    void registerShareScriptHandler(int handler){m_ShareScriptHandler = handler;}
    
    void unRegisterThirdLoginScriptHandler(){m_ThirdLoginListener = NULL;}
    void registerVideoScriptHandler(int handler){m_VideoScriptHandler = handler;}
    
public:
	void loginCallBack(bool ret);
    void thirdLoginCallBack(int errorCode,const char* loginInfo);
	void buyCallBack(bool succ,const char* orderSerial);
	void logoutCallBack();
    void shareCallBack(bool ret);
    
    void videoCallBack(int status);
    void audioCallBack(int time, const char* parm);
    
private:
	CC_SYNTHESIZE(int, m_ChannelID, ChannelID);
	CC_SYNTHESIZE(void*, m_UserData, UserData);
    CC_SYNTHESIZE(std::string, m_devceToken, DeviceToken);
    CC_SYNTHESIZE(std::string, m_wchatState, WchatState);
    
	// login call 
	CC_SYNTHESIZE(int, m_LoginScriptHandler, LoginScriptHandler);
	CC_SYNTHESIZE(CCObject*, m_LoginListener, LoginListener);
	CC_SYNTHESIZE(SEL_LoginCallBack, m_LoginCallBack, LoginCallBack);
    // third login call
    CC_SYNTHESIZE(int, m_ThirdLoginScriptHandler, ThirdLoginScriptHandler);
    CC_SYNTHESIZE(CCObject*, m_ThirdLoginListener, ThirdLoginListener);
    CC_SYNTHESIZE(SEL_ThirdLoginCallBack, m_ThirdLoginCallBack, ThirdLoginCallBack);
    // buy good
	CC_SYNTHESIZE(int, m_BuyScriptHandler, BuyScriptHandler);
	CC_SYNTHESIZE(CCObject*, m_BuyListener, BuyListener);
	CC_SYNTHESIZE(SEL_BuyCallBack, m_BuyCallBack, BuyCallBack);
    //logout
	CC_SYNTHESIZE(int, m_LogoutScriptHandler, LogoutScriptHandler);
	CC_SYNTHESIZE(CCObject*, m_LogoutListener, LogoutListener);
	CC_SYNTHESIZE(SEL_LogoutCallBack, m_LogoutCallBack, LogoutCallBack);
    //share call
    CC_SYNTHESIZE(int, m_ShareScriptHandler, ShareScriptHandler);
    CC_SYNTHESIZE(CCObject*, m_ShareListener, ShareListener);
    CC_SYNTHESIZE(SEL_ShareCallBack, m_ShareCallBack, ShareCallBack);
    
    //playVideo
    CC_SYNTHESIZE(int, m_VideoScriptHandler, VideoScriptHandler);
    CC_SYNTHESIZE(CCObject*, m_VideoListener, VideoListener);
    CC_SYNTHESIZE(SEL_VideoCallBack, m_VideoCallBack, VideoCallBack);
    
private:
	static CSDKManager* m_Instance;
};

#endif

