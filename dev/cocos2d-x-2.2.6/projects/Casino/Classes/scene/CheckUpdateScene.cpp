//
//  CheckUpdateScene.cpp
//  Casino
//
//  Created by liulihua on 16/4/13.
//
//

#include "CheckUpdateScene.h"
#include "Utility/Log.h"
#include "Config.h"
#include <math.h>
#include "Resource.h"
#include "Utility/Utils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "TDCCTalkingDataGA.h"
#endif

#define  WIN_SIZE   CCDirector::sharedDirector()->getWinSize()
string     SRC_UI_CCBI	= "logoScene.ccbi";
const    int        MIN_PERCENT	= 5;

string SRC_EFFECT_JSON	= "eff_laohujidenglu777.ExportJson";
string SRC_EFFECT_NAME	= "eff_laohujidenglu777";

string SRC_POKEEFFECT_JSON	= "pukepai01.ExportJson";
string SRC_POKEEFFECT_NAME	= "pukepai01";

cocos2d::CCScene* CheckUpdateScene::scene()
{
    CCScene *scene = CCScene::create();
    int channelID = CSDKManager::getInstance()->getChannelID();
    if (channelID == SDK_CHANNEL_APP_STORE_SLOTS || channelID == SDK_CHANNEL_SLOTS_ORIGIN_ANDROID || channelID == SDK_CHANNEL_APP_STORE_ENG
        || channelID == SDK_CHANNEL_GOOGLE_PLAY){
        SRC_UI_CCBI	= "logoScene_en.ccbi";
    }
    
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("CheckUpdateScene", CheckUpdateSceneLoader::loader());
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file. */
    // Load the scene from the ccbi-file, setting this class as
    // the owner will cause lblTestTitle to be set by the CCBReader.
    // lblTestTitle is in the TestHeader.ccbi, which is referenced
    // from each of the test scenes.
    CCNode * node = ccbReader->readNodeGraphFromFile(SRC_UI_CCBI.c_str());
    ccbReader->autorelease();
    
    if(!node) return NULL;
    scene-> addChild(node);
    return scene;
}


CheckUpdateScene::CheckUpdateScene()
: m_pVersion(NULL)
, m_pResource(NULL)
, mVersionLabelTTF(NULL)
, mLoadingLabelTTF(NULL)
, mProgressLayer(NULL)
, mProgressSprite(NULL)
, mBackgroundSprite(NULL)
, m_curPercent(MIN_PERCENT)
, m_LogoEffect(NULL)
, mLogoSprite(NULL)
,m_deltaPercent(0)
{}

bool CheckUpdateScene::init()
{
    //init variables
    m_curDownloadlstId = 1;
    m_pVersion = new CVersion;
    m_pResource = new CResource;
    #if COCOS2D_DEBUG > 0
        CELog::getInstance()->setLogLevel(eLogDebug);
    #endif
    //init UI here
    return true;
}
// ui related
void CheckUpdateScene::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
    this->mLoadingLabelTTF->setString(m_pResource->loading.c_str());
}

SEL_MenuHandler CheckUpdateScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CheckUpdateScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    return NULL;
}

bool CheckUpdateScene::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_progressLayer", CCLayer *, this->mProgressLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_progressSprite", CCSprite *, this->mProgressSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_loadingLabel", CCLabelTTF *, this->mLoadingLabelTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_versionLabel", CCLabelTTF *, this->mVersionLabelTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_background", CCSprite*, this->mBackgroundSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccb_logoSprite", CCSprite*, this->mLogoSprite);
    return false;
}

void CheckUpdateScene::ccb_buttonClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    
};

void CheckUpdateScene::ccb_updateButtonClicked(CCObject *pSender, CCControlEvent pCCControlEvent)
{
    DynamicDownloadExt::getInstance()->registDownLoadListener(this);
    m_curDownloadlstId = 0;
    m_pVersion->startDownloadById(m_curDownloadlstId);
};



CheckUpdateScene::~CheckUpdateScene()
{
    DynamicDownloadExt::getInstance()->unRegistDownLoadListener();
    CC_SAFE_DELETE(m_pVersion);
    CC_SAFE_DELETE(m_pResource);
    CC_SAFE_RELEASE(mLoadingLabelTTF);
    CC_SAFE_RELEASE(mVersionLabelTTF);
    CC_SAFE_RELEASE(mProgressLayer);
    CC_SAFE_RELEASE(mProgressSprite);
    CC_SAFE_RELEASE(mBackgroundSprite);
    CC_SAFE_RELEASE(mLogoSprite);
    CCLOG("clear CheckUpdateScene");
}

void CheckUpdateScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("CheckUpdateScene::onEnter");

    mVersionLabelTTF->setString(SHOW_VERSION.c_str());
    mVersionLabelTTF->enableShadow(CCSizeMake(1, -2), 1, 0);
    mLoadingLabelTTF->enableShadow(CCSizeMake(1, -2), 1, 0);
    
    setPercent(MIN_PERCENT);
    if(DynamicDownloadExt::getInstance()->isHasZipInDownloadDir())
    {
        bool succ = DynamicDownloadExt::getInstance()->uncompressDownloadzip();
        if(succ){
            CCDirector::sharedDirector()->reStart();
        }else{
            DynamicDownloadExt::getInstance()->cleanDownloadDir();
            showWaiting(m_pResource->waiting.c_str());
            send_verUpdate();
        }
        
    }else{
        showWaiting(m_pResource->waiting.c_str());
        send_verUpdate();
    }

	if (m_LogoEffect)
	{
		m_LogoEffect->removeFromParentAndCleanup(true);
		m_LogoEffect  = NULL;
	}
    int channelID = CSDKManager::getInstance()->getChannelID();
    if (channelID == SDK_CHANNEL_APP_STORE_SLOTS || channelID == SDK_CHANNEL_SLOTS_ORIGIN_ANDROID || channelID == SDK_CHANNEL_APP_STORE_ENG
        || channelID == SDK_CHANNEL_GOOGLE_PLAY){
      
        m_LogoEffect  = CArmatureExt::create(SRC_EFFECT_JSON.c_str(),SRC_EFFECT_NAME.c_str());
        this->addChild(m_LogoEffect);
        m_LogoEffect->play("play");
        float frameIdx = m_LogoEffect->getAnimation()->getAnimationData()->getMovement("play")->duration - 1;
        m_LogoEffect->pause(frameIdx);
        m_LogoEffect->setPosition(mLogoSprite->getPosition());
        mLogoSprite->setVisible(false);
//        m_LogoEffect->setCurAnimationComplete(this, AnimationEvent_selector(CheckUpdateScene::playEffectBack));
    }
	else if (channelID == SDK_CHANNEL_APP_STORE_ZJH || channelID == SDK_CHANNEL_ZJH_ORIGIN_ANDROID ){
		m_LogoEffect  = CArmatureExt::create(SRC_POKEEFFECT_JSON.c_str(),SRC_POKEEFFECT_NAME.c_str());
		mBackgroundSprite->addChild(m_LogoEffect);
		m_LogoEffect->play("1");
		m_LogoEffect->setPosition(ccp(560, 350));
	}
}

void CheckUpdateScene::playEffectBack( CArmatureExt* armature, MovementEventType type, const char * name)
{
    if (m_LogoEffect)
    {
        //m_LogoEffect->play("play1");
    }
}

void CheckUpdateScene::showWaiting(const char * text)
{
    mLoadingLabelTTF->setString(text);
}
void CheckUpdateScene::setPercent(float p){
//    CCLOG("----------%f", p);
    float runrunPos = WIN_SIZE.width*MIN(p,100)/100.f;
//    mProgressSprite->setPositionX(runrunPos);
    m_curPercent = MAX( p,0);
    m_curPercent = MIN( m_curPercent,100);
    
    mProgressSprite->setScaleX((WIN_SIZE.width/185) * m_curPercent/100);
}

void CheckUpdateScene::send_verUpdate()
{
    int channelID = CSDKManager::getInstance()->getChannelID();
    int targetPlatform = CC_TARGET_PLATFORM;
    string checkURL = VERSION_SERVER_IP;

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(checkURL.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(CheckUpdateScene::onHttpRequestCompleted));
    
    // write the post data
    char postData[1024] = {};
    sprintf(postData, "channel=%d&os=%d&version=%d", channelID, targetPlatform,INNER_VERSION);
    request->setRequestData(postData, (unsigned int)strlen(postData));
    request->setTag("verUpdate");
    CCHttpClient::getInstance()->send(request);
    request->release();
    
    CCLog("dynamic dowload path:\n%s", DynamicDownloadExt::getInstance()->getAbsoluteDownloadDir().c_str());
    CCLog("versino address:%s", checkURL.c_str());
    CCLog("channel=%d&os=%d&version=%d", channelID, targetPlatform,INNER_VERSION);
}

void CheckUpdateScene::retrySend(CCObject *obj){
    this->send_verUpdate();
}

