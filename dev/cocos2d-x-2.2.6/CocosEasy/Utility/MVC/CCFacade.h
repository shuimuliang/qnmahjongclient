#ifndef __CCFACADE_H_
#define __CCFACADE_H_

#include "core/Facade.h"

#include "cocos2d.h"
USING_NS_CC;

namespace Engine
{

	class CCIModule;
	class CCModuleManager;

	//void g_sendPackToMainThread(INotification &netNotication,const string& msgInMainThread);
	class CCFacade : public Facade
	{
	public:
		static CCFacade * getInstance();
		static void release();
		CCFacade();
		~CCFacade();
	};

}

#endif // __CCFACADE_H_

