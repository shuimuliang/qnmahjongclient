#ifndef _I_CONTROLLER_H_
#define _I_CONTROLLER_H_

#include <string>
#include "../interface/ICommand.h"

namespace Engine
{

	class IController
	{
	public:
		virtual ~IController(){}
		virtual void registerCommand(const std::string & notificationName, ICommand * ) =0; 

		virtual void executeCommand(const INotification & notification) =0; 

		virtual void removeCommand(const std::string & notificationName) =0; 

		virtual bool hasCommand(const std::string & notificationName) =0; 
	};

}
#endif //_I_CONTROLLER_H_


