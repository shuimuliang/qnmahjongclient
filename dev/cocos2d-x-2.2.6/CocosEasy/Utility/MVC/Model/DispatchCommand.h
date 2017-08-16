#ifndef _DISPATCHCOMMAND_H_
#define _DISPATCHCOMMAND_H_

#include <string>
#include <map>
using namespace std;

#include "../interface/IMediator.h"
#include "../interface/ICommand.h"

namespace Engine
{

	class DispatchCommand:public ICommand
	{
	public:	
		void addReceiver(IMediator* receiveId);	
	private:		
		map<string,bool> m_ReceiversState;
		map<string,IMediator*> m_Receivers;
		virtual void execute(INotification & notification);

	};

}
#endif //_DISPATCHCOMMAND_H_


