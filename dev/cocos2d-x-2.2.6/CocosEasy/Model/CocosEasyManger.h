#ifndef __CE_COCOS_EASY_MANAGER_H__
#define __CE_COCOS_EASY_MANAGER_H__

#include "Common/CocosEasyDef.h"

NS_CC_EASY_BEGIN

class CCocosEasyManager
{
public:
	static CCocosEasyManager* getInstance();
	void start();
	void end();

protected:
	CCocosEasyManager(){}

private:
	static CCocosEasyManager* m_Instance;
};


NS_CC_EASY_END;

#endif