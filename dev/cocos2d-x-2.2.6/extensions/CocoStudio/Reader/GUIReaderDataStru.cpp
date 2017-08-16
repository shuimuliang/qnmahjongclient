#include "GUIReaderDataStru.h"
#include "GUIReaderDataStruWidgetTree.h"
#include "GUIReaderDataStruWidgetFactory.h"
USING_NS_CC_EXT;

cocos2d::extension::GUIDataStruManager::GUIDataStruManager()
{
	init();
}

cocos2d::extension::GUIDataStruManager::~GUIDataStruManager()
{
    // delete m_pDataPool
    map<string, GUIDataStru*>::iterator it = m_dataPool.begin();

    for(; it != m_dataPool.end() ; ++it)
    {
        CC_SAFE_DELETE(it->second);
    }
	// delete GUIReaderDataStruWidgetFactory
	GUIDataStruWidgetFactory::delInstance();
}

void cocos2d::extension::GUIDataStruManager::init()
{
	GUIDataStruWidgetFactory::getInstance();
}

extension::GUIDataStru * extension::GUIDataStruManager::loadDataStru(const char *fileName)
{
	// get strFilePath 
	std::string jsonpath = fileName;
	int pos = jsonpath.find_last_of('/');
	std::string strFilePath = jsonpath.substr(0,pos+1);
	jsonpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);

	// get data from pool
	GUIDataStru *result = m_dataPool[jsonpath];
	if(result)return result;

	// read file data
	unsigned long size = 0;
	unsigned char *pBytes = NULL;
	pBytes = CCFileUtils::sharedFileUtils()->getFileData(jsonpath.c_str() , "r" , &size);
	if(pBytes == NULL && strcmp((const char*)pBytes , "") == 0)
	{
		CCLOG("read file[%s] error!\n" , fileName);
		return result;
	}

	// parse to json
	std::string load_str = std::string((const char*)pBytes, size);
	rapidjson::Document *jsonDict = new rapidjson::Document;
	jsonDict->Parse<0>(load_str.c_str());
	if(jsonDict->HasParseError())
	{
		CCLOG("GetParseError %s\n" , jsonDict->GetParseError());
		return result;
	}

	// convert json to data struct
	result = new GUIDataStru;
	if(!result->loadJson(jsonDict)){
		delete result;
		result = NULL;
	}else{
		result->setFilePath(strFilePath);
		m_dataPool[jsonpath] = result;
	}
	CC_SAFE_DELETE_ARRAY(pBytes);
	delete jsonDict;
	return result;
}

cocos2d::ui::Widget * cocos2d::extension::GUIDataStruManager::createWidget(const char *fileName)
{
	GUIDataStru *pDataStru = loadDataStru(fileName);
	if(!pDataStru) return NULL;
	
	// design size
	float designWidth = pDataStru->getDesignWidth();
	float designHeight = pDataStru->getDesignHeight();
	if(designWidth<=0||designHeight<=0)
	{
		printf("Read design size error!\n");
		GUIReader::shareReader()->storeFileDesignSize(fileName , CCDirector::sharedDirector()->getWinSize());
	}
	else
	{
		GUIReader::shareReader()->storeFileDesignSize(fileName , CCSizeMake(designWidth , designHeight));
	}

	// read textrue
	const vector<string> &textures = pDataStru->getPngTextures();
	for(std::size_t i = 0; i < textures.size(); ++i)
	{
		string fullPath = pDataStru->getFilePath();
		fullPath += textures.at(i);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fullPath.c_str());
	}

	// widget tree
	ui::Widget *result = widgetByData(pDataStru->getWidgetTree() , pDataStru->getFilePath());
	CC_ASSERT(result);
	if(result->getContentSize().equals((CCSizeZero)))
	{
		result->setSize(CCSizeMake(designWidth,designHeight));
	}
	//action don't use
	return result;
}

cocos2d::ui::Widget * cocos2d::extension::GUIDataStruManager::widgetByData(GUIDataStruWidget *pWidgetData, const string &filePath)
{
	//create widget
	cocos2d::ui::Widget* widget = ObjectFactory::getInstance()->createGUI(pWidgetData->getClassName());
	CC_ASSERT(widget);
	pWidgetData->readWidget(widget ,filePath);

	//children
	vector<GUIDataStruWidget*> &arrChildren = pWidgetData->getChildren();
	for(std::size_t i = 0; i < arrChildren.size() ; ++i)
	{
		GUIDataStruWidget *pChildrenData = arrChildren.at(i);
		CC_ASSERT(pChildrenData);
		cocos2d::ui::Widget* childrenWidget = widgetByData(pChildrenData , filePath);
		widget->addChild(childrenWidget , childrenWidget->getZOrder() , childrenWidget->getTag());
	}
	return widget;
}


cocos2d::extension::GUIDataStru::GUIDataStru()
	:m_pWidgetTree(NULL)
{

}

cocos2d::extension::GUIDataStru::~GUIDataStru()
{
	CC_SAFE_DELETE(m_pWidgetTree);
}

bool cocos2d::extension::GUIDataStru::loadJson(const rapidjson::Document *jsonDict)
{
	if(!jsonDict) return false;

	//don't need version check
	 const char* fileVersion = DICTOOL->getStringValue_json(*jsonDict, "version");
	 m_designWidth = DICTOOL->getFloatValue_json(*jsonDict, "designWidth");
	 m_designHeight = DICTOOL->getFloatValue_json(*jsonDict, "designHeight");

	 //load widgetTree
	 const rapidjson::Value& widgetTree = DICTOOL->getSubDictionary_json(*jsonDict, "widgetTree");
	 m_pWidgetTree = GUIDataStruWidgetFactory::getInstance()->createByJson(widgetTree);
	 if(m_pWidgetTree){
		 return true;
	 }
	return false;
}

const vector<string> & cocos2d::extension::GUIDataStru::getPngTextures()
{
	return m_pngTextures;
}