void CheckUpdateScene::onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    CCHttpClient::getInstance()->destroyInstance();
    
    if (!response){
        CCLog("no  response");
        return;
    }
    
    // You can get original request type from: response->request->reqType
    const char* tag = response->getHttpRequest()->getTag();
    if (0 != strlen(tag)){
        CCLog("%s completed", tag);
    }else{
        tag = "";
    }
    int statusCode = response->getResponseCode();
    CCLog("res update http response code: %d", statusCode);
    if (!response->isSucceed()){
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        char buf[100];
        sprintf(buf, "%d", statusCode);
        string b = buf;
        EventParamMap paramMap;
        paramMap.insert(EventParamPair("errorcode",b));
        TDCCTalkingDataGA::onEvent("version_check",&paramMap);
#endif
        startGame();
        return;
    }
    
    // check is verUpdate
    if (strcmp(tag, "verUpdate") == 0){
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++){
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
        
        
        std::string bufffff(buffer->begin(),buffer->end());
        // update CVersion data
        m_pVersion->recv_verUpdate(bufffff.c_str(), (unsigned int)buffer->size());
        // check status
        switch(m_pVersion->getVersionStatus())
        {
            case optionalUpdate:
                showInstallUpdate(false);
                break;
                
            case forceUpdate:
                showInstallUpdate(true);
                break;
                
            case resUpdate:
                startResUpdate();
                break;
                
            default:
                startGame();
                break;
        }
        
    }
}
void CheckUpdateScene::startResUpdate()
{
    //check if url is empty
    if(!m_pVersion->lastPackInfo || m_pVersion->lastPackInfo->url.empty())
    {
        startGame();
        return;
    }
    
    DynamicDownloadExt::getInstance()->registDownLoadListener(this);
    m_curDownloadlstId = 0;
    m_pVersion->startDownloadById(m_curDownloadlstId);
}

void CheckUpdateScene::showInstallUpdate(bool isForce)
{
    //check if url is empty
    if(!m_pVersion->lastPackInfo || m_pVersion->lastPackInfo->url.empty())
    {
        startGame();
        return;
    }
    string title = m_pVersion->lastPackInfo->showVersion + m_pResource->installHint;
    string msg = m_pVersion->getShowContent();
    const char* button1Title = (m_pResource->installNow).c_str();
    const char* button2Title = (m_pResource->installLater).c_str();
    if(isForce) {   button2Title = NULL;}
    CCEPlugin::messageBox2(title.c_str(), msg.c_str(), button1Title, button2Title, this, callfuncO_selector(CheckUpdateScene::downloadNewVersion));
}

void CheckUpdateScene::downloadNewVersion(CCObject *obj){
    int index = ((CCInteger*)obj) -> getValue();
    EVersionStatus versionStatus = m_pVersion->getVersionStatus();
    if (index == 1){
        string url = "https://itunes.apple.com/app/id1009220598";
        if (m_pVersion && m_pVersion->lastPackInfo){
            url = m_pVersion->lastPackInfo->url;
        }
        CCEPlugin::openURL(url);
        if (versionStatus == forceUpdate)  {
            showInstallUpdate(true);
        }else{
            startGame();
        }
    }else if(index == 2){
        startGame();
    }
}

void CheckUpdateScene::updatePercent(){
    CCDelayTime *pDelayAction = CCDelayTime::create(.05f);
    CCCallFunc *pFunc = CCCallFunc::create(this,callfunc_selector(CheckUpdateScene::updatePercent));
    CCSequence *pSequence = CCSequence::createWithTwoActions(pDelayAction,pFunc);
    runAction(pSequence);
    setPercent(m_curPercent + m_deltaPercent);
}

void CheckUpdateScene::startGame()
{
    showWaiting(m_pResource->loading.c_str());
    m_deltaPercent = (100-m_curPercent)/30.f;  // modified for loading
    updatePercent();
    CCDelayTime *pDelayAction = CCDelayTime::create(2.0f);
    CCCallFunc *pFunc = CCCallFunc::create(this,callfunc_selector(CheckUpdateScene::handelStartGame));
    CCSequence *pSequence = CCSequence::createWithTwoActions(pDelayAction,pFunc);
    runAction(pSequence);
}

void CheckUpdateScene::handelStartGame()
{
     CCApplication::sharedApplication()->startGame();
}


void CheckUpdateScene::onStart(string downLoadPath)
{
    CEInfo("=============================start");
}

