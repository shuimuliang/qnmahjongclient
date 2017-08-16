#include "SceneTransit.h"
#include "ResourceLoader.h"

NS_CC_EASY_BEGIN

CLoadingTransitLayer* CLoadingTransitLayer::create()
{
	CLoadingTransitLayer *pRet = new CLoadingTransitLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void CLoadingTransitLayer::onEnter()
{
	CCLayer::onEnter();
}


void CLoadingTransitLayer::enterNextScene()
{
	CCDirector::sharedDirector()->replaceScene(m_pNextScene);
	CC_SAFE_RELEASE_NULL(m_pNextScene);
}

void CLoadingTransitLayer::resourceLoadCallBack(float percent,bool hasFinished)
{
	// send event 
	loadStep(percent,hasFinished);
	
	// auto transit scene
	bool autoTransit = CSceneTransitMgr::getInstance()->isAutoTransit();
	if(!autoTransit) return;
	if(!hasFinished) return;
	enterNextScene();
}

void CLoadingTransitLayer::loadStep(float percent,bool hasFinished)
{
	if(!m_LoadStepScriptHandler) return;
	
	CCArray* pArrayArgs = CCArray::createWithCapacity(2);
	pArrayArgs->addObject(CCFloat::create(percent));
	pArrayArgs->addObject(CCBool::create(hasFinished));
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(m_LoadStepScriptHandler,pArrayArgs);
}

void CLoadingTransitLayer::setNextScene( CCScene* pScene )
{
	if(!pScene){astBack;}
	CC_SAFE_RELEASE(m_pNextScene);
	m_pNextScene = pScene;
	CC_SAFE_RETAIN(m_pNextScene);
}

void CLoadingTransitLayer::startResourceLoad()
{
	if (CResourceLoader::getInstance()->isRunning()){astBack;}

	// unload
	CResourceLoader::getInstance()->unloadAllResource();

	// load
	CResourceLoader::getInstance()->run(m_ResName.c_str(),this,
		resStepLoad_selector(CLoadingTransitLayer::resourceLoadCallBack),true);
}




CSceneTransitMgr* CSceneTransitMgr::m_Instance = NULL;

CSceneTransitMgr::CSceneTransitMgr()
: m_AutoTransit(false)
, m_pLoadingLayer(NULL)
{

}


CSceneTransitMgr* CSceneTransitMgr::getInstance()
{
	if(m_Instance) return m_Instance;
	m_Instance = new CSceneTransitMgr;
	return m_Instance;
}

void CSceneTransitMgr::end()
{
	if(m_pLoadingLayer) CC_SAFE_RELEASE(m_pLoadingLayer->getParent());
	m_pLoadingLayer = NULL;
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}


void CSceneTransitMgr::runWithScene( CCScene* pScene )
{
	if(!pScene){astBack;}
	if( CCDirector::sharedDirector()->getRunningScene() ) {astBack;}
	CCDirector::sharedDirector()->runWithScene(pScene);
}


void CSceneTransitMgr::replaceScene( CCScene* pScene, const char* resName )
{
	assert(CResourceLoader::getInstance()->isSleep());
	if(!pScene){astBack;}
	if(!resName || !m_pLoadingLayer)
	{
		CCDirector::sharedDirector()->replaceScene(pScene);
		return;
	}
	
	// run resource loader
	m_pLoadingLayer->setNextScene(pScene);
	m_pLoadingLayer->setLoadResName(resName);
	CCDirector::sharedDirector()->replaceScene((CCScene*)m_pLoadingLayer->getParent());
}

void CSceneTransitMgr::setLoadingLayer( CLoadingTransitLayer* pLayer )
{
	if(!pLayer) return;
	if(m_pLoadingLayer) CC_SAFE_RELEASE(m_pLoadingLayer->getParent());
	m_pLoadingLayer = pLayer;

	CCScene* pScene = CCScene::create();
	pScene->addChild(m_pLoadingLayer);
	CC_SAFE_RETAIN(pScene);
}





NS_CC_EASY_END