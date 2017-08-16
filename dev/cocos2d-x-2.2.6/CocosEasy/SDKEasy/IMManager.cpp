//#if CASINO_VOICE

#include "IMManager.h"
#import "CocosEasyPlugin.h"


CIMManager* CIMManager::m_Instance = NULL;

CIMManager::CIMManager()
:m_uploadCallBack(NULL)
,m_uploadListener(NULL)
,m_uploadScriptHandler(0)
,m_downloadCallBack(NULL)
,m_downloadListener(NULL)
,m_downloadScriptHandler(0)
{
    m_uploadpath = CCFileUtils::sharedFileUtils()->getWritablePath() + "upload.mp3";
    m_downloadpath = CCFileUtils::sharedFileUtils()->getWritablePath() + "download.mp3";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string SDCARD_PATH = "/mnt/sdcard/";
    CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
        if(pFileUtils->isFileExist(SDCARD_PATH))
    {
        m_uploadpath = SDCARD_PATH + "upload.mp3";
        m_downloadpath = SDCARD_PATH + "download.mp3";
    }
#endif
}

CIMManager::~CIMManager(){}

CIMManager* CIMManager::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CIMManager();
	}
	return m_Instance;
}

void CIMManager::init(){
    string deviceID = CCEPlugin::getDeviceID();
    GCloudVoiceErrno error1 = gcloud_voice::GetVoiceEngine()->SetAppInfo("1850970758","f6ef511cd4a2ed1cf41a69f29ce8d4bc",deviceID.c_str());
    GCloudVoiceErrno error2 = gcloud_voice::GetVoiceEngine()->Init();
    GCloudVoiceErrno error3 = gcloud_voice::GetVoiceEngine()->SetMode(gcloud_voice::IGCloudVoiceEngine::Messages);
    GCloudVoiceErrno error4 = gcloud_voice::GetVoiceEngine()->SetNotify(this);
}

void CIMManager::OnPause(){
    gcloud_voice::GetVoiceEngine()->Pause();
}

void CIMManager::OnResume(){
    gcloud_voice::GetVoiceEngine()->Resume();
}

void CIMManager::update(float delta)
{
    gcloud_voice::GetVoiceEngine()->Poll();
}

void CIMManager::beginRecord(){
   GCloudVoiceErrno error = gcloud_voice::GetVoiceEngine()->StartRecording (m_uploadpath.c_str());
}

void CIMManager::stopRecord(bool isUpload){
    GCloudVoiceErrno error = gcloud_voice::GetVoiceEngine()->StopRecording();
    if (isUpload) {
        uploadRecord();
    }
}

void CIMManager::enterRoom(){
    GCloudVoiceErrno error5 = gcloud_voice::GetVoiceEngine()->ApplyMessageKey (6000);
    gcloud_voice::GetVoiceEngine()->SetMaxMessageLength (11000);
}

void CIMManager::uploadRecord()
{
    int ret1 = gcloud_voice::GetVoiceEngine()->UploadRecordedFile (m_uploadpath.c_str(), 60000);
}

void CIMManager::downloadRecord(const char *fileID){
    int ret = gcloud_voice::GetVoiceEngine()->DownloadRecordedFile (fileID, m_downloadpath.c_str(), 60000);
}

void CIMManager::OnApplyMessageKey(gcloud_voice::GCloudVoiceCompleteCode code){
    int i = 3;
}

void CIMManager::OnUploadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID){
    if (code == gcloud_voice::GV_ON_UPLOAD_RECORD_DONE) {
        uploadRecordCallBack(fileID);
    }
}

void CIMManager::OnDownloadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID){
    if (code == gcloud_voice::GV_ON_DOWNLOAD_RECORD_DONE) {
        int err;
        err = gcloud_voice::GetVoiceEngine()->PlayRecordedFile(filePath);
        downloadRecordCallBack(fileID);
    }
}

void CIMManager::uploadRecordCallBack(const char *fileID){
    // send c event
    if(m_uploadListener && m_uploadCallBack){
        (m_uploadListener->*m_uploadCallBack)(fileID);
    }
    
    // send lua event
    if(m_uploadScriptHandler)
    {
        CCArray* pArrayArgs = CCArray::createWithCapacity(1);
        pArrayArgs->addObject(CCString::create(fileID));
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_uploadScriptHandler,pArrayArgs);
    }
}

void CIMManager::downloadRecordCallBack(const char *fileID){
    // send c event
    if(m_downloadListener && m_downloadCallBack){
        (m_downloadListener->*m_downloadCallBack)(fileID);
    }
    
    // send lua event
    if(m_downloadScriptHandler)
    {
        CCArray* pArrayArgs = CCArray::createWithCapacity(1);
        pArrayArgs->addObject(CCString::create(fileID));
        CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_downloadScriptHandler,pArrayArgs);
    }
}
//#endif


