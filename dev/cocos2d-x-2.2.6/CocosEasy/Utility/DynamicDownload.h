#ifndef __DYNAMIC_DOWNLOAD_EXT_H__
#define __DYNAMIC_DOWNLOAD_EXT_H__

#include "Common/CocosEasyDef.h"
#include "HTTP/CCHttpDownloadQueueManager.h"
#include <queue>


using namespace Engine;
NS_CC_EASY_BEGIN

/************************************************************************/
/*e.g: DynamicDownloadExt::getInstance()->startDownload("http://192.168.0.175/log.zip","download\\hihi.zip",true);*/
/************************************************************************/
class DynamicDownloadExt: public CCObject , public CCDownloadListener
{
public:
    static DynamicDownloadExt *getInstance();
	static void end();

	void startDownload(const std::string &url , bool isZip);
    void registDownLoadListener(CCDownloadListener *pListen);
	void unRegistDownLoadListener();

    void setScriptEvent(int handleOnStart , int handleOnProcess , int handleOnComplete , int handleOnError);
	void setDownloadDir(const std::string &dir);
	bool isHasZipInDownloadDir();
	bool uncompressDownloadzip();
    void cleanDownloadDir();
    std::string& getDownloadDir(){return m_strDir;}
    const std::string getAbsoluteDownloadDir();

    virtual ~DynamicDownloadExt();
    virtual void update(float delta);
private:
	static DynamicDownloadExt	*ince;
	// define script handle
    int                         m_handleOnStart;
    int                         m_handleOnProcess;
    int                         m_handleOnComplete;
    int                         m_handleOnError;
	string						m_strDir;
    enum E_MSG_TYPE             {eMsgStart, eMsgProcess, eMsgComplete, eMsgError};
    struct StruMsg
    {
        E_MSG_TYPE mType;
        int num ;
        string msg1 ;
        string msg2;
    };
    queue<StruMsg>              m_msgs;
    pthread_mutex_t             m_mutex_receive_queue;

    CCDownloadListener         *m_pDownloadListen;

    DynamicDownloadExt();

    virtual bool init();

    virtual void onStart(string downLoadPath);

    virtual void onComplete(string downLoadPath);

    virtual void onProcess(int number , string downLoadPath);

    virtual void onFail(string errors , std::string url);

    std::string factoryDownloadPath(std::string fileName);

    void addToMainThread(StruMsg msg);

    void onStartScript(StruMsg msg);

    void onCompleteScript(StruMsg msg);

    void onProcessScript(StruMsg msg);

    void onErrorScript(StruMsg msg);
};

class DynamicDownloadQueueWrapperExt : public CCObject
{
public:
    ~DynamicDownloadQueueWrapperExt();
    static DynamicDownloadQueueWrapperExt* create(string url, string downloadPath, CCDownloadListener *downloadListener);

    CCDownloadListener* getDownloadListener();
    string getUrl();
    string getDownloadPath();
    string getDownloadDir();

    void setUnCompress(bool isUnCompress);
    void setAutoDelete(bool isAutoDelete);

    bool isUnCompress();
    bool isAutoDelete();
private:
    DynamicDownloadQueueWrapperExt(const string &url, const string &downloadPath, CCDownloadListener *downloadListener);
    /**
    * http地址
    */
    string m_strUrl;
    /**
    * 下载路径
    */
    string m_strDownloadPath;

    /**
    * 下载监听器
    */
    CCDownloadListener *m_pDownloadListen;

    /**
    * 自动解压
    */
    bool m_bIsSupportUnCompress;

    /**
    * 自动删除
    */
    bool m_AutoDelete;

    string m_strDownloadDir;

    void makeDir();
};

class DynamicDownloadThreadExt : public CCRunable, public CCObject
{
public:
    DynamicDownloadThreadExt(DynamicDownloadQueueWrapperExt *downLoadQueueWrapper);
    virtual ~DynamicDownloadThreadExt();
    virtual void run();

private:
    DynamicDownloadQueueWrapperExt *m_pDownloadQueueWrapper;
    CURL *m_curl;
    bool startDownload();

    void checkAndCreateDir();

    void uncompress();
};
NS_CC_EASY_END

#endif