#include "Config.h"
#include "CocosEasy.h"

std::string DOWNLOAD_ROOT = "casino_final";
std::string CONFIG_JSON = "config.json";
std::string SDCARD_PATH = "/mnt/sdcard/";
std::string DOWNLOAD_DIR = "casino_000012000"; // note: this directory need to be modified with a big version release


unsigned int INNER_VERSION = 0;
std::string SHOW_VERSION = "";
std::string VERSION_SERVER_IP = "";

//int LANGUAGE_DEBUG = -1;

std::string RESOUCE_JSON = "resource.json";

std::string getRealDownloadDir()
{
    int channel = CSDKManager::getInstance()->getChannelID();
    char temp[128];
    string str;
    sprintf(temp, "%s%d/%s",DOWNLOAD_ROOT.c_str(),channel,DOWNLOAD_DIR.c_str());
    string realPath(temp);
    return realPath;
}

void readConfig()
{
    std::vector<std::string> searchPaths;
    searchPaths.push_back("art/ccb/Resources");
    searchPaths.push_back("art/ccb/Resources/font");
    searchPaths.push_back("art/ccb/Scenes");
    searchPaths.push_back("art/effect");
    searchPaths.push_back("art/particle");
    searchPaths.push_back("fonts");

	// clear all paths
	CCFileUtils* pFileUtils = CCFileUtils::sharedFileUtils();
	std::vector<std::string> pathList = pFileUtils->getSearchPaths();
	pFileUtils->removeAllPaths();

	// add download path
	string downloadPath;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if(pFileUtils->isFileExist(SDCARD_PATH))
	{
		downloadPath = SDCARD_PATH + getRealDownloadDir();
	}
	else
	{
		downloadPath = pFileUtils->getWritablePath() + getRealDownloadDir();
	}

#else
	downloadPath = pFileUtils->getWritablePath() + getRealDownloadDir();
#endif
    pFileUtils->addSearchPath(downloadPath.c_str());

	DynamicDownloadExt::getInstance()->setDownloadDir(getRealDownloadDir());
        
    // recover other paths
    for(unsigned int i = 0; i < searchPaths.size(); ++i)
    {
        pFileUtils->addSearchPath((downloadPath+"/"+searchPaths[i]).c_str());
        pathList.push_back(searchPaths[i].c_str());
    }
    
    for(unsigned int i = 0; i < searchPaths.size(); ++i)
    {
        pFileUtils->addSearchPath((searchPaths[i]).c_str());
        pathList.push_back(searchPaths[i].c_str());
    }
	// recover other paths
	for(unsigned int i = 0; i < pathList.size(); ++i)
	{
		pFileUtils->addSearchPath(pathList[i].c_str());
	}
    
	// read json
	unsigned long size = 0;
	unsigned char* pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(CONFIG_JSON.c_str(), "r", &size);
	if(!pBytes)
	{
		CEFatal("can not find config.json");
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
	
	INNER_VERSION =  atoi( DICTOOL->getStringValue_json(doc, "innner_version", "000000000") );
	SHOW_VERSION = DICTOOL->getStringValue_json(doc, "show_version", "v1.0.0"); 
	VERSION_SERVER_IP = DICTOOL->getStringValue_json(doc, "version_server_ip", "");
}
