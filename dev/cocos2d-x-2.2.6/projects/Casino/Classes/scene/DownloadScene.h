#ifndef __DOWN_LOAD_SCENE_H__
#define __DOWN_LOAD_SCENE_H__

#include "CocosEasy.h"
#include "network/Proto.h"
#include "../CCPlatformMacros.h"

USING_NS_CC_EASY;
USING_NS_CC;

struct pbc_env;
class CDownPackInfo;
class CVersion;
class CResource;

enum EVersionStatus
{
	skipUpdate = 0, //not update
	resUpdate = 1,  //resource update
	installUpdate = 2,  //soft update,need installing.
};

class CDownloadScene : public CCLayer , public CCDownloadListener
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CDownloadScene, create);
	static cocos2d::CCScene* scene();
    ~CDownloadScene();
	virtual void onEnter();
public:
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    void ccb_buttonClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
    void ccb_updateButtonClicked(CCObject *pSender, CCControlEvent pCCControlEvent);
protected:
    
	//CCDownloadListener
	virtual void onStart(string downLoadPath);
	virtual void onComplete(string downLoadPath);
	virtual void onProcess(int number , string downLoadPath);
	virtual void onFail(string errors, std::string url);
protected:
	CDownloadScene();
	virtual bool init();
protected:
    void send_verUpdate();
    void retrySend(CCObject *obj);

    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
private:    
    cocos2d::CCLabelTTF * mLoadingLabelTTF;
    cocos2d::CCLabelTTF * mVersionLabelTTF;
	cocos2d::CCSprite * mBackgroundSprite;
	cocos2d::CCSprite * mLogoSprite;
    CCLayer * mProgressLayer;
    CCSprite * mProgressSprite;

    void showWaiting(const char *);
    void setPercent(float p);

    void renderATexture(CCSprite *s);
    
    void showInstallUpdate();
    void downloadNewVersion(CCObject *obj);
   
    void updatePercent();

    // ResUpdate
    void startResUpdate();
    
    //start game
    void startGame();
    void handelStartGame();

	// effect
	void playEffectBack(CArmatureExt*, MovementEventType, const char *);

	// current index of array of the download info
	int         m_curDownloadlstId;
    float         m_curPercent;
    float         m_deltaPercent;

	CVersion*   m_pVersion;
	CResource*		m_pResource;

	CArmatureExt* m_LogoEffect;
};

class CVersion
{
public:
	CVersion();
	~CVersion();
	//get data
	void recv_verUpdate(const char* pBytes, unsigned int size);
    void recv_verUpdate1(const char* pBytes, unsigned int size);

    
	// return version status,such as skip ,etc
	EVersionStatus getVersionStatus();


	// get show content
	string getShowContent();
	// 0.0/13.0
	string getStrSizeMBByIDAndPercent(unsigned int id , int percent);
	bool startDownloadById(unsigned int id);
    
	void reDownload();
    CDownPackInfo *lastPackInfo;

private:
	//status
	EVersionStatus m_eVersionStatus;
	// current download id in download list
	int m_curId;
	// controling whether to show log
	bool IS_DEBUG;
	// calculate the version status
	void calVersionStatus(int version);
	void sliceVersionTo3(int version , int &first , int &second , int &third);
	float getFullSize();
    
	void debug(const char *pChar);
    
    // proto ,saved from server
    CCArray *packList;
    //download size(mb)
    float m_fFullSize;
};

class CDownPackInfo:public CCObject
{
public:
    CDownPackInfo();
    ~CDownPackInfo();
    void initWithDictionary(const rapidjson::Value& dic);
    
public:
    string url;
    string desc;
    string showVersion;

    unsigned int numVersion;
    unsigned int size;
};

class CResource{
public:
	CResource();
	string installHint;
	string waiting;
    string installNow;
    string loading;
    string errorTips;
    string netError;
    string retry;

private:
	void init();
};


class CDownloadSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CDownloadSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CDownloadScene);
};


#endif