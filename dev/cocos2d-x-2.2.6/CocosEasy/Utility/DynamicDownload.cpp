#include "DynamicDownload.h"
#include "Utility/Utils.h"

#include <stdio.h>

using namespace std;

NS_CC_EASY_BEGIN
//e.g : DynamicDownloadExt::getInstance()->startDownload("http://dev.chaoshenxy.com:8080/version/npk-master.zip","download/hihi.zip",true);
const string SDCARD_PATH = "/mnt/sdcard/";
const string DOWNLOAD_FILE_NAME = "download.zip";

DynamicDownloadExt	* DynamicDownloadExt::ince = NULL;

class header_values 
{
public:
	header_values(const string &fileName, FILE *fp)
		:m_strFiles(fileName),
		m_pFile(fp)
	{}
	std::string m_strFiles;
	FILE *m_pFile;
};

DynamicDownloadExt * DynamicDownloadExt::getInstance()	
{
	if(!ince){
		ince = new DynamicDownloadExt();
	}
	return ince;
}

void DynamicDownloadExt::end()
{
	CC_SAFE_RELEASE_NULL(ince);
}

DynamicDownloadExt::DynamicDownloadExt()
	:m_handleOnStart(-1)
	,m_handleOnProcess(-1)
	,m_handleOnComplete(-1)
	,m_handleOnError(-1)
	,m_pDownloadListen(NULL)
	,m_strDir("download")
{
	pthread_mutex_init(&m_mutex_receive_queue,NULL);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector( schedule_selector( DynamicDownloadExt::update ), this, 0.02f, false );
	init();
}

bool DynamicDownloadExt::init()
{
	CCHttpDownloadQueueManager::getInstance()->startUp();
	return true;
}

void DynamicDownloadExt::setScriptEvent(int handleOnStart,int handleOnProcess,int handleOnComplete,int handleOnError)
{
	m_handleOnStart = handleOnStart;
	m_handleOnProcess = handleOnProcess;
	m_handleOnComplete = handleOnComplete;
	m_handleOnError = handleOnError;
}

const std::string DynamicDownloadExt::getAbsoluteDownloadDir(){
    string downloadPath = factoryDownloadPath(m_strDir+"/");
    return downloadPath;
}

void DynamicDownloadExt::startDownload(const std::string &url, bool isZip)
{
	string downloadPath = factoryDownloadPath(m_strDir+"/"+DOWNLOAD_FILE_NAME);
	DynamicDownloadQueueWrapperExt* wrapper = DynamicDownloadQueueWrapperExt::create(url,downloadPath,this);
	wrapper->setUnCompress(isZip);
	DynamicDownloadThreadExt* thread =  new DynamicDownloadThreadExt(wrapper);
	CCHttpDownloadQueueManager::getInstance()->addRequest(thread);
	thread->release();
}

std::string DynamicDownloadExt::factoryDownloadPath(std::string fileName)
{
	std::string path = SDCARD_PATH;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	if( !CCFileUtils::sharedFileUtils()->isFileExist( SDCARD_PATH ) )
	{
		path = CCFileUtils::sharedFileUtils()->getWritablePath();
	}

#else
	path = CCFileUtils::sharedFileUtils()->getWritablePath();
#endif
	std::string DIR_ROOT = "";
	path += DIR_ROOT + fileName;
	return path;
}

void DynamicDownloadExt::onStart(string downLoadPath)
{
	StruMsg msg;
	msg.mType = eMsgStart;
	msg.msg1 = downLoadPath;
	addToMainThread(msg);
}

void DynamicDownloadExt::onComplete(string downLoadPath)
{
	StruMsg msg;
	msg.mType = eMsgComplete;
	msg.msg1 = downLoadPath;
	addToMainThread(msg);
}

void DynamicDownloadExt::onProcess(int number , string downLoadPath)
{
	StruMsg msg;
	msg.mType = eMsgProcess;
	msg.num = number;
	msg.msg1 = downLoadPath;
	addToMainThread(msg);
}

