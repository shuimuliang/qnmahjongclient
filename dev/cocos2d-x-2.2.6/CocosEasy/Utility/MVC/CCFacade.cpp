#include "CCFacade.h"
#include "CCNetFacade.h"
#include "CCModuleManager.h"

#include "cocos2d.h"
using namespace cocos2d;

/*
void g_sendPackToMainThread( INotification &netNotication,const string& msgInMainThread )
{
Packet* packet =(Packet*) netNotication.getBody();
Notification noti(msgInMainThread);
noti.addCCObjectType(packet);
noti.setNum(netNotication.getNum());
CCNetFacade::getInstance()->sendNotificationToMainThread(noti);
}
*/

namespace Engine
{

	static CCFacade *s_ince = NULL;

	CCFacade * CCFacade::getInstance()
	{
		if(s_ince == NULL){
			s_ince = new CCFacade();
		}
		return s_ince;
	}


	CCFacade::CCFacade()
	{
	}

	CCFacade::~CCFacade()
	{
	}

	void CCFacade::release()
	{
		CC_SAFE_DELETE(s_ince);
	}
}