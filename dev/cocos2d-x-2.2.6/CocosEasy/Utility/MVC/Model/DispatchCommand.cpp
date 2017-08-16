#include "DispatchCommand.h"
#include "../interface/IMediator.h"

using namespace std;

namespace Engine
{

	void DispatchCommand::addReceiver(IMediator* receiveId )
	{	
		CC_ASSERT(receiveId);  	
		m_ReceiversState[receiveId->getName()]=true;
		m_Receivers[receiveId->getName()]=receiveId;
	}



	void DispatchCommand::execute( INotification & notification )
	{
		map<string,bool>::iterator it= m_ReceiversState.begin();
		map<string,bool>::iterator endit=m_ReceiversState.end();
		while(it!=endit)
		{
			if (it->second)
			{
				it->second=false;
				m_Receivers[it->first]->execute(notification);
			}		
			++it;
		}
	}

}