void DynamicDownloadExt::onFail(string errors,std::string url)
{
	StruMsg msg;
	msg.mType = eMsgError;
	msg.msg1 = errors;
	msg.msg2 = url;
	addToMainThread(msg);
}

DynamicDownloadExt::~DynamicDownloadExt()
{
	m_pDownloadListen = NULL;
	CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
	pthread_mutex_destroy(&m_mutex_receive_queue);
}

void DynamicDownloadExt::registDownLoadListener(CCDownloadListener *pListen)
{
	m_pDownloadListen = pListen;
}

void DynamicDownloadExt::update(float delta)
{
	pthread_mutex_lock(&m_mutex_receive_queue);
	while(!m_msgs.empty()){
		StruMsg msg = m_msgs.front();
		switch(msg.mType){
		case eMsgStart:
			{
				if(m_pDownloadListen)m_pDownloadListen->onStart(msg.msg1);
				onStartScript(msg);
			}
			break;
		case eMsgProcess:
			{
				if(m_pDownloadListen)m_pDownloadListen->onProcess(msg.num,msg.msg1);
				onProcessScript(msg);
			}
			break;
		case eMsgComplete:
			{
				if(m_pDownloadListen)m_pDownloadListen->onComplete(msg.msg1);
				onCompleteScript(msg);
			}
			break;
		case eMsgError:
			{
				if(m_pDownloadListen)m_pDownloadListen->onFail(msg.msg1,msg.msg2);
				onErrorScript(msg);
			}
			break;
		}
		m_msgs.pop();
	}
	pthread_mutex_unlock(&m_mutex_receive_queue);
}

void DynamicDownloadExt::addToMainThread(StruMsg msg)
{
	pthread_mutex_lock(&m_mutex_receive_queue);
	m_msgs.push(msg);
	pthread_mutex_unlock(&m_mutex_receive_queue);
}

void DynamicDownloadExt::onStartScript(StruMsg msg)
{
	if (m_handleOnStart <= 0) return;

	CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	pArrayArgs->addObject(CCString::create(msg.msg1));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handleOnStart,pArrayArgs);

}

void DynamicDownloadExt::onCompleteScript(StruMsg msg)
{
	if(m_handleOnComplete <= 0) return;
	CCArray* pArrayArgs = CCArray::createWithCapacity(1);
	pArrayArgs->addObject(CCString::create(msg.msg1));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handleOnComplete,pArrayArgs);
}

void DynamicDownloadExt::onProcessScript(StruMsg msg)
{
	if (m_handleOnProcess <= 0) return;
	CCArray* pArrayArgs = CCArray::createWithCapacity(2);
	pArrayArgs->addObject(CCInteger::create(msg.num));
	pArrayArgs->addObject(CCString::create(msg.msg1));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handleOnProcess,pArrayArgs);
}


void DynamicDownloadExt::onErrorScript(StruMsg msg)
{
	if(m_handleOnError <= 0) return;
	CCArray* pArrayArgs = CCArray::createWithCapacity(2);
	pArrayArgs->addObject(CCString::create(msg.msg1));
	pArrayArgs->addObject(CCString::create(msg.msg2));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_handleOnError,pArrayArgs);
}

void DynamicDownloadExt::setDownloadDir(const std::string &dir)
{
	m_strDir = dir;
}

bool DynamicDownloadExt::isHasZipInDownloadDir()
{
	string src = CCFileUtils::sharedFileUtils()->fullPathForFilename(DOWNLOAD_FILE_NAME.c_str());
	return CCFileUtils::sharedFileUtils()->isFileExist(src);
}

bool DynamicDownloadExt::uncompressDownloadzip()
{
	string src = CCFileUtils::sharedFileUtils()->fullPathForFilename(DOWNLOAD_FILE_NAME.c_str());
	if(!CCFileUtils::sharedFileUtils()->isFileExist(src)) return false;
	bool succ = utils::uncompress(src);
	utils::deleteFile(src.c_str());
    return succ;
}