void CheckUpdateScene::onComplete(string downLoadPath)
{
    CEInfo("===========================complete");
    ++m_curDownloadlstId;
    //download next or download fail
    if (m_pVersion->startDownloadById(m_curDownloadlstId) == false)
    {
        
        /* {"versionlist":
         [{"url": "www.baidu.com/lua.zip",
         "showversion": "show v1.2.0",
         "des": "desc",
         "filesize": 321321,
         "numversion": 100100100}]}*/
        CCDirector::sharedDirector()->reStart();
    }
}

void CheckUpdateScene::onProcess(int number , string downLoadPath)
{
    CEInfo("%s:%dp",downLoadPath.c_str(),number);
    string strCurSize =m_pVersion->getStrSizeMBByIDAndPercent(m_curDownloadlstId,number);
    
    number = MIN(MIN_PERCENT+number, 100);
    char s[100];
    sprintf(s,"%s  %d%%" ,strCurSize.c_str(), number);
    mLoadingLabelTTF->setString(s);
    setPercent(number);
}

void CheckUpdateScene::onFail(string errors, std::string url)
{
    CEError("%s,%s",errors.c_str() , url.c_str());
    m_pVersion->reDownload();
}


///////// ****************  CVersion  ***********//////////////////////
CVersion::CVersion()
: m_eVersionStatus(skipUpdate),
IS_DEBUG(false),
m_fFullSize(0.0f),
lastPackInfo(NULL),
m_curId(0)
{
    packList = CCArray::create();
    packList->retain();
}

CVersion::~CVersion()
{
    packList->release();
    packList = NULL;
    lastPackInfo = NULL;
}
/*{
     url = "www.baidu.com/lua.zip",
     des = ,
     numversion = "100100100",
     showversion = "1.2.0",
     filesize = "10240",
     updatetype = 1,
     }*/
    
    /*
     url = "http://7u2l4h.com1.z0.glb.clouddn.com/patch2.zip";
     desc = "更新日志：1、新增大转盘玩法！！2、老虎机玩法更新！：3、新增大转盘玩法！！4、老虎机玩法更新！";
     showVersion = "0.8.4";
     numVersion = atoi("000008004");
     packSize = 4 * 1024;
     calVersionStatus(numVersion);
     m_fFullSize = getFullSize();
     */

void CVersion::recv_verUpdate(const char* pBytes, unsigned int size)
{
    if(!pBytes)
    {
        CEFatal("can not find config.json");
        return;
    }
    std::string load_str((const char*)pBytes, size);
    // parse
    rapidjson::Document doc;
    doc.Parse<0>(load_str.c_str());
    if(doc.HasParseError())
    {
        CEFatal("offset: %d error:%s", doc.GetErrorOffset(), doc.GetParseError());
        return;
    }
    packList->removeAllObjects();
    int count = DICTOOL->getArrayCount_json(doc, "versionlist");
    for (int i=0; i< count; i++) {
        CDownPackInfo *pa = new CDownPackInfo();
        const rapidjson::Value &packDic = DICTOOL->getDictionaryFromArray_json(doc, "versionlist", i);
        pa->initWithDictionary(packDic);
        packList->addObject(pa);
        if ( i == 0){
            lastPackInfo = pa;
        }
    }
    calVersionStatus(lastPackInfo);
    m_fFullSize = getFullSize();
}

EVersionStatus CVersion::getVersionStatus()
{
    return m_eVersionStatus;
}

void CVersion::calVersionStatus(CDownPackInfo *lastPackInfo)
{
    unsigned int numVersion = INNER_VERSION;
    EVersionStatus updateType = skipUpdate;
    if (lastPackInfo){
        numVersion = lastPackInfo->numVersion;
        updateType = lastPackInfo->updateType;
    }
    //calculate slice num
    int tgtFirst , tgtSecond , tgtThird ;
    int curFirst , curSecond , curThird;
    sliceVersionTo3(numVersion , tgtFirst , tgtSecond , tgtThird);
    sliceVersionTo3(INNER_VERSION , curFirst , curSecond , curThird);
    
    //show debug
    if(IS_DEBUG)
    {
        CCLog("server(%d) version: %d %d %d", numVersion ,tgtFirst, tgtSecond, tgtThird);
        CCLog("client(%d) version %d %d %d", INNER_VERSION , curFirst, curSecond, curThird);
    }
    
    m_eVersionStatus = skipUpdate;
    //get status
    if (updateType == resUpdate
        && tgtFirst == curFirst
        && tgtSecond == curSecond){
            m_eVersionStatus = updateType;
    }else if(updateType == optionalUpdate || updateType == forceUpdate){
        if(tgtFirst > curFirst || tgtSecond > curSecond ){
            m_eVersionStatus = updateType;
        }
    }
}

