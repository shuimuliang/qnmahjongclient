#include "CocosEasy.h"
#include "CocoStudio/Reader/GUIReader.h"
#include "CocoStudio/Armature/utils/CCArmatureDataManager.h"
#include "CocoStudio/Reader/WidgetReader/WidgetReader.h"

NS_CC_EASY_BEGIN

CCocosEasyManager* CCocosEasyManager::m_Instance = NULL;

CCocosEasyManager* CCocosEasyManager::getInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CCocosEasyManager();
	}
	return m_Instance;
}

void CCocosEasyManager::start()
{

}

void CCocosEasyManager::end()
{
	CC_SAFE_DELETE(m_Instance); 
	m_Instance = NULL;

	// cocos easy
	CResourceLoader::getInstance()->end();
	CSceneTransitMgr::getInstance()->end();
	CDBManager::getInstance()->end();
	CSDKManager::getInstance()->end();
	CAnalysis::getInstance()->end();
	CEventManager::end();
	DynamicDownloadExt::end();
	CCHttpDownloadQueueManager::end();
	// ==================== cocostudio ==================== //
	GUIReader::shareReader()->purge();
	WidgetReader::getInstance()->purge();
	CCArmatureDataManager::sharedArmatureDataManager()->purge();
	DictionaryHelper::shareHelper()->purgeDictionaryHelper();
	ObjectFactory::getInstance()->destroyInstance();
	
	// action manager
	ActionManager::shareManager()->releaseActions();
	ActionManager::shareManager()->purge();
	
	CELog::getInstance()->end();
	// script engine
	CCScriptEngineManager::purgeSharedManager();

	
}









NS_CC_EASY_END;