#include "ISend.h"
#include "../core/Facade.h"

namespace Engine
{

	void ISend::sendNotification( INotification & noti)
	{
		if(pFacade!=NULL){
			pFacade->sendNotification(noti);
		}
	}
	void ISend::sendNotification(const std::string & name){
		if(pFacade!=NULL){
			pFacade->sendNotification(name);
		}
	}

	ISend::~ISend()
	{
		pFacade = NULL;
	}

	ISend::ISend()
	{
		pFacade = NULL;
	}

}