void DynamicDownloadExt::cleanDownloadDir(){
    string downloadPath = factoryDownloadPath(m_strDir+"/");
    utils::cremoveDir(downloadPath.c_str());
}

void DynamicDownloadExt::unRegistDownLoadListener()
{
	// wild pointer or null pointer or good pointer
	m_pDownloadListen = NULL;
}


/*======================================DynamicDownloadThreadExt===================================================*/
DynamicDownloadThreadExt::DynamicDownloadThreadExt(DynamicDownloadQueueWrapperExt *downLoadQueueWrapper)
{
	this->m_pDownloadQueueWrapper = downLoadQueueWrapper;
	this->m_pDownloadQueueWrapper->retain();
}

DynamicDownloadThreadExt::~DynamicDownloadThreadExt()
{
	CC_SAFE_RELEASE(m_pDownloadQueueWrapper);
}

void DynamicDownloadThreadExt::run()
{
	//ƒø¬ººÏ≤È“‘º∞¥¥Ω®
	checkAndCreateDir();

	startDownload();
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	header_values* values=(header_values*)userdata;
	FILE *fp = values->m_pFile;
	size_t written = fwrite(ptr, size, nmemb, fp);
	return written;
}

static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	if(nowDownloaded > 0.0000000001)
	{
		DynamicDownloadQueueWrapperExt* wrapper = (DynamicDownloadQueueWrapperExt*)(ptr);
		wrapper->getDownloadListener()->onProcess((int)(nowDownloaded/totalToDownload*100),wrapper->getDownloadPath());
	}
	return 0;
}

