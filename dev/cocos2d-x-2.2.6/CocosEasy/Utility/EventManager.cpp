#include "EventManager.h"

NS_CC_EASY_BEGIN

static CEventManager* m_Instance = NULL;

CEventManager::CEventManager()
: m_EventDict(NULL)
{
	m_EventDict = CCDictionary::create();
	CC_SAFE_RETAIN(m_EventDict);

}

CEventManager::~CEventManager()
{
	CC_SAFE_RELEASE(m_EventDict);
}

CEventManager* CEventManager::getInstance()
{
	if(m_Instance) return m_Instance;
	m_Instance = new CEventManager;
	return m_Instance;
}

void CEventManager::end()
{
	CC_SAFE_DELETE(m_Instance);
	m_Instance = NULL;
}

void CEventManager::registerScriptEvent(int msg,int handler)
{
	if(m_EventDict->objectForKey(msg)!=NULL){
		CCLOG("ERROR:registerScriptEvent with the same key:%d",msg);
		return;
	}
	m_EventDict->setObject(CCInteger::create(handler),msg);
}

void CEventManager::unregisterEvent(int msg)
{
	m_EventDict->removeObjectForKey(msg);
}

void CEventManager::sendEvent(int msg,CCArray* pArrayArgs)
{
	CCObject *pHandler = m_EventDict->objectForKey(msg);
	if(!pHandler) return;
	int handler = ((CCInteger*)pHandler)->getValue();
	CCArray* pParm = pArrayArgs ? pArrayArgs : CCArray::create();
	CCScriptEngineManager::sharedManager()->getScriptEngine()->executeEventWithArgs(handler,pParm);
}









NS_CC_EASY_END