void CVersion::sliceVersionTo3(int version , int &first , int &second , int &third)
{
    third = version % 1000;
    version = floor(version / 1000);
    second = version % 1000;
    version = floor(version / 1000);
    first = version;
}

void CVersion::debug(const char *pChar)
{
    if(IS_DEBUG)
    {
        CCLog("%s", pChar);
    }
}

float CVersion::getFullSize()
{
    CDownPackInfo *pa = NULL;
    
    float result = 0.0f;
    int count = (int)packList->count();
    for (int i = 0; i< count; i++) {
        CDownPackInfo* pa = (CDownPackInfo *)packList->objectAtIndex(i);
        result += pa->size;
    }
    return result/(1024*1024);
}

string CVersion::getShowContent()
{
    string result = "error in getFirstContent";
    if(lastPackInfo != NULL){
        result = lastPackInfo->desc;
    }
    return result;
}

string CVersion::getStrSizeMBByIDAndPercent(unsigned int id , int percent)
{
    // check if id is useful
    if(id >= packList->count())
    {
        return "error";
    }
    CDownPackInfo* pa = (CDownPackInfo *)packList->objectAtIndex(id);
    // get last size
    float tempSize = pa->size * percent/100.f;
    
    // get sum of size before
    if(id>0){
        for(unsigned int i =0; i<id-1; ++i)
        {
            CDownPackInfo* tpa = (CDownPackInfo *)packList->objectAtIndex(i);
            tempSize += tpa->size;
        }
    }
    
    // transfer to string
    float showSize = tempSize/(1024*1024.f);
    showSize = showSize>0.01?showSize:0.01;
    char s[1000];
    sprintf(s,"%.2fmb/%.2fmb",showSize,m_fFullSize);
    return s;
}

bool CVersion::startDownloadById(unsigned int id){
    if(id >= packList->count())
    {
        return false;
    }
    
    CDownPackInfo *downInfo = (CDownPackInfo *)packList->objectAtIndex(id);
    m_curId = id;
    DynamicDownloadExt::getInstance()->startDownload(downInfo->url,true);
    return true;
}

void CVersion::reDownload()
{
    startDownloadById(m_curId);
}

////////// ****************  CResource  ***********//////////////////////
CResource::CResource()
: installHint("")
, waiting("")
, installNow("")
, installLater("")
, loading("")
, errorTips("")
, netError("")
, retry("")
{
    init();
    
    string installHint;
    string waiting;
    string installNow;
    string installLater;
    string loading;
    string errorTips;
    string netError;
    string retry;
}

void CResource::init()
{
    unsigned long size = 0;
    unsigned char* pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(RESOUCE_JSON.c_str(), "r", &size);
    
    if(!pBytes)
    {
        CEFatal("can not find resource.json");
        return;
    }
    
    std::string load_str((const char*)pBytes, size);
    CC_SAFE_DELETE_ARRAY(pBytes);
    // parse
    rapidjson::Document doc;
    doc.Parse<0>(load_str.c_str());
    
    if(doc.HasParseError())
    {
        CEFatal("offset: %d error:%s", doc.GetErrorOffset(), doc.GetParseError());
        return;
    }
    
    installHint = DICTOOL->getStringValue_json(doc, "installHint", "");
    waiting = DICTOOL->getStringValue_json(doc , "waiting" , "");
    installNow = DICTOOL->getStringValue_json(doc , "installNow" , "");
    installLater = DICTOOL->getStringValue_json(doc , "installLater" , "");
    loading = DICTOOL->getStringValue_json(doc , "loading" , "");
    errorTips = DICTOOL->getStringValue_json(doc , "errorTips" , "");
    netError = DICTOOL->getStringValue_json(doc , "netError" , "");
    retry = DICTOOL->getStringValue_json(doc , "retry" , "");
}


////////// ****************  CDownPackInfo  ***********//////////////////////
CDownPackInfo::CDownPackInfo()
: url(""),
desc(""),
showVersion(""),
numVersion(0),
size(0),
updateType(skipUpdate)
{
}

CDownPackInfo::~CDownPackInfo()
{
}

void CDownPackInfo::initWithDictionary(const rapidjson::Value& dic){
    url.assign( DICTOOL->getStringValue_json(dic, "url") );
    desc.assign(DICTOOL->getStringValue_json(dic, "des") );
    showVersion.assign(DICTOOL->getStringValue_json(dic, "showversion") );
    numVersion = DICTOOL->getIntValue_json(dic, "numversion");
    size = DICTOOL->getIntValue_json(dic, "filesize");
    updateType = (EVersionStatus)DICTOOL->getIntValue_json(dic, "updatetype");
}
