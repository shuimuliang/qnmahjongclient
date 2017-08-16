#ifndef _I_SEND_H_
#define _I_SEND_H_

#include "../interface/INotification.h"

namespace Engine
{

	class Facade;

	class ISend
	{
	public:
		ISend();
		virtual ~ISend();
		inline void setFacade(Facade *p){pFacade = p;}
		void sendNotification(INotification &  );
		void sendNotification(const std::string & name);

	protected:
		Facade *pFacade;
	};

}
#endif //_I_SEND_H_