bool DynamicDownloadThreadExt::startDownload()
{
	//≥ı ºªØ≤Œ ˝
	unsigned char tryTimes = 3;
	CURLcode errorCode = CURL_LAST;
	//≥ı ºªØcurl
	m_curl = curl_easy_init();
	if(!m_curl)
	{
		CCLOG("init curl error");
		return false;
	}
	string downloadPath = m_pDownloadQueueWrapper->getDownloadPath();

	struct curl_slist *cHeaders = NULL;

	header_values pWriteData(downloadPath,NULL);
	pWriteData.m_pFile = fopen(downloadPath.c_str(), "wb");
	if(!pWriteData.m_pFile)
	{
		CCLOG("can not create file %s",downloadPath.c_str());
		return false;
	}
	long retCode = 0;
	do 
	{
		m_pDownloadQueueWrapper->getDownloadListener()->onStart(downloadPath);
		//…Ë÷√œ¬‘ÿµÿ÷∑
		errorCode = curl_easy_setopt(m_curl,CURLOPT_URL,m_pDownloadQueueWrapper->getUrl().c_str());
		if(errorCode!=CURLE_OK)
		{
			break;
		}

		//‘ –Ì±ª◊•»°µƒ¡¥Ω”Ã¯◊™
		errorCode = curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
		if (errorCode != CURLE_OK) 
		{
			break;
		}
		//≤È’“¥Œ ˝,∑¿÷π≤È’“Ã´…Ó
		errorCode = curl_easy_setopt(m_curl, CURLOPT_MAXREDIRS, 10);
		if (errorCode != CURLE_OK) 
		{
			break;
		}
		//»Áπ˚ƒ„œÎ»√PHP‘⁄∑¢…˙¥ÌŒÛ(HTTP¥˙¬Î∑µªÿ¥Û”⁄µ»”⁄300) ±£¨≤ªœ‘ æ£¨…Ë÷√’‚∏ˆ—°œÓŒ™“ª»À∑«¡„÷µ°£ƒ¨»œ––Œ™ «∑µªÿ“ª∏ˆ’˝≥£“≥£¨∫ˆ¬‘¥˙¬Î°£
		errorCode = curl_easy_setopt(m_curl, CURLOPT_FAILONERROR, 1);
		if (errorCode != CURLE_OK) 
		{
			break;
		}
		//∆Ù”√ ±ª·≥¢ ‘–ﬁ∏ƒ‘∂≥ÃŒƒµµ÷–µƒ–≈œ¢
		errorCode = curl_easy_setopt(m_curl, CURLOPT_FILETIME, 1L);
		if (errorCode != CURLE_OK) 
		{
			break;
		}
		//Ω˚”√µÙalarm’‚÷÷≥¨ ±,‘≠“Ú≤ªÃ´√˜¡À
		errorCode = curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
		if (errorCode != CURLE_OK) 
		{
			break;
		}
		//…Ë÷√œ¬‘ÿªÿµ˜∫Ø ˝
		errorCode = curl_easy_setopt(m_curl , CURLOPT_WRITEFUNCTION , downLoadPackage);
		if(errorCode != CURLE_OK)
		{
			break;
		}

		errorCode = curl_easy_setopt(m_curl , CURLOPT_WRITEDATA , &pWriteData);
		if(errorCode != CURLE_OK)
		{
			break;
		}

		//…Ë÷√œ¬‘ÿΩ¯∂»ªÿµ˜∫Ø ˝
		errorCode = curl_easy_setopt(m_curl ,  CURLOPT_NOPROGRESS , false);
		if(errorCode != CURLE_OK)
		{
			break;
		}

		errorCode = curl_easy_setopt(m_curl , CURLOPT_PROGRESSFUNCTION , progressFunc);
		if(errorCode != CURLE_OK)
		{
			break;
		}
        /* add by liulihua for disable ssl verify */
        errorCode = curl_easy_setopt(m_curl , CURLOPT_SSL_VERIFYPEER , false);
        if(errorCode != CURLE_OK)
        {
            break;
        }
        errorCode = curl_easy_setopt(m_curl , CURLOPT_SSL_VERIFYHOST , false);
        if(errorCode != CURLE_OK)
        {
            break;
        }
        /* add by liulihua for disable ssl verify */
        errorCode = curl_easy_setopt(m_curl , CURLOPT_PROGRESSDATA , m_pDownloadQueueWrapper);
		if(errorCode != CURLE_OK)
		{
			break;
		}

		errorCode = curl_easy_perform(m_curl);//’‚“ª≤Ω «∂¬»˚≤Ÿ◊˜
		if(errorCode != CURLE_OK)
		{
			break;
		}

		int fileSize = ftell(pWriteData.m_pFile);
		if(fileSize == 0){
			CCLOG("%s:download fail" , downloadPath.c_str());
			break;
		}

		//≈–∂œªÒ»°œÏ”¶µƒhttpµÿ÷∑ «∑Ò¥Ê‘⁄,»Ù¥Ê‘⁄‘Ú∑µªÿ200,400“‘…œ‘ÚŒ™≤ª¥Ê‘⁄,“ª∞„≤ª¥Ê‘⁄Œ™404¥ÌŒÛ  
		errorCode = curl_easy_getinfo(m_curl , CURLINFO_RESPONSE_CODE , &retCode);
		if(CURLE_OK == errorCode && retCode >= 400)
		{
			--tryTimes;
			CCLOG("error ,retry download");
			//πÿ±’Œƒº˛e
			fclose(pWriteData.m_pFile);
			//÷ÿ÷√curl
			if(m_curl)
			{
				curl_easy_cleanup(m_curl);
				m_curl = curl_easy_init();
			}
			if(tryTimes==0){
				m_pDownloadQueueWrapper->getDownloadListener()->onFail(downloadPath,m_pDownloadQueueWrapper->getUrl());
			}
		}
		break;
	} while (tryTimes>0);
	int err = fclose(pWriteData.m_pFile);
	if(err){
		CCLOG("can't close file %s",downloadPath.c_str());
		m_pDownloadQueueWrapper->getDownloadListener()->onFail(downloadPath,m_pDownloadQueueWrapper->getUrl());
	}else{
		pWriteData.m_pFile = NULL;
		if(retCode>=400||errorCode!=CURLE_OK)
		{
			//…æ≥˝Œƒº˛
			string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(downloadPath.c_str());
			utils::deleteFile(fileName.c_str());
			m_pDownloadQueueWrapper->getDownloadListener()->onFail(downloadPath,m_pDownloadQueueWrapper->getUrl());
		}else
		{
			if(m_pDownloadQueueWrapper->isUnCompress())
			{
				uncompress();
				//delete zip file
				string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(downloadPath.c_str());
				utils::deleteFile(fileName.c_str());
				
			}
			m_pDownloadQueueWrapper->getDownloadListener()->onComplete(downloadPath);
		}
	}
	//terry lu add 
	if(m_curl){
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
	return true;
}

void DynamicDownloadThreadExt::checkAndCreateDir()
{
	utils::mkdirs(m_pDownloadQueueWrapper->getDownloadDir().c_str());
}

void DynamicDownloadThreadExt::uncompress()
{
	string downloadPath = m_pDownloadQueueWrapper->getDownloadPath();
	size_t zipPos = downloadPath.find(".zip");
	size_t zipDpos = downloadPath.find(".ZIP");
	if (zipPos != std::string::npos || zipDpos != std::string::npos)
	{
		utils::uncompress(downloadPath);
	}	 
}
/*============DynamicDownloadQueueWrapperExt===================*/


DynamicDownloadQueueWrapperExt::DynamicDownloadQueueWrapperExt(const string &url,const string &downloadPath,CCDownloadListener *downloadListener)
	:m_bIsSupportUnCompress(false),
	m_AutoDelete(false)
{
	m_strUrl = url;
	m_strDownloadPath = downloadPath;
	CC_ASSERT(downloadListener);
	m_pDownloadListen = downloadListener;
	CCObject *temObj = dynamic_cast<CCObject*>(m_pDownloadListen);
	CC_ASSERT(temObj);
	temObj->retain();
	makeDir();
}

DynamicDownloadQueueWrapperExt::~DynamicDownloadQueueWrapperExt()
{
	CCObject* object = dynamic_cast<CCObject*>(m_pDownloadListen);
	CC_ASSERT(object);
	CC_SAFE_RELEASE(object);
}

DynamicDownloadQueueWrapperExt* DynamicDownloadQueueWrapperExt::create(string url, string downloadPath,CCDownloadListener *downloadListener)
{
	DynamicDownloadQueueWrapperExt *result = new DynamicDownloadQueueWrapperExt(url,downloadPath,downloadListener);
	result->autorelease();
	result->retain();
	return result;
}

string DynamicDownloadQueueWrapperExt::getUrl()
{
	return m_strUrl;
}

string DynamicDownloadQueueWrapperExt::getDownloadPath()
{
	return m_strDownloadPath;
}

void DynamicDownloadQueueWrapperExt::setUnCompress(bool isUnCompress)
{
	m_bIsSupportUnCompress = isUnCompress;
}

void DynamicDownloadQueueWrapperExt::setAutoDelete(bool isAutoDelete)
{
	m_AutoDelete = isAutoDelete;
}

bool DynamicDownloadQueueWrapperExt::isUnCompress()
{
	return m_bIsSupportUnCompress;
}

bool DynamicDownloadQueueWrapperExt::isAutoDelete()
{
	return m_AutoDelete;
}

CCDownloadListener* DynamicDownloadQueueWrapperExt::getDownloadListener()
{
	return m_pDownloadListen;
}

string DynamicDownloadQueueWrapperExt::getDownloadDir()
{
	return m_strDownloadDir;
}

void DynamicDownloadQueueWrapperExt::makeDir()
{
	m_strDownloadDir = m_strDownloadPath;
	size_t spos = m_strDownloadPath.find_last_of(".");
	if(spos != std::string::npos)
	{
		//»Áπ˚¬∑æ∂÷–”–.
		size_t pos = m_strDownloadPath.find_last_of("/");
		size_t ppos = m_strDownloadPath.find_last_of("\\");


		if (pos != std::string::npos)
		{
			m_strDownloadDir = m_strDownloadPath.substr(0,pos);
		}else if (ppos != std::string::npos)
		{
			m_strDownloadDir = m_strDownloadPath.substr(0,ppos);
		}
	}
}

NS_CC_EASY_END