#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN


class CEventManager:public CCObject
{
public:
	~CEventManager();
	static CEventManager* getInstance();
	static void end();

	void registerScriptEvent(int msg, int handler);
	void unregisterEvent(int msg);
	
	void sendEvent(int msg,CCArray* pArrayArgs = NULL);

protected:
	CEventManager();
	CCDictionary* m_EventDict;
	
};




NS_CC_EASY_END

#endif