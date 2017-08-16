//#if CASINO_VOICE

#ifndef __CE_IM_MANAGER_H__
#define __CE_IM_MANAGER_H__

#import "../GCloudVoice/include/GCloudVoice.h"
#include "Common/CocosEasyDef.h"

using namespace gcloud_voice;

typedef void (CCObject::*SEL_UploadRecordCallBack)(const char*);
#define UploadRecordCallBack_selector(_SELECTOR) (SEL_UploadRecordCallBack)(&_SELECTOR)
typedef void (CCObject::*SEL_DownloadRecordCallBack)(const char*);
#define DownloadRecordCallBack_selector(_SELECTOR) (SEL_DownloadRecordCallBack)(&_SELECTOR)


class CIMManager : public IGCloudVoiceNotify, public CCObject
{
private:
    std::string m_uploadpath;
    std::string m_downloadpath;

public:
	CIMManager();
	~CIMManager();
	static CIMManager* getInstance();
    void init();
    void OnPause();
    void OnResume();
    void update(float delta);
    
    void beginRecord();
    void stopRecord(bool isUpload = false);
    void uploadRecord();
    void enterRoom();
    void downloadRecord(const char *fileID);
    
    virtual void OnApplyMessageKey(GCloudVoiceCompleteCode code);
    virtual void OnUploadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID);
    virtual void OnDownloadFile(GCloudVoiceCompleteCode code, const char *filePath, const char *fileID);

public:
    void registerUploadScriptHandler(int handler){m_uploadScriptHandler = handler;}
    void registerDownloadScriptHandler(int handler){m_downloadScriptHandler = handler;}
    
    void uploadRecordCallBack(const char *fileID);
    void downloadRecordCallBack(const char *fileID);
    
private:
	static CIMManager* m_Instance;
    
private:
    // upload record callback
    CC_SYNTHESIZE(int, m_uploadScriptHandler, UploadScriptHandler);
    CC_SYNTHESIZE(CCObject*, m_uploadListener, UploadListener);
    CC_SYNTHESIZE(SEL_UploadRecordCallBack, m_uploadCallBack, UploadCallBack);

    // download record callback
    CC_SYNTHESIZE(int, m_downloadScriptHandler, DownloadScriptHandler);
    CC_SYNTHESIZE(CCObject*, m_downloadListener, DownloadListener);
    CC_SYNTHESIZE(SEL_DownloadRecordCallBack, m_downloadCallBack, DownloadCallBack);
};

#endif
//#endif

